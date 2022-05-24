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

#include "line_info_parser.hpp"

#include "line_info.hpp"
#include "test_include.hpp"

TEST_CASE("parse the matching string of depth 1") {
  constexpr auto str = "├── document";
  auto const res = untree::line_info_parser(str);
  REQUIRE(res.has_value());
  REQUIRE(res->first == untree::line_info{"document", 1});
}

TEST_CASE("parse the matching string of depth 2") {
  constexpr auto str = "│   ├── simple.cc";
  auto const res = untree::line_info_parser(str);
  REQUIRE(res.has_value());
  REQUIRE(res->first == untree::line_info{"simple.cc", 2});
}

TEST_CASE("parse the non-matching string") {
  constexpr auto str = "   ";
  auto const res = untree::line_info_parser(str);
  REQUIRE_FALSE(res.has_value());
}
