#ifndef KANGARU_DI_SAMPLE_ACCOUNT_REPOSITORY_HPP
#define KANGARU_DI_SAMPLE_ACCOUNT_REPOSITORY_HPP

#include "account_repository.hpp"
#include "repository.hpp"

namespace Model {
    class Account;
}

namespace Repository {
    struct AccountRepository : public BaseRepository<Model::Account> {
    };
}

#endif //KANGARU_DI_SAMPLE_ACCOUNT_REPOSITORY_HPP
