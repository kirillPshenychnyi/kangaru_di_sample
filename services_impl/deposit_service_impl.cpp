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

            savings.withdraw(currentBalance);
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

    DepositServiceImpl::DepositServiceImpl(
            Repository::DepositRepository& depositRepo, Repository::AccountRepository& accountRepo)
    : BaseClass(depositRepo), accountRepo_(accountRepo)
    {}

    Model::EntityID DepositServiceImpl::createCurrentDeposit(Model::EntityID ownerId, double initialBalance,
                                                             double rateOfInterest,
                                                             double overDraftLimit) {
        using namespace Model;
        auto owner = accountRepo_.resolveEntity(ownerId);

        if(!owner){
            throw std::domain_error((boost::format(Exceptions::unknownAccount) % boost::uuids::to_string(ownerId)).str());
        }

        return create<Model::CurrentDeposit>(generator_(), overDraftLimit, initialBalance, rateOfInterest, *owner);
    }

    Model::EntityID
    DepositServiceImpl::createSavingsDeposit(Model::EntityID ownerId, double initialBalance, double minBalance,
                                             double rateOfInterest) {
        using namespace Model;

        auto owner = accountRepo_.resolveEntity(ownerId);

        if(!owner){
            throw std::domain_error((boost::format(Exceptions::unknownAccount) % boost::uuids::to_string(ownerId)).str());
        }

        return create<Model::SavingsDeposit>(generator_(), minBalance, initialBalance, rateOfInterest, *owner);
    }

    Model::EntityID
    DepositServiceImpl::createFixedDeposit(Model::EntityID ownerId, double initialBalance, double rateOfInterest) {
        using namespace Model;

        auto owner = accountRepo_.resolveEntity(ownerId);
        if(!owner){
            throw std::domain_error((boost::format(Exceptions::unknownAccount) % boost::uuids::to_string(ownerId)).str());
        }

        return create<Model::FixedDeposit>(generator_(), *owner, initialBalance, rateOfInterest);
    }

    void DepositServiceImpl::refill(Model::EntityID depositID, double amount) {
        using namespace Model;

        auto deposit = repository_.resolveEntity(depositID);

        if(!deposit) {
            throw std::domain_error((boost::format(Exceptions::unknownDeposit) % boost::uuids::to_string(depositID)).str());
        }

        if(Model::DepositCast<CurrentDeposit>().cast(*deposit)){
            throw std::domain_error(Exceptions::wrongFixedDepositOperation);
        }

        deposit->refill(amount);
    }

    double DepositServiceImpl::getRateOfInterest(Model::EntityID depositID) const {
        auto deposit = repository_.resolveEntity(depositID);

        if(!deposit) {
            throw std::domain_error((boost::format(Model::Exceptions::unknownDeposit) % boost::uuids::to_string(depositID)).str());
        }

        return deposit->getRateOfInterest();
    }

    double DepositServiceImpl::getBalance(Model::EntityID depositID) const {
        auto deposit = repository_.resolveEntity(depositID);

        if(!deposit) {
            throw std::domain_error((boost::format(Model::Exceptions::unknownDeposit) % boost::uuids::to_string(depositID)).str());
        }

        return deposit->getBalance();
    }

    void DepositServiceImpl::withdraw(Model::EntityID depositID, double amount) {
        auto deposit = repository_.resolveEntity(depositID);

        if(!deposit) {
            throw std::domain_error((boost::format(Model::Exceptions::unknownDeposit) % boost::uuids::to_string(depositID)).str());
        }

        Withdrawer::withdraw(*deposit, amount);
    }

    void DepositServiceImpl::setOverdraftLimit(Model::EntityID depositID, double overdraftLimit) {
        using namespace Model;

        auto deposit = repository_.resolveEntity(depositID);

        if(!deposit) {
            throw std::domain_error((boost::format(Exceptions::unknownDeposit) % boost::uuids::to_string(depositID)).str());
        }

        auto currentDeposit = Model::DepositCast<CurrentDeposit>().cast(*deposit);

        if(!currentDeposit){
            throw std::domain_error((boost::format(Exceptions::depositExpected) % "Current").str());
        }

        currentDeposit->setOverdraftLimit(overdraftLimit);
    }

    double DepositServiceImpl::closeDeposit(Model::EntityID depositID) {
        return 0;
    }

}
}