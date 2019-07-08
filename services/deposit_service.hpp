#ifndef KANGARU_DI_SAMPLE_DEPOSIT_SERVICE_HPP
#define KANGARU_DI_SAMPLE_DEPOSIT_SERVICE_HPP

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

#include "model/typedefs.hpp"

namespace Model {
    class SavingsDeposit;
    class CurrentDeposit;
    class FixedDeposit;
}

namespace Service {
    struct DepositService : public boost::noncopyable {

            virtual ~DepositService() noexcept = default;

            virtual Model::EntityID createSavingsDeposit(
                    const Model::EntityID& ownerId, double initialBalance, double minBalance, double rateOfInterest) = 0;

            virtual Model::EntityID createCurrentDeposit(
                    const Model::EntityID& ownerId, double initialBalance,
                    double overDraftLimit) = 0;

            virtual Model::EntityID createFixedDeposit(
                    const Model::EntityID& ownerId, double initialBalance, double rateOfInterest) = 0;

            virtual boost::optional<const Model::SavingsDeposit&> getSavingsDeposit(const Model::EntityID& depositId) const = 0;

            virtual boost::optional<const Model::CurrentDeposit&> getCurrentDeposit(const Model::EntityID& depositId) const = 0;

            virtual boost::optional<const Model::FixedDeposit&> getFixedDeposit(const Model::EntityID& depositId) const = 0;

            virtual void refill(const Model::EntityID& depositId, double amount) = 0;

            virtual double getRateOfInterest(const Model::EntityID& depositId) const = 0;

            virtual double getBalance(const Model::EntityID& depositId) const = 0;

            virtual void withdraw(const Model::EntityID& depositId, double amount) = 0;

            virtual void setOverdraftLimit(const Model::EntityID& depositId, double overdraftLimit) = 0;

            virtual void onBankPeriodPassed(const Model::EntityID& depositID) = 0;

            virtual int getDepositsAmount() const = 0;

            virtual double closeDeposit(const Model::EntityID& depositId) = 0;
    };
}

#endif //KANGARU_DI_SAMPLE_DEPOSIT_SERVICE_HPP
