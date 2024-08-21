cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(BUILD_SHARED_LIBS OFF)
add_definitions(-DOPENCV_STATIC)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

project ("DecVST")
add_executable (${PROJECT_NAME} "DecVST.cpp" )

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

set(OpenCV_DIR "D:/OpenCV/opencv4.10.0/build/x64/vc17/staticlib")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
