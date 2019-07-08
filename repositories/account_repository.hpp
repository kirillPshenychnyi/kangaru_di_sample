#ifndef KANGARU_DI_SAMPLE_ACCOUNT_REPOSITORY_HPP
#define KANGARU_DI_SAMPLE_ACCOUNT_REPOSITORY_HPP

#include "repository.hpp"

namespace Model {
    class Account;
    class Employee;
}

namespace Repository {
    struct AccountRepository : public BaseRepository<Model::Account> {
        virtual boost::optional<Model::Employee&> findEmployee(Model::EntityID id) = 0;

        virtual boost::optional<const Model::Employee&> getEmployee(Model::EntityID id) = 0;
    };
}

#endif //KANGARU_DI_SAMPLE_ACCOUNT_REPOSITORY_IMPL_HPP
