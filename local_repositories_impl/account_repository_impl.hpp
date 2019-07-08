#ifndef KANGARU_DI_SAMPLE_ACCOUNT_REPOSITORY_IMPL_HPP
#define KANGARU_DI_SAMPLE_ACCOUNT_REPOSITORY_IMPL_HPP

#include "model/employee.hpp"
#include "model/visitors/account_visitor.hpp"
#include "model/employee.hpp"
#include "model/visitors/account_visitor.hpp"
#include "repository_base_impl.hpp"
#include "repositories/account_repository.hpp"

namespace Repository {
namespace LocalRepo {
    class AccountRepositoryImpl : public BaseRepositoryImpl<Model::Account, Repository::AccountRepository> {
        public:
            boost::optional<Model::Employee&> findEmployee(Model::EntityID id) final {
                return resolveSpecificEntity<Model::Employee, Model::AccountCast>(id);
            }

            boost::optional<const Model::Employee&> getEmployee(Model::EntityID id) final {
                auto resolved = resolveSpecificEntity<Model::Employee, Model::AccountCast>(id);
                return resolved ? boost::optional<const Model::Employee&>(*resolved) : boost::optional<const Model::Employee&>();
            }
    };
}
}

#endif //KANGARU_DI_SAMPLE_ACCOUNT_REPOSITORY_IMPL_HPP
