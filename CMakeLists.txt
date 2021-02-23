cmake_minimum_required(VERSION 3.17)
project(monorepo_jstraceski_a1)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(include/IL)
include_directories(include/glad)
include_directories(include/KHR)
include_directories(src)

add_executable(a1
        include/KHR/khrplatform.h
        src/Game.cpp include/Game.h
        src/glad.cpp include/glad/glad.h

        include/LOpenGL.h
        include/TinyMath.hpp
        include/Entity.h
        src/Clip.cpp include/Clip.h

        include/IL/il.h
        include/IL/ilu.h
        include/IL/ilut.h

        src/ResourceManager.cpp include/ResourceManager.h
        src/main.cpp)


find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
