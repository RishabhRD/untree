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
#include <sstream>

// TODO: figure out why argparse conan fails to compile, then use it form conan
#include "argparse.hpp"
#include "directory_view.hpp"
#include "disk_directory.hpp"
#include "no_op_directory.hpp"
#include "untree.hpp"
#include "verbose_directory.hpp"

auto main(int argc, char** argv) -> int {
  try {
    argparse::ArgumentParser program("untree");
    program.add_argument("directory")
        .help("root directory for new directory structure");
    program.add_argument("--verbose")
        .help("print created files/directories")
        .default_value(false)
        .implicit_value(true);
    program.add_argument("--dry-run")
        .help("print created files/directories without actually creating")
        .default_value(false)
        .implicit_value(true);
    program.add_argument("--create")
        .help("")
        .default_value(false)
        .implicit_value(true);
    try {
      program.parse_args(argc, argv);
    } catch (std::runtime_error const& err) {
      std::cerr << err.what() << std::endl;
      std::cerr << program;
      return 1;
    }
    auto const dir = std::filesystem::path(program.get("directory"));
    auto const is_verbose = program["--verbose"];
    auto const is_dry_run = program["--dry-run"];
    auto const to_create = program["--create"];
    if (!std::filesystem::exists(dir)) {
      if (to_create == false and is_dry_run == false) {
        std::cerr << dir.native() << " doesn't exist!!!\n";
        return 1;
      }
      if (to_create == true and is_dry_run == false) {
        try {
          std::filesystem::create_directory(dir);
        } catch (std::filesystem::filesystem_error const& err) {
          std::cerr << "failed to create directory : " << dir.native()
                    << std::endl;
          std::cerr << err.what() << std::endl;
          return 1;
        }
      }
    }
    std::stringstream stream;
    stream << std::cin.rdbuf();
    auto const str{stream.str()};
    if (is_dry_run == true) {
      untree::parse_tree(untree::directory_view{untree::verbose_directory{
                             untree::no_op_directory{}}},
                         str);
    } else if (is_verbose == true) {
      untree::parse_tree(untree::directory_view{untree::verbose_directory{
                             untree::disk_directory{dir}}},
                         str);
    } else {
      untree::parse_tree(untree::directory_view{untree::disk_directory{dir}},
                         str);
    }
  } catch (std::exception const& ex) {
    std::cerr << ex.what() << std::endl;
    return 1;
  }
  return 0;
}
