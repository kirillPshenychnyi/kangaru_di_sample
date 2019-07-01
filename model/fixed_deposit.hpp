#ifndef KANGARU_DI_SAMPLE_FIXED_DEPOSIT_HPP
#define KANGARU_DI_SAMPLE_FIXED_DEPOSIT_HPP

#include "deposit.hpp"

namespace Model {
    class FixedDeposit : public Deposit {
        public:
            FixedDeposit(Model::EntityID id, double initialBalance, double rateOfInterest, const Account& owner)
            : Deposit(id, owner, initialBalance, rateOfInterest) {}
    };
}

#endif //KANGARU_DI_SAMPLE_FIXED_DEPOSIT_HPP
