#include <iostream>
#include <fstream> 
#include <string>

using namespace std;


/**
 *  reads and prints the file f
 *  param: string f is the name of the file
 *  return: -1 if error, 0 if everything ok 
 */
int print_file(string f)
{
    ifstream lista(f);
    
    string linha;
    
    if (!lista)
    {
        return -1;
    }

    //flote

    else
    {
        while(getline(lista, linha))
        {
            cout << linha << endl;
        }
    }
    
    lista.close();
    
    return 0;
}

/**
 *  counts and prints the number of chars in file f
 *  param: string f is the name of the file
 *  return: -1 if error, 0 if everything ok 
 */
int count_characters(string f)
{
    return 0;
}


/**Test code. Do not change**/

int main() {
	print_file();
    count_characters();
    return 0;
}

