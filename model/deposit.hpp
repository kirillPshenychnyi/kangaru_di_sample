#ifndef KANGARU_DI_SAMPLE_DEPOSIT_HPP
#define KANGARU_DI_SAMPLE_DEPOSIT_HPP

#include "entity.hpp"

namespace Model {
    class Account;
    class DepositVisitor;

    class Deposit : public Entity {
        public:
            Deposit(const Model::EntityID& id, const Account& owner, double initialBalance, double rateOfInterest)
            : Entity(id), owner_(owner), rateOfInterest_(rateOfInterest), balance_(initialBalance) {}

            const Account& getAccount() const { return owner_; }

            void refill(double amount) {
                balance_ += amount;
            }

            void withdraw(double amount) {
                balance_ -= amount;
            }

            double getBalance() const {
                return balance_; }

            double getRateOfInterest() const {
                return rateOfInterest_;
            }

            virtual void accept(DepositVisitor& visitor) = 0;

        private:
            const Account& owner_;
            const double rateOfInterest_;
            double balance_;
    };
}

#endif //KANGARU_DI_SAMPLE_DEPOSIT_HPP
