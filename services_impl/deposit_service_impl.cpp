#include "deposit_service_impl.hpp"

#include "repositories/account_repository.hpp"
#include "repositories/deposit_repository.hpp"

#include "model/savings_deposit.hpp"
#include "model/fixed_deposit.hpp"
#include "model/current_deposit.hpp"
#include "model/exceptions_messages.hpp"

#include <boost/format.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace Service {
namespace Impl {

    struct Withdrawer : public Model::DepositVisitor {

        static void withdraw(Model::Deposit& deposit, double amount) {
            Withdrawer withdrawer;
            withdrawer.ammount_ = amount;
            deposit.accept(withdrawer);
        }

        void visit(Model::SavingsDeposit& savings) override {
            double currentBalance = savings.getBalance();
            double minBalance = savings.getMinBalance();

            if(currentBalance - ammount_ < minBalance) {
                throw std::domain_error(Model::Exceptions::minBalanceViolation);
            }

            savings.withdraw(ammount_);
        }

        void visit(Model::FixedDeposit& entity) override {
            throw std::domain_error(Model::Exceptions::wrongFixedDepositOperation);
        }

        void visit(Model::CurrentDeposit& current) override {
            double currentBalance = current.getBalance();
            double newBalance = currentBalance - ammount_;

            if(newBalance < 0){
                double overdraftLimit = current.getOverdraftLimit();
                double allowedSum = currentBalance > 0 ? overdraftLimit : overdraftLimit + currentBalance;

                if(std::abs(newBalance) > allowedSum){
                    throw std::domain_error(Model::Exceptions::ovedraftOverflow);
                }
            }

            current.withdraw(ammount_);
        }

        double ammount_ = 0.0;
    };

    struct YearProcessor : public Model::DepositVisitor {
        static void process(Model::Deposit& deposit) {
            YearProcessor processor;
            deposit.accept(processor);
        }

        void visit(Model::SavingsDeposit& savings) override {
            savings.refill(savings.getBalance() * (savings.getRateOfInterest() / 100.0));
        }

        void visit(Model::FixedDeposit& entity) override {
            static double maturityInterest = 0.2;
            double maturityCharge = entity.getMaturity() * maturityInterest;
            entity.refill(entity.getBalance() * ((entity.getRateOfInterest() + maturityCharge) / 100.0));
            entity.mature();
        }

        void visit(Model::CurrentDeposit& current) override {
            // Current deposit has 0 rate of interest. The holder pays operational charge
            static const double operationalChargeInterest = 0.2;
            double operationalCharge = current.getOverdraftLimit() * operationalChargeInterest;
            current.withdraw(operationalCharge);
        }
    };


    DepositServiceImpl::DepositServiceImpl(
            Repository::DepositRepository& depositRepo, Repository::AccountRepository& accountRepo)
    : BaseClass(depositRepo), accountRepo_(accountRepo)
    {}

    Model::EntityID
    DepositServiceImpl::createCurrentDeposit(const Model::EntityID& ownerId, double initialBalance,
                                                             double overDraftLimit) {
        using namespace Model;
        auto owner = accountRepo_.resolveEntity(ownerId);

        if(!owner){
            throw std::domain_error((boost::format(Exceptions::unknownAccount) % boost::uuids::to_string(ownerId)).str());
        }

        return create<Model::CurrentDeposit>(generator_(), overDraftLimit, initialBalance, *owner);
    }

    Model::EntityID
    DepositServiceImpl::createSavingsDeposit(const Model::EntityID& ownerId, double initialBalance, double minBalance,
                                             double rateOfInterest) {
        using namespace Model;

        auto owner = accountRepo_.resolveEntity(ownerId);

        if(!owner){
            throw std::domain_error((boost::format(Exceptions::unknownAccount) % boost::uuids::to_string(ownerId)).str());
        }

        return create<Model::SavingsDeposit>(generator_(), minBalance, initialBalance, rateOfInterest, *owner);
    }

    Model::EntityID
    DepositServiceImpl::createFixedDeposit(const Model::EntityID& ownerId, double initialBalance, double rateOfInterest) {
        using namespace Model;

        auto owner = accountRepo_.resolveEntity(ownerId);
        if(!owner){
            throw std::domain_error((boost::format(Exceptions::unknownAccount) % boost::uuids::to_string(ownerId)).str());
        }

        return create<Model::FixedDeposit>(generator_(), *owner, initialBalance, rateOfInterest);
    }

