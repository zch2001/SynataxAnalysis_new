//
// Created by lenovo on 2021/11/10.
//

#include "intermediateCode.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include<fstream>
using namespace std;

vector<Code> code;
int memory[200000];
vector<int>operation;
string format;
vector<IDENT>ident;
vector<vector<IDENT>>storeIdent;
vector<vector<int>>storeOperation;
extern vector<string>input;
int memoryCount = 0;
int globarPart = -1;
int Mainflag = 0;
ofstream outputfile;
void inCode(int instruct,string x, string y){
    Code temp;
    temp.instruct = instruct;
    temp.x = x;
    temp.y = y;
    code.push_back(temp);
}
void recordVar(string name, int size){
    struct IDENT temp;
    temp.name = name;
    temp.size = size;
    temp.address = memoryCount;
    temp.rowsize = 1;
    memoryCount++;
    ident.push_back(temp);
}
void defineVar(string name) {  /* def */
    operation.clear();
    recordVar(name,1);
}

void recordSize(){    /* rcdsize */
    ident.back().rowsize = operation.back();
}

void mallocMemory()    /* palloc */
{
    memoryCount += operation.back() - 1;
    ident.back().size = operation.back();
    operation.pop_back();
}

void Add(){
    int Top = operation.back();
    operation.pop_back();
    operation.back() += Top;
}
void Sub(){
    int Top = operation.back();
    operation.pop_back();
    operation.back() -= Top;
}
void Mult(){
    int Top = operation.back();
    operation.pop_back();
    operation.back() *= Top;
}
void Div(){
    int Top = operation.back();
    operation.pop_back();
    operation.back() /= Top;
}
void Mod(){
    int Top = operation.back();
    operation.pop_back();
    operation.back() %= Top;
}
void pushAdress(string name){   /* adress */    //x.x 为name
    reverse(ident.begin(),ident.end());
    for (auto &x : ident){
        if(x.name == name){
            operation.push_back(x.address);
            break;
        }
    }
    reverse(ident.begin(),ident.end());
}
void Assign(){            /* assign */
    int value = operation.back();
    operation.pop_back();
    int Address = operation.back();
    memory[Address] = value;
    //operation.pop_back(); // 把赋值完的地址弹出栈
}

void PutNum(string number){            /* putNum */
    int num = atoi(number.c_str());
    //operation.clear();
    operation.push_back(num);
}
void moveAdress(){     /* moveadress */
    operation.back()++;
}
void load(){      /* lod */
    int value = operation.back();
    operation.back() = memory[value];
}
void offset_1(){   /* offset1 */
    int offset = operation.back();
    operation.pop_back();
    operation.back() += offset;
}
void offset_2(string wordname){ /* offset2 */
    int rowsize;
    vector<IDENT>::iterator v;
    for(v = ident.end()-1;wordname != (*v).name; v--);
    rowsize = (*v).rowsize;
    int line;
    line = operation.back();
    operation.pop_back();   // 弹出列
    operation.back() = operation.back() * rowsize + line;  //计算偏移
    int offset = operation.back();
    operation.pop_back();   // 弹出偏移
    operation.back() += offset;
}
void moveToEnd(string varCount){   /* moveToend */
    int count = atoi(varCount.c_str());
    int move = ident.back().rowsize - count;
    operation.back() += move;
}
void ToMinus(){    /* toMinus */
    operation.back() = -operation.back();
}
void ToNOT(){    /* toNOT */
    operation.back() = !operation.back();
}

