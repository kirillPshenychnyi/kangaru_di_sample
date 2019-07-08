#ifndef KANGARU_DI_SAMPLE_BOOTSRAPPER_HPP
#define KANGARU_DI_SAMPLE_BOOTSRAPPER_HPP

#include <boost/noncopyable.hpp>

#include <kangaru/include/kangaru/kangaru.hpp>

namespace Service {
    struct AccountService;
    struct DepositService;
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

    class Bootstrapper : public boost::noncopyable {
        public:
            virtual ~Bootstrapper() = default;

            Service::DepositService& getDepositService() {
                return container_.service<DepositServiceService>();
            }

            Service::AccountService& getAccountService() {
                return container_.service<AccountServiceService>();
            }

        private:
            virtual void initRepos() = 0;

            virtual void initServices() = 0;

        protected:
            kgr::container container_;
    };
}

#endif //KANGARU_DI_SAMPLE_BOOTSRAPPER_HPP
