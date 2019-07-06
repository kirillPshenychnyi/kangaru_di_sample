#ifndef KANGARU_DI_SAMPLE_DEPOSIT_VISITOR_HPP
#define KANGARU_DI_SAMPLE_DEPOSIT_VISITOR_HPP

#include "model/utils/generic_visitor.hpp"
#include "model/utils/default_visitor.hpp"
#include "model/utils/cast_visitor.hpp"

#include "model/employee.hpp"

namespace Model {
    class SavingsDeposit;
    class FixedDeposit;
    class CurrentDeposit;

    DECLARE_VISITOR(Deposit, SavingsDeposit, FixedDeposit, CurrentDeposit)
    DECLARE_DEFAULT_VISITOR(Deposit, SavingsDeposit, FixedDeposit, CurrentDeposit)
    DEFINE_CAST(Deposit)
}

#endif //KANGARU_DI_SAMPLE_DEPOSIT_VISITOR_HPP
