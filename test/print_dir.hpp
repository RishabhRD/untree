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

#include "in_memory_directory.hpp"

namespace test_util {
template <typename T>
void print_n_times(int n, T ele) {
  for (int i = 0; i < n; ++i) {
    std::cout << ele;
  }
}

inline void print_file(int depth, untree::in_memory_file const& file) {
  print_n_times(depth, ' ');
  std::cout << file.path().native() << '\n';
}

inline void print_dir(int depth,  // NOLINT
                      untree::in_memory_directory const& dir) {
  print_n_times(depth, ' ');
  std::cout << dir.path().native() << '\n';
  for (auto const& entry : dir) {
    if (entry.is_file()) {
      print_file(depth + 2, entry.get_file());
    } else {
      print_dir(depth + 2, entry.get_dir());
    }
  }
}

inline void print_dir(untree::in_memory_directory const& dir) {
  print_dir(0, dir);
}
}  // namespace test_util
