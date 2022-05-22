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

#include <filesystem>
#include <iostream>

#include "concepts.hpp"
#include "entry.hpp"

namespace untree {
template <file File>
struct verbose_file {
  explicit verbose_file(std::filesystem::path path) : file_(std::move(path)) {}

  void create() {
    file_.create();
    std::cout << "touch " << file_.path().native() << '\n';
  }

  [[nodiscard]] auto path() const -> std::filesystem::path const& {
    return file_.path();
  }

  auto path() -> std::filesystem::path& { return file_.path(); }

  [[nodiscard]] auto get_base_file() const -> File const& { return file_; }

  auto get_base_file() -> File& { return file_; }

 private:
  File file_;
};

template <directory Directory>
struct verbose_directory {
  using file_type = verbose_file<typename Directory::file_type>;
  explicit verbose_directory(std::filesystem::path path)
      : dir_(std::move(path)) {}

  void create() {
    dir_.create();
    std::cout << "mkdir " << dir_.path().native() << '\n';
  }

  [[nodiscard]] auto path() const -> std::filesystem::path const& {
    return dir_.path();
  }

  auto path() -> std::filesystem::path& { return dir_.path(); }

  void push_back(entry_t<verbose_directory> entry) {
    if (entry.index() == 0)
      dir_.push_back(std::get<0>(std::move(entry)).get_base_file());
    else
      dir_.push_back(std::get<1>(std::move(entry)).get_base_dir());
  }

  [[nodiscard]] auto get_base_dir() const -> Directory const& { return dir_; }

  auto get_base_dir() -> Directory& { return dir_; }

 private:
  Directory dir_;
};

template <entry Entry>
using verbose_entry =
    entry_t<verbose_directory<typename Entry::directory_type>>;

}  // namespace untree
