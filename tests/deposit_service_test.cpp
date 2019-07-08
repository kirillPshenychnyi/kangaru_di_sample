#include <catch2/catch.hpp>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <boost/format.hpp>

#include "test_container_bootstrapper.hpp"

#include "services/account_service.hpp"
#include "services/deposit_service.hpp"

#include "model/employee.hpp"
#include "model/exceptions_messages.hpp"
#include "model/savings_deposit.hpp"
#include "model/current_deposit.hpp"
#include "model/fixed_deposit.hpp"

namespace Test {

TEST_CASE("no_deposits", "[deposits]") {
    TestContainerBootstrapper bootstrapper;
    REQUIRE(bootstrapper.getDepositService().getDepositsAmount() == 0);
}

TEST_CASE("deposits_creation", "[deposits]") {
    TestContainerBootstrapper bootstrapper;
    Service::DepositService& depositService = bootstrapper.getDepositService();
    Service::AccountService& accountService = bootstrapper.getAccountService();

    Model::EntityID accountID = accountService.createAccount("John Dow", "dow@gmail.com", "37aAsddkg9822");

    SECTION("savings_deposit"){
        Model::EntityID depositID = depositService.createSavingsDeposit(accountID, 5000, 500, 0.2);
        auto savings = depositService.getSavingsDeposit(depositID);

        REQUIRE(savings.is_initialized());

        REQUIRE(savings->getBalance() == 5000);
        REQUIRE(savings->getMinBalance() == 500);
        REQUIRE(savings->getRateOfInterest() == 0.2);
    }

    SECTION("fixed_deposit"){
        Model::EntityID depositID = depositService.createFixedDeposit(accountID, 5000, 0.2);
        auto fixed = depositService.getFixedDeposit(depositID);

        REQUIRE(fixed.is_initialized());

        REQUIRE(fixed->getBalance() == 5000);
        REQUIRE(fixed->getRateOfInterest() == 0.2);
    }

    SECTION("current_deposit"){
        Model::EntityID depositID = depositService.createCurrentDeposit(accountID, 5000, 2000);
        auto current = depositService.getCurrentDeposit(depositID);

        REQUIRE(current.is_initialized());

        REQUIRE(current->getBalance() == 5000);
        REQUIRE(current->getRateOfInterest() == 0.0);
        REQUIRE(current->getOverdraftLimit() == 2000);
    }
}

TEST_CASE("deposits_withdraw", "[deposits]") {
    TestContainerBootstrapper bootstrapper;
    Service::DepositService& depositService = bootstrapper.getDepositService();
    Service::AccountService& accountService = bootstrapper.getAccountService();

    Model::EntityID accountID = accountService.createAccount("John Dow", "dow@gmail.com", "37aAsddkg9822");

    SECTION("savings_deposit"){
        Model::EntityID depositID = depositService.createSavingsDeposit(accountID, 5000, 500, 0.2);
        depositService.withdraw(depositID, 4000);
        REQUIRE(depositService.getBalance(depositID) == 1000);
    }

    SECTION("fixed_deposit"){
        Model::EntityID depositID = depositService.createFixedDeposit(accountID, 5000, 0.2);
        REQUIRE_THROWS_WITH(depositService.withdraw(depositID, 1000), Model::Exceptions::wrongFixedDepositOperation);
    }

    SECTION("current_deposit"){
        Model::EntityID depositID = depositService.createCurrentDeposit(accountID, 5000, 2000);
        depositService.withdraw(depositID, 5000);
        REQUIRE(depositService.getBalance(depositID) == 0.0);
    }
}

TEST_CASE("deposits_refill", "[deposits]") {
    TestContainerBootstrapper bootstrapper;
    Service::DepositService& depositService = bootstrapper.getDepositService();
    Service::AccountService& accountService = bootstrapper.getAccountService();

    Model::EntityID accountID = accountService.createAccount("John Dow", "dow@gmail.com", "37aAsddkg9822");

    SECTION("savings_deposit"){
        Model::EntityID depositID = depositService.createSavingsDeposit(accountID, 5000, 500, 0.2);
        depositService.refill(depositID, 4000);
        REQUIRE(depositService.getBalance(depositID) == 9000);
    }

    SECTION("refill_zero_amount_exception"){
        Model::EntityID depositID = depositService.createCurrentDeposit(accountID, 5000, 2000);
        REQUIRE_THROWS_WITH(depositService.refill(depositID, 0.0), Model::Exceptions::negativeValue);
    }

    SECTION("refill_fixed_deposit"){
        Model::EntityID depositID = depositService.createFixedDeposit(accountID, 5000, 0.2);
        REQUIRE_THROWS_WITH(depositService.refill(depositID, 1000.0), Model::Exceptions::wrongFixedDepositOperation);
    }
}

}