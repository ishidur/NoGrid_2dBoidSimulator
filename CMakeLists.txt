cmake_minimum_required(VERSION 3.7)
project(boid)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        boid/BaseBoid.cpp
        boid/BaseBoid.h
        boid/Block.cpp
        boid/Block.h
        boid/Direction.cpp
        boid/Direction.h
        boid/Grid.cpp
        boid/Grid.h
        boid/main.cpp
        boid/parameters.h
        boid/Point.cpp
        boid/Point.h
        boid/resource.h
        boid/stdafx.cpp
        boid/stdafx.h
        boid/targetver.h)

add_executable(boid ${SOURCE_FILES})