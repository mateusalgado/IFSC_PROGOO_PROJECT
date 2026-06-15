cmake_minimum_required(VERSION 3.22)
project(CalculadoraIphone VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(ENGINE_SOURCES
    src/engine/Operation.cpp
    src/engine/BinaryOperation.cpp
    src/engine/UnaryOperation.cpp
    src/engine/Memory.cpp
    src/engine/CalculatorEngine.cpp
)

set(UI_SOURCES
    src/ui/CalculatorButton.cpp
    src/ui/CalcDisplay.cpp
    src/ui/StandardPanel.cpp
    src/ui/ScientificPanel.cpp
    src/ui/MainWindow.cpp
)

qt_add_executable(CalculadoraIphone
    src/main.cpp
    ${ENGINE_SOURCES}
    ${UI_SOURCES}
    resources/resources.qrc
)

target_include_directories(CalculadoraIphone PRIVATE src)

target_link_libraries(CalculadoraIphone PRIVATE Qt6::Widgets)

# Tests (habilitado quando code/tests/ estiver no repositório)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    enable_testing()
    add_subdirectory(tests)
endif()
