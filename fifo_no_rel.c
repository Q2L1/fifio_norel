#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

#define FIFO_PATH "/tmp/hello_fifo"
#define BUFFER_SIZE 100

int main(void) {
    char str[] = "Hello world";
    char str1[BUFFER_SIZE];  // Буфер для чтения строки
    pid_t child_pid;

    // Создаем именованный канал (FIFO)
    mkfifo(FIFO_PATH, 0666);

    // Создаем новый процесс
    child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid > 0) {
        // Родительский процесс
        int fd_write = open(FIFO_PATH, O_WRONLY);
        if (fd_write == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Пишем данные в канал
        write(fd_write, str, sizeof(str));

        close(fd_write);  // Закрываем дескриптор записи
    }
    else {
        // Дочерний процесс
        int fd_read = open(FIFO_PATH, O_RDONLY);
        if (fd_read == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Читаем данные из канала
        ssize_t bytesRead = read(fd_read, str1, sizeof(str1));
        if (bytesRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        close(fd_read);  // Закрываем дескриптор чтения

        // Выводим прочитанную строку
        printf("\nReceived message in child process: %.*s\n", (int)bytesRead, str1);
    }

    // Удаляем FIFO
    unlink(FIFO_PATH);

    return 0;
}
