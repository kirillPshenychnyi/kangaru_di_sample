#ifndef KANGARU_DI_SAMPLE_ACCOUNT_SERVICE_HPP
#define KANGARU_DI_SAMPLE_ACCOUNT_SERVICE_HPP

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <string>

#include "model/typedefs.hpp"

namespace Model {
    struct Account;
    struct Employee;
}

namespace Service {
    struct AccountService : public boost::noncopyable {
        virtual ~AccountService() noexcept = default;

        virtual Model::EntityID createAccount(std::string name, std::string email, std::string passwordHash) = 0;

        virtual Model::EntityID createEmployeeAccount(
                std::string name, std::string email, std::string passwordHash, Model::Position accessLevel) = 0;

        virtual boost::optional<const Model::Account&> getAccount(Model::EntityID entityId) const = 0;

        virtual boost::optional<const Model::Employee&> getEmployee(Model::EntityID entityId) const = 0;

        virtual void changeEmail(Model::EntityID entityId, std::string newEmail) = 0;

        virtual void changePassword(Model::EntityID entityId, std::string newPassword) = 0;

        virtual void promote(Model::EntityID entityId, Model::Position newAccessLevel) = 0;

        virtual void deleteAccount(Model::EntityID entityId) = 0;

        virtual int getAccountsSize() const = 0;
    };
}

#endif //KANGARU_DI_SAMPLE_ACCOUNT_SERVICE_HPP
