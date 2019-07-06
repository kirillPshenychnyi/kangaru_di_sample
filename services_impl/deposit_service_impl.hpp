#ifndef KANGARU_DI_SAMPLE_DEPOSIT_SERVICE_IMPL_HPP
#define KANGARU_DI_SAMPLE_DEPOSIT_SERVICE_IMPL_HPP

#include "services/deposit_service.hpp"
#include "base_service_impl.hpp"

namespace Repository {
    struct DepositRepository;
    struct AccountRepository;
}

namespace Service {
namespace Impl {

    class DepositServiceImpl : public BaseServiceImpl<Repository::DepositRepository, Service::DepositService> {
            using BaseClass = BaseServiceImpl<Repository::DepositRepository, Service::DepositService>;

        public:
            DepositServiceImpl(Repository::DepositRepository& depositRepo, Repository::AccountRepository& accountRepo);

            Model::EntityID createSavingsDeposit(
                    Model::EntityID ownerId, double initialBalance, double minBalance, double rateOfInterest) final;

            Model::EntityID createCurrentDeposit(
                    Model::EntityID ownerId, double initialBalance,
                    double rateOfInterest, double overDraftLimit) final;

            Model::EntityID createFixedDeposit(
                    Model::EntityID ownerId, double initialBalance, double rateOfInterest) final;

            void refill(Model::EntityID depositID, double amount) final;

            double getRateOfInterest(Model::EntityID depositID) const final;

            double getBalance(Model::EntityID depositID) const final;

            void withdraw(Model::EntityID depositID, double amount) final;

            void setOverdraftLimit(Model::EntityID depositID, double overdraftLimit) final;

            double closeDeposit(Model::EntityID depositID) final;

        private:
            Repository::AccountRepository& accountRepo_;
    };
}
}

#endif //KANGARU_DI_SAMPLE_DEPOSIT_SERVICE_IMPL_HPP
