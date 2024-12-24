#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/wait.h>

void print_uid(const char *label) {
    printf("%s UID: %d\n", label, getuid());
}
void print_gid(const char *label2) {
    printf("%s GID: %d\n", label2, getgid());
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <username>\n", argv[0]);
        return 1;
    }

    const char *username = argv[1];
    int pipefd[2]; // pipefd[0] - для чтения, pipefd[1] - для записи
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        close(pipefd[0]); 

        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        print_uid("Child before setuid");
        print_gid("Child before setgid");

        struct passwd *pw = getpwnam(username); 
        if (pw == NULL) {
            perror("getpwnam failed");
            return 1;
        }

        // смена uid
        if (setuid(pw->pw_uid) < 0) {
            perror("setuid failed");
            return 1;
        }

        // смена gid
        if (setgid(pw->pw_gid) < 0) {
            perror("setgid failed");
            return 1;
        }

        print_uid("Child after setuid");
        print_gid("Child after setgid");

        exit(0);
    } else {
        close(pipefd[1]); 

        print_uid("Parent process");
        print_gid("Parent process");

        char buffer[256];
        ssize_t bytesRead;
        
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '0';
            printf("%s", buffer);
        }

        close(pipefd[0]);
        wait(NULL);
    }

    return 0;
}
