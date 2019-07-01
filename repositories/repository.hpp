#ifndef KANGARU_DI_SAMPLE_REPOSITORY_HPP
#define KANGARU_DI_SAMPLE_REPOSITORY_HPP

#include <boost/noncopyable.hpp>
#include <boost/optional>

#include "model/typedefs.hpp"

namespace Repository {
    template <typename T>
    struct BaseRepository : public boost::noncopyable {
        virtual ~BaseRepository() = default;

        virtual boost::optional<T&> resolveEntity(Model::EntityID id) = 0;

        virtual int getSize() const = 0;

        virtual void insert(std::unique_ptr<T> entity) = 0;

        virtual void remove(Model::EntityID id) = 0;
    };
}

#endif //KANGARU_DI_SAMPLE_REPOSITORY_HPP