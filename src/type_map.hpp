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

#include "type_list.hpp"
#include "type_pair.hpp"

template <class... Pairs>
using type_map = type_list<Pairs...>;

template <class T>
struct is_map : std::false_type {};

template <class T, class U, class... Rest>
struct is_map<type_list<type_pair<T, U>, Rest...>> : is_map<type_list<Rest...>> {};

template <>
struct is_map<type_list<>> : std::true_type {};

//==================================================================================================
template <class Key, class T>
struct map_element;

template <class Key, class T>
using map_element_t = is_type_t<map_element<Key, T>>;

template <class Key, class T, class U, class... Rest>
struct map_element<Key, type_map<type_pair<T, U>, Rest...>> : map_element<Key, type_map<Rest...>> {
};

template <class Key, class T, class... Rest>
struct map_element<Key, type_map<type_pair<Key, T>, Rest...>> : is_type<T> {};

//==================================================================================================
template <class Key, class Value, class T>
using map_push_front = list_push_front<type_pair<Key, Value>, T>;

template <class Key, class Value, class T>
using map_push_front_t = list_push_front_t<type_pair<Key, Value>, T>;

//==================================================================================================
template <class Key, class T, size_t index = 0>
struct map_element_index;

template <class Key, class T, class U, size_t index, class... Rest>
struct map_element_index<Key, type_map<type_pair<T, U>, Rest...>, index>
    : map_element_index<Key, type_map<Rest...>, index + 1> {};

template <class Key, class T, size_t index, class... Rest>
struct map_element_index<Key, type_map<type_pair<Key, T>, Rest...>, index> : index_constant<index> {
};

//==================================================================================================
template <class T>
struct map_value_list;

template <class T>
using map_value_list_t = is_type_t<map_value_list<T>>;

template <>
struct map_value_list<type_map<>> : is_type<type_list<>> {};

template <class T, class U, class... Rest>
struct map_value_list<type_map<type_pair<T, U>, Rest...>>
    : is_type<list_push_front_t<U, map_value_list_t<type_map<Rest...>>>> {};

//==================================================================================================
template <class T>
struct map_key_list;

template <class T>
using map_key_list_t = is_type_t<map_key_list<T>>;

template <>
struct map_key_list<type_map<>> : is_type<type_list<>> {};

template <class T, class U, class... Rest>
struct map_key_list<type_map<type_pair<T, U>, Rest...>>
    : is_type<list_push_front_t<T, map_key_list_t<type_map<Rest...>>>> {};