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

#ifndef LIBQ_TYPE_TRAITS_TWO_FOLD_HPP
#define LIBQ_TYPE_TRAITS_TWO_FOLD_HPP

namespace q {

namespace detail {

template<
	typename From,
	typename To,
	template< typename, typename > class Fold,
	class Nil,
	template< typename, typename > class Generator,
	class DifferingSize,
	bool SameSize = From::size == To::size
>
struct two_fold;

template<
	typename... From,
	typename... To,
	template< typename, typename > class Fold,
	class Nil,
	template< typename, typename > class Generator,
	class DifferingSize
>
struct two_fold<
	q::arguments< From... >,
	q::arguments< To... >,
	Fold,
	Nil,
	Generator,
	DifferingSize,
	false
>
: DifferingSize
{ };

template<
	typename... From,
	typename... To,
	template< typename, typename > class Fold,
	class Nil,
	template< typename, typename > class Generator,
	class DifferingSize
>
struct two_fold<
	q::arguments< From... >,
	q::arguments< To... >,
	Fold,
	Nil,
	Generator,
	DifferingSize,
	true
>
: Fold<
	Generator<
		typename q::arguments< From... >::first_type,
		typename q::arguments< To... >::first_type
	>,
	two_fold<
		typename q::arguments< From... >::rest_arguments,
		typename q::arguments< To... >::rest_arguments,
		Fold,
		Nil,
		Generator,
		DifferingSize,
		sizeof...( From ) == sizeof...( To )
	>
>
{ };

template<
	template< typename, typename > class Fold,
	class Nil,
	template< typename, typename > class Generator,
	class DifferingSize
>
struct two_fold<
	q::arguments< >,
	q::arguments< >,
	Fold,
	Nil,
	Generator,
	DifferingSize,
	true
>
: Nil
{ };

} // namespace detail

template<
	typename From,
	typename To,
	template< typename, typename > class Fold,
	class Nil,
	template< typename, typename > class Generator,
	class DifferingSize
>
struct two_fold
: detail::two_fold<
	From,
	To,
	Fold,
	Nil,
	Generator,
	DifferingSize,
	From::size::value == To::size::value
>
{
	typedef detail::two_fold<
		From,
		To,
		Fold,
		Nil,
		Generator,
		DifferingSize,
		From::size::value == To::size::value
	> type;
};

} // namespace q

#endif // LIBQ_TYPE_TRAITS_TWO_FOLD_HPP
