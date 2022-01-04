//
// Created by lenovo on 2021/12/12.
//
#include "error.h"

varlist::varlist(string name,string kind,string type,int lev,int interpret,int array)//初始化
{
    this->name=name;
    this->kind=kind;
    this->type=type;
    this->lev=lev;
    this->interpret=interpret;
    this->array=array;
}

string varlist::outname()
{
    return name;
}
string varlist::outkind()
{
    return kind;
}
string varlist::outtype()
{
    return type;
}
int varlist::outlev()
{
    return lev;
}
int varlist::outinterpret()
{
    return interpret;
}
void varlist::ininterpret(int temp)
{
    interpret=temp;
    return;
}
int varlist::outarray()
{
    return array;
}
//
funclist::funclist(string proc,string para,int parray)//初始化
{
    this->proc=proc;
    this->para=para;
    this->parray=parray;
}
string funclist::outproc()
{
    return proc;
}
string funclist::outpara()
{
    return para;
}
int funclist::outparray()
{
    return parray;
}
ErrorE::ErrorE(string Epara,int Eparray)//初始化
{
    this->Epara=Epara;
    this->Eparray=Eparray;
}

string ErrorE::outEpara()
{
    return Epara;
}
int ErrorE::outEparray()
{
    return Eparray;
}