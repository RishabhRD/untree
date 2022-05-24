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
#include <variant>
#include <vector>

#include "directory_view.hpp"
#include "entry.hpp"

namespace untree {
namespace detail {
template <typename DirectoryType>
struct entry_t
    : public std::variant<typename DirectoryType::file_type, DirectoryType> {
  using file_type = typename DirectoryType::file_type;
  using directory_type = DirectoryType;
  using std::variant<file_type, DirectoryType>::variant;

  [[nodiscard]] auto get_dir() const -> directory_type const& {
    return std::get<1>(*this);
  }

  auto get_dir() -> directory_type& { return std::get<1>(*this); }

  [[nodiscard]] auto get_file() const -> file_type const& {
    return std::get<0>(*this);
  }

  auto get_file() -> directory_type& { return std::get<0>(*this); }

  [[nodiscard]] auto is_file() const { return this->index() == 0; }

  [[nodiscard]] auto is_dir() const { return this->index() == 1; }
};
}  // namespace detail

struct in_memory_file {
  explicit in_memory_file(std::filesystem::path path)
      : path_(std::move(path)) {}

  [[nodiscard]] auto path() const -> std::filesystem::path const& {
    return path_;
  }

  auto path() -> std::filesystem::path& { return path_; }

  friend auto operator==(in_memory_file const&, in_memory_file const&) noexcept
      -> bool = default;

 private:
  std::filesystem::path path_;
};

struct in_memory_directory
    : public std::vector<detail::entry_t<in_memory_directory>> {
  using file_type = in_memory_file;
  using entry_type = detail::entry_t<in_memory_directory>;
  using std::vector<entry_type>::vector;

  explicit in_memory_directory(std::string_view name)
      : parent_(nullptr), path_(name) {}

  explicit in_memory_directory(std::string_view name,
                               in_memory_directory& parent)
      : parent_(&parent), path_(parent_->path() / name) {}

  explicit in_memory_directory(std::string_view name,
                               std::initializer_list<entry_type> lst)
      : std::vector<entry_type>{lst}, parent_(nullptr), path_(name) {}

  explicit in_memory_directory(std::string_view name,
                               std::initializer_list<entry_type> lst,
                               in_memory_directory& parent)
      : std::vector<entry_type>{lst},
        parent_(&parent),
        path_(parent_->path() / name) {}

  [[nodiscard]] auto path() const -> std::filesystem::path const& {
    return path_;
  }

  auto path() -> std::filesystem::path& { return path_; }

  void add_file(std::string_view name) { push_back(file_type(path_ / name)); }

  auto add_dir(std::string_view name) {
    push_back(in_memory_directory(name, *this));
    return directory_view{back().get_dir()};
  }

  [[nodiscard]] auto operator==(in_memory_directory const& dir) const -> bool {
    return path_ == dir.path_ and
           std::equal(this->begin(), this->end(), dir.begin(), dir.end());
  }

  auto parent() { return directory_view{*parent_}; }

 private:
  in_memory_directory* parent_;
  std::filesystem::path path_;
};

using in_memory_entry = detail::entry_t<in_memory_directory>;

}  // namespace untree
