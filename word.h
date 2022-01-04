//
// Created by lenovo on 2021/9/28.
//
#include <bits/stdc++.h>
using namespace std;
#ifndef GZR_WORD_H
#define GZR_WORD_H

class word//单词类
{
private:
    string content;//内容
    string category;//种类
    int row;

public:
    word (string content,string category,int row);//初始化
    string outContent();//输出内容
    string outCategory();//输出种类
    int RowNum();

};


#endif //GZR_WORD_H
