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

#include <string_view>

namespace untree {

template <typename Directory>
struct directory_view;

template <typename Directory>
struct directory_view<Directory&> {
 private:
  Directory* dir_;

 public:
  using dir_type = Directory;

  explicit directory_view(Directory& dir) : dir_(&dir) {}

  auto add_file(std::string_view path) { dir_->add_file(path); }

  auto add_dir(std::string_view path) { return dir_->add_dir(path); }

  auto parent() { return dir_->parent(); }

  auto dir() -> Directory& { return *dir_; }
};

template <typename Directory>
struct directory_view<Directory&&> {
 private:
  Directory dir_;

 public:
  using dir_type = Directory;

  explicit directory_view(Directory dir) : dir_(std::move(dir)) {}

  auto add_file(std::string_view path) { dir_.add_file(path); }

  auto add_dir(std::string_view path) { return dir_.add_dir(path); }

  auto parent() { return dir_.parent(); }

  auto dir() const { return dir_; }
};

template <typename Directory>
directory_view(Directory&) -> directory_view<Directory&>;

template <typename Directory>
directory_view(Directory&&) -> directory_view<Directory&&>;
}  // namespace untree
