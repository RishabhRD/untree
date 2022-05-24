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
#include <iostream>
#include <istream>
#include <optional>
#include <string>

#include "entry.hpp"
#include "line_info.hpp"
#include "line_info_parser.hpp"

namespace untree {
namespace detail {

inline auto parse_info_from_stream(std::istream& in)
    -> std::optional<line_info> {
  if (std::string str; std::getline(in, str)) {
    if (auto info = line_info_parser(str)) {
      return info->first;
    }
  }
  return {};
}

inline auto get_file_type(int old_depth, int new_depth) {
  if (new_depth > old_depth) return entry_type::directory;
  return entry_type::file;
}

}  // namespace detail

class end_entry_iterator {};

class entry_iterator {
  std::istream* in{};
  std::optional<line_info> cur_line;
  std::optional<line_info> next_line;

 public:
  explicit entry_iterator(std::istream* in_arg) : in(in_arg) {
    cur_line = detail::parse_info_from_stream(*in);
    next_line = detail::parse_info_from_stream(*in);
  }

  friend auto operator==(entry_iterator const& cur,
                         end_entry_iterator /*unused*/) -> bool {
    return cur.cur_line == std::nullopt;
  }

  friend auto operator!=(entry_iterator const& cur,
                         end_entry_iterator /*unused*/) -> bool {
    return cur.cur_line != std::nullopt;
  }

  auto operator++() -> entry_iterator& {
    cur_line = std::move(next_line);
    next_line = detail::parse_info_from_stream(*in);
    return *this;
  }

  // precondition: cur_line != nullopt
  auto operator*() const -> entry {
    return {cur_line->name, cur_line->depth,
            detail::get_file_type(cur_line->depth, get_depth_from_next_line())};
  }

 private:
  [[nodiscard]] auto get_depth_from_next_line() const -> int {
    if (next_line) {
      return next_line->depth;
    }
    return 0;
  }
};

class entry_range {
  std::istream* in;

 public:
  explicit entry_range(std::istream& in_arg) : in(&in_arg) {}

  auto begin() { return entry_iterator{in}; }

  [[nodiscard]] static auto end() { return end_entry_iterator{}; }
};

}  // namespace untree
