# Versão mínima do CMake e nome do projeto
cmake_minimum_required(VERSION 3.10)
project(SegmentacaoGrafos VERSION 1.0)

# Define o padrão do C++ para 17 (obrigatório no trabalho) 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Informa ao CMake onde encontrar os arquivos de cabeçalho
include_directories(include vendor)

# Encontra todos os arquivos .cpp na pasta src e os adiciona à variável SOURCES
file(GLOB SOURCES "src/*.cpp")

# Define o nome do executável final e quais fontes usar
add_executable(segmenter ${SOURCES})

# Mensagem final
message(STATUS "Configuração concluída. Para compilar, entre na pasta 'build' e execute 'cmake ..' e depois 'make'.")