#include <iostream>
using namespace std;


// function that receives a two-dimensional array as a pointer 
// and calculates the double of each element in the array.

void doublePointers(int* matrix, int lines, int col)
{
    int i;

    for(i=0; i<(lines * col); i++)
    {
        *(matrix + i) = (*(matrix + i) * 2);
    }
}


// function that receives a two-dimensional array as a reference 
// and calculates the double of each element in the array.

void doubleReference( int (&matrix)[3][3], int lines, int col)
{
    int i, j;

    for (i = 0; i < lines; i++)
    {
        for (j = 0; j < col; j++) 
        {
            matrix[i][j] *= 2;
        }
    }
}


// function that receives a two-dimensional array as a reference
// and prints the elements per line.

void print( int (&matrix)[3][3], int lines, int col) 
{
    int i, j;

    for (i = 0; i < lines; i++)
    {
        cout << "[ ";

        for (j = 0; j < col; j++) 
        {
            cout << matrix[i][j] << " ";
        }
        
        cout << " ]" << endl;
    }
}


int main() {
    int matrix[][3] = {{1, 2, 3}, {4, 12, 6}, {7, 8, 9}};
    int lines = 3, col = 3;
    cout << "Matrix:"<<endl; 
    print(matrix,lines,col);
    doublePointers(&matrix[0][0],lines,col);
    cout << "Matrix double using the function with pointers is: " <<endl;
    print(matrix,lines,col);
    doubleReference(matrix,lines,col);
    cout << "Matrix double using the function with reference is: " <<endl;
    print(matrix,lines,col);
    return 0;
}