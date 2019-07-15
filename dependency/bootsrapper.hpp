#ifndef KANGARU_DI_SAMPLE_BOOTSRAPPER_HPP
#define KANGARU_DI_SAMPLE_BOOTSRAPPER_HPP

#include "third_party/kangaru/kangaru.hpp"

namespace Dependency {
    struct AccountServiceService;
    struct DepositServiceService;
}

namespace Service {
    struct AccountService;
    struct DepositService;
    auto service_map(Service::AccountService&) -> Dependency::AccountServiceService;
    auto service_map(Service::DepositService&) -> Dependency::DepositServiceService;
}

namespace Repository {
    struct AccountRepository;
    struct DepositRepository;
}

namespace Dependency {

    struct AccountRepositoryService : kgr::abstract_service<Repository::AccountRepository> {};
    struct DepositRepositoryService : kgr::abstract_service<Repository::DepositRepository> {};

    struct AccountServiceService : kgr::abstract_service<Service::AccountService> {};
    struct DepositServiceService : kgr::abstract_service<Service::DepositService> {};

    class Bootstrapper {
        public:
            Bootstrapper() = default;
            virtual ~Bootstrapper() = default;

            Bootstrapper(Bootstrapper&& other) noexcept = default;
            Bootstrapper& operator=(Bootstrapper&& other) noexcept = default;

            virtual Service::DepositService& getDepositService() {
                return container_.service<DepositServiceService>();
            }

            virtual Service::AccountService& getAccountService() {
                return container_.service<AccountServiceService>();
            }

            template <typename Callable>
            void invoke(Callable callback) {
                container_.invoke(callback);
            }

        protected:
            kgr::container container_;
    };
}

#endif //KANGARU_DI_SAMPLE_BOOTSRAPPER_HPP