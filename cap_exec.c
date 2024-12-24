#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/capability.h>
#include <unistd.h>
#include <sys/wait.h>

void set_capabilities() {
    struct __user_cap_header_struct header;
    struct __user_cap_data_struct data[2];

    // установка заголовка
    header.version = _LINUX_CAPABILITY_VERSION_3;
    header.pid = getpid(); 

    // установка CAP_SETUID и CAP_SETGID
    data[0].effective = (1 << CAP_SETUID) | (1 << CAP_SETGID);
    data[0].permitted = (1 << CAP_SETUID) | (1 << CAP_SETGID);
    data[0].inheritable = 0;

    // применение привилегий
    if (capset(&header, data) < 0) {
        perror("capset");
        exit(EXIT_FAILURE);
    }
}

void drop_capabilities() {
    struct __user_cap_header_struct header;
    struct __user_cap_data_struct data[2];

    // Установка заголовка
    header.version = _LINUX_CAPABILITY_VERSION_3;
    header.pid = getpid(); // Устанавливаем для текущего процесса

    // сброс всех привилегий
    data[0].effective = 0;
    data[0].permitted = 0;
    data[0].inheritable = 0;

    // применение привилегий
    if (capset(&header, data) < 0) {
        perror("capset");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <prog> <username>\n", argv[0]);
        return EXIT_FAILURE;
    }

    
    set_capabilities();

  
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
       
        execl(argv[1], argv[1], argv[2], NULL); // Передаем имя пользователя как аргумент
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        // Родительский процесс
        int status;
        waitpid(pid, &status, 0);
        
        
        //drop_capabilities();
        
        if (WIFEXITED(status)) {
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        }
    }

    return EXIT_SUCCESS;
}
