#ifndef KANGARU_DI_SAMPLE_DEPOSIT_REPOSITORY_HPP
#define KANGARU_DI_SAMPLE_DEPOSIT_REPOSITORY_HPP

#include "repository.hpp"

namespace Model {
    struct Deposit;
}

namespace Repository {
    struct DepositRepository : public BaseRepository<Model::Deposit> {
    };
}

#endif //KANGARU_DI_SAMPLE_DEPOSIT_REPOSITORY_HPP
