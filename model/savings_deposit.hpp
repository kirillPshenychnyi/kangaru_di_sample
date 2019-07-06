#ifndef KANGARU_DI_SAMPLE_SAVINGS_DEPOSIT_HPP
#define KANGARU_DI_SAMPLE_SAVINGS_DEPOSIT_HPP

#include "deposit.hpp"
#include "visitors/deposit_visitor.hpp"

namespace Model {
    class SavingsDeposit : public Deposit {
        public:
            SavingsDeposit(Model::EntityID id, double minBalance, double initialBalance, double rateOfInterest, const Account& owner)
                : Deposit(id, owner, initialBalance, rateOfInterest), minBalance_(minBalance) {}

            double getMinBalance() const { return minBalance_; }

            void accept(DepositVisitor& visitor) final {
                visitor.visit(*this);
            }

        private:
            const double minBalance_;
    };
}

#endif //KANGARU_DI_SAMPLE_SAVINGS_DEPOSIT_HPP
