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
#include <filesystem>

#include "entry.hpp"

namespace untree {

template <typename F>
concept file = std::constructible_from<F, std::filesystem::path>;

template <typename Directory>
concept directory =
    std::constructible_from<Directory, std::filesystem::path> &&
    requires(Directory dict) {
       typename Directory::file_type;
       { dict.push_back(std::declval<entry_t<Directory>>()) } -> std::same_as<void>;
    } &&
    file<typename Directory::file_type>;

template <typename Entry>
concept entry =
   requires(Entry e){
     typename Entry::file_type;
     typename Entry::directory_type;
   } &&
   file<typename Entry::file_type> &&
   directory<typename Entry::directory_type>;
}  // namespace untree
