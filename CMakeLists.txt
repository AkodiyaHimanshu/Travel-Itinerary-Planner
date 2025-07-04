# CMakeList.txt : CMake project for Travel Itinerary Planner, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Travel Itinerary Planner")

# Add source to this project's executable.
add_executable (CMakeTarget "main.cpp" "main.h" "include/version.h" "include/Itinerary.h" "src/StorageManager.h" "src/StorageManager.cpp" "include/PackingItem.h" "src/PackingManager.h" "src/PackingManager.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

# Include FetchContent for downloading dependencies
include(FetchContent)

# Add nlohmann/json library
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3 # Specify a version tag for stability
)
FetchContent_MakeAvailable(json)

# Link libraries
target_link_libraries(CMakeTarget PRIVATE
  nlohmann_json::nlohmann_json
)