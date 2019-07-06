#ifndef KANGARU_DI_SAMPLE_DEPOSIT_SERVICE_HPP
#define KANGARU_DI_SAMPLE_DEPOSIT_SERVICE_HPP

#include <boost/noncopyable.hpp>

#include "model/typedefs.hpp"

namespace Service {
    struct DepositService : public boost::noncopyable {

            virtual ~DepositService() noexcept = default;

            virtual Model::EntityID createSavingsDeposit(
                    Model::EntityID depositId, double initialBalance, double minBalance, double rateOfInterest) = 0;

            virtual Model::EntityID createCurrentDeposit(
                    Model::EntityID depositId, double initialBalance,
                    double rateOfInterest, double overDraftLimit) = 0;

            virtual Model::EntityID createFixedDeposit(
                    Model::EntityID depositId, double initialBalance, double rateOfInterest) = 0;

            virtual void refill(Model::EntityID depositId, double amount) = 0;

            virtual double getRateOfInterest(Model::EntityID depositId) const = 0;

            virtual double getBalance(Model::EntityID depositId) const = 0;

            virtual void withdraw(Model::EntityID depositId, double amount) = 0;

            virtual void setOverdraftLimit(Model::EntityID depositId, double overdraftLimit) = 0;

            virtual double closeDeposit(Model::EntityID depositId) = 0;
    };
}

#endif //KANGARU_DI_SAMPLE_DEPOSIT_SERVICE_HPP
