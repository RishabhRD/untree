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

#include "verbose_entry.hpp"

#include <concepts>

#include "concepts.hpp"
#include "in_memory_entry.hpp"
#include "test_include.hpp"

using file_t = untree::verbose_file<untree::in_memory_file>;
using dir_t = untree::verbose_directory<untree::in_memory_directory>;
using entry_t = untree::verbose_entry<untree::in_memory_entry>;

TEST_CASE("file concepts test") { static_assert(untree::file<file_t>); }

TEST_CASE("directory concepts test") {
  static_assert(untree::directory<dir_t>);
  static_assert(std::same_as<dir_t::file_type, file_t>);
}

TEST_CASE("entry concepts test") {
  static_assert(untree::entry<entry_t>);
  static_assert(std::same_as<entry_t::file_type, file_t>);
  static_assert(std::same_as<entry_t::directory_type, dir_t>);
}
