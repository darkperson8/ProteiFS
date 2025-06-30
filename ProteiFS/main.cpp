#include <iostream>
#include <filesystem>
#include <memory>
#include "File.h"
#include "Directory.h"

int main() {
    try {
        // 1. Основная директория
        Directory root = makeDirectory("demo_dir");

        // 2. Создаём несколько поддиректорий
        std::cout << "Creating subdirectories...\n";
        Directory sub1 = makeDirectory(root.getPath() / "subdir1");
        Directory sub2 = makeDirectory(root.getPath() / "subdir2");
        Directory sub3 = makeDirectory(root.getPath() / "subdir3");

        // 3. В корне создаём несколько файлов
        std::cout << "Creating files in root...\n";
        File f1 = makeFile(root.getPath() / "root1.txt");
        File f2 = makeFile(root.getPath() / "root2.log");

        // 4. В каждой поддиректории создаём по файлу
        std::cout << "Creating files in subdirectories...\n";
        File sf1 = makeFile(sub1.getPath() / "file1.txt");
        File sf2 = makeFile(sub2.getPath() / "file2.txt");
        File sf3 = makeFile(sub3.getPath() / "file3.txt");

        // 5. Пишем разные строки
        std::cout << "Writing lines to files...\n";
        f1.writeLine("Root file line A");
        f1.writeLine("Root file line B");
        sf1.writeLine("Sub1: Hello");
        sf2.writeLine("Sub2: Hello");
        sf3.writeLine("Sub3: Hello");

        // 6. Читаем и выводим содержимое каждого файла
        std::cout << "Reading all files...\n";
        auto printFile = [&](File& file) {
            std::cout << "-- " << file.getName() << " --\n";
            std::string line;
            file.resetReadPos();
            while (file.readLine(line)) {
                std::cout << line << "\n";
            }
            };
        printFile(f1);
        printFile(f2);
        printFile(sf1);
        printFile(sf2);
        printFile(sf3);

        // 7. Перечисляем все записи полиморфно
        std::cout << "\nDirectory entries in root:\n";
        for (const auto& entry : root.getEntries()) {
            std::cout << " [" << ((entry->type() == FileBase::Type::File) ? "F" : "D")
                << "] " << entry->getName() << "\n";
        }

        // 8. Удаляем всё
        std::cout << "Cleaning up...\n";
        sf1.remove(); sf2.remove(); sf3.remove(); // удаляем файлы в поддиректориях
        sub1.remove(); sub2.remove(); sub3.remove(); // удаляем поддиректории
        f1.remove(); f2.remove(); // удаляем файлы в корне
        root.remove(); // удаляем корень

        std::cout << "Demo completed successfully." << std::endl;
        std::cout << "Press Enter to finish..." << std::endl;
        std::cin.get();
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 2;
    }
    return 0;
}
