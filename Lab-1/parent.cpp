#include <iostream>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/wait.h>
#include <fcntl.h>
#include <vector>

int main() {
    std::string name;
    std::cout << "Enter the name of file: ";
    std::cin >> name;

    int fd1[2], fd2[2];
    int temp = pipe(fd1);
    if (temp == -1) {
        std::cerr << "An error occured with creating a pipe1" << '\n';
        return 1;
    }
    temp = pipe(fd2);
    if (temp == -1) {
        std::cerr << "An error occured with creating a pipe2" << '\n';
        return 1;
    }

    int write1 = fd1[1], read1 = fd1[0];
    int write2 = fd2[1], read2 = fd2[0];

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "An error occured with creating a child process" << '\n';
        return 1;
    }
    if (pid == 0) {
        //child process
        close(write1);
        close(read2);
        temp = dup2(read1, fileno(stdin));
        if (temp == -1) {
            std::cerr << "An error occured with redirecting input" << '\n';
            return 1;
        }
        temp = dup2(write2, fileno(stdout));
        if (temp == -1) {
            std::cerr << "An error occured with redirecting output" << '\n';
            return 1;
        }
        temp = execl("child", "child", name.c_str(), NULL);
        if (temp == -1) {
            std::cerr << "An error occured with runing program from a child process" << '\n';
            return 1;
        }
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        //parent process
        close(read1);
        close(write2);
        float value;
        char c;
        std::vector<float> vec;
        while(scanf("%f%c", &value, &c) > 0) {
            vec.push_back(value);
            if (c == '\n') {
                int size = vec.size();
                dprintf(write1, "%d ", size);
                for (int i = 0; i < size; ++i) {
                    dprintf(write1, "%f ", vec[i]);
                }
                vec.clear();
            }
        }
        close(write1);
        close(read2);
        wait(nullptr);
    }
    return 0;
}