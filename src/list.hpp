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

    struct List {};  // is a list

    template <class... Elements>
    struct list : List {
        using boxes = std::tuple<box<Elements>...>;
        static constexpr size_t size = sizeof...(Elements);
    };

    template <class... Elements>
    constexpr size_t list<Elements...>::size;

    template <class T>
    using is_list = std::is_base_of<List, T>;

    template <class L, size_t index>
    struct list_element : Box {
        static_assert(is_list<L>::value, "L is not a list");
        static_assert(index < L::size, "index out of bounds");
        using type = unbox_t<std::tuple_element_t<index, typename L::boxes>>;
    };

    template <class T, size_t index>
    using list_element_t = unbox_t<list_element<T, index>>;

    template <class L, class ToFind>
    struct find_element {
        static_assert(is_list<L>::value, "parameter L is not a list");

        template <size_t index>
        static constexpr size_t helper(std::tuple<>) {
            return index;  // return list size if not found
        }

        template <size_t index, class... Rest>
        static constexpr size_t helper(std::tuple<box<ToFind>, Rest...>) {
            return index;
        }

        template <size_t index, class First, class... Rest>
        static constexpr size_t helper(std::tuple<First, Rest...>) {
            return helper<index + 1>(std::tuple<Rest...>());
        }

        static constexpr size_t value = helper<0>(typename L::boxes());
        static_assert(value < L::size, "type not foud in list");
    };

    template <class L, class ToFind>
    constexpr size_t find_element<L, ToFind>::value;

    template <class L, class ToAdd>
    struct list_push_front : Box {
        static_assert(is_list<L>::value, "parameter L is not a list");

        template <class... Elements>
        static auto helper(list<Elements...>) {
            return list<ToAdd, Elements...>();
        }

        using type = decltype(helper(L()));
    };

    template <class L, class ToAdd>
    using list_push_front_t = unbox_t<list_push_front<L, ToAdd>>;

    template <class L, template <class> class F, class Combinator>
    struct map_and_fold_list {
        static_assert(is_list<L>::value, "L is not a list");
        static_assert(L::size > 0, "L is empty");
        using T = decltype(F<list_element_t<L, 0>>::value);

        template <class Last>
        static constexpr T helper(std::tuple<box<Last>>) {
            return F<Last>::value;
        }

        template <class First, class Second, class... Rest>
        static constexpr T helper(std::tuple<box<First>, Second, Rest...>) {
            return Combinator()(F<First>::value, helper(std::tuple<Second, Rest...>()));
        }

        static constexpr T value = helper(typename L::boxes());
    };

    template <class L, template <class> class F, class Combinator>
    constexpr
        typename map_and_fold_list<L, F, Combinator>::T map_and_fold_list<L, F, Combinator>::value;

};  // namespace minimpl

//==================================================================================================
// TESTS
TEST_CASE("List tests") {
    using namespace minimpl;
    using l = list<int, double, char>;
    struct l2 {};  // not a list
    CHECK(l::size == 3);
    CHECK(std::is_same<list_element_t<l, 0>, int>::value);
    CHECK(std::is_same<list_element_t<l, 1>, double>::value);
    CHECK(std::is_same<list_element_t<l, 2>, char>::value);

    CHECK(find_element<l, int>::value == 0);
    CHECK(find_element<l, double>::value == 1);
    CHECK(find_element<l, char>::value == 2);

    using l3 = list<int, list<>, double>;
    CHECK(map_and_fold_list<l3, is_list, std::logical_or<bool>>::value == true);
    CHECK(map_and_fold_list<l, is_list, std::logical_or<bool>>::value == false);

    using l4 = list_push_front_t<l, long>;
    CHECK(find_element<l4, long>::value == 0);
    CHECK(find_element<l4, int>::value == 1);
}