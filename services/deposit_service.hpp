#ifndef KANGARU_DI_SAMPLE_DEPOSIT_SERVICE_HPP
#define KANGARU_DI_SAMPLE_DEPOSIT_SERVICE_HPP

#include <boost/noncopyable.hpp>

#include "typedefs.hpp"

namespace Service {
    class DepositService : public boost::noncopyable {
            virtual Model::EntityID createSavingsDeposit(
                    Model::EntityID ownerId, double initialBalance, double minBalance, double rateOfInterest) const = 0;

            virtual Model::EntityID createCurrentDeposit(
                    Model::EntityID ownerId, double initialBalance, double minBalance,
                    double rateOfInterest, double overDraftLimit) = 0;

            virtual Model::EntityID createFixedDeposit(
                    Model::EntityID ownerId, double minBalance, double rateOfInterest) = 0;

            virtual void refill(Model::EntityID ownerId, double amount) = 0;

            virtual double getRateOfInterest(Model::EntityID ownerId) const = 0;

            virtual double getBalance(Model::EntityID ownerId) const = 0;

            virtual void withdraw(Model::EntityID ownerId) const = 0;

            virtual void setOverdraftLimit(Model::EntityID ownerId, double overdraftLimit) const = 0;

            virtual double closeDeposit(Model::EntityID ownerId) = 0;
    };
}

#endif //KANGARU_DI_SAMPLE_DEPOSIT_SERVICE_HPP
