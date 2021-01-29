#include<bits/stdc++.h>
#include <iostream> 
using namespace std;


int main()
{
    string myText;
    ifstream MyReadFile("input.txt");
    ofstream MyFile("output.txt");

    vector<int> cols_lengths;
    
    getline (MyReadFile, myText); 
    cout<<(myText.substr(12, 99));
    MyFile << myText;

    MyFile.close();
}