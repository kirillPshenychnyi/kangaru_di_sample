#ifndef KANGARU_DI_SAMPLE_REPOSITORY_HPP
#define KANGARU_DI_SAMPLE_REPOSITORY_HPP

#include <boost/noncopyable.hpp>
#include <boost/optional/optional.hpp>
#include <memory>

#include "model/typedefs.hpp"

namespace Repository {
    template <typename T>
    struct BaseRepository : public boost::noncopyable {
        virtual ~BaseRepository() noexcept = default;

        virtual boost::optional<T&> resolveEntity(Model::EntityID id) = 0;

        virtual boost::optional<const T&> resolveEntity(Model::EntityID id) const = 0;

        virtual int getSize() const = 0;

        virtual Model::EntityID insert(std::unique_ptr<T> entity) = 0;

        virtual void remove(Model::EntityID id) = 0;
    };
}

#endif //KANGARU_DI_SAMPLE_REPOSITORY_HPP
