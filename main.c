#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include <math.h>

typedef struct vec2
{
    int x;
    int y;
} vec2;

typedef struct Row
{
    int *values;
} Row;

typedef struct Matrix
{
    Row *rows;
    vec2 size;
} Matrix;

//if in windows
#ifdef _WIN32
size_t getline(char **lineptr, size_t *n, FILE *stream)
{
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL)
    {
        return -1;
    }
    if (stream == NULL)
    {
        return -1;
    }
    if (n == NULL)
    {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF)
    {
        return -1;
    }
    if (bufptr == NULL)
    {
        bufptr = malloc(128);
        if (bufptr == NULL)
        {
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while (c != EOF)
    {
        if ((p - bufptr) > (size - 1))
        {
            size = size + 128;
            bufptr = realloc(bufptr, size);
            if (bufptr == NULL)
            {
                return -1;
            }
        }
        *p++ = c;
        if (c == '\n')
        {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}
#endif //_WIN32

void getNumbersFromLine(const char *line, const ssize_t lineSize, int outBuff[], int *outBuffSize)
{
    //worst case scenario, whole line is a number. +1 for null termination
    char *currNumberBuff = malloc(sizeof(char) * lineSize + 1);
    int currNumberSize = 0;
    for (int i = 0; i < lineSize; i++)
    {
        const char currChar = line[i];
        if (currChar == ' ' || currChar == '\n')
        {
            if (currNumberSize > 0)
            {
                currNumberBuff[currNumberSize] = 0; // null terminate so atoi works
                outBuff[*outBuffSize] = atoi(currNumberBuff);
                (*outBuffSize)++;
                currNumberSize = 0;
            }
            if (currChar == '\n')
            {
                break;
            }
        }
        else
        {
            currNumberBuff[currNumberSize] = currChar;
            currNumberSize++;
        }
    }
    free(currNumberBuff);
}

Matrix getMatrixFromFile(const char *filename)
{
    Matrix matrix;
    FILE *file = fopen(filename, "r");
    assert(file != NULL);
    {
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        read = getline(&line, &len, file);
        int *numbers = malloc(sizeof(int) * matrix.size.x);
        int numberOfNumbers = 0;
        getNumbersFromLine(line, read, numbers, &numberOfNumbers);
        free(line);
        assert(numberOfNumbers == 2);
        matrix.size.y = numbers[0];
        matrix.size.x = numbers[1];
        free(numbers);
    }
    matrix.rows = malloc(sizeof(Row) * matrix.size.y);
    int currentRow = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1)
    {

        matrix.rows[currentRow].values = malloc(sizeof(int) * matrix.size.x);
        int unusedBecauseIAmABadBoiWhoDoesNoChecks = 0;
        getNumbersFromLine(line, read, matrix.rows[currentRow].values, &unusedBecauseIAmABadBoiWhoDoesNoChecks);
        currentRow++;
    }
    free(line);
    fclose(file);

    return matrix;
}

void freeMatrix(Matrix *matrix)
{
    for (int i = 0; i < matrix->size.y; i++)
    {
        free(matrix->rows[i].values);
    }
    free(matrix->rows);
}

void printMatrix(const Matrix *matrix)
{
    for (int i = 0; i < matrix->size.y; i++)
    {
        for (int j = 0; j < matrix->size.x; j++)
        {
            printf("%d ", matrix->rows[i].values[j]);
        }
        printf("\n");
    }
}



int power(int base, int exp)
{
    int result = 1;
    while(exp) { result *= base; exp--; }
    return result;
}



// Usage:
// ./a.out path/to/mymatrixfile
int main(int argc, char *argv[])
{

    Matrix pathList;
    int *weights;
    int leastWeight, leastWeightIndex;
    int numberOfRows, numberOfColumns, totalPaths;

    int i,j, sum;



    char *matrixFilename = argv[1];
    Matrix matrix = getMatrixFromFile(matrixFilename);

    printMatrix(&matrix);

    numberOfRows = matrix.size.y;
    numberOfColumns = matrix.size.x;


    totalPaths = numberOfRows * power(3, numberOfColumns-1);




    weights = malloc(sizeof(int) * totalPaths);
    for(i=0;i<totalPaths;i++) weights[i]=0; //init the table of weights


    //init path list
    pathList.rows = malloc(sizeof(Row) * totalPaths);
    for(i=0;i<totalPaths;i++) pathList.rows[i].values = malloc(sizeof(int) * numberOfColumns);

    
    for(i=0;i<totalPaths;i++) {

        for(j=numberOfColumns;j>0;j--){

            pathList.rows[i].values[j] = j;

        }

        
    }



    for(i=0;i<numberOfRows;i++){
        sum=0;
        for(j=numberOfColumns;j>0;j--){





            sum += matrix.rows[i].values[j];
            sum2 += matrix.rows[i].values[j];
            sum3 += matrix.rows[i].values[j];

            


        }

        printf("\n %d \n", sum);

    }







    // printf("\n %d \n", totalPaths);
    // printf("\n rows: %d \n", numberOfRows);
    // printf("\n columns: %d \n", numberOfColumns);




    freeMatrix(&matrix);
    freeMatrix(&pathList);

    free(weights);
}