    boost::optional<const Model::SavingsDeposit&> DepositServiceImpl::getSavingsDeposit(const Model::EntityID& depositId) const {
        return constCast(repository_.findSavingsDeposit(depositId));
    }

    boost::optional<const Model::CurrentDeposit&> DepositServiceImpl::getCurrentDeposit(
            const Model::EntityID &depositId) const {
        return constCast(repository_.findCurrentDeposit(depositId));
    }

    boost::optional<const Model::FixedDeposit&> DepositServiceImpl::getFixedDeposit(
            const Model::EntityID &depositId) const {
        return constCast(repository_.findFixedDeposit(depositId));
    }

    void DepositServiceImpl::refill(const Model::EntityID& depositID, double amount) {
        using namespace Model;

        if(amount <= 0) {
            throw std::domain_error(Model::Exceptions::negativeValue);
        }

        auto deposit = repository_.resolveEntity(depositID);

        if(!deposit) {
            throw std::domain_error((boost::format(Exceptions::unknownDeposit) % boost::uuids::to_string(depositID)).str());
        }

        if(Model::DepositCast<FixedDeposit>().cast(*deposit)){
            throw std::domain_error(Exceptions::wrongFixedDepositOperation);
        }

        deposit->refill(amount);
    }

    double DepositServiceImpl::getRateOfInterest(const Model::EntityID& depositId) const {
        auto deposit = repository_.resolveEntity(depositId);

        if(!deposit) {
            throw std::domain_error((boost::format(Model::Exceptions::unknownDeposit) % boost::uuids::to_string(depositId)).str());
        }

        return deposit->getRateOfInterest();
    }

    double DepositServiceImpl::getBalance(const Model::EntityID& depositId) const {
        auto deposit = repository_.resolveEntity(depositId);

        if(!deposit) {
            throw std::domain_error((boost::format(Model::Exceptions::unknownDeposit) % boost::uuids::to_string(depositId)).str());
        }

        return deposit->getBalance();
    }

    void DepositServiceImpl::withdraw(const Model::EntityID& depositId, double amount) {
        if(amount <= 0) {
            throw std::domain_error(Model::Exceptions::negativeValue);
        }

        auto deposit = repository_.resolveEntity(depositId);

        if(!deposit) {
            throw std::domain_error((boost::format(Model::Exceptions::unknownDeposit) % boost::uuids::to_string(depositId)).str());
        }

        Withdrawer::withdraw(*deposit, amount);
    }

    void DepositServiceImpl::setOverdraftLimit(const Model::EntityID& depositId, double overdraftLimit) {
        using namespace Model;

        auto deposit = repository_.resolveEntity(depositId);

        if(!deposit) {
            throw std::domain_error((boost::format(Exceptions::unknownDeposit) % boost::uuids::to_string(depositId)).str());
        }

        auto currentDeposit = Model::DepositCast<CurrentDeposit>().cast(*deposit);

        if(!currentDeposit){
            throw std::domain_error((boost::format(Exceptions::depositExpected) % "Current").str());
        }

        currentDeposit->setOverdraftLimit(overdraftLimit);
    }

    double DepositServiceImpl::closeDeposit(const Model::EntityID& depositId) {
        auto deposit = repository_.resolveEntity(depositId);

        if(!deposit) {
            throw std::domain_error((boost::format(Model::Exceptions::unknownDeposit) % boost::uuids::to_string(depositId)).str());
        }

        double balance = deposit->getBalance();
        repository_.remove(depositId);

        return balance;
    }

    void DepositServiceImpl::onBankPeriodPassed(const Model::EntityID& depositId) {
        auto deposit = repository_.resolveEntity(depositId);

        if(!deposit) {
            throw std::domain_error((boost::format(Model::Exceptions::unknownDeposit) % boost::uuids::to_string(depositId)).str());
        }

        YearProcessor::process(*deposit);
    }

    int DepositServiceImpl::getDepositsAmount() const {
        return repository_.getSize();
    }
}
}