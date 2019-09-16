#include <third_party/catch.hpp>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <boost/format.hpp>

#include "test_container_bootstrapper.hpp"

#include "services/account_service.hpp"
#include "services/deposit_service.hpp"

#include "model/employee.hpp"
#include "model/exceptions_messages.hpp"

namespace Test {

    TEST_CASE("accounts_are_empty", "[accounts]") {
        TestContainerBootstrapper bootstrapper{TestContainerBootstrapper::newInstance()};
        REQUIRE(bootstrapper.getAccountService().getAccountsAmount() == 0);
    }

    TEST_CASE("create_accounts", "[accounts]") {
        TestContainerBootstrapper bootstrapper{TestContainerBootstrapper::newInstance()};

        Service::AccountService& accountService = bootstrapper.getAccountService();

        SECTION("simple_account") {
            const std::string name {"John Dow"};
            const std::string email {"dow@gmail.com"};
            const std::string password {"37aAsddkg9822"};

            Model::EntityID id = accountService.createAccount(name, email, password);
            REQUIRE(bootstrapper.getAccountService().getAccountsAmount() == 1);

            auto account = accountService.getAccount(id);

            REQUIRE(account.is_initialized());
            REQUIRE(account->getName() == name);
            REQUIRE(account->getEmail() == email);
            REQUIRE(account->getPasswordHash() == password);
        }

        SECTION("employee_account") {
            const std::string name {"Sam Rio"};
            const std::string email {"rio@gmail.com"};
            const std::string password {"37aAsddkg9822"};

            Model::EntityID id = accountService.createEmployeeAccount(name, email, password, Model::Position::Operator);

            auto account = accountService.getEmployee(id);

            REQUIRE(account.is_initialized());
            REQUIRE(account->getName() == name);
            REQUIRE(account->getEmail() == email);
            REQUIRE(account->getPasswordHash() == password);
            REQUIRE(account->getPosition() == Model::Position::Operator);
        }
    }

    TEST_CASE("change_params", "[accounts]") {
        const std::string name {"John Dow"};
        const std::string email {"dow@gmail.com"};
        const std::string password {"37aAsddkg9822"};

        TestContainerBootstrapper bootstrapper{TestContainerBootstrapper::newInstance()};

        Service::AccountService& accountService = bootstrapper.getAccountService();

        Model::EntityID id = accountService.createEmployeeAccount(name, email, password, Model::Position::Operator);
        REQUIRE(bootstrapper.getAccountService().getAccountsAmount() == 1);

        auto account = accountService.getEmployee(id);

        REQUIRE(account.is_initialized());

        SECTION("change_email") {
            const std::string newEmail{ "johnDow@yandex.ua" };
            accountService.changeEmail(id, newEmail);
            REQUIRE(account->getEmail() == newEmail);
        }

        SECTION("change_password") {
            const std::string newPassword{ "johnDow@yandex.ua" };
            accountService.changePassword(id, newPassword);
            REQUIRE(account->getPasswordHash() == newPassword);
        }

        SECTION("promote") {
            accountService.promote(id, Model::Position::DepartmentManager);
            REQUIRE(account->getPosition() == Model::Position::DepartmentManager);
        }
    }

    TEST_CASE("create_exceptions", "[accounts]") {
        using namespace Model;

        TestContainerBootstrapper bootstrapper{TestContainerBootstrapper::newInstance()};
        Service::AccountService& accountService = bootstrapper.getAccountService();

        SECTION("empty_name") {
            REQUIRE_THROWS_WITH(accountService.createAccount("", "dow@gmail.com", "37aAsddkg9822"), Exceptions::wrongStringParamValue);
        }

        SECTION("empty_email") {
            REQUIRE_THROWS_WITH(accountService.createAccount("John Dow", "", "37aAsddkg9822"), Exceptions::wrongStringParamValue);
        }

        SECTION("empty_password") {
            REQUIRE_THROWS_WITH(accountService.createAccount("John Dow", "dow@gmail.com", ""), Exceptions::wrongStringParamValue);
        }
    }

    TEST_CASE("update_params_exceptions", "[accounts]") {
        using namespace Model;

        TestContainerBootstrapper bootstrapper{TestContainerBootstrapper::newInstance()};

        Service::AccountService& accountService = bootstrapper.getAccountService();

        Model::EntityID id = accountService.createEmployeeAccount("John Dow", "dow@gmail.com", "37aAsddkg9822", Model::Position::Operator);
        REQUIRE(bootstrapper.getAccountService().getAccountsAmount() == 1);

        auto account = accountService.getEmployee(id);

        REQUIRE(account.is_initialized());

        SECTION("change_email") {
            REQUIRE_THROWS_WITH(accountService.changeEmail(id, ""), Exceptions::wrongStringParamValue);
        }

        SECTION("change_password") {
            REQUIRE_THROWS_WITH(accountService.changePassword(id, ""), Exceptions::wrongStringParamValue);
        }
    }

    TEST_CASE("unknown_entity_exceptions", "[accounts]") {
        using namespace Model;

        TestContainerBootstrapper bootstrapper{TestContainerBootstrapper::newInstance()};

        Service::AccountService& accountService = bootstrapper.getAccountService();

        accountService.createEmployeeAccount("John Dow", "dow@gmail.com", "37aAsddkg9822", Model::Position::Operator);
        REQUIRE(bootstrapper.getAccountService().getAccountsAmount() == 1);

        Model::EntityID fakeId = boost::uuids::random_generator()();

        auto account = accountService.getEmployee(fakeId);

        REQUIRE_FALSE(account.is_initialized());

        std::string message = (boost::format(Exceptions::unknownAccount) % boost::uuids::to_string(fakeId)).str();

        SECTION("change_email") {
            REQUIRE_THROWS_WITH(accountService.changeEmail(fakeId, "fake@gmail.com"), message);
        }

        SECTION("change_password") {
            REQUIRE_THROWS_WITH(accountService.changePassword(fakeId, "33434"), message);
        }

        SECTION("promote") {
            REQUIRE_THROWS_WITH(accountService.promote(fakeId, Model::Position::DepartmentManager), message);
        }
    }

    TEST_CASE("invoke", "[accounts]") {
        using namespace Model;

        TestContainerBootstrapper bootstrapper{TestContainerBootstrapper::newInstance()};
        REQUIRE_NOTHROW(
                bootstrapper.invoke(
                        [](Service::AccountService& accounts, Service::DepositService& deposits){
                            REQUIRE(accounts.getAccountsAmount() == deposits.getDepositsAmount());
                        })
        );
    }

}