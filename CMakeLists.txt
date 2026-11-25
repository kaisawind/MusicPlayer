cmake_minimum_required(VERSION 3.20)
project(MusicPlayer CXX)

if(MSVC)
  enable_language(RC)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MusicPlayer WIN32
  src/app/MusicPlayer.cpp
  src/audio/AudioPlayer.cpp
  src/common/stdafx.cpp
  src/ui/PlayerView.cpp
)

target_compile_definitions(MusicPlayer PRIVATE
  UNICODE
  _UNICODE
  WIN32
  _WINDOWS
)

target_include_directories(MusicPlayer PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/app
  ${CMAKE_CURRENT_SOURCE_DIR}/src/audio
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
  ${CMAKE_CURRENT_SOURCE_DIR}/src/common
  ${CMAKE_CURRENT_SOURCE_DIR}/src/res
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libzplay
)

target_link_libraries(MusicPlayer PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libzplay/libzplay.lib
  gdi32
  comdlg32
)

if(MSVC)
  target_sources(MusicPlayer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/res/MusicPlayer.rc
  )
else()
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
            /I ${CMAKE_CURRENT_SOURCE_DIR}/src/res
            /I ${CMAKE_CURRENT_SOURCE_DIR}/src/common
            /fo ${RC_OBJECT} ${RC_UTF8}
    DEPENDS ${RC_UTF8}
            ${CMAKE_CURRENT_SOURCE_DIR}/src/res/resource.h
            ${CMAKE_CURRENT_SOURCE_DIR}/src/common/targetver.h
            ${CMAKE_CURRENT_SOURCE_DIR}/src/res/MusicPlayer.ico
            ${CMAKE_CURRENT_SOURCE_DIR}/src/res/small.ico
    VERBATIM
  )

  add_custom_target(MusicPlayerResources DEPENDS ${RC_OBJECT})
  add_dependencies(MusicPlayer MusicPlayerResources)
  # 通过链接选项传 .res；放进 target_link_libraries 会被当成 -lMusicPlayer.res
  target_link_options(MusicPlayer PRIVATE ${RC_OBJECT})
  set_property(TARGET MusicPlayer APPEND PROPERTY LINK_DEPENDS ${RC_OBJECT})
endif()

add_custom_command(TARGET MusicPlayer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libzplay/libzplay.dll
          $<TARGET_FILE_DIR:MusicPlayer>
)

# 原生构建的单元测试目标（交叉编译时不生成，测试在 Windows 上运行）
include(CTest)
if(BUILD_TESTING AND NOT CMAKE_CROSSCOMPILING)
  add_executable(MusicPlayerTests tests/test_range.cpp)
  target_include_directories(MusicPlayerTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common
  )
  add_test(NAME MusicPlayerTests COMMAND MusicPlayerTests)
endif()
