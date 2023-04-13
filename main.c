/**
 * @defgroup   MAIN main
 *
 * @brief      This file implements main. Please Comment you code
 *
 * @author     Ahmed Raza 21k-3056
 * @date       13/4/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>


/**
 * @brief      { function_description }
 *
 * @param[in]  argc  The count of arguments
 * @param      argv  The arguments array
 *
 * @return     { description_of_the_return_value }
 */


#define ROWS 10
#define COLS 10

int count_neighbors(int* grid, int row, int col, int num_rows) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= num_rows || c < 0 || c >= COLS) continue;
            count += *(grid + r * COLS + c);
        }
    }
    return count;
}

void update_grid(int* grid, int start_row, int end_row) {
    int new_grid[ROWS][COLS] = {0};
    for (int i = start_row; i <= end_row; i++) {
        for (int j = 0; j < COLS; j++) {
            int neighbors = count_neighbors(grid, i, j, ROWS);
            if (*(grid + i * COLS + j)) {
                if (neighbors == 2 || neighbors == 3) {
                    new_grid[i][j] = 1;
                }
            } else {
                if (neighbors == 3) {
                    new_grid[i][j] = 1;
                }
            }
        }
    }
    for (int i = start_row; i <= end_row; i++) {
        for (int j = 0; j < COLS; j++) {
            *(grid + i * COLS + j) = new_grid[i][j];
        }
    }
}

int main()
{
    int num_processes = 4;

    // create shared memory segment
    int fd = shm_open("/mysharedmem", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, ROWS * COLS * sizeof(int));
    int* shared_grid = (int*) mmap(NULL, ROWS * COLS * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_grid == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // initialize shared grid with random values
    srand(time(NULL));
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            *(shared_grid + i * COLS + j) = rand() % 2;
        }
    }

        // print initial grid
    printf("Initial Grid:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", *(shared_grid + i * COLS + j));
        }
        printf("\n");
    }


    pid_t pid[num_processes];

    for (int i = 0; i < num_processes; i++) {
        pid[i] = fork();
        if (pid[i] == -1) {
            perror("fork");
            exit(1);
        } else if (pid[i] == 0) { // child process
            int start_row = i * ROWS / num_processes;
            int end_row = (i + 1) * ROWS / num_processes - 1;
            if (i == num_processes - 1) {
                end_row = ROWS - 1;
            }
            update_grid(shared_grid, start_row, end_row);
            exit(0);
        }
    }

    // wait for child processes to finish
    for (int i = 0; i < num_processes; i++) {
        waitpid(pid[i], NULL, 0);
    }

    // print final grid
    printf("Final Grid:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", *(shared_grid + i * COLS + j));
        }
        printf("\n");
    }

    // detach shared memory segment
    munmap(shared_grid, ROWS * COLS * sizeof(int));
    close(fd);
    shm_unlink("/mysharedmem");

    return 0;
}
