# Minimpl

[![Build Status](https://travis-ci.org/vlanore/minimpl.svg?branch=master)](https://travis-ci.org/vlanore/minimpl) [![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/vlanore/minimpl.svg)](https://lgtm.com/projects/g/vlanore/minimpl/context:cpp) [![licence CeCILL](https://img.shields.io/badge/license-CeCILL--C-blue.svg)](http://www.cecill.info/licences.en.html)

A C++14 header-only library that provides template metaprogramming datastructures with a focus on simplicity.
Minimpl provides the following structures:
* `is_type`, the metaprogramming type identity (similar to C++20's `std::type_identity`);
* `type_pair`, a pair of types (actually a `std::tuple` with new operations);
* `type_list`, a list of types (actually a `std::tuple` with new operations);
* `type_map`, a multimap of types (actually a `type_list` of `type_pair` elements with operations on top).

Minimpl tries to respect the conventions used in the standard library as much as possible.
Here is a short usage example with `type_list`:
```c++
using l = type_list<char, double, int>;
constexpr size_t i_double = list_find<int, l>::value;  // 2
constexpr bool l_is_a_list = is_list<l>::value; // true
using e1 = list_element_t<1, l>;
// alternatively, using e1 = typename list_element<1, l>::type;
```

Minimpl uses the following header-only libraries:
* doctest - [github repo](https://github.com/onqtam/doctest) - [MIT license](utils/LICENSE.txt)
