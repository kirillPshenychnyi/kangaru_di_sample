#include <third_party/catch.hpp>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <boost/format.hpp>

#include "test_container_bootstrapper.hpp"

#include "services/account_service.hpp"
#include "model/employee.hpp"
#include "model/exceptions_messages.hpp"

namespace Test {

TEST_CASE("accounts_are_empty", "[accounts]") {
    TestContainerBootstrapper bootstrapper{TestContainerBootstrapper::newInstance()};
    REQUIRE(bootstrapper.getAccountService().getAccountsSize() == 0);
}

TEST_CASE("create_accounts", "[accounts]") {
    TestContainerBootstrapper bootstrapper{TestContainerBootstrapper::newInstance()};

    Service::AccountService& accountService = bootstrapper.getAccountService();

    SECTION("simple_account") {
        Model::EntityID id = accountService.createAccount("John Dow", "dow@gmail.com", "37aAsddkg9822");
        REQUIRE(bootstrapper.getAccountService().getAccountsSize() == 1);

        auto account = accountService.getAccount(id);

        REQUIRE(account.is_initialized());
        REQUIRE(account->getName() == "John Dow");
        REQUIRE(account->getEmail() == "dow@gmail.com");
        REQUIRE(account->getPasswordHash() == "37aAsddkg9822");
    }

    SECTION("employee_account") {
        Model::EntityID id = accountService.createEmployeeAccount("Sam Rio", "rio@gmail.com", "37aAsddkg9822", Model::Position::Operator);

        auto account = accountService.getEmployee(id);

        REQUIRE(account.is_initialized());
        REQUIRE(account->getName() == "Sam Rio");
        REQUIRE(account->getEmail() == "rio@gmail.com");
        REQUIRE(account->getPasswordHash() == "37aAsddkg9822");
        REQUIRE(account->getPosition() == Model::Position::Operator);
    }
}

TEST_CASE("change_params", "[accounts]") {
    TestContainerBootstrapper bootstrapper{TestContainerBootstrapper::newInstance()};

    Service::AccountService& accountService = bootstrapper.getAccountService();

    Model::EntityID id = accountService.createEmployeeAccount("John Dow", "dow@gmail.com", "37aAsddkg9822", Model::Position::Operator);
    REQUIRE(bootstrapper.getAccountService().getAccountsSize() == 1);

    auto account = accountService.getEmployee(id);

    REQUIRE(account.is_initialized());

    SECTION("change_email") {
        accountService.changeEmail(id, "johnDow@yandex.ua");
        REQUIRE(account->getEmail() == "johnDow@yandex.ua");
    }

    SECTION("change_password") {
        accountService.changePassword(id, "838asdl00");
        REQUIRE(account->getPasswordHash() == "838asdl00");
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
    REQUIRE(bootstrapper.getAccountService().getAccountsSize() == 1);

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

    Model::EntityID id = accountService.createEmployeeAccount("John Dow", "dow@gmail.com", "37aAsddkg9822", Model::Position::Operator);
    REQUIRE(bootstrapper.getAccountService().getAccountsSize() == 1);

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

}