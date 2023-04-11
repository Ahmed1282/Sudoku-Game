// Muhammad Mujtaba 20i-0649
// Ahmed Baig 20i-1884

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//These are the functions(prototypes) that we have created  :
void* validateTheGuess(void*);//to validate the guess
int solve(); //to solve the sudoko puzzel. It is the main driver code
int findEmptyIndex(int *, int *); //to find empty cells to make guesses

// A structure to contain rows and column and guess to put at that index 
typedef struct 
{
  int row;
  int column;
  int guess;
}val; // val is the object of structure

// these two variables will contain the count of valid and in valid guesses
int invalid=0;
int valids = 0;

// Initial unsolved grid of game 
int grid[9][9] = {    {1, 7, 4, 0, 9, 0, 6, 0, 0},
                      {0, 0, 0, 0, 3, 8, 1, 5, 7},
                      {5, 3, 0, 7, 0, 1, 0, 0, 4},
                      {0, 0, 7, 3, 4, 9, 8, 0, 0},
                      {8, 4, 0, 5, 0, 0, 3, 6, 0},
                      {3, 0, 5, 0, 0, 6, 4, 7, 0},
                      {2, 8, 6, 9, 0, 0, 0, 0, 1},
                      {0, 0, 0, 6, 2, 7, 0, 3, 8},
                      {0, 5, 3, 0, 8, 0, 0, 9, 6}
                    };

int grid1[9][9] = {    {1, 7, 4, 0, 9, 0, 6, 0, 0},
                      {0, 0, 0, 0, 3, 8, 1, 5, 7},
                      {5, 3, 0, 7, 0, 1, 0, 0, 4},
                      {0, 0, 7, 3, 4, 9, 8, 0, 0},
                      {8, 4, 0, 5, 0, 0, 3, 6, 0},
                      {3, 0, 5, 0, 0, 6, 4, 7, 0},
                      {2, 8, 6, 9, 0, 0, 0, 0, 1},
                      {0, 0, 0, 6, 2, 7, 0, 3, 8},
                      {0, 5, 3, 0, 8, 0, 0, 9, 6}
                    };


int main() 
{

  // Calling the solve function to complete the grid of Sudoku 
  if (solve()) 
  {
    // Displaying the number of valid and invalid tries
    printf("\nINVALID TRIES: %d", invalid);
    printf("\nVALID TRIES: %d", valids);

    // Displaying the original grid
    printf("\n\n\nORIGINAL GRID :\n");
    for (int x = 0; x < 9; ++x) 
    {
      for (int y = 0; y < 9; ++y) 
      {
        printf(" %d", grid1[x][y]);
      }
      printf("\n");

      // It is added just for our own convenience to see grid easily
      if (x % 3 == 2)
      { 
        printf("\n");
      }
    }

    printf("\ncorrect solution: \n");

    for (int x = 0; x < 9; ++x) 
    {
      for (int y = 0; y < 9; ++y) 
      {
        printf(" %d", grid[x][y]);
      }
      printf("\n");

      // It is added just for our own convenience to see grid easily
      if (x % 3 == 2)
      { 
        printf("\n");
      }
    }
  }
  else 
  {
    // If the solution not found, than the grid which is solved as max as possible will be displayed 

    printf("\n\nSOLUTION NOT FOUND\n\n");
    printf("\n+-----+-----+-----+\n");
    
    for (int x = 0; x < 9; ++x) 
    {
      for (int y = 0; y < 9; ++y)
      {
        printf("%d", grid[x][y]);
      } 
      printf("\n");

      // It is added just for our own convenience to see grid easily
      if (x % 3 == 2)
      {
        printf("\n");
      } 
    }
  }
  return 0;
}


// This function will validate the guess put at that index of the grid
void* validateTheGuess(void* args) 
{
	val* v	= (val*)args;

  // Assigning new_ variables 
  int new_x = v->row / 3 * 3;
  int new_y = v->column / 3 * 3;

  // checking the guess for row
  for (int x = 0; x < 9; ++x) 
  {
    if (grid[v->row][x] == v->guess)
    {
      return (void*)0;
    } 
    
    // checking the guess for column
    if (grid[x][v->column] == v->guess)
    {
      return (void*)0;
    } 

    // checking the guess for each matrix
    if (grid[new_x + (x % 3)][new_y + (x / 3)] == v->guess)
    {
      return (void*)0;
    } 
  }

  return (void*)1; 
}

// This function will find the empty indices in the grid and return their position
int findEmptyIndex(int *row, int *column) 
{
  for (int x = 0; x < 9; x++) 
  {
    for (int y = 0; y < 9; y++) 
    {
      // if found an empty index, store its position
      if (!grid[x][y]) 
      {
        *row = x;
        *column = y;

        return 1;
      }
    }
  }
  return 0;
}

// Creating threads
pthread_t tid[10];

// To return the values from thread
void* results[10];

