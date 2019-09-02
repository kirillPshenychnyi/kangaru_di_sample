#include "account_service_impl.hpp"

#include "model/employee.hpp"
#include "model/exceptions_messages.hpp"

#include "repositories/account_repository.hpp"

#include <boost/format.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace Service {
namespace Impl {

    const Model::EntityID& AccountServiceImpl::createAccount(std::string name, std::string email, std::string passwordHash) {
        if(name.empty() || email.empty() || passwordHash.empty()){
            throw std::domain_error(Model::Exceptions::wrongStringParamValue);
        }

        return create<Model::Account>(std::move(name), std::move(email), std::move(passwordHash), generator_());
    }

    void AccountServiceImpl::changeEmail(const Model::EntityID& entityId, std::string newEmail) {
        if(newEmail.empty()){
            throw std::domain_error(Model::Exceptions::wrongStringParamValue);
        }

        auto entity = repository_.resolveEntity(entityId);

        if(!entity){
            throw std::domain_error((boost::format(Model::Exceptions::unknownAccount) % boost::uuids::to_string(entityId)).str());
        }

        entity->setEmail(std::move(newEmail));
    }

    void AccountServiceImpl::changePassword(const Model::EntityID& entityId, std::string newPassword) {
        if(newPassword.empty()){
            throw std::domain_error(Model::Exceptions::wrongStringParamValue);
        }

        auto entity = repository_.resolveEntity(entityId);
        if(!entity){
            throw std::domain_error((boost::format(Model::Exceptions::unknownAccount) % boost::uuids::to_string(entityId)).str());
        }

        entity->setPassword(std::move(newPassword));
    }

    void AccountServiceImpl::promote(const Model::EntityID& entityId, Model::Position newAccessLevel) {
        using namespace Model;

        auto employee = repository_.findEmployee(entityId);

        if(!employee){
            throw std::domain_error((boost::format(Exceptions::unknownAccount) % boost::uuids::to_string(entityId)).str());
        }

        employee->promote(newAccessLevel);
    }

    boost::optional<const Model::Account&> AccountServiceImpl::getAccount(const Model::EntityID& entityId) const {
        return static_cast<const Repository::AccountRepository&>(repository_).resolveEntity(entityId);
    }

    boost::optional<const Model::Employee&> AccountServiceImpl::getEmployee(const Model::EntityID& entityId) const {
        return repository_.getEmployee(entityId);
    }

    int AccountServiceImpl::getAccountsAmount() const {
        return repository_.getSize();
    }

    void AccountServiceImpl::deleteAccount(const Model::EntityID& entityId) {
        remove(entityId);
    }

    const Model::EntityID&
    AccountServiceImpl::createEmployeeAccount(std::string name, std::string email, std::string passwordHash,
                                              Model::Position accessLevel) {
        if(name.empty() || email.empty() || passwordHash.empty()){
            throw std::domain_error(Model::Exceptions::wrongStringParamValue);
        }

        return create<Model::Employee>(std::move(name), std::move(email), std::move(passwordHash), generator_(), accessLevel);
    }

}
}