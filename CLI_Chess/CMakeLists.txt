cmake_minimum_required(VERSION 3.5)
project(Chess)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(VLD_INCLUDE_DIR "C://Program Files \(x86\)/Visual Leak Detector/include/")
find_library(VLD vld HINTS "C://Program Files \(x86\)/Visual Leak Detector/lib/Win64/")

include_directories("${VLD_INCLUDE_DIR}")
link_libraries("${VLD}")
message("${VLD_INCLUDE_DIR}")
message("${VLD}")
message("Enabling Visual Leak Detector Done")

add_executable(main src/bsp1/main.cpp ${VLDw_H}
        src/bsp1/figures/chessman/chessman.cpp
        src/bsp1/figures/chessman/chessman.h
        src/bsp1/board/chessboard.cpp
        src/bsp1/board/chessboard.h
        src/bsp1/board/coordinate/coordinate.h
        src/bsp1/figures/king/king.cpp
        src/bsp1/figures/king/king.h
        src/bsp1/figures/knight/knight.cpp
        src/bsp1/figures/knight/knight.h
        src/bsp1/figures/pawn/pawn.cpp
        src/bsp1/figures/pawn/pawn.h
        src/bsp1/figures/queen/queen.cpp
        src/bsp1/figures/queen/queen.h
        src/bsp1/figures/rook/rook.cpp
        src/bsp1/figures/rook/rook.h
        src/bsp1/figures/bishop/bishop.cpp
        src/bsp1/figures/bishop/bishop.h)

target_compile_options(main PRIVATE -Wall /W4)
