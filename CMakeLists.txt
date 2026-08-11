cmake_minimum_required(VERSION 3.16)
project(StudentuInformacineSistema VERSION 1.5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Visi šaltinio failai yra OOP_2/ aplanke
add_executable(programa
    OOP_2/OOP_2.cpp
    OOP_2/zmogus.cpp
    OOP_2/studentas.cpp
    OOP_2/testStudentas.cpp
    OOP_2/menu.cpp
    OOP_2/enter.cpp
    OOP_2/generate.cpp
    OOP_2/file.cpp
    OOP_2/print.cpp
    OOP_2/test.cpp
)

target_include_directories(programa PRIVATE ${CMAKE_SOURCE_DIR}/OOP_2)

if(MSVC)
    target_compile_options(programa PRIVATE /W4 /O2)
else()
    target_compile_options(programa PRIVATE -Wall -Wextra -O2)
endif()

# Nukopijuoja data/ aplanką šalia .exe, kad testavimo funkcijos
# rastų failus keliu "data/studentaiN.txt"
add_custom_command(TARGET programa POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/OOP_2/data
        $<TARGET_FILE_DIR:programa>/data
)
