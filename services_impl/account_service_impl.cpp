#include "account_service_impl.hpp"
#include "model/employee.hpp"
#include "repositories/account_repository.hpp"

namespace Service {
namespace Impl {

    Model::EntityID AccountServiceImpl::createAccount(std::string name, std::string email, std::string passwordHash) {
        return create<Model::Account>(std::move(name), std::move(email), std::move(passwordHash), generator_());
    }

    void AccountServiceImpl::changeEmail(Model::EntityID entityId, std::string newEmail) {
        auto entity = repository_.resolveEntity(entityId);
        if(entity){
            entity->setEmail(std::move(newEmail));
        }
    }

    void AccountServiceImpl::changePassword(Model::EntityID entityId, std::string newPassword) {
        auto entity = repository_.resolveEntity(entityId);
        if(entity) {
            entity->setPassword(std::move(newPassword));
        }
    }

    void AccountServiceImpl::promote(Model::EntityID entityId, Model::Position newAccessLevel) {
        auto employee = repository_.findEmployee(entityId);
        if(employee) {
            employee->promote(newAccessLevel);
        }
    }

    void AccountServiceImpl::deleteAccount(Model::EntityID entityId) {
        remove(entityId);
    }

    Model::EntityID
    AccountServiceImpl::createEmployeeAccount(std::string name, std::string email, std::string passwordHash,
                                              Model::Position accessLevel) {
        return create<Model::Employee>(std::move(name), std::move(email), std::move(passwordHash), generator_(), accessLevel);
    }

}
}