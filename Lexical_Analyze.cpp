//
// Created by lenovo on 2021/9/27.
//
#include <iostream>
#include "Lexical_Analyze.h"
#include "word.h"


vector <char> str;//char 类型vector存字符
vector <int> rowNum;
vector <word> result;//word类 vector存结果
map<string,string> keyword={{"main","MAINTK"},
                            {"const","CONSTTK"},
                            {"int","INTTK"},
                            {"break","BREAKTK"},
                            {"continue","CONTINUETK"},
                            {"if","IFTK"},
                            {"else","ELSETK"},
                            {"while","WHILETK"},
                            {"getint","GETINTTK"},
                            {"printf","PRINTFTK"},
                            {"return","RETURNTK"},
                            {"void","VOIDTK"}
                            };
vector <word>Lexical_Analyze()
{
    getText();//读取
    analyzeText();//分析
//    for(int i=0;i<result.size();i++)
//    {
//        cout<<result[i].outContent()<<" "<<result[i].RowNum()<<endl;
//    }
    return result;
}

void getText()
{
    FILE *in = fopen("testfile.txt","r");
    char f[1000];
    int i;
    int row=1;
    while (fgets(f,1000,in)!=NULL)
    {
        for(i=0;i<strlen(f);i++)
        {
            str.push_back(f[i]);
            rowNum.push_back(row);
        }
        row++;
    }

}

void output()
{
    ofstream  fout;
    int i;
    fout.open("output.txt");
    //cout << "output" << endl;
    for(i=0;i<result.size();i++)
    {
        fout<<result[i].outCategory()+" "+result[i].outContent()<<endl;
    }
    fout.close();
}

void analyzeText()
{
    int flag=0;
    //cout << str.size() << endl;
    while(flag<str.size())
    {
        //cout << flag << str[flag] << endl;
        if(str[flag]<=32)
        {
            flag++;
            continue;
        }
        int other=0;
        if(str[flag]=='!')
        {
            if(str[flag+1]=='=')
            {
                word temp("!=","NEQ",rowNum[flag]);
                result.push_back(temp);
                flag=flag+2;
            }
            else
            {
                word temp("!","NOT",rowNum[flag]);
                result.push_back(temp);
                flag=flag+1;
            }


        }
        else if(str[flag]=='&')
        {
            word temp("&&","AND",rowNum[flag]);
            result.push_back(temp);
            flag=flag+2;
        }
        else if(str[flag]=='|')
        {
            word temp("||","OR",rowNum[flag]);
            result.push_back(temp);
            flag=flag+2;
        }
        else if(str[flag]=='+')
        {
            word temp("+","PLUS",rowNum[flag]);
            result.push_back(temp);
            flag=flag+1;
        }
        else if(str[flag]=='-')
        {
            word temp("-","MINU",rowNum[flag]);
            result.push_back(temp);
            flag=flag+1;
        }
        else if(str[flag]=='*')
        {
            word temp("*","MULT",rowNum[flag]);
            result.push_back(temp);
            flag=flag+1;
        }
        else if(str[flag]=='/')
        {
            if(str[flag+1]=='/')
            {
                int L=1;
                while(str[flag+1+L]!='\n')
                {
                    L++;
                }
                flag=flag+1+L;
            }
            else if(str[flag+1]=='*')
            {
                int L=1;
                while(!(str[flag+1+L]=='*'&&str[flag+2+L]=='/'))
                {
                    L++;
                }
                flag=flag+1+L+1+1;
            }
            else
            {
                word temp("/","DIV",rowNum[flag]);
                result.push_back(temp);
                flag=flag+1;
            }
        }
        else if(str[flag]=='%')
        {
            word temp("%","MOD",rowNum[flag]);
            result.push_back(temp);
            flag=flag+1;
        }
        else if(str[flag]=='<')
        {
            if(str[flag+1]=='=')
            {
                word temp("<=","LEQ",rowNum[flag]);
                result.push_back(temp);
                flag=flag+2;
            }
            else
            {
                word temp("<","LSS",rowNum[flag]);
                result.push_back(temp);
                flag=flag+1;
            }

        }
        else if(str[flag]=='>')
        {
            if(str[flag+1]=='=')
            {
                word temp(">=","GEQ",rowNum[flag]);
                result.push_back(temp);
                flag=flag+2;
            }
            else
            {
                word temp(">","GRE",rowNum[flag]);
                result.push_back(temp);
                flag=flag+1;
            }

        }
        else if(str[flag]=='=')
        {
            if(str[flag+1]=='=')
            {
                word temp("==","EQL",rowNum[flag]);
                result.push_back(temp);
                flag=flag+2;
            }
            else
            {
                word temp("=","ASSIGN",rowNum[flag]);
                result.push_back(temp);
                flag=flag+1;
            }

        }
        else if(str[flag]==';')
        {
            word temp(";","SEMICN",rowNum[flag]);
            result.push_back(temp);
            flag=flag+1;
        }
        else if(str[flag]==',')
        {
            word temp(",","COMMA",rowNum[flag]);
            result.push_back(temp);
            flag=flag+1;
        }
        else if(str[flag]=='(')
        {
            word temp("(","LPARENT",rowNum[flag]);
            result.push_back(temp);
            flag=flag+1;
        }
        else if(str[flag]==')')
        {
            word temp(")","RPARENT",rowNum[flag]);
            result.push_back(temp);
            flag=flag+1;
        }
        else if(str[flag]=='[')
        {
            word temp("[","LBRACK",rowNum[flag]);
            result.push_back(temp);
            flag=flag+1;
        }
        else if(str[flag]==']')
        {
            word temp("]","RBRACK",rowNum[flag]);
            result.push_back(temp);
            flag=flag+1;
        }
        else if(str[flag]=='{')
        {
            word temp("{","LBRACE",rowNum[flag]);
            result.push_back(temp);
            flag=flag+1;
        }
        else if(str[flag]=='}')
        {
            word temp("}","RBRACE",rowNum[flag]);
            result.push_back(temp);
            flag=flag+1;
        }
        else if(str[flag]=='"')
        {
            int L=1;
            string s="\"";
            while(str[flag+L]!='\"')
            {
                s.push_back(str[flag+L]);
                L++;
            }
            s.push_back('\"');
            L++;
            word temp(s,"STRCON",rowNum[flag]);
            result.push_back(temp);
            flag=flag+L;
        }
        else
        {
            other=1;
        }
        if(!other)
        {
            continue;
        }
        if(isdigit(str[flag]))
        {
            int L=0;
            string temp="";
            while(isdigit(str[flag+L]))
            {
                temp.push_back(str[flag+L]);
                L++;
            }
            word w(temp,"INTCON",rowNum[flag]);
            result.push_back(w);
            flag=flag+L;
        }
        else
        {
            string W = "";
            int L = 0;
            while (isalpha(str[flag+L])||str[flag+L]=='_'||((isdigit(str[flag+L]))&&L>0))
            {
                W.push_back(str[flag+L]);
                L++;
            }
            if(keyword.count(W)>0)
            {
                word w(W,keyword[W],rowNum[flag]);
                result.push_back(w);
            }
            else
            {
                word w(W,"IDENFR",rowNum[flag]);
                result.push_back(w);
            }
            flag=L+flag;
        }
    }
}
