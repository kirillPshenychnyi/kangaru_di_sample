#ifndef KANGARU_DI_SAMPLE_ACCOUNT_VISITOR_HPP
#define KANGARU_DI_SAMPLE_ACCOUNT_VISITOR_HPP

#include "model/utils/generic_visitor.hpp"
#include "model/utils/default_visitor.hpp"
#include "model/utils/cast_visitor.hpp"

namespace Model {
    class Account;
    class Employee;

    DECLARE_VISITOR(Account, Account, Employee)
    DECLARE_DEFAULT_VISITOR(Account, Account, Employee)
    DEFINE_CAST(Account)
}

#endif //KANGARU_DI_SAMPLE_ACCOUNT_VISITOR_HPP
