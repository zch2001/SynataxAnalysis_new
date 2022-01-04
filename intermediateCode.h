//
// Created by lenovo on 2021/11/10.
//

#ifndef SRC_INTERMEDIATECODE_H
#define SRC_INTERMEDIATECODE_H
#include "vector"
#include "string"
using namespace std;
enum{
    /*操作栈里边的数据应为int*/
    def=0,   /*清空操作栈*///将变量名放入数据栈并开一个空间,并把地址入栈
    rcdsize,  // 记录一下数组每行多少个(次栈顶)，并记录在ident栈
    palloc, // 为变量开内存 内存大小为栈顶值-1
    add,   // 相加
    sub,   // 相减
    mult,  // 将栈顶和次栈顶元素相乘
    divi,   // 相除
    mod,    // 取模
    assign,  // 将栈顶元素赋值给次栈顶元素的地址
    moveadress, // 将adress值往后移动
    moveToend, // 赋值时将地址移动到本行结尾
    adress,  // 将元素的（首）地址放入
    offset1,  // 1维数数组偏移 栈顶值offset1 + address 放入栈顶
    offset2, // 计算在数组中的偏移 次栈顶值 * 每行元素个数 + 栈顶值 ，再将（adress + offset -1）放入栈顶
    lod,   // 将地址(栈顶)对应的值放入栈顶;
    toPositive, //
    toMinus, // 将栈顶元素改变正负
    toNOT,  // 1->0 0->1
    putNum,  // 将数字压栈
    funcBegin, // 开始定义函数的标志
    Main,    // main 函数
    Begin,   // 进入block标志
    End,    // 结束block标志 ，开始删除变量
    getint, // scanf
    Break,   // 结束函数跳转到 while的end
    Continue, // 进行下一次循环 跳转到while
    OR,      //
    AND,     //
    EQL,    //
    NEQ,    //
    LSS,      //
    LEQ,    //
    GRE,    //
    GEQ,     //
    clear,   // 清空操作栈
    IF,      //
    ELSEIF,   //
    ELSE,     //
    formatstring, //
    replace,   //
    print,     //
    whileMark, //
    endwhileMark,//
    WHILE,   //
    callFun, //
    storeOP, //
    storeID, //
    _2to1,   //
    put0,//
    funcDefine, //
    defFormalVar,
    normalVar,//
    array1Var,//
    array2Var,
    pop,
    RETURN,
    ReturnValue,
    NoneReturn,
};

struct Code{
    int instruct;
    string x;
    string y;
};
struct IDENT{
    string name;
    int size;
    int address;
    int rowsize;
};


void call_Code();
void inCode(int instruct,string x = "\0", string y = "\0");
void recordVar(string name, int size);
void mallocMemory();
void defineVar(string name);
void recordSize();
void Add();
void Sub();
void Mult();
void Div();
void Mod();
void pushAdress(string name);
void moveAdress();
void Assign();
void load();
void PutNum(string number);
void offset_1();
void offset_2(string name);
void moveToEnd(string varCount);
void ToMinus();
void ToNOT();
void FuncDefine();
void BEGIN();
void END();
void Or();
void And();
void Eql();
void Neql();
void Lss();
void Leq();
void Gre();
void Geq();
void Getint();
void If(int &i);
void ElseIf(int &i);
void Else(int &i);
void FormatString(string p);
void Replace();
void Print();
void While(int &i);
void whileMARK();
void endwhileMARK (int &i);
void BREAK(int &i);
void CONTINUE(int &i);
void StoreOpteration();
void StoreIdent();
void _2Dmto1Dm();
void putSize();
void jumptoFunc(string name,int &i);
void DefFormalVar(string name);
void NormalVar();
void Array2Var();
void Array1Var();
void MAIN();
void ReturnVALUE(int &i);
void Return(int &i);
void NONEReturn(int &i);

void test();
#endif //SRC_INTERMEDIATECODE_H
