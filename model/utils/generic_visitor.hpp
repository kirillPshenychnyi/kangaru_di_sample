#ifndef KANGARU_DI_SAMPLE_GENERIC_VISITOR_HPP
#define KANGARU_DI_SAMPLE_GENERIC_VISITOR_HPP

namespace Utils {

    template <typename ...T>
    struct Visitor;

    template <typename T, typename... Other>
    struct Visitor<T, Other...> : public Visitor<Other...> {
        virtual ~Visitor() noexcept = default;

        using BaseClass = Visitor<Other...>;
        using BaseClass::visit;

        virtual void visit(T& entity) = 0;
    };

    template <typename T>
    struct Visitor<T> {
        virtual void visit(T& entity) = 0;
    };
}

#define DECLARE_VISITOR(Name, ...)  				\
	struct Name##Visitor							\
		:	public Utils::Visitor< __VA_ARGS__ >	\
	{};

#endif //KANGARU_DI_SAMPLE_GENERIC_VISITOR_HPP
