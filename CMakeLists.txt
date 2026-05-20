cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME "DreamGame")
project(${PROJECT_NAME})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Activez Rechargement à chaud pour les compilateurs MSVC si cela est pris en charge.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	# MP build for MSVC
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc")
endif()

set(STB_DIR "${CMAKE_SOURCE_DIR}/third_party/stb") 
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/third_party/imgui") 

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp include/*.h ${STB_DIR}/*.cpp ${STB_DIR}/*.h ${IMGUI_DIR}/*.cpp ${IMGUI_DIR}/*.h)
add_executable(${PROJECT_NAME} ${SOURCES})

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)

add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/SDL3" EXCLUDE_FROM_ALL)
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL3::SDL3> $<TARGET_FILE_DIR:${PROJECT_NAME}>)

set(SDLTTF_VENDORED ON)
add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/SDL3_ttf" EXCLUDE_FROM_ALL)
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL3_ttf::SDL3_ttf> $<TARGET_FILE_DIR:${PROJECT_NAME}>)

add_subdirectory(third_party/openAL)
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:OpenAL> $<TARGET_FILE_DIR:${PROJECT_NAME}>)

add_subdirectory(third_party/assimp)

target_compile_options(${PROJECT_NAME} PUBLIC /W4 /external:anglebrackets /external:W0 /wd5045)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17) # C++17

add_definitions(-DGLEW_STATIC)
target_link_directories(${PROJECT_NAME} PUBLIC "lib/glew/lib/Release/x64")

target_include_directories(${PROJECT_NAME} PUBLIC 
  "include"
  "lib/glew/include"
  "lib/glm"
  "third_party"
)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC 
  OpenGL::GL
  SDL3_ttf::SDL3_ttf
  SDL3::SDL3
  OpenAL
  glew32s
  assimp
)