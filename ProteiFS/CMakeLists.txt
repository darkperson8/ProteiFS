# CMakeList.txt: проект CMake для ProteiFS; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (ProteiFS "main.cpp"  "FileBase.h" "File.h" "Directory.h" "Directory.cpp" "File.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ProteiFS PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
