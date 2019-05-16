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
#include "box.hpp"
#include "doctest.h"

namespace minimpl {
    // type tags
    struct IsList {};       // is a list
    struct NotAList {};     // passed a param that should have been a list but was not
    struct OutOfBounds {};  // requested index is out of bounds
    struct NotFound {};     // could not find element

    template <class... Elements>
    struct list : IsList {
        using boxes = std::tuple<box<Elements>...>;
        static constexpr size_t size = sizeof...(Elements);
    };

    template <class... Elements>
    constexpr size_t list<Elements...>::size;

    template <class T>
    using is_list = std::is_base_of<IsList, T>;

    template <class T, bool = is_list<T>::value>
    struct maybe_list : Box {
        using type = list<NotAList>;
    };

    template <class T>
    struct maybe_list<T, true> : Box {
        using type = T;
    };

    template <class T, size_t index, bool within_bounds = (index < box_t<maybe_list<T>>::size),
              bool is_list = is_list<T>::value>
    struct maybe_element : Box {
        using type = NotAList;
    };

    template <class T, size_t index>
    struct maybe_element<T, index, false, true> : Box {
        using type = OutOfBounds;
    };

    template <class T, size_t index>
    struct maybe_element<T, index, true, true> : Box {
        using type = box_t<std::tuple_element_t<index, typename T::boxes>>;
    };

    template <class T, size_t index>
    using element_t = box_t<maybe_element<T, index>>;

};  // namespace minimpl

TEST_CASE("List tests") {
    using namespace minimpl;
    using l = list<int, double, char>;
    struct l2 {};  // not a list
    CHECK(l::size == 3);
    CHECK(std::is_same<element_t<l, 0>, int>::value);
    CHECK(std::is_same<element_t<l, 1>, double>::value);
    CHECK(std::is_same<element_t<l, 2>, char>::value);
    CHECK(std::is_same<element_t<l, 3>, OutOfBounds>::value);
    CHECK(std::is_same<element_t<l2, 1>, NotAList>::value);
}