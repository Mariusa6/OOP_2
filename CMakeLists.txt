cmake_minimum_required(VERSION 3.16)
project(StudentuInformacineSistema VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Numatytasis build tipas, jei nenurodytas
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build tipas" FORCE)
endif()

# =====================================================================
# Bendra klasių biblioteka
#
# Klasės sudėtos į atskirą biblioteką, kad jas galėtų naudoti
# ir pagrindinė programa, ir unit testai — be pakartotinio kompiliavimo.
# =====================================================================
add_library(studentaiLib STATIC
    OOP_2/zmogus.cpp
    OOP_2/studentas.cpp
)

target_include_directories(studentaiLib PUBLIC ${CMAKE_SOURCE_DIR}/OOP_2)

if(MSVC)
    target_compile_options(studentaiLib PRIVATE /W4 /utf-8)
else()
    target_compile_options(studentaiLib PRIVATE -Wall -Wextra)
endif()

# =====================================================================
# Pagrindinė programa
# =====================================================================
add_executable(programa
    OOP_2/OOP_2.cpp
    OOP_2/testStudentas.cpp
    OOP_2/menu.cpp
    OOP_2/enter.cpp
    OOP_2/generate.cpp
    OOP_2/file.cpp
    OOP_2/print.cpp
    OOP_2/test.cpp
)

target_link_libraries(programa PRIVATE studentaiLib)

if(MSVC)
    target_compile_options(programa PRIVATE /W4 /O2 /utf-8)
else()
    target_compile_options(programa PRIVATE -Wall -Wextra -O2)
endif()

# Nukopijuoja data/ aplanką šalia .exe, kad testavimo funkcijos
# rastų failus keliu "data/studentaiN.txt"
if(EXISTS ${CMAKE_SOURCE_DIR}/OOP_2/data)
    add_custom_command(TARGET programa POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/OOP_2/data
            $<TARGET_FILE_DIR:programa>/data
        COMMENT "Kopijuojamas data/ aplankas"
    )
endif()

# =====================================================================
# Unit testai (Catch2)
#
# Kuriami tik jei rastas tests/catch.hpp.
# Atsisiuntimas:
#   https://github.com/catchorg/Catch2/releases/download/v2.13.10/catch.hpp
# =====================================================================
if(EXISTS ${CMAKE_SOURCE_DIR}/tests/catch.hpp)

    enable_testing()

    add_executable(unitTests tests/unitTests.cpp)
    target_link_libraries(unitTests PRIVATE studentaiLib)
    target_include_directories(unitTests PRIVATE ${CMAKE_SOURCE_DIR}/tests)

    if(MSVC)
        target_compile_options(unitTests PRIVATE /W4 /utf-8)
    else()
        target_compile_options(unitTests PRIVATE -Wall -Wextra)
    endif()

    add_test(NAME VisiUnitTestai COMMAND unitTests)

    # Atskiri testų rinkiniai pagal žymes
    add_test(NAME RuleOfFive COMMAND unitTests "[rule5]")
    add_test(NAME AbstrakciKlase COMMAND unitTests "[abstrakti]")
    add_test(NAME Operatoriai COMMAND unitTests "[io]" "[operatoriai]")

    message(STATUS "Unit testai ijungti (rastas tests/catch.hpp)")
else()
    message(WARNING
        "tests/catch.hpp nerastas — unit testai NEBUS kompiliuojami.\n"
        "  Atsisiuntimas:\n"
        "  https://github.com/catchorg/Catch2/releases/download/v2.13.10/catch.hpp\n"
        "  Idekite i tests/ aplanka.")
endif()

# =====================================================================
# Doxygen dokumentacija
#
# Paleidimas:  cmake --build . --target docs
# =====================================================================
find_package(Doxygen QUIET)

if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generuojama Doxygen dokumentacija"
        VERBATIM
    )
    message(STATUS "Doxygen rastas — galima naudoti 'docs' taikini")
else()
    message(STATUS "Doxygen nerastas — dokumentacijos generavimas negalimas")
endif()
