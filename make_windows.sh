cmake -DCMAKE_TOOLCHAIN_FILE=./Toolchain-cross-mingw32-linux.cmake .
#https://ru.stackoverflow.com/questions/1249617/%D0%9A%D1%80%D0%BE%D1%81%D1%81%D0%BA%D0%BE%D0%BC%D0%BF%D0%B8%D0%BB%D1%8F%D1%86%D0%B8%D1%8F-%D0%BF%D0%BE%D0%B4-%D0%B2%D0%B8%D0%BD%D0%B4%D0%BE%D0%B2%D1%81-%D0%BD%D0%B0-%D0%BB%D0%B8%D0%BD%D1%83%D0%BA%D1%81?ysclid=ltcyxaabbc620660529

cmake -DCMAKE_TOOLCHAIN_FILE=./cmake/Toolchain-cross-mingw32-linux.cmake -B windows_build .
