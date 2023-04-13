# Sudoku-Game
Implemented Sudoku Game in C using Multithreading, Inter-Process Communcation, Synchronization and Memory Managment.

For SUDOKO SOLVER AND VALIDATOR:

-Three functions are created to check rows, columns, and 3x3 matrix boxes of the matrix. 

-Two for loops are used in each function to check each element of the matrix.

-An array is added to compare each element with the array to check validity.

-Joinable threads are used to implement the SUDOKO SOLVER AND VALIDATOR.

-Threads work in parallel, which decreases computation time and uses system resources efficiently.

-All threads are joined with the main driver thread to receive resources for further processing.

-Mutex lock is used in the Solver to avoid the board to be read while the change is being updated.

For FILE SEARCH ENGINE:

-Threading can be used in FILE SEARCH ENGINE.

-Each file can be searched in a separate thread to improve performance.

-Semaphores can be used to limit threads performing read-write operations if a replacement is needed.

For GAME:

-Each thread can be created for each box/piece on the board.

-The whole board can be stored, and the location of each piece in each thread ID can be stored for smooth execution.
