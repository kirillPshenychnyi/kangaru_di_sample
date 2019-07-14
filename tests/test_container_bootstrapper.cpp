#include "test_container_bootstrapper.hpp"

#include "local_repositories_impl/deposit_repository_impl.hpp"
#include "local_repositories_impl/account_repository_impl.hpp"

#include "third_party/boost_di.hpp"

#include "services_impl/account_service_impl.hpp"
#include "services_impl/deposit_service_impl.hpp"

#include "repositories/deposit_repository.hpp"
#include "repositories/account_repository.hpp"

namespace Test {
    TestContainerBootstrapper::TestContainerBootstrapper(
            Service::AccountService& accountService,
            Service::DepositService& depositService,
            Repository::DepositRepository& depositRepository,
            Repository::AccountRepository& accountRepository) :
                accountService_(accountService), depositService_(depositService),
                depositRepository_(depositRepository), accountRepository_(accountRepository) {
    }

    TestContainerBootstrapper::~TestContainerBootstrapper() {
        accountRepository_.clear();
        depositRepository_.clear();
    }

    TestContainerBootstrapper TestContainerBootstrapper::newInstance() {
        using namespace boost;
        auto injector = di::make_injector(
                di::bind<Repository::AccountRepository>.to<Repository::LocalRepo::AccountRepositoryImpl>(),
                di::bind<Repository::DepositRepository>.to<Repository::LocalRepo::DepositRepositoryImpl>(),
                di::bind<Service::AccountService>.to<Service::Impl::AccountServiceImpl>(),
                di::bind<Service::DepositService>.to<Service::Impl::DepositServiceImpl>());

        return injector.create<TestContainerBootstrapper>();
    }

    Service::DepositService &TestContainerBootstrapper::getDepositService() {
        return depositService_;
    }

    Service::AccountService &TestContainerBootstrapper::getAccountService() {
        return accountService_;
    }
}
