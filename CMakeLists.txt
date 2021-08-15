cmake_minimum_required(VERSION 3.16.3)
project(FilesystemReader)
set(CMAKE_CXX_STANDARD 17)
find_package(jsoncpp REQUIRED) 
include_directories(include)
add_executable(filesystem 
    src/FilesystemGenerator.cpp 
    src/File.cpp
    src/JsonHandler.cpp
    src/Logger.cpp
    src/md5.cpp)
target_link_libraries(filesystem jsoncpp stdc++fs)
Install(TARGETS filesystem DESTINATION /bin)
