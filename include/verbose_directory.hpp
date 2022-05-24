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

#include "directory_concepts.hpp"
#include "directory_view.hpp"
#include "disk_directory.hpp"
#include "entry.hpp"
#include "in_memory_directory.hpp"

namespace untree {

template <directory Directory>
struct verbose_directory;

template <directory Directory>
struct verbose_directory<Directory&> {
  explicit verbose_directory(Directory& dir) : dir_{&dir} {}

  auto add_file(std::string_view name) {
    dir_->add_file(name);
    std::cout << "touch " << name << '\n';
  }

  auto add_dir(std::string_view name) {
    auto res = dir_->add_dir(name);
    std::cout << "mkdir " << name << '\n';
    return directory_view{verbose_directory{res.dir()}};
  }

  auto parent() -> directory_view<verbose_directory<Directory&>&&> {
    return directory_view{verbose_directory{dir_->parent().dir()}};
  }

  auto dir() -> Directory& { return *dir_; }

 private:
  Directory* dir_;
};

template <directory Directory>
struct verbose_directory<Directory&&> {
  explicit verbose_directory(Directory dir) : dir_{std::move(dir)} {}

  auto add_file(std::string_view name) {
    dir_.add_file(name);
    std::cout << "touch " << name << '\n';
  }

  auto add_dir(std::string_view name) {
    auto res = dir_.add_dir(name);
    std::cout << "mkdir " << name << '\n';
    return directory_view{verbose_directory{res.dir()}};
  }

  auto parent() -> directory_view<verbose_directory<Directory&&>&&> {
    return directory_view{verbose_directory{dir_.parent().dir()}};
  }

  auto dir() const { return dir_; }

 private:
  Directory dir_;
};

template <typename Directory>
verbose_directory(Directory&) -> verbose_directory<Directory&>;

template <typename Directory>
verbose_directory(Directory&&) -> verbose_directory<Directory&&>;

}  // namespace untree
