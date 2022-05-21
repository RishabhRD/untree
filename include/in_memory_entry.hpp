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
#include <vector>

#include "entry.hpp"

namespace untree {
struct in_memory_file {
  explicit in_memory_file(std::filesystem::path path)
      : path_(std::move(path)) {}

  [[nodiscard]] auto path() const -> std::filesystem::path const& {
    return path_;
  }

  auto path() -> std::filesystem::path& { return path_; }

  friend auto operator==(in_memory_file const&, in_memory_file const&) noexcept
      -> bool = default;

  constexpr static inline void create() {}

 private:
  std::filesystem::path path_;
};

template <typename File>
struct in_memory_directory_of
    : std::vector<untree::entry_t<in_memory_directory_of<File>>> {
  using file_type = File;
  using entry_type = untree::entry_t<in_memory_directory_of<file_type>>;

  explicit in_memory_directory_of(std::filesystem::path path)
      : path_(std::move(path)) {}

  explicit in_memory_directory_of(std::filesystem::path path,
                                  std::initializer_list<entry_type> entries)
      : std::vector<entry_type>(entries), path_(std::move(path)) {}

  [[nodiscard]] auto path() const -> std::filesystem::path const& {
    return path_;
  }

  auto path() -> std::filesystem::path& { return path_; }

  constexpr static inline void create() {}

  using std::vector<entry_type>::vector;

  friend auto operator==(in_memory_directory_of const&,
                         in_memory_directory_of const&) noexcept
      -> bool = default;

 private:
  std::filesystem::path path_;
};

using in_memory_directory = in_memory_directory_of<in_memory_file>;

using in_memory_entry = entry_t<in_memory_directory>;
}  // namespace untree
