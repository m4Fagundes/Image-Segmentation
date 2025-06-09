cmake_minimum_required(VERSION 3.10)
project(SegmentacaoGrafos VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include vendor)

file(GLOB SOURCES "src/*.cpp")

add_executable(segmenter ${SOURCES})

message(STATUS "Configuração concluída. Para compilar, entre na pasta 'build' e execute 'cmake ..' e depois 'make'.")