//solving function
//THE GUESSING PART IS IMPLIMENTED USING THREADS TO CHECK IN PARALLEL WHETHER THE GUESS WAS CORRECT OR NOT
int solve() 
{
  int row;
  int column;

  // it will end the solve function when all the indices are solved or when no solution is found
  if(!findEmptyIndex(&row, &column))
  {
    return 1;
  } 

  // dynamic allocation 
  val* data = (val*) malloc(sizeof(val));

  for (int guess = 1; guess < 10; guess++) 
  {
    // Storing the index of empty index
  	data->row = row;
  	data->column = column;

    // Assigning a guess to the index
  	data->guess = guess;
  	
    // Threads creation
  	pthread_create(&tid[guess-1],NULL, validateTheGuess, (void*) data);
    pthread_join(tid[guess-1],&results[guess-1]);
     
    // If position is received, than assigning 
    if ((int)results[guess-1]) 
    {
      // Storing the guess at index
      grid[row][column] = guess;

      // Incrementing the valid variable
      valids++;

      // Calling the solve function to check whether the guess is at right place or not
      if(solve())
      {
        return 1;
      } 

      // At first it fills the whole grid, then it will check index by index, if not appropriate than makes it zero.
      grid[row][column] = 0;
  	}
  
    // Printing the thread ids, row and column of that thread, guess at that position
    printf("----------------------\n");
    printf("Thread ID: %ld", pthread_self());
    printf("\nRow:%d",data->row);
    printf("\nCol:%d",data->column);
    printf("\nThis is invalid position for Guess:%d",data->guess);
    printf("\n");

    invalid++;
	}
  
  return 0;
}

///////////////////////////


//For the invalid entries     
int invalid; 
        
int matrix[10][2];

//------------------ Functions to implement phase 1 ------------------------------------------//


//Checking within a matrix block
void * checkSingleMatrix(void * in){

    int index = (int) in;
    int check=0;
    int value[10] = {0};
    int starting_column = matrix[index][1];
    int starting_row = matrix[index][0];
    
    for (int i = starting_row; i < starting_row + 3; ++i){

        for (int j = starting_column; j < starting_column + 3; ++j){
            int num = grid[i][j];

		    if(num<0 || num>9){
		        //printf("\nThe invalid number is: %d", num);
            }  

            if(value[num] != 0){
  		        invalid+=1;
                check=1;
  		        // printf("\nthread id of invalid thread: %ld", pthread_self());
  		        // printf("\nInvalid Square At: %d", starting_row);
  		        // printf(" %d", starting_column); 
            }
            else{
                value[num] = 1;
            }
        }
    }

	if(check == 1){
		return 0;
    }

    return (void *) 1;
}

//Checkig the rows
void * checkRows(void * in) {

	int index = (int) in;
    int check=0;
    int starting_column = matrix[index][1];
    int starting_row = matrix[index][0];

    for(int i = starting_row; i < 9; ++i){
        
        int row[10] = {0};
        for(int j = starting_column; j < 9; ++j){
            
            int num = grid[i][j];

            if(num<0 || num>9){
		      //  printf("\nThe invalid number is: %d", num);
            }

            if(row[num] != 0){
  		        invalid+=1;
                check=1;
  		        // printf("\nthread id of invalid thread: %ld", pthread_self());
              //   printf("\nInvalid row:%d ", j);
              //   printf( "\nAt column:%d ", i);
  		        return (void *) 0;
            }

            else{
                row[num] = 1;
            }
        }
    }
    
    if(check == 1){
	    return 0;
    }

    return (void *) 1;
}

//Check the columns
void * checkColumns(void * in) {

	int index = (int) in;
    int check=0;
    int starting_column = matrix[index][1];
    int starting_row = matrix[index][0];

    for(int i = starting_column; i < 9; ++i){
        
        int col[10] = {0};

        for(int j = starting_row; j < 9; ++j){
            
            int num = grid[j][i];

            if(num<0 || num>9){
	    	  //  printf("\nThe invalid number is: %d", num);
            }
            
            if(col[num] != 0){
  		        invalid+=1;
                check=1;
  		        // printf("\nthread id of invalid thread: %ld", pthread_self());
              //   printf("\nInvalid column:%d ", i);
              //   printf( "\nAt Row:%d ", j);
                return (void *) 0;
            }
            else{
                col[num] = 1;
            }
        }
    }

    if(check == 1){
	    return 0;
    }
	return (void *) 1;
}

//------------------------ Main of the code ------------------------------------------//
int main2() 
{

    //Initializing the coordinates of all 9 squares
	int start=0;
	for (int i=0; i<9; i+=3){
	    for (int j=0; j<9; j+=3)
	    {
		    matrix[start][0] = i;
		    matrix[start][1] = j;
		    start++;	
	    }
    }

    int check=1; //1 if no repeat or 0 if repeat
	int matrix_no=0;

    //Total 11 thread
    //9 for 9 squares
    //2 for validating rows and columns
      
    pthread_t thread[11];
    int index = 0;
    void* returns[11];
    pthread_create(&thread[0], NULL, checkRows, (void*) index);
	pthread_create(&thread[1], NULL, checkColumns, (void*) index);

    for(int i=0; i<9; i+=3){
    	for(int j=0; j<9; j+=3)
    	{
            matrix[matrix_no][1] = j;
    	    matrix[matrix_no][0] = i;
    	    matrix_no++;
    	}
    }
    	    
   	for (int i=2; i<11; i++){
        pthread_create(&thread[i],NULL,checkSingleMatrix,(void*)index);
    	index++;
    }
    
   	for (int i=0; i<11; i++){
   	    pthread_join(thread[i], &returns[i]);
   	    if(returns[i] == 0){
   	        check=0;
        }
   	}
    //if check = 0 Sudoku Fails
    if (check == 0){
    //    printf("\nThe Sudoku Puzzle is not solved!\n");
    }
    //if check = 1 Sudoku Pass
    else if(check == 1){
    //    printf("\nThe Sudoku Puzzle is solved.\n");
    }
    //if check != 1 Sudoku Fails
    else{
    //    printf("\nThe Sudoku Puzzle is not solved!\n");
    }
    return 0;

}