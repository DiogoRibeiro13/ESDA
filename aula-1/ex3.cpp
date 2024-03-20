#include <iostream>

using namespace std;

void sumOfIntegers()
{
    int n, i, cnt=0;

    cout << "Enter an integer:";
    cin >> n;

    for(i=0; i<n; i++)
    {
        cnt += (n-i);
    }
   
   cout << "Result:" << cnt << "\n"; 
}


int main() {

   sumOfIntegers();

    return 0;
}  
