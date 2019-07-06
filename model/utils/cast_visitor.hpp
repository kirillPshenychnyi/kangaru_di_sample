#ifndef KANGARU_DI_SAMPLE_CAST_VISITOR_HPP
#define KANGARU_DI_SAMPLE_CAST_VISITOR_HPP

#include <boost/optional/optional.hpp>

namespace Utils {
    template<typename TargetType, typename SourceRootType, typename DefaultVisitor>
    class CastVisitor : private DefaultVisitor {
        private:
            using CastResult = boost::optional<TargetType&>;

        public:
            CastResult cast(SourceRootType& _sourceObject)
            {
                castResult_.reset();
                _sourceObject.accept( *this );
                return castResult_;
            }

        private:
            void visit(TargetType& target) final {
                castResult_ = target;
            }

        private:
            CastResult castResult_;
    };
}

#define DEFINE_CAST( _construct )											\
template< typename _TargetType >											\
class _construct##Cast														\
	:	public Utils::CastVisitor<_TargetType, _construct, _construct##DefaultVisitor >	\
{																			\
};

#endif //KANGARU_DI_SAMPLE_CAST_VISITOR_HPP
