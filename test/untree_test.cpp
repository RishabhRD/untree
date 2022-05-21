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

#include "untree.hpp"

#include <algorithm>
#include <iostream>

#include "in_memory_entry.hpp"
#include "test_include.hpp"

using entry_t = untree::in_memory_entry;
using file_t = entry_t::file_type;
using dir_t = entry_t::directory_type;

TEST_CASE("single depth tree") {
  constexpr std::string_view str =
      "├── document\n"
      "├── document.cc\n"
      "├── func\n";
  auto const res = untree::parse_tree<entry_t>("/home", str);
  REQUIRE(res.has_value());
  auto entry = res->first;
  REQUIRE(entry.index() == 1);
  auto root_dir = std::get<dir_t>(entry);
  dir_t const expected_dir{"/home",
                           {
                               file_t{"/home/document"},
                               file_t{"/home/document.cc"},
                               file_t{"/home/func"},
                           }};
  REQUIRE(expected_dir == root_dir);
}

TEST_CASE("nested depth tree") {
  constexpr std::string_view str =
      "├── constexpr\n"
      "│   ├── simple.cc\n"
      "│   └── sort.cc\n"
      "└── mvc\n"
      "├── document\n"
      "├── document.cc\n";
  auto const res = untree::parse_tree<entry_t>("/home", str);
  REQUIRE(res.has_value());
  auto entry = res->first;
  REQUIRE(entry.index() == 1);
  auto root_dir = std::get<dir_t>(entry);
  dir_t const expected_dir{"/home",
                           {
                               dir_t{"/home/constexpr",
                                     {
                                         file_t{"/home/constexpr/simple.cc"},
                                         file_t{"/home/constexpr/sort.cc"},
                                     }},
                               file_t{"/home/mvc"},
                               file_t{"/home/document"},
                               file_t{"/home/document.cc"},
                           }};
  REQUIRE(expected_dir == root_dir);
}
