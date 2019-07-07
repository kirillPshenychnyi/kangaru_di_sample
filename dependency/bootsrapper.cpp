#include "bootsrapper.hpp"

#include "local_repositories_impl/account_repository_impl.hpp"
#include "local_repositories_impl/deposit_repository_impl.hpp"

#include "services_impl/account_service_impl.hpp"
#include "services_impl/deposit_service_impl.hpp"

namespace Dependency {

    struct AccountRepositoryService : kgr::abstract_service<Repository::AccountRepository> {};
    struct LocalAccountRepositoryService : kgr::single_service<Repository::LocalRepo::AccountRepositoryImpl>, kgr::overrides<AccountRepositoryService> {};

    struct DepositRepositoryService : kgr::abstract_service<Repository::DepositRepository> {};
    struct LocalDepositRepositoryService : kgr::single_service<Repository::LocalRepo::DepositRepositoryImpl>, kgr::overrides<DepositRepositoryService> {};

    struct AccountServiceService : kgr::abstract_service<Service::AccountService> {};
    struct LocalAccountServiceService : kgr::single_service<Service::Impl::AccountServiceImpl, kgr::dependency<AccountRepositoryService>>, kgr::overrides<AccountServiceService> {};

    struct DepositServiceService : kgr::abstract_service<Service::DepositService> {};
    struct LocalDepositServiceService :
            kgr::single_service<Service::Impl::DepositServiceImpl, kgr::dependency<DepositRepositoryService, AccountRepositoryService>>, kgr::overrides<DepositServiceService> {};

    Bootstpapper& Bootstpapper::getInstance() {
        static Bootstpapper s_instnace;
        return s_instnace;
    }

    Bootstpapper::Bootstpapper() {
        initRepos();
        initServices();
    }

    void Bootstpapper::initRepos() {
        container_.emplace<LocalAccountRepositoryService>();
        container_.emplace<LocalDepositRepositoryService>();
    }

    void Bootstpapper::initServices() {
        container_.emplace<LocalAccountServiceService>();
        container_.emplace<LocalDepositServiceService>();
    }
}