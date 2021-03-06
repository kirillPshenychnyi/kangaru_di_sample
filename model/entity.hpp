#ifndef KANGARU_DI_SAMPLE_ENTITY_HPP
#define KANGARU_DI_SAMPLE_ENTITY_HPP

#include <boost/noncopyable.hpp>
#include "typedefs.hpp"

namespace Model {
    class Entity : public boost::noncopyable {
        public:
            virtual ~Entity() noexcept = default;
            const EntityID& getID() const { return entity_id_; }

        protected:
            explicit Entity(const EntityID& entity_id) : entity_id_(entity_id) {}

        private:
            const EntityID entity_id_;
    };
}

#endif //KANGARU_DI_SAMPLE_ENTITY_HPP
