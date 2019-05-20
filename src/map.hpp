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
        // static_assert(map_and_fold_list<list<Pairs...>, is_pair, std::logical_and<bool>>::value,
        //               "template arguments contain non-pairs");
    };

    template <class T>
    using is_map = std::is_base_of<Map, T>;

    //==============================================================================================
    // map_element
    template <class T, class Key>
    struct map_element : Box {
        static_assert(is_map<T>::value, "T is not a map");

        static auto helper(std::tuple<>) { return box<NotFound>(); }

        template <class First, class... Rest>
        static auto helper(std::tuple<First, Rest...>) {
            return helper(std::tuple<Rest...>());
        }

        template <class Value, class... Rest>
        static auto helper(std::tuple<box<pair<Key, Value>>, Rest...>) {
            return box<Value>();
        }

        using type = unbox_t<decltype(helper(typename T::boxes()))>;
        static_assert(not std::is_same<NotFound, type>::value, "key not found in map");
    };

    template <class T, class Key>
    using map_element_t = unbox_t<map_element<T, Key>>;

    //==============================================================================================
    template <class T, class Key>
    struct map_element_index {
        static_assert(is_map<T>::value, "T is not a map");

        template <size_t index>
        static constexpr size_t helper(std::tuple<>) {
            return index;  // return list size if not found
        }

        template <size_t index, class Value, class... Rest>
        static constexpr size_t helper(std::tuple<pair<Key, Value>, Rest...>) {
            return index;
        }

        template <size_t index, class First, class... Rest>
        static constexpr size_t helper(std::tuple<First, Rest...>) {
            return helper<index + 1>(std::tuple<Rest...>());
        }

        static constexpr size_t value = helper<0>(typename T::tuple());
        static_assert(value < T::size, "type not foud in list");
    };

    template <class T, class Key>
    constexpr size_t map_element_index<T, Key>::value;

    //==============================================================================================
    template <class T>
    struct map_value_tuple : Box {
        static_assert(is_map<T>::value, "T is not a map");

        using type = typename list_map_t<T, second_t>::tuple;
    };

    template <class T>
    using map_value_tuple_t = unbox_t<map_value_tuple<T>>;

};  // namespace minimpl

//==================================================================================================
// TESTS

TEST_CASE("map tests") {
    struct key1 {};
    struct key2 {};
    struct key3 {};
    struct key4 {};
    using namespace minimpl;
    using m = map<pair<key1, int>, pair<key2, double>, pair<key3, char>>;
    struct m2 {};
    // using m3 = map<double, int>; // fails (as expected)
    // CHECK(m3::size == 2);
    CHECK(m::size == 3);
    CHECK(is_map<m>::value);
    CHECK(not is_map<m2>::value);
    CHECK(is_list<m>::value);
    CHECK(std::is_same<map_element_t<m, key1>, int>::value);
    CHECK(std::is_same<map_element_t<m, key2>, double>::value);
    CHECK(std::is_same<map_element_t<m, key3>, char>::value);
    CHECK(std::is_same<map_value_tuple_t<m>, std::tuple<int, double, char>>::value);
    // CHECK(std::is_same<map_element_t<m, key4>, NotFound>::value); // fails (as expected)
}

// TEST_CASE("Type map") {
//     using my_map = Map<Pair<prop1, int>, Pair<prop2, double>>;
//     using prop1_t = typename my_map::get<prop1>;
//     using prop2_t = typename my_map::get<prop2>;
//     constexpr int i1 = my_map::get_index<prop1>();
//     constexpr int i2 = my_map::get_index<prop2>();

//     CHECK(my_map::size() == 2);
//     CHECK((std::is_same<prop1_t, int>::value));
//     CHECK((std::is_same<prop2_t, double>::value));
//     CHECK(i1 == 0);
//     CHECK(i2 == 1);

//     using tuple_t = my_map::value_tuple_t;
//     CHECK((std::is_same<tuple<int, double>, tuple_t>::value));

//     using my_map2 = my_map::push_front<prop3, string>;
//     using prop3_t = typename my_map2::get<prop3>;
//     CHECK((std::is_same<prop3_t, string>::value));
//     CHECK(my_map2::get_index<prop3>() == 0);  // added in front
//     CHECK(my_map2::get_index<prop1>() == 1);
//     CHECK(my_map2::get_index<prop2>() == 2);
// }

// TEST_CASE("type map index to tag") {
//     using my_map = Map<Pair<prop1, int>, Pair<prop2, double>>;
//     using tag1 = my_map::get_tag<0>;
//     using tag2 = my_map::get_tag<1>;
//     CHECK((std::is_same<tag1, prop1>::value));
//     CHECK((std::is_same<tag2, prop2>::value));
// }

// TEST_CASE("Type map type_of") {
//     using my_map = Map<Pair<prop1, int&>, Pair<prop2, double>>;
//     CHECK((std::is_same<my_map::type_of<prop1>, int&>::value));
//     CHECK((std::is_same<my_map::type_of<prop2>, double>::value));
// }