void BEGIN() {          /* Begin */
    IDENT temp;
    temp.name = "begin";
    temp.size = temp.rowsize = temp.address = -1;
    ident.push_back(temp);
}
void FuncDefine() {          /* funcdefine   */
    IDENT temp;
    temp.name = "funcdefine";
    temp.size = temp.rowsize = temp.address = -1;
    ident.push_back(temp);
}
void END(){           /* End */
    vector<IDENT>::iterator v = ident.end()-1;
    if(ident.end() == ident.begin()) return;
    while((*v).name!= "begin"){
        memoryCount -= (*v).size;
        v--;
        ident.pop_back();
    }
    if(!ident.empty())ident.pop_back(); //把begin pop出来
    //operation.clear();
}
void Getint(){             /* getint */
    int num;
    cin >> num;
    operation.push_back(num);
    /*static vector<string>::iterator v = input.begin();
    operation.push_back(atoi((*v).c_str()));
    v++;*/
}
void Or(){
    int Top = operation.back();
    operation.pop_back();
    int nextTop = operation.back();
    operation.pop_back();
    if(Top || nextTop) operation.push_back(1);
    else operation.push_back(0);
}
void And(){
    int Top = operation.back();
    operation.pop_back();
    int nextTop = operation.back();
    operation.pop_back();
    if(Top && nextTop) operation.push_back(1);
    else operation.push_back(0);
}
void Eql(){
    int Top = operation.back();
    operation.pop_back();
    int nextTop = operation.back();
    operation.pop_back();
    if(Top == nextTop) operation.push_back(1);
    else operation.push_back(0);
}
void Neql(){
    int Top = operation.back();
    operation.pop_back();
    int nextTop = operation.back();
    operation.pop_back();
    if(Top != nextTop) operation.push_back(1);
    else operation.push_back(0);
}
void Lss(){
    int Top = operation.back();
    operation.pop_back();
    int nextTop = operation.back();
    operation.pop_back();
    if(nextTop < Top) operation.push_back(1);
    else operation.push_back(0);
}
void Leq(){
    int Top = operation.back();
    operation.pop_back();
    int nextTop = operation.back();
    operation.pop_back();
    if(nextTop <= Top) operation.push_back(1);
    else operation.push_back(0);
}
void Gre(){
    int Top = operation.back();
    operation.pop_back();
    int nextTop = operation.back();
    operation.pop_back();
    if(nextTop > Top) operation.push_back(1);
    else operation.push_back(0);
}
void Geq(){
    int Top = operation.back();
    operation.pop_back();
    int nextTop = operation.back();
    operation.pop_back();
    if(nextTop >= Top) operation.push_back(1);
    else operation.push_back(0);
}
void If(int &i){    /* IF */
    int interCount = 0;
    if(operation.back() == 0){
        i = i+1;    // i-> if i+1-> begin
        while(1){
            i++;
            if(code[i].instruct == Begin) interCount++;
            else if(code[i].instruct == End && interCount > 0) interCount--;
            else if(code[i].instruct == End && interCount == 0) break;
        }
    }
}
void ElseIf(int &i){    /* ElseIf */
    int interCount = 0;
    vector<int>::iterator v = operation.end() - 1;
    if(*v == 0 || v == operation.begin()){    // 说明在上一层的 if/elseif已经进入了(只有一个本层的判断) 或者 本层条件为假
        operation.pop_back();   // 弹出判断语句的值
        i = i+1; // i-> if i+1-> begin
        while(1){
            i++;
            if(code[i].instruct == Begin) interCount++;
            else if(code[i].instruct == End && interCount > 0) interCount--;
            else if(code[i].instruct == End && interCount == 0) break;
        }
    }
    else if(*v != 0){
        int temp = operation.back();
        operation.pop_back();
        operation.back() = temp;    // 把次栈顶的值改为 当前判断语句的值
    }
}
void Else(int &i){    /* ElseIf */
    int interCount = 0;
    if(operation.empty()){    // 说明在上一层的 if/elseif已经进入了
        i = i+1; // i-> if i+1-> begin
        while(1){
            i++;
            if(code[i].instruct == Begin) interCount++;
            else if(code[i].instruct == End && interCount > 0) interCount--;
            else if(code[i].instruct == End && interCount == 0) break;
        }
    }
}
void FormatString(string p){    /* formatstring */
    p.pop_back();  // 删除最后的”
    p.erase(0,1);
    for(int i =0; i < p.size(); ++i){
        if(p[i]== '\\' && p[i+1] == 'n'){
            p[i] = '\n';
            p.erase(i+1,1);
        }
    }
    format = p;
}
void Replace(){             /* replace */
     int number = operation.back();
     operation.pop_back();
     string str = to_string(number);
     int i =0;
     string temp;
     for(i = 0; i < format.size(); ++i){
         if(format[i] == '%' && format[i+1] == 'd') break;
     }
     format.replace(i,2,str);
}
void Print(){
    static int open = 0;
    open++;
    if(open == 1){
        outputfile.open("pcoderesult.txt",ios::out);
    }
    outputfile << format;
    format.clear();
}
void While(int &i){
    if(operation.back() == 0){
        for(; code[i].instruct != endwhileMark; ++i);
    }
}
void whileMARK() {          /* whileMark */
    IDENT temp;
    temp.name = "whileMARK";
    temp.size = temp.rowsize = temp.address = -1;
    ident.push_back(temp);
}

void endwhileMARK (int &i){    /*  endwhileMark, */
    for(; code[i].instruct != whileMark; --i);
}

