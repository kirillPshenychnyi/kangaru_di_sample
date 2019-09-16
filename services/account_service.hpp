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

        virtual const Model::EntityID& createAccount(const std::string& name, const std::string& email, const std::string& passwordHash) = 0;

        virtual const Model::EntityID& createEmployeeAccount(
                const std::string& name, const std::string& email, const std::string& passwordHash, Model::Position accessLevel) = 0;

        virtual boost::optional<const Model::Account&> getAccount(const Model::EntityID& entityId) const = 0;

        virtual boost::optional<const Model::Employee&> getEmployee(const Model::EntityID& entityId) const = 0;

        virtual void changeEmail(const Model::EntityID& entityId, const std::string& newEmail) = 0;

        virtual void changePassword(const Model::EntityID& entityId, const std::string& newPassword) = 0;

        virtual void promote(const Model::EntityID& entityId, Model::Position newAccessLevel) = 0;

        virtual void deleteAccount(const Model::EntityID& entityId) = 0;

        virtual int getAccountsAmount() const = 0;
    };
}

#endif //KANGARU_DI_SAMPLE_ACCOUNT_SERVICE_HPP
