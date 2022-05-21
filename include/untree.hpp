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

#include <filesystem>
#include <iostream>
#include <string>

#include "concepts.hpp"
#include "line_node.hpp"
#include "line_node_parser.hpp"
#include "parser.hpp"

namespace untree {
template <entry Entry>
class parse_tree_creator {
  std::filesystem::path cur_path;
  int depth;
  using file_type = typename Entry::file_type;
  using dir_type = typename Entry::directory_type;

 public:
  parse_tree_creator(std::filesystem::path path_arg, int depth_arg)
      : cur_path(std::move(path_arg)), depth(depth_arg) {}

  auto operator()(std::string_view str) const -> parser::parsed_t<Entry> {
    auto parser =
        line_node_parser | parser::if_satisfies([this](line_node node) {
          return node.depth == depth;
        }) |
        parser::then([this](line_node node) {
          return parse_tree_creator(cur_path / node.path, depth + 1) |
                 parser::or_with(
                     parser::always(Entry{file_type{cur_path / node.path}}));
        }) |
        parser::many1(dir_type{cur_path},
                      [](dir_type dir, Entry entry) {
                        dir.push_back(std::move(entry));
                        return dir;
                      }) |
        parser::transform([](dir_type dir) { return Entry{dir}; });
    return std::invoke(std::move(parser), str);
  }
};

template <typename Entry>
auto parse_tree(std::filesystem::path cur_path, std::string_view str) {
  return parse_tree_creator<Entry>{cur_path, 1}(str);
}
}  // namespace untree
