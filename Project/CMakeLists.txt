cmake_minimum_required(VERSION 3.29)
project(Project)

set(CMAKE_CXX_STANDARD 17)

add_executable(Project
        lcd.h
        lcd.cpp
        main.cpp
        word.cpp
        touch.cpp
        bmp.cpp
        photo.cpp
        photo.h
        music.cpp
        music.h
        video.cpp
        video.h
        v4l2.cpp
        v4l2.h
        common.cpp
        v4use.cpp
        v4use.h
)
