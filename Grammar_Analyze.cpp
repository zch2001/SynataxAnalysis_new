//
// Created by lenovo on 2021/10/15.
//
#include <iostream>
#include "Lexical_Analyze.h"
#include "Grammar_Analyze.h"
#include "word.h"
#include "error.h"
#include "intermediateCode.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include<fstream>
int gzr =0;
extern vector<Code>code;
int flag=0;
int expreturn=0;
int returnrow;
int paraflag=0;//记录有几个变量
string procname;
int flag_while=0;
int pp_flag=0;
int levflag=0;
int is_func=0;
int voidflag=0;
int intflag=0;
vector <varlist> Slist;
vector <funclist> Flist;
vector <int> flagd;
vector <string> Funcname;
vector <int> paraflagD;
vector <ErrorE> Elist;
//ofstream  cout;
void Grammar_Analyze(vector <word> result)
{
    //cout.open("output.txt");


    analyzeG(result);
};
void analyzeG(vector <word> result)
{
    CompUnit(result);
}
void CompUnit(vector <word> result)
{
    while(1)
    {

        if(result[flag].outCategory()=="CONSTTK")
        {
            if(Decl(result)==0)
            {
                break;
            }
        }
        else if(result[flag].outCategory()=="INTTK" && result[flag+1].outCategory()!="MAINTK")
        {
            if(result[flag+1].outCategory()=="IDENFR")
            {
                if(result[flag+2].outCategory()!="LPARENT")
                {

                    if (Decl(result) == 0)
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }

            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    while(1)
    {
        if((result[flag].outCategory()=="INTTK")&&result[flag+1].outCategory()!="MAINTK"||(result[flag].outCategory()=="VOIDTK"))
        {
            if(result[flag+1].outCategory()=="IDENFR")
            {
                if(result[flag+2].outCategory()=="LPARENT")
                {
                    inCode(funcBegin);
                    if(FuncDef(result)==0)
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    if(MainFuncDef(result))
    {
        //cout.open("output.txt");
        outputGT("<CompUnit>");
        //cout << "<CompUnit>" << endl;
        return ;
    }
    else
    {
        return ;
    }
}
int Decl(vector <word> result)
{
    if(ConstDecl(result))
    {
        return 1;
    }
    else if(VarDecl(result))
    {

        return 1;
    }
    else
    {
        return 0;
    }
}
int ConstDecl(vector <word> result)
{
    if(result[flag].outCategory()=="CONSTTK")
    {
        outputG(result);
        flag++;
        if(BType(result))
        {
            if(ConstDef(result))
            {
                while(result[flag].outCategory()=="COMMA")
                {
                    outputG(result);
                    /* -------------------------- Pcode ------------------------------*/
                    inCode(clear);
                    /* -------------------------- Pcode ------------------------------*/
                    flag++;
                    if(ConstDef(result)==0)
                    {
                        return 0;
                    }
                }
                if(result[flag].outCategory()=="SEMICN")
                {
                    outputG(result);
                    /* -------------------------- Pcode ------------------------------*/
                    inCode(clear);
                    /* -------------------------- Pcode ------------------------------*/
                    flag++;
                    //cout.open("output.txt");
                    outputGT("<ConstDecl>");
                    //cout << "" << endl;
                    return 1;
                }
                else
                {
                    ErrorOutputChoice(result,"i",flag-1);
                    return ErrorReturn();
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }

    }
    else
    {
        return 0;
    }
}
int BType(vector <word> result)
{
    if(result[flag].outCategory()=="INTTK")
    {
        outputG(result);
        flag++;
        return 1;
    }
    else
    {
        return 0;
    }
}
int ConstDef(vector <word> result)
{
    /* -------------------------- Pcode ------------------------------*/
    inCode(def,result[flag].outContent());
    /* -------------------------- Pcode -------------------------------*/

    if(result[flag].outCategory()=="IDENFR")
    {
        //error
        string tempID;
        int tempflag=flag;
        int timeflag=0;
        tempID=result[flag].outContent();

        if(result[flag+1].outContent()!="[")//判断是否是普通变量
        {
            timeflag=1;
            if(select(result,tempflag))//判断是否重命名
            {
                varlist Intemp(tempID,"const","int",levflag,0,0);
                Slist.push_back(Intemp);
            }
        }
        //error

        outputG(result);
        inCode(adress,result[flag].outContent());
        flag++;
        int arrayflag=0;
        while(result[flag].outCategory()=="LBRACK")
        {
            outputG(result);
            flag++;
            if(ConstExp(result)==0)
            {
                return 0;
            }
            arrayflag++;
            if(result[flag].outCategory()=="RBRACK")
            {
                outputG(result);
                flag++;
            }
            else
            {
                ErrorOutputChoice(result,"k",flag-1);
                //return ErrorReturn();
            }
        }

        //error
        if(timeflag==0)
        {
            if(select(result,tempflag))
            {
                if(arrayflag!=0)//不是一维数组
                {
                    varlist Intemp(tempID,"const","int",levflag,0,arrayflag);
                    Slist.push_back(Intemp);
                }
            }
        }
        timeflag=0;
        arrayflag=0;
        //error
        if(result[flag].outCategory()=="ASSIGN")
        {
            outputG(result);
            flag++;
            if(ConstInitVal(result))
            {
                //cout.open("output.txt");
                outputGT("<ConstDef>");
                //cout <<  << endl;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }

    }
    else
    {
        return 0;
    }
}
int ConstInitVal(vector <word> result)
{
    if(result[flag].outCategory()=="LBRACE")
    {
        outputG(result);
        flag++;
        if(ConstInitVal(result))
        {
            while(result[flag].outCategory()=="COMMA")
            {
                outputG(result);
                flag++;
                if(ConstInitVal(result)==0)
                {
                    return 0;
                }
            }
        }
        if(result[flag].outCategory()=="RBRACE")
        {
            outputG(result);
            flag++;
            //cout.open("output.txt");
            outputGT("<ConstInitVal>");
            //cout <<  << endl;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if(ConstExp(result))
    {
        //cout.open("output.txt");
        outputGT("<ConstInitVal>");
        /* -------------------------- Pcode ------------------------------*/
        inCode(assign);
        /* -------------------------- Pcode ------------------------------*/

        //cout <<  << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}
int VarDecl(vector <word> result)
{
    if(BType(result))
    {
        if(VarDef(result))
        {
            while(result[flag].outCategory()=="COMMA")
            {
                /* -------------------------- Pcode ------------------------------*/
                inCode(clear);
                /* -------------------------- Pcode ------------------------------*/
                outputG(result);
                flag++;
                if(VarDef(result)==0)
                {
                    return 0;
                }
            }
            if(result[flag].outCategory()=="SEMICN")
            {
                inCode(clear);
                outputG(result);
                flag++;
                //cout.open("output.txt");
                outputGT("<VarDecl>");
                //cout <<  << endl;
                return 1;
            }
            else
            {
                ErrorOutputChoice(result,"i",flag-1);
                return ErrorReturn();
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
int VarDef(vector <word> result)
{
    /* -------------------------- Pcode ------------------------------*/
    inCode(def,result[flag].outContent());
    /* -------------------------- Pcode -------------------------------*/

    if(result[flag].outCategory()=="IDENFR")
    {
        //error
        string tempID;
        int tempflag=flag;
        int timeflag=0;
        tempID=result[flag].outContent();

        if(result[flag+1].outContent()!="[")//判断是否是普通变量
        {
            timeflag=1;
            if(select(result,tempflag))//判断是否重命名
            {
                varlist Intemp(tempID,"var","int",levflag,0,0);
                Slist.push_back(Intemp);
            }
        }
        //error
        outputG(result);
        /* -------------------------- Pcode ------------------------------*/
        inCode(adress,result[flag].outContent());
        /* -------------------------- Pcode ------------------------------*/
        flag++;
        int arrayflag=0;
        while(result[flag].outCategory()=="LBRACK")
        {
            outputG(result);
            flag++;
            arrayflag++;
            if(ConstExp(result))
            {

                if(result[flag].outCategory()=="RBRACK")
                {
                    outputG(result);
                    flag++;
                }
                else
                {
                    ErrorOutputChoice(result,"k",flag-1);
                    //return ErrorReturn();
                }
            }
            else
            {
                return 0;
            }
        }
        //error
        if(timeflag==0)
        {
            if(select(result,tempflag))
            {
                if(arrayflag!=0)//不是一维数组
                {
                    varlist Intemp(tempID,"var","int",levflag,0,arrayflag);
                    Slist.push_back(Intemp);
                }
            }
        }
        timeflag=0;
        arrayflag=0;
        //error
        if(result[flag].outCategory()=="ASSIGN")
        {
            outputG(result);
            flag++;
            if(InitVal(result))
            {
                //cout.open("output.txt");
                outputGT("<VarDef>");
                //cout <<  << endl;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        //cout.open("output.txt");
        outputGT("<VarDef>");
        //cout <<  << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}
int InitVal(vector <word> result)
{
    if(Exp(result))
    {
        //cout.open("output.txt");
        outputGT("<InitVal>");
        /* -------------------------- Pcode ------------------------------*/
        inCode(assign);
        /* -------------------------- Pcode ------------------------------*/

        //cout <<  << endl;
        return 1;
    }
    else if(result[flag].outCategory()=="LBRACE")
    {
        outputG(result);
        flag++;
        if(InitVal(result))
        {
            while(result[flag].outCategory()=="COMMA")
            {
                outputG(result);
                flag++;
                if(InitVal(result)==0)
                {
                    return 0;
                }
            }
        }
        if(result[flag].outCategory()=="RBRACE")
        {
            outputG(result);
            flag++;
            //cout.open("output.txt");
            outputGT("<InitVal>");
            //cout <<  << endl;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
int FuncDef(vector <word> result)
{
    if(FuncType(result))
    {
        int paranum=0;//记录函数参数个数应该存在Slist的哪个位置
        if(result[flag].outCategory()=="IDENFR")
        {
            //error
            string tempID;
            int tempflag=flag;
            tempID=result[flag].outContent();

            if(select(result,tempflag))
            {
                varlist Intemp(tempID,"proc",result[flag-1].outContent(),levflag,0,-1);
                Slist.push_back(Intemp);
                paranum=Slist.size()-1;
                procname=tempID;
            }

            //error
            outputG(result);
            flag++;
            if(result[flag].outCategory()=="LPARENT")
            {
                is_func=1;
                levflag++;
                outputG(result);
                flag++;
                if(FuncFParams(result))
                {}//[]没影响
                Slist[paranum].ininterpret(paraflag);
                if(result[flag].outCategory()=="RPARENT")
                {
                    outputG(result);
                    flag++;
                    if(Block(result))
                    {
                        outputGT("<FuncDef>");
                        /* -------------------------- Pcode ------------------------------*/
                        code.pop_back(); // 把end弹出来
                        inCode(NoneReturn);
                        /* -------------------------- Pcode ------------------------------*/

                        //cout <<  << endl;
                        intflag=0;
                        voidflag=0;
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    ErrorOutputChoice(result,"j",flag-1);   //
                    if(Block(result))
                    {
                        outputGT("<FuncDef>");
                        /* -------------------------- Pcode ------------------------------*/
                        code.pop_back(); // 把end弹出来
                        inCode(NoneReturn);
                        /* -------------------------- Pcode ------------------------------*/

                        //cout <<  << endl;
                        intflag=0;
                        voidflag=0;
                        return ErrorReturn();;
                    }
                    else
                    {
                        return ErrorReturn();;
                    }
                }

            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
int MainFuncDef(vector <word> result)
{
    if(result[flag].outCategory()=="INTTK")
    {
        intflag=1;
        outputG(result);
        flag++;
        if(result[flag].outCategory()=="MAINTK")
        {
            outputG(result);
            flag++;
            if(result[flag].outCategory()=="LPARENT")
            {
                outputG(result);
                flag++;
                if(result[flag].outCategory()=="RPARENT")
                {
                    outputG(result);
                    flag++;
                    /* -------------------------- Pcode ------------------------------*/
                    inCode(Main);
                    /* -------------------------- Pcode ------------------------------*/

                    if(Block(result))
                    {
                        intflag=0;
                        //cout.open("output.txt");
                        outputGT("<MainFuncDef>");
                        //cout <<  << endl;
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    ErrorOutputChoice(result,"j",flag-1);  //
                    /* -------------------------- Pcode ------------------------------*/
                    inCode(Main);
                    /* -------------------------- Pcode ------------------------------*/

                    if(Block(result))
                    {
                        intflag=0;
                        //cout.open("output.txt");
                        outputGT("<MainFuncDef>");
                        //cout <<  << endl;
                        return ErrorReturn();
                    }
                    else
                    {
                        return ErrorReturn();
                    }
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
int FuncType(vector <word> result)
{
    if(result[flag].outCategory()=="VOIDTK")
    {
        voidflag=1;
        outputG(result);
        inCode(funcDefine,result[flag+1].outContent());
        flag++;
        //cout.open("output.txt");
        outputGT("<FuncType>");

        //cout <<  << endl;
        return 1;
    }
    else if(result[flag].outCategory()=="INTTK")
    {
        intflag=1;
        outputG(result);
        inCode(funcDefine,result[flag+1].outContent());
        flag++;
        //cout.open("output.txt");
        outputGT("<FuncType>");
        //cout <<  << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}
int FuncFParams(vector <word> result)
{
    paraflag=0;
    if(FuncFParam(result))
    {
        while(result[flag].outCategory()=="COMMA")
        {
            outputG(result);
            flag++;
            if(FuncFParam(result)==0)
            {
                return 0;
            }
        }
        //cout.open("output.txt");
        outputGT("<FuncFParams>");
        //cout <<  << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}
int FuncFParam(vector <word> result)
{
    if(BType(result))
    {
        if(result[flag].outCategory()=="IDENFR")
        {
            //error
            paraflag++;
            string para=result[flag].outContent();

            int tempflag=flag;
            int timeflag=0;

            if(result[flag+1].outContent()!="[")//判断是否是普通变量
            {
                timeflag=1;
                if(select(result,tempflag))//判断是否重命名
                {
                    varlist Intemp(para,"para",result[tempflag-1].outContent(),levflag,0,0);
                    Slist.push_back(Intemp);
                    funclist InPtemp(procname,result[flag-1].outContent(),0);
                    Flist.push_back(InPtemp);
                }
            }
            //error
            outputG(result);
            auto identName = result[flag].outContent();
            flag++;
            int arrayflag=0;
            if(result[flag].outCategory()=="LBRACK")
            {
                outputG(result);
                flag++;
                arrayflag++;
                if(result[flag].outCategory()=="RBRACK")
                {
                    outputG(result);
                    flag++;
                    while(result[flag].outCategory()=="LBRACK")
                    {
                        outputG(result);
                        flag++;
                        if(ConstExp(result)==0)
                        {
                            return 0;
                        }
                        if(result[flag].outCategory()=="RBRACK")
                        {
                            outputG(result);
                            flag++;
                        }
                        else
                        {
                            ErrorOutputChoice(result,"k",flag-1);

                            //return ErrorReturn();
                        }
                    }
                }
                else
                {
                    ErrorOutputChoice(result,"k",flag-1);
                    while(result[flag].outCategory()=="LBRACK")
                    {
                        outputG(result);
                        flag++;
                        if(ConstExp(result)==0)
                        {
                            return 0;
                        }
                        if(result[flag].outCategory()=="RBRACK")
                        {
                            outputG(result);
                            flag++;
                        }
                        else
                        {
                            ErrorOutputChoice(result,"k",flag-1);

                            //return ErrorReturn();
                        }
                    }
                    return ErrorReturn();
                }
            }
            //error
            if(timeflag==0)
            {
                if(select(result,tempflag))
                {
                    if(arrayflag!=0)//不是一维数组
                    {
                        varlist Intemp(para,"para",result[tempflag-1].outContent(),levflag,0,arrayflag);
                        Slist.push_back(Intemp);
                        funclist InPtemp(procname,result[flag-1].outContent(),arrayflag);
                        Flist.push_back(InPtemp);
                    }
                }
            }
            timeflag=0;
            arrayflag=0;
            //error
            //cout.open("output.txt");
            outputGT("<FuncFParam>");
            inCode(defFormalVar,identName);
            inCode(normalVar);
            //cout <<  << endl;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
int Block(vector <word> result)
{

    if(result[flag].outCategory()=="LBRACE")
    {
        /* -------------------------- Pcode ------------------------------*/
        inCode(Begin);
        /* -------------------------- Pcode ------------------------------*/
        //error
        if(!is_func)
        {
            levflag++;
        }
        is_func=0;
        //error
        outputG(result);
        flag++;
        while(BlockItem(result));
        if(result[flag].outCategory()=="RBRACE")
        {
            popFlist();
            levflag--;
            if(levflag==0&&intflag==1)
            {
                if(returnrow-levflag!=1)
                {
                    ErrorOutput(result,"g");
                }
            }
            returnrow=0;
            /* -------------------------- Pcode ------------------------------*/
            inCode(End);
            /* -------------------------- Pcode ------------------------------*/

            outputG(result);
            flag++;
            //cout.open("output.txt");
            outputGT("<Block>");
            //cout <<  << endl;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
int BlockItem(vector <word> result)
{
    if(Decl(result))
    {
        return 1;
    }
    else if(Stmt(result))
    {

        return 1;
    }
    else
    {
        return 0;
    }
}
int Stmt(vector <word> result)
{
    int i,j,k,TF;
    i=0;
    j=0;
    k=0;
    TF=0;
    if(result[flag].outCategory()=="IDENFR")
    {
        TF=1;
    }
    while((result[flag+i].outCategory()!="SEMICN")&&(TF==1)&&(i<result.size()))
    {
        if(result[flag+i].outCategory()=="ASSIGN")
        {
            j=1;
            k=i;
        }
        i++;
    }

    if((j==1)&&(result[flag+k+1].outCategory()!="GETINTTK"))
    {
        errorh(result);
        if(LVal(result))
        {

            /* -------------------------- Pcode ------------------------------*/
            code.pop_back(); // 把取值语句lod语句弹出栈，让栈顶为lval的地址
            /* -------------------------- Pcode ------------------------------*/

            if(result[flag].outCategory()=="ASSIGN")
            {
                outputG(result);
                flag++;
                if(Exp(result))
                {
                    /* -------------------------- Pcode ------------------------------*/
                    inCode(assign);
                    /* -------------------------- Pcode ------------------------------*/

                    if(result[flag].outCategory()=="SEMICN")
                    {
                        outputG(result);
                        flag++;
                        //cout.open("output.txt");
                        outputGT("<Stmt>");
                        /* -------------------------- Pcode ------------------------------*/
                        inCode(clear);
                        /* -------------------------- Pcode ------------------------------*/

                        //cout <<  << endl;
                        return 1;
                    }
                    else
                    {
                        ErrorOutputChoice(result,"i",flag-1);
                        return ErrorReturn();
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else if((j==1)&&(result[flag+k+1].outCategory()=="GETINTTK"))
    {
        errorh(result);
        if(LVal(result))
        {

            /* -------------------------- Pcode ------------------------------*/
            code.pop_back(); // 把取值语句lod语句弹出栈，让栈顶为lval的地址
            /* -------------------------- Pcode ------------------------------*/

            if(result[flag].outCategory()=="ASSIGN")
            {
                outputG(result);
                flag++;
                if(result[flag].outCategory()=="GETINTTK")
                {
                    inCode(getint);
                    inCode(assign);
                    outputG(result);
                    flag++;
                    if(result[flag].outCategory()=="LPARENT")
                    {
                        outputG(result);
                        flag++;
                        if(result[flag].outCategory()=="RPARENT")
                        {
                            outputG(result);
                            flag++;
                            if(result[flag].outCategory()=="SEMICN")
                            {
                                outputG(result);
                                flag++;
                                //cout.open("output.txt");
                                outputGT("<Stmt>");
                                /* -------------------------- Pcode ------------------------------*/
                                inCode(clear);
                                /* -------------------------- Pcode ------------------------------*/
                                //cout <<  << endl;
                                return 1;
                            }
                            else
                            {
                                ErrorOutputChoice(result,"i",flag-1);
                                return ErrorReturn();
                            }
                        }
                        else
                        {
                            ErrorOutputChoice(result,"j",flag-1);  //
                            if(result[flag].outCategory()=="SEMICN")
                            {
                                outputG(result);
                                flag++;
                                //cout.open("output.txt");
                                outputGT("<Stmt>");
                                /* -------------------------- Pcode ------------------------------*/
                                inCode(clear);
                                /* -------------------------- Pcode ------------------------------*/
                                //cout <<  << endl;
                                return ErrorReturn();
                            }
                            else
                            {
                                ErrorOutputChoice(result,"i",flag-1);
                                return ErrorReturn();
                            }

                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }

    else if(Exp(result))
    {
        if(result[flag].outCategory()=="SEMICN")
        {
            outputG(result);
            flag++;
            //cout.open("output.txt");
            outputGT("<Stmt>");
            /* -------------------------- Pcode ------------------------------*/
            inCode(clear);
            /* -------------------------- Pcode ------------------------------*/

            //cout <<  << endl;
            return 1;
        }
        else
        {
            ErrorOutputChoice(result,"i",flag-1);
            return ErrorReturn();
        }
    }
    else if(result[flag].outCategory()=="SEMICN")
    {
        outputG(result);
        flag++;
        //cout.open("output.txt");
        outputGT("<Stmt>");
       // cout <<  << endl;
        return 1;
    }
    else if(Block(result))
    {
        //cout.open("output.txt");
        outputGT("<Stmt>");
        //cout <<  << endl;
        return 1;
    }
    else if(result[flag].outCategory()=="IFTK")
    {
        outputG(result);
        flag++;
        if(result[flag].outCategory()=="LPARENT")
        {
            outputG(result);
            flag++;
            if(Cond(result))
            {
                if(result[flag].outCategory()=="RPARENT")
                {
                    outputG(result);
                    flag++;
                    if(Stmt(result))
                    {
                        if(result[flag].outCategory()=="ELSETK")
                        {
                            outputG(result);
                            flag++;
                            if (Stmt(result) == 0)
                            {
                                return 0;
                            }
                        }
                        //cout.open("output.txt");
                        outputGT("<Stmt>");
                        //cout <<  << endl;
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    ErrorOutputChoice(result,"j",flag-1); //
                    if(Stmt(result))
                    {
                        if(result[flag].outCategory()=="ELSETK")
                        {
                            outputG(result);
                            flag++;
                            if (Stmt(result) == 0)
                            {
                                return 0;
                            }
                        }
                        //cout.open("output.txt");
                        outputGT("<Stmt>");
                        //cout <<  << endl;
                        return ErrorReturn();
                    }
                    else
                    {
                        return ErrorReturn();
                    }
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else if(result[flag].outCategory()=="WHILETK")
    {
        flag_while=1;
        outputG(result);
        flag++;
        if(result[flag].outCategory()=="LPARENT")
        {
            outputG(result);
            flag++;
            if(Cond(result))
            {
                if(result[flag].outCategory()=="RPARENT")
                {
                    outputG(result);
                    flag++;
                    if(Stmt(result))
                    {
                        //cout.open("output.txt");
                        outputGT("<Stmt>");
                       // cout <<  << endl;
                        flag_while=0;
                        return 1;
                    }
                    else
                    {
                        flag_while=0;
                        return 0;
                    }
                }
                else
                {
                    ErrorOutputChoice(result,"j",flag-1);  //
                    if(Stmt(result))
                    {
                        //cout.open("output.txt");
                        outputGT("<Stmt>");
                        // cout <<  << endl;
                        flag_while=0;
                        return ErrorReturn();
                    }
                    else
                    {
                        flag_while=0;
                        return ErrorReturn();
                    }
                }
            }
            else
            {
                flag_while=0;
                return 0;
            }
        }
        else
        {
            flag_while=0;
            return 0;
        }
    }
    else if(result[flag].outCategory()=="BREAKTK")
    {
        if(flag_while==1)
        {
            outputG(result);
            flag++;
        }
        else
        {
            ErrorOutput(result,"m");
            flag++;
        }
        if(result[flag].outCategory()=="SEMICN")
        {
            outputG(result);
            flag++;
            //cout.open("output.txt");
            outputGT("<Stmt>");
            //cout <<  << endl;
            return 1;
        }
        else
        {
            ErrorOutputChoice(result,"i",flag-1);
            return ErrorReturn();
        }
    }
    else if(result[flag].outCategory()=="CONTINUETK")
    {
        if(flag_while==1)
        {
            outputG(result);
            flag++;
        }
        else
        {
            ErrorOutput(result,"m");
            flag++;
        }
        if(result[flag].outCategory()=="SEMICN")
        {
            outputG(result);
            flag++;
            //cout.open("output.txt");
            outputGT("<Stmt>");
            //cout << "<Stmt>" << endl;
            return 1;
        }
        else
        {
            ErrorOutputChoice(result,"i",flag-1);
            return ErrorReturn();
        }
    }
    else if(result[flag].outCategory()=="RETURNTK")
    {
        int Eflag=flag;
        if(voidflag==1)
        {
            int varflag=0;
            expreturn=0;
            outputG(result);
            flag++;
            if(Exp(result))
            {
                inCode(ReturnValue);
                varflag=1;
                expreturn=1;
            }//[]
            if(expreturn==1)
            {
                ErrorOutputChoice(result,"f",Eflag);
            }
            expreturn=0;
            if(result[flag].outCategory()=="SEMICN")
            {
                outputG(result);
                if(varflag==0)
                {
                    inCode(RETURN);
                }
                varflag=0;
                flag++;
                //cout.open("output.txt");
                outputGT("<Stmt>");
                //cout << "<Stmt>" << endl;


                return 1;
            }
            else
            {
                ErrorOutputChoice(result,"i",flag-1);
                return ErrorReturn();
            }
        }
        else if(intflag==1)
        {
            expreturn=0;
            returnrow=levflag;
            int varflag=0;
            outputG(result);
            flag++;
            if(Exp(result))
            {
                inCode(ReturnValue);
                varflag=1;
                expreturn=1;
            }//[]
            if(result[flag].outCategory()=="SEMICN")
            {
                outputG(result);
                if(varflag==0)
                {
                    inCode(RETURN);
                }
                varflag=0;
                flag++;
                //cout.open("output.txt");
                outputGT("<Stmt>");
                //cout << "<Stmt>" << endl;


                return 1;
            }
            else
            {
                ErrorOutputChoice(result,"i",flag-1);
                return ErrorReturn();
            }
        }

    }
    else if(result[flag].outCategory()=="PRINTFTK")
    {
        outputG(result);
        int error_i_flag=flag;
        flag++;
        if(result[flag].outCategory()=="LPARENT")
        {
            outputG(result);
            flag++;
            if(result[flag].outCategory()=="STRCON")
            {
                /* -------------------------- Pcode ------------------------------*/
                inCode(formatstring,result[flag].outContent());
                /* -------------------------- Pcode ------------------------------*/

                int i=1,p_flag=1;//是否出现非法字符
                pp_flag=0;//出现次数
                while(result[flag].outContent()[i]!=NULL&&result[flag].outContent()[i]!='"')
                {
                    if((result[flag].outContent()[i]==32)||(result[flag].outContent()[i]==33)||(((result[flag].outContent()[i]>=40)&&(result[flag].outContent()[i]<=126))))
                    {
                        i++;
                    }
                    else if((result[flag].outContent()[i]=='%')&&((result[flag].outContent()[i+1]!='d')))
                    {
                        p_flag=0;
                        i=i+2;
                    }
                    else if((result[flag].outContent()[i]=='%')&&((result[flag].outContent()[i+1]=='d')))
                    {
                        pp_flag++;
                        i=i+2;
                    }
                    else
                    {
                        p_flag=0;
                        i++;
                    }
//                    if((result[flag].outContent()[i]=='%')&&((result[flag].outContent()[i+1]=='d')))
//                    {
//                        pp_flag++;
//                        i++;
//                    }
//                    else if((result[flag].outContent()[i]!=32)&&(result[flag].outContent()[i]!=33)&&(((result[flag].outContent()[i]<40)||(result[flag].outContent()[i]>126))))
//                    {
//                        p_flag=0;
//                    }
//                    i++;
                }
                if(p_flag)
                {
                    outputG(result);
                    flag++;
                }
                else
                {
                    ErrorOutput(result,"a");
                    flag++;
                }
                while(result[flag].outCategory()=="COMMA")
                {
                    pp_flag--;
                    outputG(result);
                    flag++;
                    if(Exp(result)==0)
                    {
                        return 0;
                    }
                    /* -------------------------- Pcode ------------------------------*/
                    inCode(formatstring+1);
                    /* -------------------------- Pcode ------------------------------*/

                }
                /* -------------------------- Pcode ------------------------------*/
                inCode(print);
                /* -------------------------- Pcode ------------------------------*/
                if(pp_flag!=0)
                {
                    ErrorOutputChoice(result,"l",error_i_flag);
                }
                if(result[flag].outCategory()=="RPARENT")
                {
                    outputG(result);
                    flag++;
                    if(result[flag].outCategory()=="SEMICN")
                    {
                        outputG(result);
                        flag++;
                        //cout.open("output.txt");
                        outputGT("<Stmt>");
                        /* -------------------------- Pcode ------------------------------*/
                        inCode(clear);
                        /* -------------------------- Pcode ------------------------------*/
                        //cout << "<Stmt>" << endl;
                        return 1;
                    }
                    else
                    {
                        ErrorOutputChoice(result,"i",flag-1);
                        return ErrorReturn();
                    }
                }
                else
                {
                    ErrorOutputChoice(result,"j",flag-1); //
                    if(result[flag].outCategory()=="SEMICN")
                    {
                        outputG(result);
                        flag++;
                        //cout.open("output.txt");
                        outputGT("<Stmt>");
                        /* -------------------------- Pcode ------------------------------*/
                        inCode(clear);
                        /* -------------------------- Pcode ------------------------------*/
                        //cout << "<Stmt>" << endl;
                        return ErrorReturn();
                    }
                    else
                    {
                        ErrorOutputChoice(result,"i",flag-1);
                        return ErrorReturn();
                    }
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }

    else
    {
        return 0;
    }

}
int Exp(vector <word> result)
{
    if(AddExp(result))
    {
        //cout.open("output.txt");
        outputGT("<Exp>");
        //cout <<  << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}
int Cond(vector <word> result)
{
    if(LOrExp(result))
    {
        //cout.open("output.txt");
        outputGT("<Cond>");
        //cout <<  << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}
int LVal(vector <word> result)
{
    if(result[flag].outCategory()=="IDENFR")
    {
        /* -------------------------- Pcode ------------------------------*/
        inCode(adress,result[flag].outContent());
        string wordname = result[flag].outCategory();
        /* -------------------------- Pcode ------------------------------*/
        errorc(result);
        outputG(result);
        flag++;
        while(result[flag].outCategory()=="LBRACK")
        {
            outputG(result);
            flag++;
            if(Exp(result))
            {
                if(result[flag].outCategory()=="RBRACK")
                {
                    outputG(result);
                    flag++;
                }
                else
                {
                    ErrorOutputChoice(result,"k",flag-1);
                    //return ErrorReturn();
                }
            }
            else
            {
                return 0;
            }

        }
        //不考虑数组
        inCode(lod);
        //cout.open("output.txt");
        outputGT("<LVal>");
        //cout <<  << endl;
        return 1;
    }
    else
    {

        return 0;
    }
}
int PrimaryExp(vector <word> result)
{
    if(result[flag].outCategory()=="LPARENT")
    {
        outputG(result);
        flag++;
        if(Exp(result))
        {
            if(result[flag].outCategory()=="RPARENT")
            {
                outputG(result);
                flag++;
                //cout.open("output.txt");
                outputGT("<PrimaryExp>");
                //cout <<  << endl;
                return 1;
            }
            else
            {
                ErrorOutputChoice(result,"j",flag-1);  //
                return ErrorReturn();
            }
        }
        else
        {
            return 0;
        }
    }
    else if(LVal(result))
    {
        //cout.open("output.txt");
        outputGT("<PrimaryExp>");
        //cout <<  << endl;
        return 1;
    }
    else if(Number(result))
    {
        //cout.open("output.txt");
        outputGT("<PrimaryExp>");

       // cout <<  << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}
int Number(vector <word> result)
{
    if(result[flag].outCategory()=="INTCON")
    {
        inCode(putNum,result[flag].outContent());
        outputG(result);
        flag++;
        //cout.open("output.txt");
        outputGT("<Number>");
        //cout <<  << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}
int UnaryExp(vector <word> result)
{

    if((result[flag].outCategory()=="IDENFR")&&(result[flag+1].outCategory()=="LPARENT"))
    {
        //error
        Funcname.push_back(result[flag].outContent());
        int tempflag=flag;
        //error
        errorc(result);
        outputG(result);
        /* -------------------------- Pcode ------------------------------*/
        inCode(storeOP);
        word temp=result[flag];
        /* -------------------------- Pcode ------------------------------*/

        flag++;
        if(result[flag].outCategory()=="LPARENT")
        {
            outputG(result);
            flag++;

            if(FuncRParams(result)==0)
            {}

            if(errord(result,tempflag))
            {
                errore(result,tempflag);
                paraflagD.pop_back();
            }
//            paraflagD.pop_back();
            Funcname.pop_back();
            /* -------------------------- Pcode ------------------------------*/
            inCode(storeID);
            inCode(callFun,temp.outContent());
            /* -------------------------- Pcode ------------------------------*/

            if(result[flag].outCategory()=="RPARENT")
            {
                outputG(result);
                flag++;
                //cout.open("output.txt");
                outputGT("<UnaryExp>");
                //cout <<  << endl;
                return 1;
            }
            else
            {
                ErrorOutputChoice(result,"j",flag-1);  //
                return ErrorReturn();
            }
        }
        else
        {

            return 0;
        }
    }
    else if(PrimaryExp(result))
    {
        //cout.open("output.txt");
        outputGT("<UnaryExp>");
        //cout <<  << endl;
        return 1;
    }
    else if(UnaryOp(result))
    {
        string temp=result[flag-1].outContent();
        if(UnaryExp(result))
        {
            //cout.open("output.txt");
            outputGT("<UnaryExp>");
            inCode(temp == "-" ?toMinus: temp == "!"?toNOT:toPositive);
            //cout <<  << endl;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
int UnaryOp(vector <word> result)
{
    if(result[flag].outCategory()=="PLUS")
    {
        outputG(result);
        flag++;
        //cout.open("output.txt");
        outputGT("<UnaryOp>");
        //cout <<  << endl;
        return 1;
    }
    else if(result[flag].outCategory()=="MINU")
    {
        outputG(result);
        flag++;
        //cout.open("output.txt");
        outputGT("<UnaryOp>");
        //cout <<  << endl;
        return 1;
    }
    else if(result[flag].outCategory()=="NOT")
    {
        outputG(result);
        flag++;
        //cout.open("output.txt");
        outputGT("<UnaryOp>");
        //cout <<  << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}
int FuncRParams(vector <word> result)
{
    int paraflagd=0;
    if(Exp(result))
    {
        pushe(result);
        paraflagd++;

        while(result[flag].outCategory()=="COMMA")
        {
            outputG(result);
            flag++;
            if(Exp(result)==0)
            {
                return 0;
            }
            pushe(result);
            paraflagd++;
        }
        paraflagD.push_back(paraflagd);
        outputGT("<FuncRParams>");
        //cout <<  << endl;
        return 1;
    }
    else
    {
        return 0;
    }
    return 0;
}
int MulExp(vector <word> result)
{
    string NKflag="\0";
    int NK;
    if(UnaryExp(result))
    {
        while(1)
        {
            if(NKflag!="\0")
            {
                inCode(NK);
                NKflag="\0";
            }
            if(result[flag].outCategory()=="MULT")
            {
                //cout.open("output.txt");
                outputGT("<MulExp>");
                //cout <<  << endl;
                outputG(result);
                NKflag="mult";
                NK=mult;
                flag++;
                if(UnaryExp(result)==0)
                {
                    return 0;
                }
            }
            else if(result[flag].outCategory()=="DIV")
            {
                //cout.open("output.txt");
                outputGT("<MulExp>");
                //cout <<  << endl;
                outputG(result);
                NKflag="DIV";
                NK=divi;
                flag++;
                if(UnaryExp(result)==0)
                {
                    return 0;
                }
            }
            else if(result[flag].outCategory()=="MOD")
            {
                //cout.open("output.txt");
                outputGT("<MulExp>");
                //cout <<  << endl;
                outputG(result);
                NKflag="mod";
                NK=mod;
                flag++;
                if(UnaryExp(result)==0)
                {
                    return 0;
                }
            }
            else
            {
                break;
            }
        }
        //cout.open("output.txt");
        outputGT("<MulExp>");
        //cout <<  << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}
int AddExp(vector <word> result)
{
    string NKflag="\0";
    int NK;
    if(MulExp(result))
    {
        while(1)
        {
            if(NKflag!="\0")
            {
                inCode(NK);
                NKflag="\0";
            }
            if(result[flag].outCategory()=="PLUS")
            {
                //cout.open("output.txt");
                outputGT("<AddExp>");

                //cout <<  << endl;
                outputG(result);
                NKflag="PLUS";
                NK=add;
                flag++;
                if(MulExp(result)==0)
                {
                    return 0;
                }
            }
            else if(result[flag].outCategory()=="MINU")
            {
                //cout.open("output.txt");
                outputGT("<AddExp>");
                //cout <<  << endl;
                outputG(result);
                NKflag="MINU";
                NK=sub;
                flag++;
                if(MulExp(result)==0)
                {
                    return 0;
                }
            }
            else
            {
                break;
            }
        }
        //cout.open("output.txt");
        outputGT("<AddExp>");
        //cout <<  << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}
int RelExp(vector <word> result)
{
    string NKflag="\0";
    int NK;
    if(AddExp(result))
    {
        while(1)
        {
            if(NKflag!="\0")
            {
                inCode(NK);
                NKflag="\0";
            }
            if(result[flag].outCategory()=="LSS")
            {
                //cout.open("output.txt");
                outputGT("<RelExp>");
                //cout <<  << endl;
                outputG(result);
                NKflag="LSS";
                NK=LSS;
                flag++;
                if(AddExp(result)==0)
                {
                    return 0;
                }
            }
            else if(result[flag].outCategory()=="GRE")
            {
                //cout.open("output.txt");
                outputGT("<RelExp>");
                //cout <<  << endl;
                outputG(result);
                NKflag="GRE";
                NK=GRE;
                flag++;
                if(AddExp(result)==0)
                {
                    return 0;
                }
            }
            else if(result[flag].outCategory()=="LEQ")
            {
                //cout.open("output.txt");
                outputGT("<RelExp>");
                //cout <<  << endl;
                outputG(result);
                NKflag="LEQ";
                NK=LEQ;
                flag++;
                if(AddExp(result)==0)
                {
                    return 0;
                }
            }
            else if(result[flag].outCategory()=="GEQ")
            {
                //cout.open("output.txt");
                outputGT("<RelExp>");
                //cout <<  << endl;
                outputG(result);
                NKflag="GEQ";
                NK=GEQ;
                flag++;
                if(AddExp(result)==0)
                {
                    return 0;
                }
            }
            else
            {
                break;
            }
        }
        if(NKflag!="\0")
        {
            inCode(NK);
            NKflag="\0";
        }
        //cout.open("output.txt");
        outputGT("<RelExp>");
        //cout <<  << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}
int EqExp(vector <word> result)
{
    string NKflag="\0";
    int NK;
    if(RelExp(result))
    {
        while(1)
        {
            if(NKflag!="\0")
            {
                inCode(NK);
                NKflag="\0";
            }
            if(result[flag].outCategory()=="EQL")
            {
                outputGT("<EqExp>");
                //cout <<  << endl;

                outputG(result);
                NKflag="EQL";
                NK=EQL;
                flag++;

                if(RelExp(result)==0)
                {
                    return 0;
                }
            }
            else if(result[flag].outCategory()=="NEQ")
            {
                outputGT("<EqExp>");
                //cout <<  << endl;
                outputG(result);
                NKflag="NEQ";
                NK=NEQ;
                flag++;

                if(RelExp(result)==0)
                {
                    return 0;
                }

            }
            else
            {

                break;
            }
        }
        if(NKflag!="\0")
        {
            inCode(NK);
            NKflag="\0";
        }
        //cout.open("output.txt");
        outputGT("<EqExp>");
        //cout <<  << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}
int LAndExp(vector <word> result)
{
    string NKflag="\0";
    int NK;
    if(EqExp(result))
    {
        while(result[flag].outCategory()=="AND")
        {
            if(NKflag!="\0")
            {
                inCode(NK);
                NKflag="\0";
            }
            outputGT("<LAndExp>");
            //cout <<  << endl;
            outputG(result);
            NKflag="AND";
            NK=AND;
            flag++;
            if(EqExp(result)==0)
            {
                return 0;
            }
        }
        if(NKflag!="\0")
        {
            inCode(NK);
            NKflag="\0";
        }
        //cout.open("output.txt");
        outputGT("<LAndExp>");
        //cout <<  << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}
int LOrExp(vector <word> result)
{
    string NKflag="\0";
    int NK;
    if(LAndExp(result))
    {

        while(result[flag].outCategory()=="OR")
        {
            if(NKflag!="\0")
            {
                inCode(NK);
                NKflag="\0";
            }
            outputGT("<LOrExp>");
            //cout <<  << endl;
            outputG(result);
            NKflag="OR";
            NK=OR;
            flag++;
            if(LAndExp(result)==0)
            {
                return 0;
            }
        }
        if(NKflag!="\0")
        {
            inCode(NK);
            NKflag="\0";
        }
        //cout.open("output.txt");
        outputGT("<LOrExp>");
        //cout <<  << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}
int ConstExp(vector <word> result)
{
    if(AddExp(result))
    {
        //cout.open("output.txt");
        outputGT("<ConstExp>");
        //cout <<  << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}
void outputG(vector <word> result)
{
    //cout<<result[flag].outCategory()+" "+result[flag].outContent()<<endl;
}
void outputGT(string GT)
{
    //cout<<GT<<endl;
}
void ErrorOutput(vector <word> result,string ERROR) {

//    if(ERROR == "k"){
//        gzr=1;
//    }
//    else if (ERROR == "g" && gzr == 1) {
//        while (1) {}
//    }
    cout << result[flag].RowNum() << " " << ERROR << endl;

}

void ErrorOutputChoice(vector <word> result,string ERROR,int num)
{
//    if(ERROR == "k"){
//        gzr=1;
//    }
//    else if (ERROR == "g"&& gzr == 1) {
//        while (1) {}
//    }
    cout<<result[num].RowNum()<<" "<<ERROR<<endl;
}
int ErrorReturn()
{
    return 1;
}
int select(vector <word> result,int tempflag)
{
    for(int i=Slist.size()-1;i>=0;i--)
    {
        //cout<<result[tempflag].outContent()<<Slist[i].outname()<<endl;
        if(result[tempflag].outContent()==Slist[i].outname()&&Slist[i].outlev()==levflag)
        {
            ErrorOutputChoice(result,"b",tempflag);
            return 0;
        }
    }
    return 1;
}
void popFlist()
{
    for(int i=Slist.size()-1; i>=0 && Slist[i].outlev()==levflag ;i--)
    {
        Slist.pop_back();
    }

}
void errorc(vector <word> result)
{
    for(int i=Slist.size()-1;i>=0;i--)
    {
        if(result[flag].outContent()==Slist[i].outname()&&Slist[i].outlev()<=levflag)
        {
            return;
        }
    }
    ErrorOutput(result,"c");
    return;
}
void errorh(vector <word> result)
{

    for(int i=Slist.size()-1;i>=0;i--)
    {
//        cout<<result[flag].outContent()<<Slist[i].outname()<<endl;
        if(result[flag].outContent()==Slist[i].outname()&&Slist[i].outlev()<=levflag)
        {

            if(Slist[i].outkind()=="const")
            {
                ErrorOutput(result,"h");
            }
            else
            {
                break;
            }
        }
    }

    return;
}
int errord(vector <word> result,int temp)
{
    int j=Funcname.size()-1;
    int k=paraflagD.size()-1;
    if(j<0||k<0)
    {
        ErrorOutputChoice(result,"d",temp);
        return 0;
    }
    for(int i=Slist.size()-1;i>=0;i--)
    {
        if(Funcname[j]==Slist[i].outname())
        {
            if(Slist[i].outinterpret()==paraflagD[k])
            {

                return 1;
            }
            else
            {
                ErrorOutputChoice(result,"d",temp);
                int pop=paraflagD[k];
                while(pop>0&&Elist.size()!=0)
                {
                    Elist.pop_back();
                }
                return 0;
            }
        }
    }
    return 0;
}
void errore(vector <word> result,int temp)
{
    int ei=0;
    int e_flag=0;
    for(int i=Flist.size()-1;i>=0;i--)
    {
        if(Flist[i].outproc()==Funcname[Funcname.size()-1])
        {
            while(ei<paraflagD[paraflagD.size()-1]&&paraflagD.size()!=0)
            {

                if(Elist[Elist.size()-1].outEpara()!=Flist[i-ei].outpara()||Elist[Elist.size()-1].outEparray()!=Flist[i-ei].outparray())
                {
                    e_flag=1;
                }
                ei++;
                if(Elist.size()!=0)
                {
                    Elist.pop_back();
                }

            }
        }
    }
    if(e_flag==1)
    {
        ErrorOutputChoice(result,"e",temp);
        return;
    }
    return;
}
void pushe(vector <word> result)
{
    int j=flag;
    while(result[j].outCategory()!="IDENFR")
    {
        j--;
    }
    for(int i=Slist.size()-1;i>=0;i--)
    {

        if(result[j].outContent()==Slist[i].outname())
        {
            ErrorE temp(Slist[i].outtype(),Slist[i].outarray());
            Elist.push_back(temp);
            return ;
        }
    }
    cout << Elist.size() <<endl;
    return ;
}