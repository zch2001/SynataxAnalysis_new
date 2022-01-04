#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>

std::string keyword[32] ={ "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else",
    "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed",
    "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while" };//关键字
char separater[8] = { ';',',','{','}','[',']','(',')' };    //分隔符
char operate[10] = { '+','-','*','/','>','<','=','!','%','"'};     //运算符
char interval[4] = { ' ','\t','\r','\n' };
char wrong[4] = {'“','”','‘','’'};//错误集
std::string showTimes[6] = { "error","operator","separater","keyword","number","IDENTIFIER"};

class LexiaclAnalysis {
private:
    std::string inputBuffer;//存储输入
    std::vector<std::string>woldList;//存储单词表
    std::string codeString;
    int num = 0;
    int eT, oT, sT, kT, nT, iT = 0;
    int line=1;
    FILE* code=NULL;
    int errorNum = 0;
    std::string *errorInfo;
    void checkShowTimes() {
        for (unsigned i = 2; i < woldList.size();i++ ) {
            if (woldList[i] == showTimes[0]) {
                eT++;
            }
            if (woldList[i] == showTimes[1]) {
                oT++;
            }
            if (woldList[i] == showTimes[2]) {
                sT++;
            }
            if (woldList[i] == showTimes[3]) {
                kT++;
            }
            if (woldList[i] == showTimes[4]) {
                nT++;
            }
            if (woldList[i] == showTimes[5]) {
                iT++;
            }
        }
            std::cout << "there is " << eT << " error   " << oT << " operator   "<< sT << " separater   " << kT
                << " keyword   " << nT << " number   " << iT << " IDENTIFIER" << std::endl;
    }
    bool isKeyword(std::string word) {
        for (int i = 0; i < 32; i++) {
            if (keyword[i] == word)
                return true;
        }
        return false;
    }
    bool isSeparater(char input) {
        for (int i = 0; i < 8; i++) {
            if (separater[i] == input)
                return true;
        }
        return false;
    }
    bool isWrong(char input) {
        for (int i = 0; i < 2; i++) {
            if (wrong[i]==input)
                return true;
        }
        return false;
    }
    bool isOperator(char input) {
        for (int i = 0; i < 10; i++) {
            if (operate[i] == input)
                return true;
        }
        return false;
    }
    bool isInterval(char input) {
        if (input=='\n') {
            line++;
        }
        for (int i = 0; i < 4; i++) {
            if (interval[i] == input)
                return true;
        }
        return false;
    }
    bool isLetter(char input) {
        if ((input >= 'a' && input <= 'z')|| (input >= 'A' && input <= 'Z'))
            return true;
        return false;
    }
    bool isDigit(char input) {
        if (input >= '0' && input <= '9')
            return true;
        return false;
    }
    template <class T>
    int value(T* a, int n, T str) {
        for (int i = 0; i < n; i++) {
            if (a[i] == str)
                return i + 1;
        }
        return -1;
    }
    void destoryInputBUffer() {
        inputBuffer.erase(inputBuffer.begin(), inputBuffer.end());
    }
    void freeWoldLsit() {
        woldList.clear();
    }
    void LexScan() {
        std::string temp = "";
        inputBuffer = "";
        std::cout << codeString << std::endl;
        for (int i = 0; i < codeString.size(); i++) {
            if (codeString[i]=='/'&&codeString[i+1]=='/') {
                while (codeString[i] != '\n')
                    i++;
            }
            if (codeString[i] == '/' && codeString[i+1] == '*') {
                i += 2;
                while (codeString[i] != '*' && codeString[i + 1] != '/') {
                    i++;
                }
                if (codeString[i] == '*' && codeString[i + 1] == '/') {
                    i++;
                }
            }
            else {
            if (isInterval(codeString[i]))
                continue;
                if (isOperator(codeString[i])) {
                    woldList.push_back(std::to_string(value(operate, (sizeof(operate) / sizeof(operate[0])), codeString[i])));
                    temp += codeString[i];
                    woldList.push_back(temp);
                    woldList.push_back("operator");
                    num++;
                    temp = "";
                    continue;
                }
                if (isSeparater(codeString[i])) {
                    woldList.push_back(std::to_string(value(separater, (sizeof(separater) / sizeof(separater[0])), codeString[i])));
                    temp += codeString[i];
                    woldList.push_back(temp);
                    woldList.push_back("separater");
                    num++;
                    temp = "";
                    continue;
                }
                if (isWrong(codeString[i])) {
                    woldList.push_back("-1");
                    temp += codeString[i];
                    woldList.push_back(temp);
                    woldList.push_back("error");
                    errorNum++;
                    *(errorInfo + errorNum - 1) = "error " + std::to_string(errorNum) + " in line " + std::to_string(line);
                    temp = "";
                    goto err;
                }
                if (isDigit(codeString[i])) {
                    temp += codeString[i];
                    while (isDigit(codeString[i + 1])) {
                        i++;
                        temp += codeString[i];
                        if (isLetter(codeString[i+1])) {
                            while (isLetter(codeString[i+1])){
                                i++;
                                temp += codeString[i];
                            }
                            woldList.push_back("-1");
                            woldList.push_back(temp);
                            woldList.push_back("error");
                            errorNum++;
                            *(errorInfo + errorNum - 1) = "error " + std::to_string(errorNum) + " in line " + std::to_string(line);
                            temp = "";
                            goto err;
                        }
                    }
                    woldList.push_back("101");
                    woldList.push_back(temp);
                    woldList.push_back("number");
                    temp = "";
                    continue;
                }
                err:
                if (isLetter(codeString[i])) {
                    temp += codeString[i];
                    while (isDigit(codeString[i + 1]) || isLetter(codeString[i + 1])) {
                        i++;
                        temp += codeString[i];
                    }
                    if (isKeyword(temp)) {
                        woldList.push_back(std::to_string(value(keyword, (sizeof(keyword) / sizeof(keyword[0])), temp)));
                        woldList.push_back(temp);
                        woldList.push_back("keyword");
                        temp = "";
                        continue;
                    }
                    else {
                        woldList.push_back("100");
                        woldList.push_back(temp);
                        woldList.push_back("IDENTIFIER");
                        temp = "";
                        continue;
                    }
                }
            }
        }
    }
public:
    LexiaclAnalysis() {
        fopen_s(&code,"./resorse.txt", "r");
        char c;
        while ((c = getc(code)) != EOF){
            codeString += c;
        }
        errorInfo = new std::string[100];
    }
    LexiaclAnalysis(char *address) {
        fopen_s(&code, address, "r");
        char c;
        while ((c = getc(code)) != EOF) {
            codeString += c;
        }
        errorInfo = new std::string[100];
    }
    void LexPrint() {
        LexScan();
        std::cout<<"值\t"<<"单词\t"<<"记号\t"<<std::endl;
        for (unsigned i = 0; i < woldList.size(); ){
        std::cout << woldList[i] << "\t" << woldList[i + 1] << "\t" << woldList[i + 2] << std::endl;
        i += 3;
        }
        std::cout << "there is " << line << " line" << std::endl;
        for (int i = 0; i < errorNum; i++){
            if (errorInfo[i] != "") {

                std::cout << *(errorInfo+i) << "\n";
            }
            else{
                break;
            }
        }
        checkShowTimes();
        destoryInputBUffer();
        freeWoldLsit();
        delete[]errorInfo;
    }
};
int main(){
    LexiaclAnalysis lex = LexiaclAnalysis();
    lex.LexPrint();
    return 0;
}