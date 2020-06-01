//
//  main.c
//  bbm203
//
//  Created by AhmetBayrak on 26.10.2018.
//  Copyright © 2018 AhmetBayrak. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int** readFile(FILE *file, int iRow, int iCol)                  // Reads the file, creates a 2D array and then fill it with data
{
    int iRowTemp = 0;
    int iColTemp = 0;
    
    int **maze;
    maze = (int**)malloc(sizeof(int*)*(iRow));                    //creates rows dynamically
    
    for(iRowTemp = 0 ; iRowTemp < iRow ; iRowTemp++)
    {
        maze[iRowTemp] = (int*)malloc(sizeof(int)*(iCol));        //creates coloumns dynamically
    }
    
    for(iRowTemp=0 ; iRowTemp < iRow ; iRowTemp++)
    {
        for( iColTemp=0 ; iColTemp < iCol ; iColTemp++)
        {
            fscanf(file, "%d ", &maze[iRowTemp][iColTemp]);     // fill the 2D array with data from the file
        }
    }
    
    fclose(file);                                               // closes the file
    
    return maze;
}

int matrixMult(int keyLength, int** mapMaze, int centerX, int centerY, int** keyMaze)   // multiplication of sub-matrix of map matrix and key matrix
{                                                                                       // [centerX][centerY] represents [0][0] of sub-matrix of map matrix
    int total = 0;
    int iRow = 0;
    int iCol = 0;
    
    for(iRow = 0 ; iRow < keyLength ; iRow++)
    {
        for(iCol = 0; iCol < keyLength; iCol++)
        {
            total += mapMaze[centerX+iRow][centerY+iCol] * keyMaze[iRow][iCol];
        }
    }
    
    return total;
}

void findTreasure(int **mapMaze, int **keyMaze, int centerX, int centerY, int keyLength, int mapRow, int mapCol, FILE *outFile)                                                                //
{
    int total = 0;
    total = matrixMult(keyLength, mapMaze, centerX, centerY, keyMaze);              // calculate multiplication of sub-matrix of map matrix and key matrix
    
    int shift = keyLength/2;                                // for finding center of sub-matrix
    int treasureFound = 0;                                  // determine whether treasure found for exit situation
    
    fprintf(outFile, "%d,%d:%d\n", centerX+shift, centerY+shift, total);            // write the current center of sub-matrix of map matrix and the result of
                                                                                    //      multiplication of sub-matrix of map matrix and key matrix
    int mod = 5;                            // initializing mod variable
    mod = total %5;                         // takes mod5 of total
    
    switch(mod) {
            
        case 0:                             // If mod5 = 0 then find treasure
            treasureFound = 1;
            fclose(outFile);                // closes the output file
            
            free(mapMaze);                  // frees the multidimensional array for avoiding memory leak
            free(keyMaze);                  // frees the multidimensional array for avoiding memory leak
            exit(treasureFound);            // exit the program
            break;
            
        case 1:                             // If mod5 = 1 then it means go up
            
            centerX -= keyLength;           // going up
            if(centerX < 0)                 // if there is no up then go down
            {
                centerX += keyLength*2;     // going down
            }
            findTreasure(mapMaze, keyMaze, centerX, centerY, keyLength, mapRow, mapCol, outFile);  // call function recursively
            break;
            
        case 2:                             // If mod5 = 2 then it means go down
            
            centerX += keyLength;           // going down
            if(centerX > mapRow-keyLength)  // if there is no down then go up
            {
                centerX -= keyLength*2;     // going up

            }
            findTreasure(mapMaze, keyMaze, centerX, centerY, keyLength, mapRow, mapCol, outFile);  // call function recursively
            break;
            
        case 3:                             // If mod5 = 3 then it means go right
            
            centerY += keyLength;           // going right
            if(centerY > mapCol-keyLength)  // if there is no right then go left
            {
                centerY -= keyLength*2;     // going left
            }
            findTreasure(mapMaze, keyMaze, centerX, centerY, keyLength, mapRow, mapCol, outFile);  // call function recursively
            break;
            
        case 4:                             // If mod5 = 4 then it means go left
            
            centerY -= keyLength;           // going left
            if(centerY < 0)                 // if there is no left then go right
            {
                centerY += keyLength*2;     // going right
            }
            findTreasure(mapMaze, keyMaze, centerX, centerY, keyLength, mapRow, mapCol, outFile);  // call function recursively
            break;
            
        default:
            printf("default case\n");
    }
    
}

int main(int argc, const char* argv[]) {
    
    int mapRow = atoi(argv[1]);                             // number of row of map matrix

    int keyRow = atoi(argv[2]);                             // number of row of key matrix
    int keyCol = atoi(argv[2]);                             // number of coloumn of key matrix
    
    int mapCol = 0;                                         // initialize number of coloumn of map matrix
    
    const char s[1] = "x";                                  // parse according to x to find number of coloumn
    char *token;
    
    token = strtok(argv[1], s);
    
    while( token != NULL ) {
        mapCol = atoi(token);
        token = strtok(NULL, s);
    }
    
    FILE *mapFile = fopen(argv[3], "r");                    // takes the name of map matrix and open the file
    FILE *keyFile = fopen(argv[4], "r");                    // takes the name of key matrix and open the file
    FILE *outFile = fopen(argv[5], "w");                    // takes the name of output file and open the file
    
    int **mapMaze = readFile(mapFile, mapRow, mapCol);      // creates and fills the 2D array of map matrix
    int **keyMaze = readFile(keyFile, keyRow, keyCol);      // creates and fills the 2D array of key matrix
    
    int centerX = 0;                                        // starting point of x
    int centerY = 0;                                        // starting point of y
    
    findTreasure(mapMaze, keyMaze, centerX, centerY, keyRow, mapRow, mapCol, outFile);     // start the function
    
    return 0;
}
