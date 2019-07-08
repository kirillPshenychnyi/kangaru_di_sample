#ifndef KANGARU_DI_SAMPLE_ACCOUNT_SERVICE_IMPL_HPP
#define KANGARU_DI_SAMPLE_ACCOUNT_SERVICE_IMPL_HPP

#include "services/account_service.hpp"
#include "base_service_impl.hpp"

namespace Repository {
    struct AccountRepository;
}

namespace Service {
namespace Impl {

    class AccountServiceImpl : public BaseServiceImpl<Repository::AccountRepository, AccountService> {

            using BaseClass = BaseServiceImpl<Repository::AccountRepository, AccountService>;

        public:
            using BaseClass::BaseServiceImpl;

            Model::EntityID createAccount(std::string name, std::string email, std::string passwordHash) final;

            Model::EntityID createEmployeeAccount(
                    std::string name, std::string email, std::string passwordHash, Model::Position accessLevel) final;

            boost::optional<const Model::Account&> getAccount(Model::EntityID entityId) const final;

            boost::optional<const Model::Employee&> getEmployee(Model::EntityID entityId) const final;

            void changeEmail(Model::EntityID entityId, std::string newEmail) final;

            void changePassword(Model::EntityID entityId, std::string newPassword) final;

            void promote(Model::EntityID entityId, Model::Position newAccessLevel) final;

            int getAccountsSize() const final;

            void deleteAccount(Model::EntityID entityId) final;
    };

}
}

#endif //KANGARU_DI_SAMPLE_ACCOUNT_SERVICE_IMPL_HPP
