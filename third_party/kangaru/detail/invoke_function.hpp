#ifndef KGR_KANGARU_INCLUDE_KANGARU_DETAIL_INVOKE_FUNCTION_HPP
#define KGR_KANGARU_INCLUDE_KANGARU_DETAIL_INVOKE_FUNCTION_HPP

#include "meta_list.hpp"
#include "void_t.hpp"
#include "traits.hpp"
#include "service_map.hpp"
#include "detection.hpp"

#include "define.hpp"

namespace kgr {
namespace detail {

/*
 * Checks if some given pointer to member function is invokable
 * 
 * Assumes that P is a reflectable function type.
 */
template<typename Map, typename T, typename P, typename... Args>
struct is_pointer_invokable_helper {
private:
	template<typename U, typename V, typename... As, std::size_t... S>
	static decltype(
		void((std::declval<U>().*std::declval<V>())(
			std::declval<service_type<mapped_service_t<function_argument_t<S, V>, Map>>>()...,
			std::declval<As>()...
		)), std::true_type{}
	) test(seq<S...>, int);
	
	template<typename..., typename U>
	static std::false_type test(U const&, void*);
	
public:
	using type = decltype(test<T, P, Args...>(tuple_seq_minus<function_arguments_t<P>, sizeof...(Args)>{}, 0));
};

/*
 * Alias for the is_pointer_invokable_helper trait
 */
template<typename Map, typename T, typename P, typename... Args>
struct is_pointer_invokable : is_pointer_invokable_helper<Map, T, P, Args...>::type {};

/*
 * Trait that checks if a class T has a call operator callable using given service map
 */
template<typename, typename, typename, typename, typename = void>
struct is_template_call_callable : std::false_type {};

/*
 * Specialization of is_template_call_callable when the call operator is callable
 */
template<typename Map, typename T, typename... TArgs, typename... Args>
struct is_template_call_callable<
	Map, T, meta_list<TArgs...>, meta_list<Args...>,
	enable_if_t<is_pointer_invokable<Map, T,
	
	// Some MSVC vesions cannot parse the valid syntax corretly. We must write
	// the expression in that way in order to compile.
#ifdef KGR_KANGARU_MSVC_NO_DEPENDENT_TEMPLATE_KEYWORD
	decltype(exact(&T::operator()<TArgs...>)),
#elif defined(KGR_KANGARU_MSVC_EXACT_DECLTYPE)
	// GCC won't accept taking the address of a generic lambda if the address is sent to a function like exact.
	decltype(exact(&T::template operator()<TArgs...>)),
#else
	// Vanilla syntax
	decltype(&T::template operator()<TArgs...>),
#endif // KGR_KANGARU_MSVC_NO_DEPENDENT_TEMPLATE_KEYWORD
	
	Args...>::value>
> : std::true_type {};

/*
 * Trait that returns the type of the first matching callable template call operator
 */
template<typename, typename, typename, typename, typename = void>
struct get_template_call {};

/*
 * Specialization of get_template_call_helper when current template arguments does
 * not result in a callable template call operator.
 * 
 * Will call recursively the next iteration of the trait.
 */
template<typename Map, typename T, typename Head, typename... Tail, typename... Args>
struct get_template_call<
	Map, T, meta_list<Head, Tail...>, meta_list<Args...>,
	enable_if_t<!is_template_call_callable<Map, T, meta_list<Head, Tail...>, meta_list<Args...>>::value>
> : get_template_call<Map, T, meta_list<Tail...>, meta_list<Args...>> {};

/*
 * Specialization of get_template_call_helper when a callable candidate is found.
 */
template<typename Map, typename T, typename... TArgs, typename... Args>
struct get_template_call<
	Map, T, meta_list<TArgs...>, meta_list<Args...>,
	enable_if_t<is_template_call_callable<Map, T, meta_list<TArgs...>, meta_list<Args...>>::value>
> {
	// Some MSVC vesions cannot parse the valid syntax corretly. We must write
	// the expression in that way in order to compile.
#ifdef KGR_KANGARU_MSVC_NO_DEPENDENT_TEMPLATE_KEYWORD
	using type = decltype(exact(&T::operator()<TArgs...>));
#elif defined(KGR_KANGARU_MSVC_EXACT_DECLTYPE)
	// GCC won't accept taking the address of a generic lambda if the address is sent to a function like exact.
	using type = decltype(exact(&T::template operator()<TArgs...>));
#else
	// Vanilla syntax
	using type = decltype(&T::template operator()<TArgs...>);
#endif
};

/*
 * Alias for get_template_call_helper::type
 */
template<typename Map, typename T, typename... Args>
using get_template_call_t = typename get_template_call<Map, T, meta_list<Args...>, meta_list<Args...>>::type;

/*
 * function_trait equivalent for an invoke function.
 * It has to choose if it's a lambda, generic lambda or a function.
 * 
 * This specialization is when T is not a function pointer and does not have a non overloaded operator()
 */
template<bool>
struct invoke_function_helper {
	template<typename Map, typename T, typename... Args>
	using type = function_traits<detected_t<get_template_call_t, Map, T, Args...>>;
};

/*
 * function_trait equivalent for an invoke function.
 * It has to choose if it's a lambda, generic lambda or a function.
 * 
 * This specialization is when T is function pointer or have a operator()
 */
template<>
struct invoke_function_helper<true> {
	template<typename Map, typename T, typename... Args>
	using type = function_traits<T>;
};

/*
 * function_trait equivalent for an invoke function.
 * It has to choose if it's a lambda, generic lambda or a function.
 */
template<typename Map, typename T, typename... Args>
using invoke_function = typename invoke_function_helper<has_call_operator<T>::value || std::is_pointer<T>::value>::template type<Map, T, Args...>;

/*
 * Alias for invoke_function::argument_types, a meta list of argument types.
 */
template<typename Map, typename T, typename... Args>
using invoke_function_arguments_t = typename invoke_function<Map, T, Args...>::argument_types;

/*
 * Alias for invoke_function::argument_type, the type of the nth argument.
 */
template<std::size_t n, typename Map, typename T, typename... Args>
using invoke_function_argument_t = meta_list_element_t<n, typename invoke_function<Map, T, Args...>::argument_types>;

/*
* Alias for invoke_function::return_type, the type of the nth argument.
*/
template<typename Map, typename T, typename... Args>
using invoke_function_result_t = typename invoke_function<Map, T, Args...>::return_type;

/*
 * Metafunction that returns a partially applied trait.
 * Used for is_invokable.
 */
template<typename Map, typename T, typename... Args>
struct curry_is_invokable {
	template<typename... Services>
	using trait = is_callable<T, detected_t<service_type, detected_t<mapped_service_t, Services, Map>>..., Args...>;
};

/*
 * Checks if the function T can be invoked by the container given Map as the
 * service map and Args as additional arguments.
 * 
 * Assumes that T is a reflectable function type.
 */
template<typename Map, typename T, typename... Args>
using is_invokable = bool_constant<
	is_detected<invoke_function_arguments_t, Map, T, Args...>::value &&
	expand_minus_n<
		sizeof...(Args),
		detected_or<meta_list<>, invoke_function_arguments_t, Map, T, Args...>,
		curry_is_invokable<Map, T, Args...>::template trait
	>::value
>;

} // namespace detail
} // namespace kgr

#include "undef.hpp"

#endif // KGR_KANGARU_INCLUDE_KANGARU_DETAIL_INVOKE_FUNCTION_HPP
