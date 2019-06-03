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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "is_type.hpp"
#include "type_list.hpp"
#include "type_map.hpp"
#include "type_pair.hpp"

TEST_CASE("is_type tests") {
    using T = is_type<double>;
    CHECK(std::is_same<typename T::type, double>::value);
    CHECK(std::is_same<is_type_t<T>, double>::value);
}

TEST_CASE("Pair test") {
    using p = type_pair<int, double>;
    struct p2 {};  // not a pair

    CHECK(std::is_same<first_t<p>, int>::value);
    CHECK(std::is_same<second_t<p>, double>::value);
    // CHECK(std::is_same<second_t<p2>, double>::value);  // fails (as it should)
    CHECK(is_pair<p>::value);
    CHECK(not is_pair<p2>::value);
}

TEST_CASE("List tests") {
    using l = type_list<int, double, char>;
    struct l2 {};  // not a list
    CHECK(is_list<l>::value);
    CHECK(not is_list<l2>::value);

    CHECK(list_size<l>::value == 3);
    CHECK(std::is_same<list_element_t<0, l>, int>::value);
    CHECK(std::is_same<list_element_t<1, l>, double>::value);
    CHECK(std::is_same<list_element_t<2, l>, char>::value);

    CHECK(list_find<int, l>::value == 0);
    CHECK(list_find<double, l>::value == 1);
    CHECK(list_find<char, l>::value == 2);
    CHECK(list_contains<int, l>::value);
    CHECK(list_contains<char, l>::value);
    CHECK(!list_contains<long, l>::value);
    CHECK(!list_contains<std::string, l>::value);

    using l3 = type_list<int, type_list<>, double>;
    CHECK(list_map_to_value<is_list, bool, l3>::value[0] == false);
    CHECK(list_map_to_value<is_list, bool, l3>::value[1] == true);
    CHECK(list_map_to_value<is_list, bool, l3>::value[2] == false);
    CHECK(list_reduce_to_value<is_list, std::logical_or<bool>, bool, false, l3>::value == true);
    CHECK(list_reduce_to_value<is_list, std::logical_or<bool>, bool, false, l>::value == false);

    using l4 = list_push_front_t<long, l>;
    CHECK(list_find<long, l4>::value == 0);
    CHECK(list_find<int, l4>::value == 1);

    using l5 = type_list<is_type<int>, is_type<char>>;
    CHECK(std::is_same<list_map_t<is_type_t, l5>, type_list<int, char>>::value);
}

TEST_CASE("map tests") {
    struct key1 {};
    struct key2 {};
    struct key3 {};
    struct key4 {};
    using m = type_map<type_pair<key1, int>, type_pair<key2, double>, type_pair<key3, char>>;
    struct m2 {};
    using m3 = type_map<type_pair<key1, double>, double, type_pair<char, double>>;
    CHECK(is_map<m>::value);
    CHECK(is_list<m>::value);
    CHECK(not is_map<m2>::value);
    CHECK(not is_map<m3>::value);

    CHECK(std::is_same<map_element_t<key1, m>, int>::value);
    CHECK(std::is_same<map_element_t<key2, m>, double>::value);
    CHECK(std::is_same<map_element_t<key3, m>, char>::value);
    CHECK(std::is_same<map_value_list_t<m>, type_list<int, double, char>>::value);
    CHECK(std::is_same<map_key_list_t<m>, type_list<key1, key2, key3>>::value);
    CHECK(map_element_index<key1, m>::value == 0);
    CHECK(map_element_index<key2, m>::value == 1);
    CHECK(map_element_index<key3, m>::value == 2);

    using m4 = map_push_front_t<key4, char, m>;
    CHECK(std::is_same<map_element_t<key2, m4>, double>::value);
    CHECK(std::is_same<map_element_t<key4, m4>, char>::value);
    CHECK(map_element_index<key1, m4>::value == 1);
    CHECK(map_element_index<key2, m4>::value == 2);
    CHECK(map_element_index<key4, m4>::value == 0);
}