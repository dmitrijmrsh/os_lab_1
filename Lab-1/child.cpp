#include <iostream>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <string>

int main(int argc, char* argv[]) {
    int file = open(argv[1], O_CREAT | O_WRONLY, S_IRWXU);
    if (file == -1) {
        write(STDERR_FILENO, "An error occured with opening a file", 37);
        return 1;
    }
    int temp = ftruncate(file, 0);
    if (temp == -1) {
        write(STDERR_FILENO, "An error occured with clearing a file", 38);
        return 1;
    }
    std::string value, answer;
    bool flag = false;
    bool one_number = true;
    bool nothing = true;
    char c;
    float result;
    while (read(STDIN_FILENO, &c, 1)) {
        if (c != '\n' ) {
            if (c !=  ' ') {
                nothing = false;
                value += c;
            } else {
                one_number = false;
                if (!flag) {
                    flag = true;
                    result = std::stof(value);
                } else {
                    if (std::stof(value) == 0) {
                        write(STDERR_FILENO, "An error occured with division by zero\n", 40);
                        exit(EXIT_FAILURE);
                    }
                    result /= std::stof(value);
                }
                value = "";
            }
        } else {
            if (nothing) {
                continue;
            }
            if (one_number) {
                result = std::stof(value);
            } else {
                if (std::stof(value) == 0) {
                    write(STDERR_FILENO, "An error occured with division by zero\n", 40);
                    exit(EXIT_FAILURE);
                }
                result /= std::stof(value);
            }
            flag = false;
            one_number = true;
            nothing = true;
            value = "";
            answer = std::to_string(result);
            for (int i = 0; i < answer.size(); ++i) {
                write(file, &answer[i], 1);
                write(STDOUT_FILENO, &answer[i], 1);
            }
            write(file, "\n", 1);
            write(STDOUT_FILENO, "\n", 1);
        }
    }
    
    return 0;
}