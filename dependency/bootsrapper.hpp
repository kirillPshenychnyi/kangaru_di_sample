#ifndef KANGARU_DI_SAMPLE_BOOTSRAPPER_HPP
#define KANGARU_DI_SAMPLE_BOOTSRAPPER_HPP

namespace Service {
    struct AccountService;
    struct DepositService;
}

namespace Repository {
    struct AccountRepository;
    struct DepositRepository;
}

namespace Dependency {

    class Bootstrapper {
        public:
            virtual ~Bootstrapper() = default;

            virtual Service::DepositService& getDepositService() = 0;

            virtual Service::AccountService& getAccountService() = 0;
    };
}

#endif //KANGARU_DI_SAMPLE_BOOTSRAPPER_HPP
