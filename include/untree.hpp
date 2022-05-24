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

#include "directory_concepts.hpp"
#include "entry.hpp"
#include "entry_parser.hpp"
#include "parser.hpp"

namespace untree {
namespace detail {

template <typename DirView>
constexpr auto get_parent_n(int n, DirView dir) {
  for (int i = 0; i < n; ++i) {
    dir = dir.parent();
  }
  return dir;
}
}  // namespace detail

// creates directory structure of str inside root_dir
// precondition: root_dir is already created
template <is_directory_view DirView>
auto parse_tree(DirView root_dir, std::string_view str)
    -> parser::parsed_t<DirView> {
  struct state_t {
    DirView dir;
    int depth;
  };
  auto res = parser::many(
      entry_parser, state_t{root_dir, 0}, [](state_t cur, entry entry) {
        auto dir = detail::get_parent_n(cur.depth - entry.depth + 1,
                                        std::move(cur.dir));
        auto depth = entry.depth - 1;
        if (entry.kind == entry_type::file) {
          dir.add_file(entry.name);
        } else {
          dir = std::move(dir).add_dir(entry.name);
          ++depth;
        }
        return state_t{std::move(dir), depth};
      })(str);
  if (!res) return std::nullopt;
  return std::pair{root_dir, res->second};
}
}  // namespace untree
