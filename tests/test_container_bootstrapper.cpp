#include "test_container_bootstrapper.hpp"

#include "local_repositories_impl/account_repository_impl.hpp"
#include "local_repositories_impl/deposit_repository_impl.hpp"

#include "services_impl/account_service_impl.hpp"
#include "services_impl/deposit_service_impl.hpp"

namespace DP = Dependency;

struct LocalAccountRepositoryService :
        kgr::single_service<Repository::LocalRepo::AccountRepositoryImpl>,
        kgr::overrides<DP::AccountRepositoryService> {};

struct LocalDepositRepositoryService :
        kgr::single_service<Repository::LocalRepo::DepositRepositoryImpl>,
        kgr::overrides<DP::DepositRepositoryService> {};

struct LocalAccountServiceService :
        kgr::single_service<Service::Impl::AccountServiceImpl, kgr::dependency<DP::AccountRepositoryService>>,
        kgr::overrides<DP::AccountServiceService> {};

struct LocalDepositServiceService :
        kgr::single_service<Service::Impl::DepositServiceImpl, kgr::dependency<DP::DepositRepositoryService, DP::AccountRepositoryService>>,
        kgr::overrides<DP::DepositServiceService> {};

namespace Test {
    TestContainerBootstrapper::TestContainerBootstrapper() {
        // order matters
        initRepos();
        initServices();
    }

    TestContainerBootstrapper TestContainerBootstrapper::newInstance() {
        return {};
    }

    void TestContainerBootstrapper::initRepos() {
        container_.emplace<LocalAccountRepositoryService>();
        container_.emplace<LocalDepositRepositoryService>();
    }

    void TestContainerBootstrapper::initServices() {
        container_.emplace<LocalAccountServiceService>();
        container_.emplace<LocalDepositServiceService>();
    }
}