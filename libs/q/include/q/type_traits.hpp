/*
 * Copyright 2013 Gustaf Räntilä
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LIBQ_TYPE_TRAITS_HPP
#define LIBQ_TYPE_TRAITS_HPP

#include <q/pp.hpp>
#include <q/type_traits/core.hpp>
#include <q/type_traits/operators.hpp>
#include <q/type_traits/convertible.hpp>
#include <q/type_traits/arguments.hpp>
#include <q/type_traits/fold.hpp>
#include <q/type_traits/two_fold.hpp>
#include <q/type_traits/arguments_impl.hpp>
#include <q/type_traits/hierarchical_condition.hpp>

namespace q {


template< typename... T >
struct is_nothrow_default_constructible
: hierarchically_satisfies_all_conditions<
	std::is_nothrow_default_constructible,
	T...
>
{ };

template< >
struct is_nothrow_default_constructible< void >
: std::true_type
{ };

template< typename... T >
struct is_copy_constructible
: hierarchically_satisfies_all_conditions<
	std::is_copy_constructible,
	T...
>
{ };

template< >
struct is_copy_constructible< void >
: std::true_type
{ };

template< typename... T >
struct is_nothrow_copy_constructible
: hierarchically_satisfies_all_conditions<
	std::is_nothrow_copy_constructible,
	T...
>
{ };

template< >
struct is_nothrow_copy_constructible< void >
: std::true_type
{ };

template< typename... T >
struct is_copy_assignable
: hierarchically_satisfies_all_conditions<
	std::is_copy_assignable,
	T...
>
{ };

template< >
struct is_copy_assignable< void >
: std::true_type
{ };

template< typename... T >
struct is_move_constructible
: hierarchically_satisfies_all_conditions<
	std::is_move_constructible,
	T...
>
{ };

template< >
struct is_move_constructible< void >
: std::true_type
{ };



/**
 * Generic version of a fold used to check if a certain type checker applies to
 * all types in the pack T.
 */
template< template< typename > class Operator, typename Nil, typename... T >
struct satisfies_all
: fold<
	q::arguments< T... >,
	generic_operator<
		Operator, logic_and
	>::template fold_type,
	Nil
>
{ };





/**
 * Same as q::arguments but specialized for tuples, to create a generic
 * generator for the tuple's inner arguments
 */
template< typename Tuple >
struct tuple_arguments
: public detail::tuple_arguments< Tuple >
{ };

/**
 * Exposes the arguments from a @c std::tuple wrapped in a @c q::arguments,
 * since @c tuple_arguments strictly isn't the same type as @c arguments,
 * which inhibits type specialization.
 */
template< typename Tuple >
struct arguments_from_tuple
{
	typedef typename tuple_arguments< Tuple >::this_type type;
};


/**
 * Determines if a tuple can be unpacked (and moved/copied) into a set of
 * arguments defined with a @c q::arguments wrapper.
 */
template< typename Tuple, class Args >
struct tuple_convertible_to_arguments
: public detail::tuple_convertible_to_arguments< Tuple, Args >
{ };

/**
 * Determines if a tuple can be unpacked (and moved/copied) into a set of
 * arguments.
 */
template< typename Tuple, typename... Args >
struct tuple_unpackable_to
: public tuple_convertible_to_arguments< Tuple, arguments< Args... > >
{ };

/**
 * Determines if a tuple can be unpacked (and moved/copied) into a set of
 * arguments defined by another tuple.
 */
template< typename TupleFrom, typename TupleTo >
struct tuple_convertible_to_tuple
: public detail::tuple_convertible_to_arguments<
	TupleFrom,
	typename tuple_arguments< TupleTo >::this_type
>
{ };

template< class... T >
struct any_type_is_a_reference
: fold<
	q::arguments< T... >,
	generic_operator<
		std::is_reference, logic_or
	>::template fold_type,
	std::false_type
>
{ };

template< class... T >
struct all_types_are_non_references
: bool_type< !any_type_is_a_reference< T... >::value >
{ };

template< std::size_t... Indices >
struct index_tuple
{
	template< std::size_t N >
	struct append
	{
		typedef index_tuple< Indices..., N > type;
	};
};

template< std::size_t Size >
struct make_index_tuple
{
	typedef typename make_index_tuple< Size - 1 >::type
		::template append< Size - 1 >::type type;
};

template<>
struct make_index_tuple< static_cast< std::size_t >( 0 ) >
{
	typedef index_tuple< > type;
};

template< typename... Types >
struct variadic_index_sequence
{
	typedef typename make_index_tuple< sizeof...( Types ) >::type type;
};


} // namespace q

#endif // LIBQ_TYPE_TRAITS_HPP
