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

#include "box.hpp"
#include "doctest.h"

namespace minimpl {
    // type tags
    struct Pair {};  // object is a pair

    // a pair of types
    template <class First, class Second>
    struct pair : Pair {
        using first = First;
        using second = Second;
    };

    template <>
    struct default_value<Pair> {
        using type = pair<Invalid, Invalid>;
    };

    // type trait for "T is a pair"
    template <class T>
    using is_pair = std::is_base_of<Pair, T>;

    using maybe_pair = maybe<Pair>;

    template <class T>
    struct first_f : Function<RawType> {
        using result = typename T::first;
    };

    template <class T>
    struct second_f : Function<RawType> {
        using result = typename T::second;
    };

    // get first pair element without "typename *::type"
    template <class T>
    using pair_first = get<bind<maybe_pair::make<T>, first_f>>;

    // get second pair element without "typename *::type"
    template <class T>
    using pair_second = get<bind<maybe_pair::make<T>, second_f>>;

};  // namespace minimpl

TEST_CASE("Pair test") {
    using namespace minimpl;

    using p = pair<int, double>;
    struct p2 {};  // not a pair

    CHECK(std::is_same<pair_first<p>, int>::value);
    CHECK(std::is_same<pair_second<p>, double>::value);
    CHECK(std::is_same<pair_second<p2>, NotA<Pair>>::value);
    CHECK(is_pair<p>::value);
    CHECK(not is_pair<p2>::value);
}