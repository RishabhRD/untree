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

#include <concepts>
#include <directory_view.hpp>
#include <type_traits>

namespace untree {

template <typename DirView, typename Directory>
concept is_directory_view_of =
    std::same_as<DirView, directory_view<std::decay_t<Directory>&>> ||
    std::same_as<DirView, directory_view<std::decay_t<Directory>&&>>;

template <typename DirView>
concept is_directory_view =
    std::same_as<DirView, directory_view<std::decay_t<typename DirView::dir_type>&>> ||
    std::same_as<DirView, directory_view<std::decay_t<typename DirView::dir_type>&&>>;

template <typename Directory>
concept directory = requires(Directory dict) {
  { dict.add_dir(std::declval<std::string_view>()) } -> is_directory_view_of<Directory>;
  { dict.add_file(std::declval<std::string_view>()) } -> std::same_as<void>;
  { dict.parent() } -> is_directory_view_of<Directory>;
};

}  // namespace untree
