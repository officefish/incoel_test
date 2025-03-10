#include <iostream>
#include <unistd.h>
#include <cstdlib>

int main() {
    // Проверяем UID процесса
    if (geteuid() != 0) {
        std::cerr << "Ошибка: программа должна быть запущена от root!" << std::endl;
        return 1;
    }

    // Дополнительная проверка через системный вызов
    if (std::system("id -u") != 0) {
        std::cerr << "Ошибка: проверка через системный вызов показала, что программа не запущена от root!" << std::endl;
        return 1;
    }

    std::cout << "Программа запущена от root." << std::endl;
    return 0;
}