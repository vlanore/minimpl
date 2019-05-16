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

#include "doctest.h"
#include "maybe.hpp"

namespace minimpl {
    // type tags
    struct Box {};  // type tag for is_box trait

    // "box" struct used to pass type info around without needing to instantiate objects
    template <class T>
    struct box : Box {
        using type = T;
    };

    // default value
    template <>
    struct default_value<Box> {
        using type = box<Invalid>;
    };

    // is_box type trait
    template <class T>
    using is_box = has_tag<Box, T>;

    using maybe_box = maybe<Box>;

    template <class T>
    struct unbox_f : Function<RawType> {
        using result = typename T::type;
    };

    // box_t to get box::type without writing "typename box::type"
    template <class T>
    using unbox = get<bind<maybe_box::make<T>, unbox_f>>;

};  // namespace minimpl

//==================================================================================================
// TESTS
TEST_CASE("Box tests") {
    using namespace minimpl;

    using t = box<double>;
    struct t2 {};  // NOT a box
    CHECK(std::is_same<unbox<t>, double>::value);
    CHECK(std::is_same<unbox<t2>, NotA<Box>>::value);
    CHECK(is_box<t>::value);
    CHECK(not is_box<t2>::value);
}