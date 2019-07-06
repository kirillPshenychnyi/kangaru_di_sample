#ifndef KANGARU_DI_SAMPLE_ACCOUNT_HPP
#define KANGARU_DI_SAMPLE_ACCOUNT_HPP

#include <string>
#include "entity.hpp"

namespace Model {

    struct AccountVisitor;

    class Account : public Entity {
        public:
            Account(std::string name, std::string email, std::string passwordHash, Model::EntityID id) :
                Entity(id), name_(std::move(name)), email_(std::move(email)), passwordHash_(std::move(passwordHash)) {}

            const std::string& getName() const {
                return name_;
            }

            const std::string& getEmail() const {
                return email_;
            }

            const std::string& getPasswordHash() const {
                return passwordHash_;
            }

            void setPassword(std::string passwordHash) {
                passwordHash_ = std::move(passwordHash);
            }

            void setEmail(std::string passwordHash) {
                email_ = std::move(passwordHash);
            }

            virtual void accept(AccountVisitor& visitor);

        private:
            const std::string name_;
            std::string email_;
            std::string passwordHash_;
    };
}

#endif //KANGARU_DI_SAMPLE_ACCOUNT_HPP
