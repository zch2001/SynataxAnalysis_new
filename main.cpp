#include <iostream>
#include "Lexical_Analyze.h"
#include "Grammar_Analyze.h"
#include "output.h"
#include <bits/stdc++.h>//调全库
#include "vector"
#include "string"
#include "vector"
#include "intermediateCode.h"
extern vector<Code>code;
using namespace std;
int main() {
    vector <word> result;
    freopen("error.txt","w",stdout);
    result=Lexical_Analyze();//词法分析
    Grammar_Analyze(result);
    //output(result);//输出
    /*for (auto &x : code) {
        switch(x.instruct){
            case def:
                cout << "def" << endl;
                break;
            case palloc:
                cout << "palloc" << endl;
                break;
            case rcdsize:
                cout << "rcdsize" << endl;
                break;
            case assign:
                cout << "assign" << endl;
                break;
            case lod:
                cout << "lod" << endl;
                break;
            case mult:
                cout << "mult" << endl;
                break;
            case add:
                cout << "add" << endl;
                break;
            case sub:
                cout << "sub" << endl;
                break;
            case divi:
                cout << "div" << endl;
                break;
            case mod:
                cout << "mod" << endl;
                break;
            case offset1:
                cout << "offset1" << endl;
                break;
            case offset2:
                cout << "offset2" << endl;
                break;
            case adress:
                cout << "adress" << endl;
                break;
            case toPositive:
                cout << "toPositive" << endl;
                break;
            case toMinus:
                cout << "toMinus" << endl;
                break;
            case toNOT:
                cout << "toNOT" << endl;
                break;
            case putNum:
                cout << "putNum" << endl;
                break;
            case moveadress:
                cout << "moveadress" << endl;
                break;
            case moveToend:
                cout << "moveToend" << endl;
                break;
            case Main:
                cout << "main" << endl;
                break;
            case Begin:
                cout << "Begin" << endl;
                break;
            case End:
                cout << "End" << endl;
                break;
            case Break:
                cout << "Break" << endl;
                break;
            case Continue:
                cout << "Continue" << endl;
                break;
            case OR:
                cout << "OR" << endl;
                break;
            case AND:
                cout << "AND" << endl;
                break;
            case EQL:
                cout << "EQL" << endl;
                break;
            case NEQ:
                cout << "NEQL" << endl;
                break;
            case LSS:
                cout << "LSS" << endl;
                break;
            case LEQ:
                cout << "LEQ" << endl;
                break;
            case GRE:
                cout << "GRE" << endl;
                break;
            case GEQ:
                cout << "GEQ" << endl;
                break;
            case clear:
                cout << "clear" << endl;
                break;
            case IF:
                cout << "IF" << endl;
                break;
            case ELSEIF:
                cout << "ELSEIF" << endl;
                break;
            case ELSE:
                cout << "ELSE" << endl;
                break;
            case formatstring:
                cout << "formatstring" << endl;
                break;
            case print:
                cout << "print" << endl;
                break;
            case funcBegin:
                cout << "funcBegin" << endl;
                break;
            case whileMark:
                cout << "whileMark" << endl;
                break;
            case endwhileMark:
                cout << "endwhileMark" << endl;
                break;
            case WHILE:
                cout << "WHILE" << endl;
                break;
            case storeOP:
                cout << "storeOP" << endl;
                break;
            case storeID:
                cout << "storeID" << endl;
                break;
            case _2to1:
                cout << "_2to1" << endl;
                break;
            case put0:
                cout << "putsize" << endl;
                break;
            case callFun:
                cout << "callFun" << endl;
                break;
            case funcDefine:
                cout << "funcDefine" << endl;
                break;
            case normalVar:
                cout << "normalVar" << endl;
                break;
            case array1Var:
                cout << "array1Var" << endl;
                break;
            case array2Var:
                cout << "array2Var" << endl;
                break;
            case defFormalVar:
                cout << "defFormalVar" << endl;
                break;
            case pop:
                cout << "pop" << endl;
                break;
            case RETURN:
                cout << "RETURN" << endl;
                break;
            case ReturnValue:
                cout << "ReturnValue" << endl;
                break;
            case NoneReturn:
                cout << "NoneReturn" << endl;
                break;
        }
    }*/
    //call_Code();
    //test();
    return 0;
}
