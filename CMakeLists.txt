cmake_minimum_required(VERSION 3.20)
project(MusicPlayer CXX)

if(MSVC)
  message(FATAL_ERROR "Use MusicPlayer.sln for MSVC builds; this CMake project targets zig/MinGW.")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MusicPlayer
  src/CFilesProcessor.cpp
  src/CMusicProcessor.cpp
  src/Common.cpp
  src/CViewerProcessor.cpp
  src/MusicPlayer.cpp
  src/stdafx.cpp
)

target_compile_definitions(MusicPlayer PRIVATE
  UNICODE
  _UNICODE
  WIN32
  _WINDOWS
)

target_include_directories(MusicPlayer PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libzplay
)

target_link_libraries(MusicPlayer PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libzplay/libzplay.lib
  gdi32
)

target_link_options(MusicPlayer PRIVATE
  -municode
  -Wl,--subsystem,windows
)

# MinGW/zig 的 rc 工具不能读 UTF-16LE 的 .rc，先转成 UTF-8 再编译
find_program(ICONV_EXECUTABLE NAMES iconv REQUIRED)
find_program(ZIG_EXECUTABLE NAMES zig REQUIRED)

set(RC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/res/MusicPlayer.rc)
set(RC_UTF8 ${CMAKE_CURRENT_BINARY_DIR}/MusicPlayer.utf8.rc)
set(RC_OBJECT ${CMAKE_CURRENT_BINARY_DIR}/MusicPlayer.res)

add_custom_command(
  OUTPUT ${RC_UTF8}
  COMMAND ${ICONV_EXECUTABLE} -f UTF-16LE -t UTF-8 -o ${RC_UTF8} ${RC_SOURCE}
  DEPENDS ${RC_SOURCE}
  VERBATIM
)

add_custom_command(
  OUTPUT ${RC_OBJECT}
  COMMAND ${ZIG_EXECUTABLE} rc /c 65001 /y
          /I ${CMAKE_CURRENT_SOURCE_DIR}/src
          /I ${CMAKE_CURRENT_SOURCE_DIR}/src/res
          /fo ${RC_OBJECT} ${RC_UTF8}
  DEPENDS ${RC_UTF8}
          ${CMAKE_CURRENT_SOURCE_DIR}/src/resource.h
          ${CMAKE_CURRENT_SOURCE_DIR}/src/targetver.h
          ${CMAKE_CURRENT_SOURCE_DIR}/src/res/MusicPlayer.ico
          ${CMAKE_CURRENT_SOURCE_DIR}/src/res/small.ico
  VERBATIM
)

add_custom_target(MusicPlayerResources DEPENDS ${RC_OBJECT})
add_dependencies(MusicPlayer MusicPlayerResources)
# 通过链接选项传 .res；放进 target_link_libraries 会被当成 -lMusicPlayer.res
target_link_options(MusicPlayer PRIVATE ${RC_OBJECT})
set_property(TARGET MusicPlayer APPEND PROPERTY LINK_DEPENDS ${RC_OBJECT})

add_custom_command(TARGET MusicPlayer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libzplay/libzplay.dll
          $<TARGET_FILE_DIR:MusicPlayer>
)
