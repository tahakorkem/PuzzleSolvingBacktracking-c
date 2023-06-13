#include <stdio.h>
#include <stdlib.h>

void createHashtable(int **hashtable, int n, int *m);

void printMatrix(char *colors[8], int **matrix, int n, int untilRow);

int solve(char *colors[], int **matrix, int n, int *hashtable, int m, int whichRow, int isDetailedMode);

int checkIfRowValid(int **matrix, int n, int whichRow, int *hashtable, int m);

int checkIfNValid(int n);

int main() {
    char *colors[8] = {"Yellow", "Green", "Brown", "Red", "Pink", "Blue", "Purple", "Orange"};
    int choice;
    int n;
    int isDetailedMode;

    printf("1-Basic Mode\n");
    printf("2-Detailed Mode\n");
    printf("Select mode: ");
    scanf("%d", &choice);

    isDetailedMode = choice == 2;

    printf("\n");

    // read n from input
    do {
        printf("Enter n: ");
        scanf("%d", &n);
    } while (!checkIfNValid(n));

    // list colors user will use it
    printf("\nYou will fill %dx%d puzzle with numbers from 1 to %d.\n", n, n, n);
    printf("\nEach number represents a color. Here is the list of colors:\n");
    int i;
    for (i = 0; i < n; i++) {
        printf("%d: %s\n", i + 1, colors[i]);
    }

    int m;
    int *hashtable;
    createHashtable(&hashtable, n, &m);

    int **matrix = (int **) malloc(sizeof(int *) * n);
    for (i = 0; i < n; i++) {
        matrix[i] = (int *) malloc(sizeof(int) * n);
    }

    // read matrix from input
    printf("\nEnter the matrix:\n");
    int j;
    for (i = 0; i < n; i++) {
        // user will enter in format 1 2 3 ... N
        // user must enter 1 to n
        // and this line must not contain duplicate numbers
        // if not, we will ask user to enter again
        do {
            printf("Enter row %d in form of (X1 X1 ... Xn): ", i + 1);
            for (j = 0; j < n; j++) {
                scanf("%d", &matrix[i][j]);
            }
        } while (!checkIfRowValid(matrix, n, i, hashtable, m));
    }

    printf("\n---------------------------");
    printf("\nThe matrix you entered is:\n");
    printMatrix(colors, matrix, n, n - 1);
    printf("---------------------------\n");

    int solution = solve(colors, matrix, n, hashtable, m, 0, isDetailedMode);
    if (solution) {
        printf("\nFound solution!\n");
        printMatrix(colors, matrix, n, n - 1);
    } else {
        printf("\nNo solution found!\n");
    }

    return 0;
}


int checkIfNValid(int n) {
    if (n < 3) {
        printf("n must be greater than or equal to 3. Please enter again.\n");
        return 0;
    }
    if (n > 8) {
        printf("n must be less than or equal to 8. Please enter again.\n");
        return 0;
    }
    return 1;
}


void printMatrix(char *colors[8], int **matrix, int n, int untilRow) {
    int i, j;
    for (i = 0; i <= untilRow; i++) {
        printf("%d || ", i);
        for (j = 0; j < n; j++) {
            //printf("%d ", matrix[i][j]);
            // print color instead of number
            printf("%-6s ", colors[matrix[i][j] - 1]);
        }
        printf("\n");
    }
}

int hash(int key, int m) {
    return key % m;
}

int hasHashtable(int *hashtable, int key, int m) {
    // use linear probing to find the key
    int i = hash(key, m);
    int count = 0;
    while (count < m && hashtable[i] != -1) {
        if (hashtable[i] == key) {
            return 1;
        }
        i = (i + 1) % m;
        count++;
    }
    return 0;
}

int insertHashtable(int *hashtable, int key, int m) {
    // use linear probing to insertHashtable the key
    int i = hash(key, m);
    int count = 0;
    while (count < m && hashtable[i] != -1) {
        if (hashtable[i] == key) {
            return 0;
        }
        i = (i + 1) % m;
        count++;
    }
    if (count < m) {
        hashtable[i] = key;
        return 1;
    }
    return 0;
}

