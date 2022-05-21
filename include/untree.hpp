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
#include <string>

#include "concepts.hpp"
#include "line_node.hpp"
#include "line_node_parser.hpp"
#include "parser.hpp"

namespace untree {
template <directory Directory>
class parse_tree_creator {
  Directory root_dir;
  int depth;
  using file_type = typename Directory::file_type;
  using dir_type = Directory;
  using entry_type = entry_t<Directory>;

 public:
  parse_tree_creator(dir_type root_dir_arg, int depth_arg)
      : root_dir(std::move(root_dir_arg)), depth(depth_arg) {}

  auto operator()(std::string_view str) const -> parser::parsed_t<entry_type> {
    auto parser =
        line_node_parser | parser::if_satisfies([this](line_node node) {
          return node.depth == depth;
        }) |
        parser::then([this](line_node node) {
          // TODO: precondition for parse_tree is dir should be created that is
          // not being satisfied here.
          return parse_tree_creator(dir_type{root_dir.path() / node.path},
                                    depth + 1) |
                 parser::or_with(parser::always(
                     entry_type{file_type{root_dir.path() / node.path}}));
        }) |
        parser::many1(root_dir,
                      [](dir_type dir, entry_type entry) {
                        std::visit([](auto& e) { e.create(); }, entry);
                        dir.push_back(std::move(entry));
                        return dir;
                      }) |
        parser::transform([](dir_type dir) { return entry_type{dir}; });
    return std::invoke(std::move(parser), str);
  }
};

// creates directory structure of str inside root_dir
// precondition: root_dir is already created
template <directory Directory>
auto parse_tree(Directory root_dir, std::string_view str) {
  return parse_tree_creator<Directory>{root_dir, 1}(str);
}
}  // namespace untree