void BrK_Ctn (){           /* endwhileMark */   // 类比end  因为break和continue少了end环节
    vector<IDENT>::iterator v = ident.end()-1;
    while((*v).name!= "whileMARK"){
        memoryCount -= (*v).size;
        v--;
        ident.pop_back();
    }
    ident.pop_back(); //把whileMARK pop出来
    operation.clear();
}

void BREAK(int &i){
    for(; code[i].instruct != endwhileMark; ++i);
    ++i;
    BrK_Ctn();
}
void CONTINUE(int &i){
    for(; code[i].instruct != whileMark; --i);
    --i;
    BrK_Ctn();
}
void StoreOpteration(){            /* storeOP */
    storeOperation.push_back(operation);
    operation.clear();
}
void StoreIdent(){               /* storeID */
    storeIdent.push_back(ident);
    auto x = ident.end()-1;
    for(;(x->address >= globarPart || x->address == -1) && x >= ident.begin();x--){
        ident.pop_back();
    }

}

void _2Dmto1Dm(){
    int row = operation.back();  //第几行
    operation.pop_back();
    int Address = operation.back();
    operation.pop_back();
    reverse(ident.begin(),ident.end());
    for(auto &x:ident){
        if(x.address == Address){
            int offset = row * x.rowsize;
            //Address += offset;
            operation.push_back(Address);
            operation.push_back(offset);
            break;
        }
    }
    reverse(ident.begin(),ident.end());
}
void putSize(){     /* put0 */
    /*int Address = operation.back();
    reverse(ident.begin(),ident.end());
    for(auto &x:ident){
        if(x.address == Address){
            int size = x.size;
            operation.push_back(size);
            break;
        }
    }
    reverse(ident.begin(),ident.end());*/
    operation.push_back(0);              // 补位
}
void MAIN(){
    globarPart = memoryCount;
}

void jumptoFunc(string name,int &p){   /* callFun */
    int i = 0;
    storeOperation.back().push_back(p);
    for(int i = 0; i < code.size(); i++){
        if(code[i].instruct == funcDefine && code[i].x == name){
            p = i - 1;
            break;
        }
    }
}
void DefFormalVar(string name){   /* defFormalVar */
    recordVar(name,1);
}
void NormalVar(){          /* normalVar */
    reverse(operation.begin(),operation.end());
    int value = operation.back();    //栈底的值
    operation.pop_back();
    reverse(operation.begin(),operation.end());
    int Address = ident.back().address; //栈顶的地址
    memory[Address] = value;
}

void Array2Var(){
    reverse(operation.begin(),operation.end());
    int RAddress = operation.back();  // 栈底实参的地址
    operation.pop_back();
    reverse(operation.begin(),operation.end());
    int rowsize,size;
    vector<IDENT>temp = storeIdent.back();
    reverse(temp.begin(),temp.end());
    for(auto &x:temp){                 // 取出二维数组大小
        if(x.address == RAddress){
            rowsize = x.rowsize;
            size = x.size;
            break;
        }
    }
    ident.back().rowsize = rowsize;
    ident.back().size = size;
    for(int i = 0; i < size; ++i){
        memory[memoryCount - 1] = memory[RAddress++];
        memoryCount++;
    }
}
void Array1Var(){
    reverse(operation.begin(),operation.end());
    int RAddress = operation.back();
    operation.pop_back();
    int offset = operation.back();
    operation.pop_back();
    reverse(operation.begin(),operation.end());

    int size;
    vector<IDENT>temp = storeIdent.back();
    reverse(temp.begin(),temp.end());
    for(auto &x:temp){                 // 取出一维数组大小
        if(x.address == RAddress){
            size = x.size;
            break;
        }
    }
    for(int i = 0; i < offset; ++i){
        RAddress++;
    }

    for(int i = 0; i < size; ++i){
        memory[memoryCount - 1] = memory[RAddress++];
        memoryCount++;
    }
}
void ReturnVALUE(int &i){       /* ReturnValue */
    if(storeOperation.empty()) return;
    int value = operation.back();
    operation.clear();

    operation = storeOperation.back();
    storeOperation.pop_back();

    int returnAddress = operation.back();
    operation.pop_back();

    operation.push_back(value);
    i = returnAddress;

    ident.clear();
    ident = storeIdent.back();
    storeIdent.pop_back();
}
void Return(int &i){
    operation.clear();
    operation = storeOperation.back();
    storeOperation.pop_back();
    i = operation.back();
    operation.pop_back();
    ident.clear();
    ident = storeIdent.back();
    storeIdent.pop_back();
}
void NONEReturn(int &i){
    Return(i);
}