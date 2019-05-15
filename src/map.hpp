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
#include "utils.hpp"
using std::tuple;

namespace minimpl {
    // template <class... Decls>
    // struct Map {
    //     //==========================================================================================
    //     // get / get_index

    //     // return of get_helper
    //     template <class Value, int _index>
    //     struct GetReturn {
    //         using value = Value;
    //         static constexpr int index = _index;
    //     };

    //     // helper to recursively look for key in map (base case)
    //     template <class Key, int index>
    //     static auto get_helper(tuple<>) {
    //         return NotFound();
    //     }

    //     // helper to recursively look for key in map
    //     template <class RequestedKey, int index, class Key, class Value, class... DeclRest>
    //     static auto get_helper(tuple<Pair<Key, Value>, DeclRest...>) {
    //         using if_equal = GetReturn<Value, index>;
    //         using if_not_equal =
    //             decltype(get_helper<RequestedKey, index + 1>(tuple<DeclRest...>()));
    //         constexpr bool equality = std::is_same<RequestedKey, Key>::value;
    //         return std::conditional_t<equality, if_equal, if_not_equal>();
    //     }  // note that return value is a default-constructed Value object (FIXME?)

    //     // type alias for the result of the get function
    //     template <class Key>
    //     using get = typename decltype(get_helper<Key, 0>(tuple<Decls...>()))::value;

    //     // type alias for the result of the get function
    //     template <class Key>
    //     static constexpr int get_index() {
    //         return decltype(get_helper<Key, 0>(tuple<Decls...>()))::index;
    //     }

    //     //==========================================================================================
    //     // push_front
    //     template <class Key, class Type>
    //     using push_front = decltype(Map<Pair<Key, Type>, Decls...>());

    //     //==========================================================================================
    //     // value_tuple_t (tuple type alias)

    //     // base case of helper below
    //     static auto value_tuple_helper(tuple<>) { return TypeList<>(); }

    //     // goes through the list of Key/Value pairs and compiles a list of Values
    //     template <class Key, class Value, class... Rest>
    //     static auto value_tuple_helper(tuple<Pair<Key, Value>, Rest...>) {
    //         auto recursive_call = value_tuple_helper(tuple<Rest...>());
    //         return recursive_call.template add_type_front<Value>();
    //     }

    //     // type alias that corresponds to a tuple of all the values
    //     // (to be used in tagged tuple, at least)
    //     using value_tuple_t = typename decltype(value_tuple_helper(tuple<Decls...>()))::tuple;

    //     //==========================================================================================
    //     // other info on map (index to tag, size, type of field)

    //     // returns tag associated with index
    //     template <int index>
    //     using get_tag = typename std::tuple_element_t<index, tuple<Decls...>>::tag;

    //     // number of entries in map (static int)
    //     static constexpr size_t size() { return sizeof...(Decls); }

    //     // alias to type of field associated to specified tag
    //     template <class Tag>
    //     using type_of = typename std::tuple_element_t<get_index<Tag>(), value_tuple_t>;
    // };
};  // namespace minimpl

// using std::string;
// using namespace minimpl;

// struct prop1 {};
// struct prop2 {};
// struct prop3 {};

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