#ifndef KANGARU_DI_SAMPLE_DEPOSIT_REPOSITORY_IMPL_HPP
#define KANGARU_DI_SAMPLE_DEPOSIT_REPOSITORY_IMPL_HPP

#include "repository_base_impl.hpp"
#include "repositories/deposit_repository.hpp"

#include "model/fixed_deposit.hpp"
#include "model/current_deposit.hpp"
#include "model/savings_deposit.hpp"
#include "model/visitors/deposit_visitor.hpp"

namespace Repository {
namespace LocalRepo {

    class DepositRepositoryImpl : public BaseRepositoryImpl<Model::Deposit, Repository::DepositRepository> {
        public:
            boost::optional<Model::SavingsDeposit&> findSavingsDeposit(Model::EntityID id) final {
                return resolveSpecificEntity<Model::SavingsDeposit, Model::DepositCast>(id);
            }

            boost::optional<Model::FixedDeposit&> findFixedDeposit(Model::EntityID id) final {
                return resolveSpecificEntity<Model::FixedDeposit, Model::DepositCast>(id);
            }

            boost::optional<Model::CurrentDeposit&> findCurrentDeposit(Model::EntityID id) final {
                return resolveSpecificEntity<Model::CurrentDeposit, Model::DepositCast>(id);
            }
    };
}
}

#endif //KANGARU_DI_SAMPLE_DEPOSIT_REPOSITORY_IMPL_HPP
