//
// Created by lenovo on 2021/10/15.
//
#include <iostream>
#include "Lexical_Analyze.h"
#include "word.h"


void output(vector <word>result)
{
    ofstream  fout;
    int i;
    fout.open("output.txt");
    //cout << "output" << endl;

    for(i=0;i<result.size();i++)
    {
        fout<<result[i].outCategory()+" "+result[i].outContent()<<endl;
    }
    fout.close();
}