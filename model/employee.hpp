#ifndef KANGARU_DI_SAMPLE_EMPLOYEE_HPP
#define KANGARU_DI_SAMPLE_EMPLOYEE_HPP

#include "account.hpp"

namespace Model {
    class Employee : public Model::Account {
        public:
            Employee(std::string name, std::string email, std::string passwordHash, Model::EntityID id, Position accessLevel)
                : Account(std::move(name), std::move(email), std::move(passwordHash), id), position_(accessLevel) {}

            Position getPosition() const {
                return position_;
            }

            void promote(Position accessLevel) {
                position_ = accessLevel;
            }

            void accept(AccountVisitor& visitor) override;

        private:
            Position position_;
    };
}

#endif //KANGARU_DI_SAMPLE_EMPLOYEE_HPP
