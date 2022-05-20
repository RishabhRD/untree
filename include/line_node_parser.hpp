/*
 * MIT License
 *
 * Copyright (c) 2022 Rishabh Dwivedi<rishabhdwivedi17@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "line_node.hpp"
#include "parser.hpp"

namespace untree {
namespace detail {
inline constexpr auto seperator_parser =
    parser::str("    ")                     //
    | parser::or_with(parser::str("└── "))  //
    | parser::or_with(parser::str("├── "))  //
    | parser::or_with(parser::str("│   "))  //
    | parser::or_with(parser::str("│\u00A0\u00A0 "));

inline constexpr auto depth_count_parser =
    parser::many(seperator_parser, 0, [](auto num, auto) { return num + 1; });

inline constexpr auto name_parser =
    parser::many_if([](auto c) { return c != '\n'; }) |
    parser::ignore(parser::symbol('\n'));
}  // namespace detail

inline constexpr auto line_node_parser =
    parser::then(detail::depth_count_parser, [](auto depth) {
      return parser::transform(detail::name_parser, [depth](auto str) {
        return line_node{depth, str};
      });
    });

}  // namespace untree
