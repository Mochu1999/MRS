cmake_minimum_required(VERSION 3.20)
project(Telemetry)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#won't re-run CMake automaticaly if the build system has changed
set(CMAKE_SUPPRESS_REGENERATION ON)

# Collect source files
#createSolutions.bat must be run again to view newly created or moved elements
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ../Source/*.cpp) #compiled
file(GLOB_RECURSE HDR_FILES CONFIGURE_DEPENDS ../Source/*.h ../Source/*.hpp) #files not directly compiled
file(GLOB_RECURSE RES_FILES CONFIGURE_DEPENDS ../Resources/*) #files not compiled
#These are in the project directoty folder, containing main.cpp and specific source files
file(GLOB LOCAL_SRC CONFIGURE_DEPENDS *.cpp)
file(GLOB LOCAL_HDR CONFIGURE_DEPENDS *.h *.hpp)

add_executable(Telemetry ${SRC_FILES} ${LOCAL_SRC})

# Make .h, .hpp and resources appear in Visual Studio
target_sources(Telemetry PRIVATE ${HDR_FILES} ${RES_FILES} ${LOCAL_HDR})

# Organizes folders mirroring real directory structure
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/.." FILES ${SRC_FILES} ${HDR_FILES} ${RES_FILES} ${LOCAL_SRC} ${LOCAL_HDR})


# Sets the working directory in Framework
set_property(TARGET Telemetry PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/..")

# Sets Telemetry as the startup project (the one that runs with F5)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Telemetry)

set_target_properties(Telemetry PROPERTIES MSVC_RUNTIME_LIBRARY "$<$<CONFIG:Debug>:MultiThreadedDebugDLL>")

set_target_properties(Telemetry PROPERTIES COMPILE_DEFINITIONS "_DEBUG;_CONSOLE;GLEW_STATIC")

target_include_directories(Telemetry PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/../Source"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Source/Core"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Source/Composites"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Source/Stability"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Source/Deprecated"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Source/Common"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Source/Graphics"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Source/Graphics/BasicsRendering"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Resources/External/Includes"
)

target_compile_options(Telemetry PRIVATE /W3 /sdl)
target_compile_options(Telemetry PRIVATE $<$<CONFIG:Debug>:/Zi>)
set_property(TARGET Telemetry PROPERTY VS_GLOBAL_SupportJustMyCodeDebugging "true")

#MSVC behaves like a strict C++ compiler (GCC/Clang like)
target_compile_options(Telemetry PRIVATE /permissive-)

target_link_directories(Telemetry PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/External/Libs")
file(GLOB EXTERNAL_DLLS "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/External/dlls/*.dll")


#linker input, first in a semicolon format and then it's translated to the format CMake expects
set(_EXTRA_LIBS "glew32s.lib;glfw3.lib;opengl32.lib;freetype.lib")
# Split the semicolon string into a CMake list and link them
string(REPLACE ";" ";" _EXTRA_LIBS_LIST "${_EXTRA_LIBS}")
target_link_libraries(Telemetry PRIVATE ${_EXTRA_LIBS_LIST})


#Copies external DLLs to output build dir
add_custom_command(TARGET Telemetry POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EXTERNAL_DLLS} $<TARGET_FILE_DIR:Telemetry>
)

set(build_root "${CMAKE_CURRENT_SOURCE_DIR}/build")

#Puts build outputs (.exe, .dll, .lib) in build/release or build/debug
set_target_properties(Telemetry PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${build_root}/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY "${build_root}/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY "${build_root}/$<CONFIG>"
)
