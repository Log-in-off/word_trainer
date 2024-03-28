cmake_minimum_required(VERSION 3.16)
project (vebs)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
    NameOpenXLSXlib                   
    GIT_REPOSITORY https://github.com/troldal/OpenXLSX.git
    #GIT_TAG
)
FetchContent_MakeAvailable(NameOpenXLSXlib)
#или так
#FetchContent_Populate(NameOpenXLSXlib)
#add_subdirectory(${nameopenxlsxlib_SOURCE_DIR} ${nameopenxlsxlib_BINARY_DIR})

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(fmt URL https://github.com/fmtlib/fmt/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(fmt)


configure_file(words.xlsx ${CMAKE_BINARY_DIR}/words.xlsx COPYONLY)
configure_file(config.json ${CMAKE_BINARY_DIR}/config.json COPYONLY)

add_executable(vebs src/main.cpp)
target_link_libraries(vebs PRIVATE OpenXLSX::OpenXLSX nlohmann_json::nlohmann_json fmt::fmt)
target_compile_features(vebs PRIVATE cxx_std_17)
target_link_options(vebs PRIVATE -static-libgcc -static-libstdc++)

#set(MAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/Toolchain-cross-mingw32-linux.cmake")