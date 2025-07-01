# tests/CMakeLists.txt
file(GLOB_RECURSE TEST_SOURCES "*.cpp")

# Use a testing framework like Google Test
find_package(GTest REQUIRED)

add_executable(travel_planner_tests ${TEST_SOURCES})
target_include_directories(travel_planner_tests PRIVATE ../include)
target_link_libraries(travel_planner_tests PRIVATE GTest::GTest GTest::Main)

add_test(NAME travel_planner_tests COMMAND travel_planner_tests)