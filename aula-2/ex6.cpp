#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Person
{
    string name;
    int age;
    string country;
};


/*Function to import from a file to a vector of a struct
* @return 0 se a importação teve sucesso
* @return -1 se ocorrer um erro 
*/

int import(const string name_file, Person (&v)[10])
{
    if (name_file.length() <= 0)
    {
        return -1;
    }

    ifstream file(name_file);
    if (!file.is_open())
    {
        return -1;
    }

    stringstream ss;
    string save;
    int pos = 0;

    while (getline(file, save) && pos < 10)
    {
        ss.clear;
        ss << save;
        getline(ss, v[pos].name, ',');
        ss >> v[pos].age;
        ss.ignore();
        getline(ss >> ws, v[pos].country);

        pos++;
    }

    file.close();

    return 0;
}   


void print (Person (&v)[10])
{
    for(int i=0;i<10;i++)
    {
        cout << "Name= "<<v[i].name << "--- Age= "<< v[i].age << "--- Country= "<<v[i].country<<endl; 
    }
}


int main()
{
    Person vperson[10];
    int result;
    result=import("lista.txt",vperson);
    if (result==0)
    {
    print(vperson);
    } else
    {
        cout << "return: "<< result << endl;
    }
    result=import("",vperson);
    if (result==0)
    {
    print(vperson);
    } else
    {
        cout << "return: "<< result << endl;
    }
}