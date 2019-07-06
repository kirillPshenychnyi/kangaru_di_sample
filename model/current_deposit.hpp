#ifndef KANGARU_DI_SAMPLE_CURRENT_DEPOSIT_HPP
#define KANGARU_DI_SAMPLE_CURRENT_DEPOSIT_HPP

#include "deposit.hpp"
#include "visitors/deposit_visitor.hpp"

namespace Model {
    class CurrentDeposit : public Deposit {
        public:
            CurrentDeposit(Model::EntityID id, double overdraftLimit, double initialBalance, double rateOfInterest, const Account& owner)
            : Deposit(id, owner, initialBalance, rateOfInterest), overdraftLimit_(overdraftLimit) {}

        public:
            double getOverdraftLimit() const {
                return overdraftLimit_;
            }

            double setOverdraftLimit(double overdraftLimit) {
                overdraftLimit_ = overdraftLimit;
            }

            void accept(DepositVisitor& visitor) final {
                visitor.visit(*this);
            }

        private:
            double overdraftLimit_;
    };
}

#endif //KANGARU_DI_SAMPLE_CURRENT_DEPOSIT_HPP
