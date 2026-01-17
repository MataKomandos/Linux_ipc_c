#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {

    mkfifo("dane.fifo", 0666);    
    int n = atoi(argv[1]);
    char bufor[50]; 

    for(int i=0; i<n; i++) {
        if(fork() == 0) {
            srand(time(NULL) ^ getpid());
            int val = rand() % 100;             
            sprintf(bufor, "%d %d", getpid(), val);
            int fd = open("dane.fifo", O_WRONLY);
            write(fd, bufor, 50); 
            close(fd);
            exit(0);
        }
    }
    int fd = open("dane.fifo", O_RDONLY);
    int suma = 0, pid_dziecka, liczba_dziecka;
    while(read(fd, bufor, 50) > 0) {
        sscanf(bufor, "%d %d", &pid_dziecka, &liczba_dziecka);
        printf("Odebrano -> PID: %d, Wartosc: %d\n", pid_dziecka, liczba_dziecka);
        suma += liczba_dziecka;
    }
    printf("Suma calkowita: %d\n", suma);    
    close(fd);
    unlink("dane.fifo");
    return 0;
}