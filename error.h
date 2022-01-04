//
// Created by lenovo on 2021/12/12.
//

#ifndef GZR_ERROR_H
#include <bits/stdc++.h>
using namespace std;
#define GZR_ERROR_H
class varlist//
{
private:
    string name;
    string kind;
    string type;
    int lev;
    int interpret;
    int array;

public:
    varlist(string name,string kind,string type,int lev,int interpret,int array);//初始化
    string outname();
    string outkind();
    string outtype();
    int outlev();
    int outinterpret();
    void ininterpret(int);
    int outarray();

};
class funclist
{
private:
    string proc;
    string para;
    int parray;

public:
    funclist(string proc,string para,int parray);//初始化
    string outproc();
    string outpara();
    int outparray();

};

class ErrorE
{
private:
    string Epara;
    int Eparray;
public:
    ErrorE(string para,int Eparray);//初始化
    string outEpara();
    int outEparray();
};
#endif //GZR_ERROR_H
