#ifndef KANGARU_DI_SAMPLE_REPOSITORY_BASE_IMPL_HPP
#define KANGARU_DI_SAMPLE_REPOSITORY_BASE_IMPL_HPP

#include "repositories/repository.hpp"
#include "model/typedefs.hpp"

#include <boost/unordered_map.hpp>

namespace Repository {
namespace LocalRepo {

    template<typename T, typename Repo>
    class BaseRepositoryImpl : public Repo {
        private:
            using EntityPtr = std::unique_ptr<T>;
            using EntitySet = boost::unordered_map<Model::EntityID, EntityPtr>;

        public:
            virtual ~BaseRepositoryImpl() = default;

            boost::optional<T&> resolveEntity(Model::EntityID id) final {
                auto entity = enteties.find(id);
                return entity == enteties.end() ? boost::optional<T&>() : *entity->second;
            }

            int getSize() const final {
                return enteties.size();
            }

            void insert(EntityPtr entity) final {
                Model::EntityID id = entity->getID();
                enteties.emplace(id, std::move(entity));
            }

            void remove(Model::EntityID id) final {
                enteties.erase(id);
            }

        protected:
            template <typename Target, template<typename> class CastVisitor>
            boost::optional<Target&> resolveSpecificEntity(Model::EntityID id) {
                auto base = resolveEntity(id);
                if(base) {
                    return CastVisitor<Target>().cast(*base);
                }

                return boost::optional<Target&>();
            }

        private:
            EntitySet enteties;
    };
}
}

#endif //KANGARU_DI_SAMPLE_REPOSITORY_BASE_IMPL_HPP
