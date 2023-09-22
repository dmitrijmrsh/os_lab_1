#include <iostream>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    int file = open(argv[1], O_CREAT | O_WRONLY, S_IRWXU);
    if (file == -1) {
        std::cerr << "An error occured with opening a file" << '\n';
        return 1;
    }
    int temp = ftruncate(file, 0);
    if (temp == -1) {
        std::cerr << "An error occured with clearing a file" << '\n';
        return 1;
    }
    int value;
    float result, x;
    while (std::cin >> value) {
        for (int i = 0; i < value; ++i) {
            std::cin >> x;
            if (i == 0) {
                result = x;
            } else if (x == 0) {
                std::cerr << "An error occured with division by zero " << value << ' ' << x << '\n';
                exit(EXIT_FAILURE);
            } else {
                result /= x;
            }
        }
        dprintf(file, "%f\n", result);
    }
    return 0;
}