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

            const Model::EntityID& createAccount(std::string name, std::string email, std::string passwordHash) final;

            const Model::EntityID& createEmployeeAccount(
                    std::string name, std::string email, std::string passwordHash, Model::Position accessLevel) final;

            boost::optional<const Model::Account&> getAccount(const Model::EntityID& entityId) const final;

            boost::optional<const Model::Employee&> getEmployee(const Model::EntityID& entityId) const final;

            void changeEmail(const Model::EntityID& entityId, std::string newEmail) final;

            void changePassword(const Model::EntityID& entityId, std::string newPassword) final;

            void promote(const Model::EntityID& entityId, Model::Position newAccessLevel) final;

            int getAccountsAmount() const final;

            void deleteAccount(const Model::EntityID& entityId) final;
    };

}
}

#endif //KANGARU_DI_SAMPLE_ACCOUNT_SERVICE_IMPL_HPP
