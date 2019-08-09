cmake_minimum_required(VERSION 3.5)
enable_testing()

find_package(PkgConfig)
pkg_check_modules(ncurses ncurses)
add_subdirectory(test-controller)
add_subdirectory(test1)
add_subdirectory(test-controller-lights)
add_subdirectory(test-tracksection)
