#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

 

#define MAX_ROWS_OR_COLS 25

 

typedef struct Vec2
{
    int x;
    int y;
} Vec2;

 

typedef struct Row
{
    int *values;
} Row;

 

typedef struct Matrix
{
    Row *rows;
    Vec2 size;
} Matrix;

 

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

 

int totRows, totCols;
int dp[MAX_ROWS_OR_COLS + 1][MAX_ROWS_OR_COLS + 1];
int mat[MAX_ROWS_OR_COLS + 1][MAX_ROWS_OR_COLS + 1];
int path[MAX_ROWS_OR_COLS + 1];

 

int min(int a, int b) {
    return a > b ? b : a;
}

 

int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

 

int cv(int i, int a){//Get the number of rows corresponding to upper, middle and lower
    if(i == 1 && a == -1) return totRows;
    if(i == totRows && a == 1) return 1;
    if(a == 0) return i;
    return i + a;
}

 

int dfs(int i, int j){
    if(j == totCols) return dp[i][j] = mat[i][j];
    if(dp[i][j] != -1) return dp[i][j];
    int ans = INT_MAX;
    for(int a = -1; a <= 1; a++)
        ans = min(ans, mat[i][j] + dfs(cv(i, a), j + 1));
    return dp[i][j] = ans;
}

 


void getNumbersFromLine(const char *line, const size_t lineSize, int outBuff[], int *outBuffSize)
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
        size_t read;
        read = getline(&line, &len, file);
        int *numbers = malloc(sizeof(int) * 26);
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
    size_t read;

 

    while ((read = getline(&line, &len, file)) != -1)
    {
        if (line[0] != '\n') // skip random newlines
        {
            matrix.rows[currentRow].values = malloc(sizeof(int) * 26);
            int unusedBecauseIAmABadBoiWhoDoesNoChecks = 0;
            getNumbersFromLine(line, read, matrix.rows[currentRow].values, &unusedBecauseIAmABadBoiWhoDoesNoChecks);
            currentRow++;
        }
    }
    free(line);
    fclose(file);

 

    return matrix;
}

 

// Usage: ./a.out path/to/matrix
void main(int argc, char *argv[])
{
    char *matrixFilename = argv[1];
    Matrix matrix = getMatrixFromFile(matrixFilename);

 

    totRows = matrix.size.y;
    totCols = matrix.size.x;

 

        for(int i = 1; i <= totRows; i++)
            for(int j = 1; j <= totCols; j++)
                mat[i][j] = matrix.rows[i-1].values[j-1];
    memset(dp, -1, sizeof(dp));
        int ans = INT_MAX, r;
        for(int i = 1; i <= totRows; i++)
            if(ans > dfs(i, 1)){
                r = i;
                ans = dfs(i, 1);
            }
        path[1] = r;
        for(int i = 2; i <= totCols; i++){
            int row[3], c2 = 0;
            for(int a = -1; a <= 1; a++)
                row[c2++] = cv(r, a);//Record three lines
            qsort(row, 4, sizeof(int), cmpfunc);//Ensure that the number of rows in each column is lexicographically minimum
            for(int j = 0; j < 3; j++)
                if(dp[r][i - 1] - mat[r][i - 1] == dp[row[j]][i]){
                    path[i] = r = row[j];//Update and record the number of answer lines
                    break;
                }
        }

 


        for(int i = 1; i <= totCols; i++)
            printf("%d ", path[i]);
        printf("\n%d\n", ans);
 //   }
}
