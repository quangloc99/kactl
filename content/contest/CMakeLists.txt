cmake_minimum_required(VERSION 3.14)
project(Contest)

set(CMAKE_CXX_STANDARD 17)
# remember to remove when do stress testing
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -Wextra -Wall")

add_definitions(-DLOCAL)

foreach (file A B C D E F G H I J K L M N)
    add_executable(${file} ${file}.cpp)
endforeach (file)
