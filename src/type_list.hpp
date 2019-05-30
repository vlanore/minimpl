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

// #include <array>
// #include <numeric>

// namespace minimpl {
//     // list metatype
//     // to be used as a tag to identify list types
//     struct List {};

//     // list type trait
//     template <class T>
//     using is_list = std::is_base_of<List, T>;

//     // list default class ()
//     template <class... Elements>
//     struct list : List {
//         using boxes = std::tuple<box<Elements>...>;  // to be used when instantiation is
//         needed
//         using tuple = std::tuple<Elements...>;
//         static constexpr size_t size = sizeof...(Elements);
//     };

//     template <class... Elements>
//     constexpr size_t list<Elements...>::size;  // needed for linking

//     //==============================================================================================
//     // get element at index index, fails if L is not a list or if out of bounds
//     template <class L, size_t index>
//     struct list_element : Box {
//         static_assert(is_list<L>::value, "L is not a list");
//         static_assert(index < L::size, "index out of bounds");
//         using type = unbox_t<std::tuple_element_t<index, typename L::boxes>>;
//     };

//     template <class T, size_t index>
//     using list_element_t = unbox_t<list_element<T, index>>;

//     //==============================================================================================
//     template <class L, template <class> class Condition>
//     struct list_find_if_nocheck {
//         static_assert(is_list<L>::value, "parameter L is not a list");

//         template <size_t index>
//         static constexpr size_t helper(std::tuple<>) {
//             return index;  // return list size if not found
//         }

//         template <size_t index, class First, class... Rest>
//         static constexpr size_t helper(std::tuple<First, Rest...>) {
//             constexpr bool condition_ok = Condition<unbox_t<First>>::value;
//             auto recursive_call = helper<index + 1>(std::tuple<Rest...>());
//             return condition_ok ? index : recursive_call;
//         }

//         static constexpr size_t value = helper<0>(typename L::boxes());
//     };

//     template <class L, template <class> class Condition>
//     constexpr size_t list_find_if_nocheck<L, Condition>::value;

//     //==============================================================================================
//     template <class L, template <class> class Condition>
//     struct list_find_if {
//         static_assert(is_list<L>::value, "parameter L is not a list");
//         static constexpr size_t value = list_find_if_nocheck<L, Condition>::value;
//         static_assert(value < L::size, "type not foud in list");
//     };

//     template <class L, template <class> class Condition>
//     constexpr size_t list_find_if<L, Condition>::value;

//     //==============================================================================================
//     template <class L, class ToFind>
//     struct list_find {
//         static_assert(is_list<L>::value, "parameter L is not a list");

//         template <class T>
//         using is_tofind = std::is_same<T, ToFind>;

//         static constexpr size_t value = list_find_if<L, is_tofind>::value;
//     };

//     template <class L, class ToFind>
//     constexpr size_t list_find<L, ToFind>::value;

//     //==============================================================================================
//     template <class L, class ToFind>
//     struct list_contains {
//         static_assert(is_list<L>::value, "parameter L is not a list");

//         template <class T>
//         using is_tofind = std::is_same<T, ToFind>;

//         static constexpr bool value = list_find_if_nocheck<L, is_tofind>::value < L::size;
//     };

//     template <class L, class ToFind>
//     constexpr bool list_contains<L, ToFind>::value;

//     //==============================================================================================
//     template <class L, class ToAdd>
//     struct list_push_front : Box {
//         static_assert(is_list<L>::value, "parameter L is not a list");

//         template <class... Elements>
//         static auto helper(list<Elements...>) {
//             return list<ToAdd, Elements...>();
//         }

//         using type = decltype(helper(L()));
//     };

//     template <class L, class ToAdd>
//     using list_push_front_t = unbox_t<list_push_front<L, ToAdd>>;

//     //==============================================================================================
//     template <class L, template <class> class F, class T>
//     struct list_map_to_value {
//         static_assert(is_list<L>::value, "L is not a list");

//         template <class... Elements>
//         static constexpr auto make(std::tuple<Elements...>) {
//             return std::array<T, L::size>{{F<unbox_t<Elements>>::value...}};
//         }

//         static constexpr std::array<T, L::size> value = make(typename L::boxes());
//     };

//     template <class L, template <class> class F, class T>
//     constexpr std::array<T, L::size> list_map_to_value<L, F, T>::value;

//     //==============================================================================================
//     template <class L, template <class> class F, class Combinator, class T, T Zero>
//     struct list_reduce_to_value {
//         static_assert(is_list<L>::value, "L is not a list");

//         static constexpr T reduce(const std::array<T, L::size>& a) {
//             T result = Zero;
//             for (size_t i = 0; i < L::size; i++) { result = Combinator()(result, a[i]); }
//             return result;
//         }

//         static constexpr T value = reduce(list_map_to_value<L, F, T>::value);
//     };

//     template <class L, template <class> class F, class Combinator, class T, T Zero>
//     constexpr T list_reduce_to_value<L, F, Combinator, T, Zero>::value;

//     //==============================================================================================
//     template <class L, template <class> class F>
//     struct list_map : Box {
//         static_assert(is_list<L>::value, "L is not a list");

//         static auto helper(std::tuple<>) { return list<>(); }

//         template <class First, class... Rest>
//         static auto helper(std::tuple<First, Rest...>) {
//             using recursive_call = decltype(helper(std::tuple<Rest...>()));
//             return list_push_front_t<recursive_call, F<unbox_t<First>>>();
//         }

//         using type = decltype(helper(typename L::boxes()));
//     };

//     template <class L, template <class> class F>
//     using list_map_t = unbox_t<list_map<L, F>>;

// };  // namespace minimpl
