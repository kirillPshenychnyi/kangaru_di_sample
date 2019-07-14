#ifndef KGR_KANGARU_INCLUDE_KANGARU_DETAIL_AUTOCALL_TRAITS_HPP
#define KGR_KANGARU_INCLUDE_KANGARU_DETAIL_AUTOCALL_TRAITS_HPP

#include "meta_list.hpp"
#include "service_map.hpp"
#include "utils.hpp"
#include "traits.hpp"
#include "injected.hpp"

namespace kgr {
namespace detail {

/*
 * Alias to the parameter list of an invoke call expression in autocall.
 */
template<typename T>
using invoke_call_parameters = typename T::parameters;

/*
 * Trait that check if an autocall entry is an invoke call expression.
 */
template<typename T>
using is_invoke_call = is_detected<invoke_call_parameters, T>;

/*
 * Trait that check if the autocall entry is a member function autocall.
 */
template<typename T, typename F>
using is_member_autocall = std::is_member_function_pointer<typename F::value_type>;

/*
 * Trait that check if the type `T` is a pointer to nonmember function.
 */
template<typename T>
using is_nonmember_function_pointer = bool_constant<
	std::is_function<typename std::remove_pointer<T>::type>::value &&
	std::is_pointer<T>::value
>;

/*
 * Trait that check if the autocall entry is a member function autocall.
 */
template<typename T, typename F>
using is_nonmember_autocall = bool_constant<(
	meta_list_size<
		function_arguments_t<
			conditional_t<
				is_nonmember_function_pointer<typename F::value_type>::value,
				typename F::value_type,
				void(*)()
			>
		>
	>::value > 0
)>;

/*
 * Alias to the service map of a autocall entry.
 * Right now, it's only possible to set the map to all entry.
 */
template<typename T, typename F>
using autocall_selected_map_t = typename T::map;

/*
 * This class will get the member autocall function member of the service definition.
 *
 * From a desing point of view, maybe the autocall function part of the service
 * definition should not exist, and moved to the container.
 */
template<typename, typename, typename = void>
struct autocall_function_helper;

/*
 * This spetialization of autocall_function_helper will get the autocall function when the autocall function is a member and nonmeber autocall.
 */
template<typename T, typename F>
struct autocall_function_helper<T, F, enable_if_t<(is_nonmember_autocall<T, F>::value || is_member_autocall<T, F>::value) && !is_invoke_call<F>::value>> {
private:
	static void autocall(inject_t<container_service> cs, T& service) {
		T::template autocall_helper<T, autocall_selected_map_t<T, F>, F>(
			conditional_t<
				is_member_autocall<T, F>::value,
				function_seq<typename F::value_type>,
				detected_t<seq_drop_first_t, function_seq<typename F::value_type>>
			>{},
			std::move(cs),
			service
		);
	}
	
public:
	using type = std::integral_constant<
		decltype(&autocall_function_helper::autocall),
		&autocall_function_helper::autocall
	>;
};

/*
 * This spetialization of autocall_function_helper will get the member autocall when the autocall function is invoke call expression.
 */
template<typename T, typename F>
struct autocall_function_helper<T, F, enable_if_t<is_invoke_call<F>::value>> {
private:
	template<typename... Args>
	struct function {
		static void autocall(inject_t<Args>... others, T& service) {
			service.call(F::value, std::forward<inject_t<Args>>(others).forward()...);
		}
	};
	
	template<std::size_t... S>
	using function_constant = std::integral_constant<
		decltype(&function<meta_list_element_t<S, invoke_call_parameters<F>>...>::autocall),
		&function<meta_list_element_t<S, invoke_call_parameters<F>>...>::autocall
	>;
	
	template<std::size_t... S>
	static auto get_function(seq<S...>) -> function_constant<S...>;
	
public:
	using type = decltype(get_function(tuple_seq<invoke_call_parameters<F>>{}));
};

/*
 * Alias to the autocall_function_helper trait to ease it's usage.
 */
template<typename T, typename F>
using autocall_function = typename autocall_function_helper<T, F>::type;

/*
 * This trait extract what services are needed for calling an autocall function.
 * This is mostly used for other trait to validate an autocall function.
 */
template<typename, typename, typename = void>
struct autocall_services_helper;

/*
 * This is the case for an invoke call.
 * Since all required services are listed, we simply take those.
 */
template<typename T, typename F>
struct autocall_services_helper<T, F, enable_if_t<is_invoke_call<F>::value>> {
	using type = invoke_call_parameters<F>;
};

/*
 * This is the case for an autocall member and nonmember function call.
 * Since the arguments are the injected service type, we must use a map and extract the definition type out of each arguments.
 */
template<typename T, typename F>
struct autocall_services_helper<T, F, enable_if_t<!is_invoke_call<F>::value && (is_member_autocall<T, F>::value || is_nonmember_autocall<T, F>::value)>> {
private:
	template<typename U>
	using mapped_type = detected_t<mapped_service_t, U, autocall_selected_map_t<T, F>>;
	
	using arguments = conditional_t<
		is_member_autocall<T, F>::value,
		function_arguments_t<typename F::value_type>,
		detected_t<meta_list_pop_front_t, function_arguments_t<typename F::value_type>>
	>;
	
public:
	using type = conditional_t<
		all_of_traits<arguments, is_complete_map, autocall_selected_map_t<T, F>>::value,
		meta_list_transform_t<arguments, mapped_type>,
		nonesuch
	>;
};

/*
 * This is an alias for the list of autocall functions in a service.
 */
template<typename T>
using autocall_functions_t = typename T::autocall_functions;

/*
 * Trait that checks if the service T has autocall functions.
 */
template<typename T>
using has_autocall = is_detected<autocall_functions_t, T>;

/*
 * This gets the integral_constant found in autocall_function
 */
template<typename T, typename F>
using autocall_function_t = typename autocall_function<T, F>::value_type;

/*
 * This returns the nth autocall function type in the autocall list of a service.
 */
template<typename T, std::size_t I>
using autocall_nth_function = autocall_function<T, meta_list_element_t<I, autocall_functions_t<T>>>;

/*
 * This returns the value type of autocall_nth_function
 */
template<typename T, std::size_t I>
using autocall_nth_function_t = autocall_function_t<T, meta_list_element_t<I, autocall_functions_t<T>>>;

/*
 * This is an alias for the argument list of an autocall function.
 */
template<typename T, typename F>
using autocall_services = typename autocall_services_helper<T, F>::type;

/*
 * This checks if an entry in the autocall list is actually a valid autocall function type.
 */
template<typename T, typename F>
using is_valid_autocall_function = std::integral_constant<bool,
	is_invoke_call<F>::value || is_member_autocall<T, F>::value || is_nonmember_autocall<T, F>::value
>;

/*
 * Meta trait to apply a trait over all autocall entry in a autocall list
 */
template<template<typename...> class Trait, typename T>
using autocall_trait = all_of_traits<detected_or<meta_list<>, autocall_functions_t, T>, Trait, T>;

/*
 * Trait that check if every injected arguments of a particular autocall entry are valid services.
 * 
 * However, we don't validate autocall checks for these injected services.
 */
template<typename T, typename F>
using is_autocall_entry_valid = all_of_traits<detected_t<autocall_services, T, F>, service_check>;

/*
 * Validity check for autocall entries in a service T
 */
template<typename T>
using is_autocall_valid = autocall_trait<is_autocall_entry_valid, T>;

} // namespace detail
} // namespace kgr

#endif // KGR_KANGARU_INCLUDE_KANGARU_DETAIL_AUTOCALL_TRAITS_HPP
