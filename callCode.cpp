//
// Created by lenovo on 2021/11/11.
//
#include "intermediateCode.h"
#include "iostream"
using namespace std;
extern int memory[100000];
extern vector<int>operation;
extern vector<Code> code;
extern vector<IDENT>ident;
extern vector<int>operation;
extern vector<vector<IDENT>>storeIdent;
extern vector<vector<int>>storeOperation;
extern int memoryCount;
void call_Code(){
    int i = 0;
    for (;i < code.size();i++) {
        if(code[i].instruct == funcBegin){
            for(i = 0; code[i].instruct != Main; ++i);
        }
        switch(code[i].instruct){
            case def:defineVar(code[i].x);
                break;
            case palloc:mallocMemory();
                break;
            case assign:Assign();
                break;
            case lod:load();
                break;
            case mult:Mult();
                break;
            case add:Add();
                break;
            case sub:Sub();
                break;
            case divi:Div();
                break;
            case mod:Mod();
                break;
            case rcdsize:recordSize();
                break;
            case offset1:offset_1();
                break;
            case offset2:offset_2(code[i].x);
                break;
            case adress:
                pushAdress(code[i].x);
                break;
            case moveadress:moveAdress();
                break;
            case toPositive:
                break;
            case toMinus:ToMinus();
                break;
            case toNOT:ToNOT();
                break;
            case putNum:PutNum(code[i].x);
                break;
            case moveToend: moveToEnd(code[i].x);
                break;
            case Main:MAIN();
                break;
            case Begin:BEGIN();
                break;
            case End:END();
                break;
            case clear:operation.clear();
                break;
            case getint:Getint();
                break;
            case OR:Or();
                break;
            case AND:And();
                break;
            case EQL:Eql();
                break;
            case NEQ:Neql();
                break;
            case LSS:Lss();
                break;
            case LEQ:Leq();
                break;
            case GRE:Gre();
                break;
            case GEQ:Geq();
                break;
            case IF:If(i);
                break;
            case ELSEIF:ElseIf(i);
                break;
            case ELSE:Else(i);
                break;
            case formatstring:FormatString(code[i].x);
                break;
            case replace:Replace();
                break;
            case print:Print();
                break;
            case WHILE: While(i);
                break;
            case whileMark: whileMARK();
                break;
            case endwhileMark: endwhileMARK(i);
                break;
            case Break: BREAK(i);
                break;
            case Continue: CONTINUE(i);
                break;
            case storeOP:StoreOpteration();
                break;
            case storeID:StoreIdent();
                break;
            case _2to1:_2Dmto1Dm();
                break;
            case put0:putSize();
                break;
            case callFun: jumptoFunc(code[i].x,i);
                break;
            case funcDefine:FuncDefine();
                break;
            case array1Var:Array1Var();
                break;
            case array2Var:Array2Var();
                break;
            case defFormalVar:DefFormalVar(code[i].x);
                break;
            case pop:operation.pop_back();
                break;
            case normalVar:NormalVar();
                break;
            case ReturnValue:ReturnVALUE(i);
                break;
            case RETURN:Return(i);
                break;
            case NoneReturn:NONEReturn(i);
                break;
        }
    }
}
void test(){
    cout << "--------memory------" << endl;
    for(int i = 0; i < memoryCount; ++i){
        cout << i << " " <<  memory[i] << endl;
    }
    cout << "--------operation------" << endl;
    for(auto &x:operation){
        cout << x << endl;
    }
    cout << "-----------------" << endl;
    cout << "memorySize is " << memoryCount << endl;
    cout << "-------ident------" << endl;
    for(auto &x:ident){
        cout << x.name + " adress: " << x.address << " size: " << x.size << " rowsize: " << x.rowsize << endl;
    }
    cout << "-----------------" << endl;
}