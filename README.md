[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-718a45dd9cf7e7f842a935f5ebbe5719a5e09af4491e668f4dbf3b35d5cca122.svg)](https://classroom.github.com/online_ide?assignment_repo_id=10779657&assignment_repo_type=AssignmentRepo)
# Assign 02 - Multiprocess Conway Game
|Name|Id|
|-|-|
|Ahmed Raza|k21-3056|


## Intro
This program is an implementation of Conway's Game of Life, which is a simulation of the growth and death of cells in a grid. The program uses shared memory to store the grid, and forks child processes to update each row of the grid. A pipe is used for inter-process communication between the parent and child processes.

## Approach
The main function initializes the shared memory segment for the grid and assigns it to a 2D array using a pointer. It then initializes the grid with random values, creates a new shared memory segment for the updated grid, and assigns it to a new 2D array.

Next, the program creates a pipe for inter-process communication between the parent and child processes. It then forks child processes to update each row of the grid. The child processes read the row index from the pipe, update their assigned row of the grid according to the rules of the game, and write the updated row back to the parent process through the pipe. The parent process continues to fork child processes until all rows of the grid have been updated.

After all child processes have finished, the updated grid is copied from the new shared memory segment to the original one. The program then prints the updated grid to the console. Finally, the shared memory segments are detached and deleted, and the program exits.

Overall, this program provides a parallel implementation of Conway's Game of Life using shared memory and forked child processes, which can be used to simulate larger grids and speed up the computation time.

## Problems Faced
making multithreaded from single thread

## Online / Chat GPT Help
Mention where you got help for your assignment and if you used chat gpt (not encouraged) mention the questions you gave and the reply you got that you are using in the assignment.

## Screenshots
![image](https://user-images.githubusercontent.com/126978743/231852491-7e6ed392-090b-4461-8021-eec19a9a10ba.png)

