#ifndef KANGARU_DI_SAMPLE_FIXED_DEPOSIT_HPP
#define KANGARU_DI_SAMPLE_FIXED_DEPOSIT_HPP

#include "deposit.hpp"
#include "visitors/deposit_visitor.hpp"

namespace Model {
    class FixedDeposit : public Deposit {
        public:
            using Deposit::Deposit;

            void accept(DepositVisitor& visitor) final {
                visitor.visit(*this);
            }
    };
}

#endif //KANGARU_DI_SAMPLE_FIXED_DEPOSIT_HPP
