find_package(Qt6 REQUIRED COMPONENTS Test)

set(ENGINE_SOURCES_TEST
    ../src/engine/Operation.cpp
    ../src/engine/BinaryOperation.cpp
    ../src/engine/UnaryOperation.cpp
    ../src/engine/Memory.cpp
    ../src/engine/CalculatorEngine.cpp
)

qt_add_executable(testes_engine
    TestEngine.cpp
    ${ENGINE_SOURCES_TEST}
)

target_include_directories(testes_engine PRIVATE ../src)
target_link_libraries(testes_engine PRIVATE Qt6::Test Qt6::Widgets)

add_test(NAME EngineTests COMMAND testes_engine)
