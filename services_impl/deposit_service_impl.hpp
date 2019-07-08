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
                    const Model::EntityID& ownerId, double initialBalance, double minBalance, double rateOfInterest) final;

            Model::EntityID createCurrentDeposit(
                    const Model::EntityID& ownerId, double initialBalance,
                    double overDraftLimit) final;

            Model::EntityID createFixedDeposit(
                    const Model::EntityID& ownerId, double initialBalance, double rateOfInterest) final;

            boost::optional<const Model::SavingsDeposit&> getSavingsDeposit(const Model::EntityID& depositId) const final;

            boost::optional<const Model::CurrentDeposit&> getCurrentDeposit(const Model::EntityID& depositId) const final;

            boost::optional<const Model::FixedDeposit&> getFixedDeposit(const Model::EntityID& depositId) const final;

            void refill(const Model::EntityID& depositId, double amount) final;

            double getRateOfInterest(const Model::EntityID& depositId) const final;

            double getBalance(const Model::EntityID& depositId) const final;

            void withdraw(const Model::EntityID& depositId, double amount) final;

            void setOverdraftLimit(const Model::EntityID& depositId, double overdraftLimit) final;

            double closeDeposit(const Model::EntityID& depositId) final;

            void onBankPeriodPassed(const Model::EntityID& depositId) final;

            int getDepositsAmount() const final;

        private:
            Repository::AccountRepository& accountRepo_;
    };
}
}

#endif //KANGARU_DI_SAMPLE_DEPOSIT_SERVICE_IMPL_HPP
