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

#include "entry_range.hpp"

#include <sstream>

#include "test_include.hpp"

TEST_CASE("tree input stream correctness") {
  constexpr auto str =
      "├── document\n"
      "├── document.cc\n"
      "├── func\n";
  std::stringstream in(str);
  untree::entry_range rng{in};
  auto fst = rng.begin();
  REQUIRE(*fst == untree::entry{"document", 1, untree::entry_type::file});
  ++fst;
  REQUIRE(*fst == untree::entry{"document.cc", 1, untree::entry_type::file});
  ++fst;
  REQUIRE(*fst == untree::entry{"func", 1, untree::entry_type::file});
}

TEST_CASE("tree input with non parsable lines stream correctness") {
  constexpr auto str =
      "├── document\n"
      "├── document.cc\n"
      "├── func\n"
      " \n";
  std::stringstream in(str);
  untree::entry_range rng{in};
  auto fst = rng.begin();
  REQUIRE(*fst == untree::entry{"document", 1, untree::entry_type::file});
  ++fst;
  REQUIRE(*fst == untree::entry{"document.cc", 1, untree::entry_type::file});
  ++fst;
  REQUIRE(*fst == untree::entry{"func", 1, untree::entry_type::file});
  ++fst;
  REQUIRE(fst == untree::end_entry_iterator{});
}
