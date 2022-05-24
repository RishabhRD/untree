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

#include "directory_view.hpp"

namespace untree {

struct disk_directory {
  explicit disk_directory(std::filesystem::path path)
      : path_(std::move(path)) {}

  [[nodiscard]] auto path() const -> std::filesystem::path const& {
    return path_;
  }

  auto add_file(std::string_view name) const {
    auto const res_path = path_ / name;
    if (!std::filesystem::exists(res_path)) {
      std::ofstream os{res_path.native()};
    }
  }

  auto add_dir(std::string_view name) {
    auto res_path = path_ / name;
    if (!std::filesystem::exists(res_path)) {
      std::filesystem::create_directory(res_path);
    }
    return directory_view{disk_directory{std::move(res_path)}};
  }

  auto parent() { return directory_view{disk_directory{path_.parent_path()}}; }

 private:
  std::filesystem::path path_;
};

}  // namespace untree
