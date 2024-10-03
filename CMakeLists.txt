# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.28)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "v")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

if(${WIN32})
#this method adds the necessary compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0501 /EHsc")
#this adds a preprocessor definition to the project
add_definitions(-D_WIN32_WINNT=0x0501)
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (POLICY CMP0079)
  cmake_policy(SET CMP0079 NEW)
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/meals.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

project ("restapi")

find_package(Crow CONFIG REQUIRED)
find_package(SQLiteCpp CONFIG REQUIRED)
find_package(opentelemetry-cpp CONFIG REQUIRED)

add_executable (restapi "restapi.cpp" "restapi.h" "Limiter.cpp" "Limiter.h" "Routes.cpp" "Routes.h" "Database.cpp" "DataBase.h" "utility.h" "DBMeal.h" "traceservice.h" "traceservice.cpp")
target_link_libraries(restapi PRIVATE Crow::Crow SQLiteCpp opentelemetry-cpp::api opentelemetry-cpp::common opentelemetry-cpp::ostream_span_exporter )
