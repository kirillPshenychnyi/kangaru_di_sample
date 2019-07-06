
#include "employee.hpp"
#include "visitors/account_visitor.hpp"

namespace Model {

    void Employee::accept(AccountVisitor &visitor) {
        visitor.visit(*this);
    }
}