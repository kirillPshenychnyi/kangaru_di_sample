#ifndef KANGARU_DI_SAMPLE_DEFAULT_VISITOR_HPP
#define KANGARU_DI_SAMPLE_DEFAULT_VISITOR_HPP

namespace Utils {
    template <typename BaseVisitor, typename... Types>
    struct DefaultVisitor;

    template <typename BaseVisitor, typename Type>
    struct DefaultVisitor<BaseVisitor, Type>: public BaseVisitor {

        void visit(Type& type) override {}
    };

    template <typename BaseVisitor, typename Type, typename... Types>
    struct DefaultVisitor<BaseVisitor, Type, Types...> : public DefaultVisitor<BaseVisitor, Types...> {

        using DefaultVisitor<BaseVisitor, Types...>::visit;

        void visit(Type& type) override {}
    };
}

#define DECLARE_DEFAULT_VISITOR(Name, ...)							    \
	struct Name##DefaultVisitor											\
		:	public Utils::DefaultVisitor<Name##Visitor, __VA_ARGS__>	\
	{};

#endif //KANGARU_DI_SAMPLE_DEFAULT_VISITOR_HPP