void clearHashtable(int *hashtable, int m) {
    int i;
    for (i = 0; i < m; i++) {
        hashtable[i] = -1;
    }
}

int isPrimeNumber(int number) {
    int i;
    for (i = 2; i * i <= number; i++) {
        if (number % i == 0) {
            return 0;
        }
    }
    return 1;
}

int findMagicNumberAbove(int n) {
    int i = n + 1;
    while (!isPrimeNumber(i)) {
        i++;
    }
    return i;
}

void createHashtable(int **hashtable, int n, int *m) {
    *m = findMagicNumberAbove(n);
    *hashtable = (int *) malloc(sizeof(int) * (*m));
    clearHashtable(*hashtable, *m);
}

int isValid(int **matrix, int n, int *hashtable, int m, int untilRow) {
    // we check the matrix until the row untilRow
    // if the matrix is valid until the row untilRow, we return 1
    // valid means that the elements in the same column are different

    int i, j;
    for (i = 0; i < n; i++) {

        clearHashtable(hashtable, m);

        for (j = 0; j <= untilRow; j++) {
            // check if the element in the same column is different
            // using hash
            if (hasHashtable(hashtable, matrix[j][i], m)) {
                return 0;
            }
            insertHashtable(hashtable, matrix[j][i], m);
        }
    }
    return 1;
}

void rotateRowOnce(int **matrix, int n, int row) {
    int i;
    int temp = matrix[row][n - 1];
    for (i = n - 1; i > 0; i--) {
        matrix[row][i] = matrix[row][i - 1];
    }
    matrix[row][0] = temp;
}

int isRowInRange(int **matrix, int n, int whichRow, int from, int to) {
    int i;
    for (i = 0; i < n; i++) {
        if (matrix[whichRow][i] < from || matrix[whichRow][i] > to) {
            return 0;
        }
    }
    return 1;
}

int isRowUniqueSet(int **matrix, int n, int whichRow, int *hashtable, int m) {
    clearHashtable(hashtable, m);
    int i;
    for (i = 0; i < n; i++) {
        if (hasHashtable(hashtable, matrix[whichRow][i], m)) {
            return 0;
        }
        insertHashtable(hashtable, matrix[whichRow][i], m);
    }
    return 1;
}

int checkIfRowValid(int **matrix, int n, int whichRow, int *hashtable, int m) {
    if (!isRowInRange(matrix, n, whichRow, 1, n)) {
        printf("This row contains number out of range. Please enter again.\n");
        return 0;
    }
    if (!isRowUniqueSet(matrix, n, whichRow, hashtable, m)) {
        printf("This row contains duplicate numbers. Please enter again.\n");
        return 0;
    }
    return 1;
}

int solve(char *colors[], int **matrix, int n, int *hashtable, int m, int whichRow, int isDetailedMode) {
    // we use backtracking to find a solution
    // we try to fill the matrix row by row
    // if the matrix is valid until the current row, we continue
    // if the matrix is not valid, we backtrack
    // if it is backtracked, we rotate the row once and try again

    if (whichRow == n) {
        // we have found a solution
        return 1;
    }

    if (isDetailedMode) {
        printf("\nLooking for solution for row %d...\n", whichRow);
    }

    // if we rotate n times, we reach the original matrix
    // so we can stop here and return 0
    int i;
    for (i = 0; i < n; i++) {
        if (isDetailedMode) {
            printMatrix(colors, matrix, n, whichRow);
        }
        if (isValid(matrix, n, hashtable, m, whichRow)) {
            if (isDetailedMode) {
                printf("This row is valid. Continue to next row.\n");
            }
            if (solve(colors, matrix, n, hashtable, m, whichRow + 1, isDetailedMode)) {
                if (isDetailedMode) {
                    printf("\nBacktracking with solution :) from row %d.\n", whichRow + 1);
                }
                return 1;
            }
        } else {
            if (isDetailedMode) {
                printf("This row is not valid.\n");
            }
        }
        if (isDetailedMode) {
            printf("\nRotate row %d once.\n", whichRow);
        }
        rotateRowOnce(matrix, n, whichRow);
    }
    if (isDetailedMode) {
        printf("Backtracking with no solution :( from row %d.\n", whichRow);
    }
    return 0;
}
