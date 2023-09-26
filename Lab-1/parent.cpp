#include <iostream>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/wait.h>
#include <fcntl.h>
#include <vector>

int main() {
    std::string name;
    char c = 1;
    write(STDOUT_FILENO, "Enter the name of file: ", 24);
    while (c != '\n') {
        read(STDIN_FILENO, &c, sizeof(char));
        if (c != '\n') {
            name += c;
        }
    }

    int fd1[2], fd2[2];
    int temp = pipe(fd1);
    if (temp == -1) {
        write(STDERR_FILENO, "An error occured with creating a pipe1", 39);
        return 1;
    }
    temp = pipe(fd2);
    if (temp == -1) {
        write(STDERR_FILENO, "An error occured with creating a pipe2", 39);
        return 1;
    }

    int write1 = fd1[1], read1 = fd1[0];
    int write2 = fd2[1], read2 = fd2[0];

    pid_t pid = fork();
    if (pid == -1) {
        write(STDERR_FILENO, "An error occured with creating a child process", 47);
        return 1;
    }
    if (pid == 0) {
        //child process
        close(write1);
        close(read2);
        temp = dup2(read1, STDIN_FILENO);
        if (temp == -1) {
            write(STDERR_FILENO, "An error occured with redirecting input", 40);
            return 1;
        }
        temp = dup2(write2, STDOUT_FILENO);
        if (temp == -1) {
            write(STDERR_FILENO, "An error occured with redirecting output", 41);
            return 1;
        }
        temp = execl("child", "child", name.c_str(), NULL);
        if (temp == -1) {
            write(STDERR_FILENO, "An error occured with runing program from a child process", 58);
            return 1;
        }
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        //parent process
        close(read1);
        close(write2);
        char c;
        char prev = '?';
        std::vector<char> vec;
        int countvalues = 0;
        while(read(STDIN_FILENO, &c, 1)) {
            vec.push_back(c);
            if (c == '\n') {
                if ((prev >= '0') && (prev <= '9')) {
                    ++countvalues;
                }
                for (int i = 0; i < vec.size(); ++i) {
                    write(write1, &vec[i], 1);
                }
                vec.clear();
            }
            prev = c;
        }
        int temp = 0;
        while (read(read2, &c, 1)) {
            write(STDOUT_FILENO, &c, 1);
            if (c == '\n') {
                ++temp;
            }
            if (temp == countvalues) {
                break;
            }
            //write(STDOUT_FILENO, &c, 1);
        }
        close(write1);
        close(read2);
        wait(nullptr);
    }
    return 0;
}