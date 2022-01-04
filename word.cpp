//
// Created by lenovo on 2021/9/28.
//
#include "word.h"

word::word(string content, string category,int row)//初始化
{
    this->content=content;
    this->category=category;
    this->row=row;
}

string word::outContent()
{
    return content;
}
string word::outCategory()
{
    return category;
}
int word::RowNum()
{
    return row;
}