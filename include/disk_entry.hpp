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

#include <entry.hpp>
#include <filesystem>
#include <fstream>

namespace untree {

struct disk_file {
  explicit disk_file(std::filesystem::path path) : path_(std::move(path)) {}

  void create() {
    if (!std::filesystem::exists(path_)) {
      std::ofstream of(path_.native());
    }
  }

  [[nodiscard]] auto path() const -> std::filesystem::path const& {
    return path_;
  }

  auto path() -> std::filesystem::path& { return path_; }

 private:
  std::filesystem::path path_;
};

template <typename File>
struct disk_directory_of {
  using file_type = File;
  explicit disk_directory_of(std::filesystem::path path)
      : path_(std::move(path)) {}

  constexpr static void push_back(
      entry_t<disk_directory_of<File>> const& /*unused*/) {}

  void create() {
    if (!std::filesystem::exists(path_)) {
      std::filesystem::create_directory(path_);
    }
  }

  [[nodiscard]] auto path() const -> std::filesystem::path const& {
    return path_;
  }

  auto path() -> std::filesystem::path& { return path_; }

 private:
  std::filesystem::path path_;
};

using disk_directory = disk_directory_of<disk_file>;
using disk_entry = entry_t<disk_directory>;
}  // namespace untree
