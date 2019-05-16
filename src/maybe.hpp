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

#include <type_traits>
#include "doctest.h"

namespace minimpl {

    // type tags
    struct Error {};  // tag for error types
    template <class T>
    struct NotA : Error {};  // error : wrong type
    struct NoError {};       // to used in place of error when no error
    struct Invalid {};       // to build default values
    struct Monad {};         // to mark monads
    template <class Return>
    struct Function {  // tag for metafunctions
        using return_type = Return;
    };
    template <class Tag>
    struct default_value {
        using type = Invalid;
    };
    struct RawType {};
    template <>
    struct default_value<RawType> {
        using type = Invalid;
    };

    // type traits
    template <class Tag, class T>
    struct has_tag : std::is_base_of<Tag, T> {};

    template <class T>
    struct has_tag<RawType, T> : std::true_type {};

    template <class T>
    using is_error = has_tag<Error, T>;

    // maybe class : represents either a value or an error
    template <class Tag, class DefaultValue = typename default_value<Tag>::type>
    struct maybe {
        template <class E>
        struct make_error : Monad {
            using error = E;
            using value = DefaultValue;
        };

        template <class T, bool has_tag = has_tag<Tag, T>::value,
                  bool is_error = is_error<T>::value>
        struct make : Monad {
            using error = NotA<Tag>;
            using value = DefaultValue;
        };

        template <class T, bool has_tag>
        struct make<T, has_tag, true> : Monad {
            using error = T;
            using value = DefaultValue;
        };

        template <class T>
        struct make<T, true, false> : Monad {
            using error = NoError;
            using value = T;
        };
    };

    // maybe class operations
    template <class MX>
    using is_valid = std::is_same<NoError, typename MX::error>;

    template <class MX>
    using get_value = typename MX::value;

    template <class MX>
    using get_error = typename MX::error;

    template <class MX>
    using get = std::conditional_t<is_valid<MX>::value, get_value<MX>, get_error<MX>>;

    template <class X, template <class> class F>
    using apply = typename F<X>::result;

    template <class X, template <class> class F>
    using return_type = typename F<X>::return_type;

    template <class MX, template <class> class F>
    using bind = std::conditional_t<
        is_valid<MX>::value,
        typename maybe<return_type<get_value<MX>, F>>::template make<apply<get_value<MX>, F>>, MX>;

};  // namespace minimpl

//==================================================================================================
// TESTS
namespace testing {
    struct Tag {};

    template <class T>
    struct MyType : Tag {
        using type = T;
    };

    template <class T>
    struct f {
        using result = MyType<T>;
        using return_type = Tag;
    };
};  // namespace testing

TEST_CASE("Maybe test") {
    using namespace minimpl;
    using namespace testing;
    using maybe_mytype = maybe<Tag, MyType<Invalid>>;

    using t1 = maybe_mytype::make<double>;
    using t2 = maybe_mytype::make<MyType<double>>;
    CHECK(not is_valid<t1>::value);
    CHECK(is_valid<t2>::value);
    CHECK(std::is_same<get<bind<t1, f>>, NotA<Tag>>::value);
    CHECK(std::is_same<get<bind<t2, f>>, MyType<MyType<double>>>::value);
    CHECK(std::is_same<get<bind<bind<t2, f>, f>>, MyType<MyType<MyType<double>>>>::value);
}