#ifndef KANGARU_DI_SAMPLE_DEPOSIT_REPOSITORY_HPP
#define KANGARU_DI_SAMPLE_DEPOSIT_REPOSITORY_HPP

#include "repository.hpp"

namespace Model {
    class Deposit;
    class SavingsDeposit;
    class CurrentDeposit;
    class FixedDeposit;
}

namespace Repository {
    struct DepositRepository : public BaseRepository<Model::Deposit> {
        public:
            virtual boost::optional<Model::SavingsDeposit&> findSavingsDeposit(Model::EntityID id) = 0;

            virtual boost::optional<Model::FixedDeposit&> findFixedDeposit(Model::EntityID id) = 0;

            virtual boost::optional<Model::CurrentDeposit&> findCurrentDeposit(Model::EntityID id) = 0;
    };
}

#endif //KANGARU_DI_SAMPLE_DEPOSIT_REPOSITORY_HPP
