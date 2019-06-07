/*Copyright or © or Copr. CNRS (2019). Contributors:
- Vincent Lanore. vincent.lanore@gmail.com

This software is a computer program whose purpose is to provide a header-only library with simple
template metaprogramming datastructures (list, map) and utilities.

This software is governed by the CeCILL-C license under French law and abiding by the rules of
distribution of free software. You can use, modify and/ or redistribute the software under the terms
of the CeCILL-C license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and rights to copy, modify and redistribute
granted by the license, users are provided only with a limited warranty and the software's author,
the holder of the economic rights, and the successive licensors have only limited liability.

In this respect, the user's attention is drawn to the risks associated with loading, using,
modifying and/or developing or reproducing the software by the user in light of its specific status
of free software, that may mean that it is complicated to manipulate, and that also therefore means
that it is reserved for developers and experienced professionals having in-depth computer knowledge.
Users are therefore encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or data to be ensured and,
more generally, to use and operate it in the same conditions as regards security.

The fact that you are presently reading this means that you have had knowledge of the CeCILL-C
license and that you accept its terms.*/

#pragma once

#include <tuple>
#include "is_type.hpp"

template <class... Ts>
using type_list = std::tuple<Ts...>;

template <class T>
using list_size = std::tuple_size<T>;

template <size_t i, class T>
using list_element = std::tuple_element<i, T>;

template <size_t i, class T>
using list_element_t = std::tuple_element_t<i, T>;

template <size_t i>
using index_constant = std::integral_constant<size_t, i>;

//==================================================================================================
template <class T>
struct is_list : std::false_type {};

template <class... Ts>
struct is_list<type_list<Ts...>> : std::true_type {};

//==================================================================================================
template <class ToFind, class T>
struct list_contains;

template <class ToFind, class... Rest>
struct list_contains<ToFind, type_list<ToFind, Rest...>> : std::true_type {};

template <class ToFind>
struct list_contains<ToFind, type_list<>> : std::false_type {};

template <class ToFind, class First, class... Rest>
struct list_contains<ToFind, type_list<First, Rest...>>
    : list_contains<ToFind, type_list<Rest...>> {};

//==================================================================================================
template <class ToFind, class T, size_t i = 0>
struct list_find;

template <class ToFind, size_t i>
struct list_find<ToFind, type_list<>, i> : index_constant<i> {};

template <class ToFind, class... Rest, size_t i>
struct list_find<ToFind, type_list<ToFind, Rest...>, i> : index_constant<i> {};

template <class ToFind, class First, class... Rest, size_t i>
struct list_find<ToFind, type_list<First, Rest...>, i>
    : list_find<ToFind, type_list<Rest...>, i + 1> {};

//==================================================================================================
template <class ToAdd, class T>
struct list_push_front;

template <class ToAdd, class T>
using list_push_front_t = is_type_t<list_push_front<ToAdd, T>>;

template <class ToAdd, class... Ts>
struct list_push_front<ToAdd, type_list<Ts...>> : is_type<type_list<ToAdd, Ts...>> {};

//==================================================================================================
template <template <class> class F, class T>
struct list_map;

template <template <class> class F, class T>
using list_map_t = is_type_t<list_map<F, T>>;

template <template <class> class F>
struct list_map<F, type_list<>> : is_type<type_list<>> {};

template <template <class> class F, class First, class... Rest>
struct list_map<F, type_list<First, Rest...>>
    : list_push_front<F<First>, list_map_t<F, type_list<Rest...>>> {};

//==================================================================================================
template <template <class> class F, class ValueT, class List>
struct list_map_to_value;

template <template <class> class F, class ValueT, class... Ts>
struct list_map_to_value<F, ValueT, type_list<Ts...>> {
    static constexpr std::array<ValueT, sizeof...(Ts)> value = {{F<Ts>::value...}};
};

template <template <class> class F, class ValueT, class... Ts>
constexpr std::array<ValueT, sizeof...(Ts)> list_map_to_value<F, ValueT, type_list<Ts...>>::value;

//==================================================================================================
template <template <class> class F, class Combinator, class T, T Zero, class L>
struct list_reduce_to_value;

template <template <class> class F, class Combinator, class T, T Zero, class... Ts>
struct list_reduce_to_value<F, Combinator, T, Zero, type_list<Ts...>> {
    static constexpr T reduce(const std::array<T, sizeof...(Ts)>& a) {
        T result = Zero;
        for (size_t i = 0; i < sizeof...(Ts); i++) { result = Combinator()(result, a[i]); }
        return result;
    }

    static constexpr T value = reduce(list_map_to_value<F, T, type_list<Ts...>>::value);
};

template <template <class> class F, class Combinator, class T, T Zero, class... Ts>
constexpr T list_reduce_to_value<F, Combinator, T, Zero, type_list<Ts...>>::value;

//==================================================================================================
template <template <class> class F, class L>
using list_and = list_reduce_to_value<F, std::logical_and<bool>, bool, true, L>;

template <template <class> class F, class L>
using list_or = list_reduce_to_value<F, std::logical_or<bool>, bool, false, L>;