# ============================================================================
#
# This file hosts all tests.
#
# If INCLUDE_PILE_SOURCES_DIRECTLY is ON then RESQLITEUN_SOURCES and
# RESQLITEUN_HEADERS are defined we add them to the executable. RESQLITEUN_LIBRARY
# is empty.
#
# On the other hand if INCLUDE_PILE_SOURCES_DIRECTLY is
# OFF then RESQLITEUN_SOURCES and RESQLITEUN_HEADERS are empty and we link
# against RESQLITEUN_LIBRARY that is now the name of the target that
# builds the library.
#
# ============================================================================

find_package(GTest REQUIRED)

find_package(ICU 58 REQUIRED
    COMPONENTS uc i18n dt)

include_directories(${GTEST_INCLUDE_DIRS} ${ICU_INCLUDE_DIRS})

if (NOT INCLUDE_PILE_SOURCES_DIRECTLY)
    set (RESQLITEUN_LIBRARY resqliteun)
else()
    unset (RESQLITEUN_LIBRARY CACHE)
endif()

add_executable(test_basic
    ${RESQLITEUN_SOURCES}
    ${RESQLITEUN_HEADERS}
    ../sqlite/sqlite3.c
    basic.cc)

if (UNIX)
    find_package(Threads REQUIRED)
    list(APPEND GTEST_BOTH_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
    add_definitions(-fPIC)
endif()

target_link_libraries(test_basic
    ${RESQLITEUN_LIBRARY}
    ${GTEST_BOTH_LIBRARIES}
    ${ICU_UC_LIBRARIES}
    ${ICU_I18N_LIBRARIES}
    ${ICU_DT_LIBRARIES})

qt5_use_modules(test_basic Core Sql)

add_test(
    NAME test_basic
    COMMAND $<TARGET_FILE:test_basic>)
