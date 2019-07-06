#include "model/account.hpp"
#include "model/visitors/account_visitor.hpp"

namespace Model {

    void Account::accept(AccountVisitor& visitor) {
        visitor.visit(*this);
    }
}