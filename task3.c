#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    mkfifo("fifo_A", 0666);
    mkfifo("fifo_B", 0666);

    if (fork() == 0) {
        int fd_A = open("fifo_A", O_RDONLY);
        int fd_B = open("fifo_B", O_WRONLY);

        char bufor[10];
        read(fd_A, bufor, 10);
        printf("[DZIECKO] Odebrano: %s\n", bufor);
        printf("[DZIECKO] Wysyłam: PONG\n");
        write(fd_B, "PONG", 5);
        close(fd_A);
        close(fd_B);
        exit(0);
    } 
    else {
        int fd_A = open("fifo_A", O_WRONLY);
        int fd_B = open("fifo_B", O_RDONLY);

        printf("[RODZIC] Wysyłam: PING\n");
        write(fd_A, "PING", 5);
        char bufor[10];
        read(fd_B, bufor, 10);
        printf("[RODZIC] Odebrano: %s\n", bufor);
        wait(NULL); 
        close(fd_A);
        close(fd_B);
        unlink("fifo_A");
        unlink("fifo_B");
    }
    return 0;
}