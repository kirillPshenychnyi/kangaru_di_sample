#ifndef KANGARU_DI_SAMPLE_TYPEDEFS_HPP
#define KANGARU_DI_SAMPLE_TYPEDEFS_HPP

#include <boost/uuid/uuid.hpp>

namespace Model {
    using EntityID = boost::uuids::uuid;

    enum class Position {Operator, DepartmentManager};
}

#endif //KANGARU_DI_SAMPLE_TYPEDEFS_HPP
