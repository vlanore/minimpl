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

#include "list.hpp"
#include "pair.hpp"

namespace minimpl {

    struct Map {};       // metatype (type tag for maps)
    struct NotFound {};  // when map_element does not find key

    template <class... Pairs>
    struct map : Map, list<Pairs...> {
        static_assert(list_reduce_to_value<list<Pairs...>, is_pair, std::logical_and<bool>, bool,
                                           true>::value,
                      "template arguments contain non-pairs");
    };

    template <class T>
    using is_map = std::is_base_of<Map, T>;

    //==============================================================================================
    template <class L>
    struct list_to_map : Box {
        static_assert(is_list<L>::value, "L is not a list");

        template <class... Elements>
        static auto helper(std::tuple<Elements...>) {
            return map<Elements...>();
        }

        using type = decltype(helper(typename L::tuple()));
    };

    template <class L>
    using list_to_map_t = unbox_t<list_to_map<L>>;

    //==============================================================================================
    template <class M, class Key, class Value>
    struct map_push_front : Box {
        using type = list_to_map_t<list_push_front_t<M, pair<Key, Value>>>;
    };

    template <class M, class Key, class Value>
    using map_push_front_t = unbox_t<map_push_front<M, Key, Value>>;

    //==============================================================================================
    template <class T, class Key>
    struct map_element_index {
        static_assert(is_map<T>::value, "T is not a map");

        template <class E>
        using is_correct_entry = std::is_same<Key, first_t<E>>;

        static constexpr size_t value = list_find_if<T, is_correct_entry>::value;
    };

    template <class T, class Key>
    constexpr size_t map_element_index<T, Key>::value;

    //==============================================================================================
    // map_element
    template <class T, class Key>
    struct map_element : Box {
        static_assert(is_map<T>::value, "T is not a map");
        using type = second_t<list_element_t<T, map_element_index<T, Key>::value>>;
    };

    template <class T, class Key>
    using map_element_t = unbox_t<map_element<T, Key>>;

    //==============================================================================================
    template <class T>
    struct map_value_list : Box {
        static_assert(is_map<T>::value, "T is not a map");

        using type = list_map_t<T, second_t>;
    };

    template <class T>
    using map_value_list_t = unbox_t<map_value_list<T>>;

    //==============================================================================================
    template <class T>
    struct map_key_list : Box {
        static_assert(is_map<T>::value, "T is not a map");

        using type = list_map_t<T, first_t>;
    };

    template <class T>
    using map_key_list_t = unbox_t<map_key_list<T>>;

};  // namespace minimpl
