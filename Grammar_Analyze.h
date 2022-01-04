//
// Created by lenovo on 2021/10/15.
//

#ifndef GZR_GRAMMAR_ANALYZE_H
#define GZR_GRAMMAR_ANALYZE_H
#include <bits/stdc++.h>//调全库
using namespace std;

void Grammar_Analyze(vector <word>);
void analyzeG(vector <word>);
void outputG(vector <word>);
void outputGT(string);
void ErrorOutput(vector <word>,string);
void ErrorOutputChoice(vector <word>,string,int);
int ErrorReturn ();
void CompUnit(vector <word>);
int Decl(vector <word>);
int ConstDecl(vector <word>);
int BType(vector <word>);
int ConstDef(vector <word>);
int ConstInitVal(vector <word>);
int VarDecl(vector <word>);
int VarDef(vector <word>);
int InitVal(vector <word>);
int FuncDef(vector <word>);
int MainFuncDef(vector <word>);
int FuncType(vector <word>);
int FuncFParams(vector <word>);
int FuncFParam(vector <word>);
int Block(vector <word>);
int BlockItem(vector <word>);
int Stmt(vector <word>);
int Exp(vector <word>);
int Cond(vector <word>);
int LVal(vector <word>);
int PrimaryExp(vector <word>);
int Number(vector <word>);
int UnaryExp(vector <word>);
int UnaryOp(vector <word>);
int FuncRParams(vector <word>);
int MulExp(vector <word>);
int AddExp(vector <word>);
int RelExp(vector <word>);
int EqExp(vector <word>);
int LAndExp(vector <word>);
int LOrExp(vector <word>);
int ConstExp(vector <word>);
int select(vector <word>,int);
void popFlist();
void errorc(vector <word>);
void errorh(vector <word>);
int errord(vector <word>,int);
void errore(vector <word>,int);
void pushe(vector <word>);


#endif //GZR_GRAMMAR_ANALYZE_H
