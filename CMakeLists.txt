# Требуется версия CMake больше или равная указанной ниже
# Requires CMake version greater than or equal to the following
cmake_minimum_required(VERSION 3.20)

# Указываем имя проекта и используемый язык
# We indicate the name of the project and the language used
project(arkanoid-from-ncurses-in-sdl3-c LANGUAGES C)

# Пишем имя приложения. Будет создан файл app.exe
# We write the name of the application. The file app.exe will be created
add_executable(app)

# Указываем, где хранятся файлы исходников приложения
# Specify where the application source files are stored
target_sources(app
PRIVATE
    src/main.c
)

# Ищем библиотеку по указанному пути в -DSDL_DIR
# We search for the library at the specified path in -DSDL_DIR
find_package(SDL3)

# Линкуем библиотеки
# Linking libraries
target_link_libraries(app PRIVATE SDL3::SDL3)

# Сборка будет статической
# The build will be static
target_link_options(app PRIVATE -static)
