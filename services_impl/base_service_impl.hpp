#ifndef KANGARU_DI_SAMPLE_BASE_SERVICE_IMPL_HPP
#define KANGARU_DI_SAMPLE_BASE_SERVICE_IMPL_HPP

#include <memory>
#include <boost/uuid/random_generator.hpp>
#include <boost/optional.hpp>

#include "model/typedefs.hpp"

namespace Service {
namespace Impl {

    template <typename Repository, typename Service>
    class BaseServiceImpl : public Service {
        public:
            explicit BaseServiceImpl(Repository& repository) : repository_(repository), generator_() {}

        public:
            void remove(Model::EntityID entityId) {
                repository_.remove(entityId);
            }

            template <typename Entity, typename ... Args>
            Model::EntityID create(Args&&... args) {
                return repository_.insert(std::make_unique<Entity>(std::forward<Args>(args)...));
            }

        protected:
            template <typename Entity>
            boost::optional<const Entity&> constCast(boost::optional<Entity&> entity) const {
                return entity ? boost::optional<const Entity&>(*entity) : boost::optional<const Entity&>();
            }

        protected:
            Repository& repository_;
            boost::uuids::random_generator generator_;
    };
}
}

#endif //KANGARU_DI_SAMPLE_BASE_SERVICE_IMPL_HPP
