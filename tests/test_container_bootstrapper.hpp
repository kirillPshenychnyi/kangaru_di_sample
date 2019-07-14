#ifndef KANGARU_DI_SAMPLE_TEST_CONTAINER_BOOTSTRAPPER_HPP
#define KANGARU_DI_SAMPLE_TEST_CONTAINER_BOOTSTRAPPER_HPP

#include "dependency/bootsrapper.hpp"

namespace Test {

    class TestContainerBootstrapper : public Dependency::Bootstrapper {
        public:
            TestContainerBootstrapper(
                    Service::AccountService& accountService,
                    Service::DepositService& depositService,
                    Repository::DepositRepository& depositRepository_,
                    Repository::AccountRepository& accountRepository_);

            ~TestContainerBootstrapper() final;

            static TestContainerBootstrapper newInstance();

            Service::DepositService& getDepositService() final;

            Service::AccountService& getAccountService() final;

        private:
            Service::AccountService& accountService_;
            Service::DepositService& depositService_;

            Repository::DepositRepository& depositRepository_;
            Repository::AccountRepository& accountRepository_;
    };
}

#endif //KANGARU_DI_SAMPLE_TEST_CONTAINER_BOOTSTRAPPER_HPP
