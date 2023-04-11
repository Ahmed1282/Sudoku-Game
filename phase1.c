// Muhammad Mujtaba 20i-0649
// Ahmed Baig 20i-1884

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mut1;

//Grid for the Sudoku
int grid[9][9] = {  
            {6, 2, 4, 5, 9, 9, 1, 8, 7},
            {5, 1, 9, 7, 2, 8, 6, 3, 4},
            {8, 3, 7, 6, 1, 4, 2, 9, 5},
            {1, 4, 3, 8, 6, 5, 7, 2, 9},
            {9, 5, 8, 2, 4, 7, 3, 6, 1},
            {7, 6, 2, 3, 9, 1, 4, 5, 8},
            {3, 7, 1, 9, 5, 6, 8, 4, 2},
            {4, 9, 6, 1, 8, 2, 5, 7, 3},
            {2, 8, 5, 4, 7, 3, 9, 1, 6}
};

//For the invalid entries     
int invalid; 
        
int matrix[10][2];

//------------------ Functions to implement phase 1 ------------------------------------------//


//Checking within a matrix block
void * checkSingleMatrix(void * in)
{
    pthread_mutex_lock(&mut1);

    int index = (int) in;
    int check=0;
    int value[10] = {0};
    int starting_column = matrix[index][1];
    int starting_row = matrix[index][0];
    
    for (int i = starting_row; i < starting_row + 3; ++i){

        for (int j = starting_column; j < starting_column + 3; ++j){
            int num = grid[i][j];

		    if(num<0 || num>9){
		        printf("\nThe invalid number is: %d", num);
            }  

            if(value[num] != 0){
  		        invalid+=1;
                check=1;
  		        printf("\nthread id of invalid thread: %ld", pthread_self());
  		        printf("\nInvalid Square At: %d", starting_row);
  		        printf(" %d", starting_column); 
            }
            else{
                value[num] = 1;
            }
        }
    }

	if(check == 1)
    {
        pthread_mutex_unlock(&mut1);
		return 0;
    }
    // thread cancelation
    // else if(check==-1)
    // {
    //     pthread_cancel(pthread_self());
    // }

    pthread_mutex_unlock(&mut1);
    return (void *) 1;
}

//Checkig the rows
void * checkRows(void * in) 
{
    pthread_mutex_lock(&mut1);


	int index = (int) in;
    int check=0;
    int starting_column = matrix[index][1];
    int starting_row = matrix[index][0];

    for(int i = starting_row; i < 9; ++i){
        
        int row[10] = {0};
        for(int j = starting_column; j < 9; ++j){
            
            int num = grid[i][j];

            if(num<0 || num>9){
		        printf("\nThe invalid number is: %d", num);
            }

            if(row[num] != 0){
  		        invalid+=1;
                check=1;
  		        printf("\nthread id of invalid thread: %ld", pthread_self());
                printf("\nInvalid row:%d ", j);
                printf( "\nAt column:%d ", i);

                pthread_mutex_unlock(&mut1);
  		        return (void *) 0;
            }

            else{
                row[num] = 1;
            }
        }
    }
    
    if(check == 1)
    {
        pthread_mutex_unlock(&mut1);
	    return 0;
    }
    // thread cancelation
    // else if(check==-1)
    // {
    //     pthread_cancel(pthread_self());
    // }

    pthread_mutex_unlock(&mut1);
    return (void *) 1;
}

//Check the columns
void * checkColumns(void * in) 
{
    pthread_mutex_lock(&mut1);

	int index = (int) in;
    int check=0;
    int starting_column = matrix[index][1];
    int starting_row = matrix[index][0];

    for(int i = starting_column; i < 9; ++i){
        
        int col[10] = {0};

        for(int j = starting_row; j < 9; ++j){
            
            int num = grid[j][i];

            if(num<0 || num>9){
	    	    printf("\nThe invalid number is: %d", num);
            }
            
            if(col[num] != 0){
  		        invalid+=1;
                check=1;
  		        printf("\nthread id of invalid thread: %ld", pthread_self());
                printf("\nInvalid column:%d ", i);
                printf( "\nAt Row:%d ", j);

                pthread_mutex_unlock(&mut1);
                return (void *) 0;
            }
            else{
                col[num] = 1;
            }
        }
    }

    if(check == 1)
    {
        pthread_mutex_unlock(&mut1);
	    return 0;
    }
    // thread cancelation
    // else if(check==-1)
    // {
    //     pthread_cancel(pthread_self());
    // }

    pthread_mutex_unlock(&mut1);
	return (void *) 1;
}

//------------------------ Main of the code ------------------------------------------//
int main() 
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

    pthread_mutex_init(&mut1,NULL);
      
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
        printf("\nThe Sudoku Puzzle is not solved!\n");
    }
    //if check = 1 Sudoku Pass
    else if(check == 1){
        printf("\nThe Sudoku Puzzle is solved.\n");
    }
    //if check != 1 Sudoku Fails
    else{
        printf("\nThe Sudoku Puzzle is not solved!\n");
    }

    pthread_exit(NULL);  
    return 0;

}