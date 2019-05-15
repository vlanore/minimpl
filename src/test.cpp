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

#include <string>
#include "map.hpp"
using std::string;
using namespace minimpl;

struct prop1 {};
struct prop2 {};
struct prop3 {};

TEST_CASE("Type map") {
    using my_map = Map<Pair<prop1, int>, Pair<prop2, double>>;
    using prop1_t = typename my_map::get<prop1>;
    using prop2_t = typename my_map::get<prop2>;
    constexpr int i1 = my_map::get_index<prop1>();
    constexpr int i2 = my_map::get_index<prop2>();

    CHECK(my_map::size() == 2);
    CHECK((std::is_same<prop1_t, int>::value));
    CHECK((std::is_same<prop2_t, double>::value));
    CHECK(i1 == 0);
    CHECK(i2 == 1);

    using tuple_t = my_map::value_tuple_t;
    CHECK((std::is_same<tuple<int, double>, tuple_t>::value));

    using my_map2 = my_map::push_front<prop3, string>;
    using prop3_t = typename my_map2::get<prop3>;
    CHECK((std::is_same<prop3_t, string>::value));
    CHECK(my_map2::get_index<prop3>() == 0);  // added in front
    CHECK(my_map2::get_index<prop1>() == 1);
    CHECK(my_map2::get_index<prop2>() == 2);
}

TEST_CASE("type map index to tag") {
    using my_map = Map<Pair<prop1, int>, Pair<prop2, double>>;
    using tag1 = my_map::get_tag<0>;
    using tag2 = my_map::get_tag<1>;
    CHECK((std::is_same<tag1, prop1>::value));
    CHECK((std::is_same<tag2, prop2>::value));
}

TEST_CASE("Type map type_of") {
    using my_map = Map<Pair<prop1, int&>, Pair<prop2, double>>;
    CHECK((std::is_same<my_map::type_of<prop1>, int&>::value));
    CHECK((std::is_same<my_map::type_of<prop2>, double>::value));
}