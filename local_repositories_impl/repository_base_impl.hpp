#ifndef KANGARU_DI_SAMPLE_REPOSITORY_BASE_IMPL_HPP
#define KANGARU_DI_SAMPLE_REPOSITORY_BASE_IMPL_HPP

#include "repositories/repository.hpp"
#include "model/typedefs.hpp"

#include <boost/unordered_map.hpp>

namespace Repository {
namespace LocalRepo {

    template<typename TEntity, typename Repo>
    class BaseRepositoryImpl : public Repo {
        private:
            using EntityPtr = std::unique_ptr<TEntity>;
            using EntitySet = boost::unordered_map<Model::EntityID, EntityPtr>;

        public:
            virtual ~BaseRepositoryImpl() = default;

            boost::optional<TEntity&> resolveEntity(Model::EntityID id) final {
                auto entity = entities_.find(id);
                return entity == entities_.end() ? boost::optional<TEntity&>() : *entity->second;
            }

            boost::optional<const TEntity&> resolveEntity(Model::EntityID id) const final {
                auto entity = entities_.find(id);
                return entity == entities_.end() ? boost::optional<const TEntity&>() : *entity->second;
            }

            int getSize() const final {
                return entities_.size();
            }

            const Model::EntityID& insert(EntityPtr entity) final {
                Model::EntityID id = entity->getID();
                return entities_.emplace(id, std::move(entity)).first->second->getID();
            }

            void remove(Model::EntityID id) final {
                entities_.erase(id);
            }

        protected:
            template <typename Target, template<typename> class CastVisitor>
            boost::optional<Target&> resolveSpecificEntity(Model::EntityID id) {
                auto base = resolveEntity(id);
                if(base) {
                    return CastVisitor<Target>().cast(*base);
                }

                return {};
            }

        private:
            EntitySet entities_;
    };
}
}

#endif //KANGARU_DI_SAMPLE_REPOSITORY_BASE_IMPL_HPP
