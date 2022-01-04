#include <iostream>
#include <string>
#include <stack>
#include <sstream>
#include <stdexcept>

class SynAna
{
public:
    SynAna() = default;
    void analyse(std::string in) {
        try {
            analyseFun(in);
        }
        catch (std::exception& e) {
            std::cerr << "! EXCEPTION: " << e.what() << std::endl;
            return ;
        }
    }

private:
    std::stack<int>state;
    std::stack<std::string>symbol;
    std::stack<std::string>w;
    std::string action[30][8] = {
   /* (      )     +     -     *     /    num    $   */
    {"S4" , ""  , ""  , ""  , ""  , ""  ,"S5" , ""  },             //0
    { ""  , ""  ,"S6" ,"S7" , ""  , ""  , ""  ,"ACC"},          //1
    { ""  , ""  ,"R3" ,"R3" ,"S8" ,"S9" , ""  ,"R3" },       //2
    { ""  , ""  ,"R6" ,"R6" ,"R6" ,"R6" , ""  ,"R6" },       //3
    {"S13", ""  , ""  , ""  , ""  , ""  ,"S14", ""  },           //4
    { ""  , ""  ,"R8" ,"R8" ,"R8" ,"R8" , ""  ,"R8" },       //5
    {"S4" , ""  , ""  , ""  , ""  , ""  ,"S5" , ""  },             //6
    {"S4" , ""  , ""  , ""  , ""  , ""  ,"S5" , ""  },             //7
    {"S4" , ""  , ""  , ""  , ""  , ""  ,"S5" , ""  },             //8
    {"S4" , ""  , ""  , ""  , ""  , ""  ,"S5" , ""  },             //9
    { ""  ,"S19","S20","S21", ""  , ""  , ""  , ""  },        //10
    { ""  ,"R3" ,"R3" ,"R3" ,"S22","S23", ""  , ""  },     //11
    { ""  ,"R6" ,"R6" ,"R6" ,"R6" ,"R6" , ""  , ""  },       //12
    {"S13", ""  , ""  , ""  , ""  , ""  ,"S14", ""  },           //13
    { ""  ,"R8" ,"R8" ,"R8" ,"R8" ,"R8" , ""  , ""  },       //14
    { ""  , ""  ,"R1" ,"R1" ,"S8" ,"S9" , ""  ,"R1" },       //15
    { ""  , ""  ,"R2" ,"R2" ,"S8" ,"S9" , ""  ,"R2" },       //16
    { ""  , ""  ,"R4" ,"R4" ,"R4" ,"R4" , ""  ,"R4" },       //17
    { ""  , ""  ,"R5" ,"R5" ,"R5" ,"R5" , ""  ,"R5" },       //18
    { ""  , ""  ,"R7" ,"R7" ,"R7" ,"R7" , "" , "R7" },       //19
    {"S13", ""  , ""  , ""  , ""  , ""  ,"S14", ""  },           //20
    {"S13", ""  , ""  , ""  , ""  , ""  ,"S14", ""  },           //21
    {"S13", ""  , ""  , ""  , ""  , ""  ,"S14", ""  },           //22
    {"S13", ""  , ""  , ""  , ""  , ""  ,"S14", ""  },           //23
    { ""  ,"S28","S20","S21", ""  , ""  , ""  , ""  },        //24
    { ""  ,"R1" ,"R1" ,"R1" ,"S22","S23", ""  , ""  },     //25
    { ""  ,"R4" ,"R4" ,"R4" ,"R4" ,"R4" , ""  , ""  },       //26
    { ""  ,"R5" ,"R5" ,"R5" ,"R5" ,"R5" , ""  , ""  },       //27
    { ""  ,"R7" ,"R7" ,"R7" ,"R7" ,"R7" , ""  , ""  },       //28
    { ""  ,"R2" ,"R2" ,"R2" ,"S22","S23", ""  , ""  } };   //29
    float synaGoto[30][3] = {
    {1,2,3},            //0
    {NAN,NAN,NAN},      //1
    {NAN,NAN,NAN},      //2
    {NAN,NAN,NAN},      //3
    {10,11,12},         //4
    {NAN,NAN,NAN},      //5
    {NAN,15,3},         //6
    {NAN,16,3},         //7
    {NAN,NAN,17},       //8
    {NAN,NAN,18},       //9
    {NAN,NAN,NAN},      //10
    {NAN,NAN,NAN},      //11
    {NAN,NAN,NAN},      //12
    {24,11,12},         //13
    {NAN,NAN,NAN},      //14
    {NAN,NAN,NAN},      //15
    {NAN,NAN,NAN},      //16
    {NAN,NAN,NAN},      //17
    {NAN,NAN,NAN},      //18
    {NAN,NAN,NAN},      //19
    {NAN,25,12},        //20
    {NAN,29,12},        //21
    {NAN,NAN,26},       //22
    {NAN,NAN,27},       //23
    {NAN,NAN,NAN},      //24
    {NAN,NAN,NAN},      //25
    {NAN,NAN,NAN},      //26
    {NAN,NAN,NAN},      //27
    {NAN,NAN,NAN},      //28
    {NAN,NAN,NAN}       //29
    };
    const char* expre[9] = { nullptr,"E->E+T","E->E-T","E->T","T->T*F","T->T/F","T->F","F->(E)","F->num" };
    void reduce(std::string actionString) {
        switch (std::atoi(actionString.c_str() + 1)) {
        case 1: //用产生式E->E+T
            for (int i = 0; i < 3; i++) {
                state.pop();
                symbol.pop();
            }
            //将当前元素push进去
            symbol.push("E");
            if (synaGoto[state.top()][0] == NAN) throw std::runtime_error("gotoException");
            state.push(synaGoto[state.top()][0]);
            break;
        case 2: //用产生式E->E-T
            for (int i = 0; i < 3; i++) {
                state.pop();
                symbol.pop();
            }
            symbol.push("E");
            if (synaGoto[state.top()][0] == NAN) throw std::runtime_error("gotoException");
            state.push(synaGoto[state.top()][0]);
            break;
        case 3://用产生式E->T
            state.pop();
            symbol.pop();
            symbol.push("E");
            if (synaGoto[state.top()][0] == NAN) throw std::runtime_error("gotoException");
            state.push(synaGoto[state.top()][0]);
            break;
        case 4:  //用产生式T->T*F
            for (int i = 0; i < 3; i++) {
                state.pop();
                symbol.pop();
            }
            //将当前元素push进去
            symbol.push("T");
            if (synaGoto[state.top()][1] == NAN) throw std::runtime_error("gotoException");
            state.push(synaGoto[state.top()][1]);
            break;
        case 5: //用产生式T->T/F
            for (int i = 0; i < 3; i++) {
                state.pop();
                symbol.pop();
            }
            //将当前元素push进去
            symbol.push("T");
            if (synaGoto[state.top()][1] == NAN) throw std::runtime_error("gotoException");
            state.push(synaGoto[state.top()][1]);
            break;
        case 6: //用产生式T->F
            state.pop();
            symbol.pop();
            symbol.push("T");
            if (synaGoto[state.top()][1] == NAN) throw std::runtime_error("gotoException");
            state.push(synaGoto[state.top()][1]);
            break;
        case 7: //用产生式F->(E)
            //pop出3个元素
            for (int i = 0; i < 3; i++) {
                state.pop();
                symbol.pop();
            }
            symbol.push("F");
            if (synaGoto[state.top()][2] == NAN) throw std::runtime_error("gotoException");
            state.push(synaGoto[state.top()][2]);
            break;
        case 8: //用产生式F->num
            state.pop();
            symbol.pop();
            symbol.push("F");
            if (synaGoto[state.top()][2] == NAN) throw std::runtime_error("gotoException");
            state.push(synaGoto[state.top()][2]);
            break;
        }
    }
    template <typename T>
    std::string stack_to_str(std::stack<T> st) { //输出栈 转换为字符串
        std::stringstream ss;
        ss << '[';
        std::stack<T> stack;
        while (!st.empty()) {
            stack.push(st.top());
            st.pop();
        }
        for (; !stack.empty(); stack.pop()) {
            T element = stack.top();
            ss << element;
            ss << ", ";
        }
        std::string s = ss.str().substr(0, ss.str().length() - 2); //删除最后一个", "
        s += ']';
        return s;
    }
    void output(int step, std::string action_string) {
        // 输出步骤序号
        std::cout << step << "\t";
        // 输出状态栈
        std::printf("%-30s", stack_to_str(state).c_str());
        // 输出符号栈
        std::printf("%-26s\t", stack_to_str(symbol).c_str());
        // 输出输入符号流
        std::string expressionString;
        for (auto dump = w; !dump.empty(); dump.pop())
            expressionString += dump.top();
        std::printf("%25s", (expressionString).c_str());
        // 输出分析动作
        switch (action_string[0]) {
        case 'R':
            std::cout << "\treduce " << expre[std::atoi(action_string.c_str() + 1)] << std::endl;
            break;
        case 'S':
            std::cout << "\tshift " << action_string.c_str() + 1 << std::endl;
            break;
        case 'A':
            std::cout << "\tACC" << std::endl;
            break;
        }
    }
    void analyseFun(std::string input) {
        input += "$";
        std::cout << "输入为：" << input << "\n分析结果：\n";
        std::stack<std::string> s;
        for (int i = 0; i < input.length(); i++) {
            if (input[i] != 'n') s.push(std::string{ input[i] });
            else {
                char temp[4] = { 0 };
                temp[0] = input[i];
                temp[1] = input[i + 1];
                temp[2] = input[i + 2];
                s.push(temp);
                i = i + 2;
            }
        }
        std::cout << "步骤\t状态栈\t\t\t      符号栈\t\t\t\t             \t输入符号流\t分析动作\n";
        while (!s.empty()) {
            w.push(s.top());
            s.pop();
        }
        state.push(0);
        symbol.push("$");
        int step = 0;
        while (!w.empty()) {
            //栈顶状态
            step++;
            int temp_state = state.top();
            std::string actionString;
            std::string temp_s = w.top();
            if (w.top() == "(") {
                //遇到“(”时的动作
                actionString = action[temp_state][0];
                if (actionString == std::string(""))
                    throw std::runtime_error("reduceException ->" + w.top());
                //移进
                if (actionString[0] == 'S') {
                    output(step, actionString);
                    state.push(std::atoi(actionString.c_str() + 1));
                    symbol.push(w.top());
                    w.pop();
                }
                //规约
                else if (actionString[0] == 'R') {
                    output(step, actionString);
                    reduce(actionString);
                }
            }
            else if (w.top() == ")") {
                //遇到“)”时的动作
                actionString = action[temp_state][1];
                if (actionString == std::string(""))
                    throw std::runtime_error("reduceException ->" + w.top());
                //移进
                if (actionString[0] == 'S') {
                    output(step, actionString);
                    state.push(std::atoi(actionString.c_str() + 1));
                    symbol.push(w.top());
                    w.pop();
                }
                //规约
                else if (actionString[0] == 'R') {
                    output(step, actionString);
                    reduce(actionString);
                }
            }
            else if (w.top() == "+") {
                //遇到“+”时的动作
                actionString = action[temp_state][2];
                if (actionString == std::string(""))
                    throw std::runtime_error("reduceException ->" + w.top());
                //移进
                if (actionString[0] == 'S') {
                    output(step, actionString);
                    state.push(std::atoi(actionString.c_str() + 1));
                    symbol.push(w.top());
                    w.pop();
                }
                //规约
                else if (actionString[0] == 'R') {
                    output(step, actionString);
                    reduce(actionString);
                }
            }
            else if (w.top() == "-") {
                //遇到“-”时的动作
                actionString = action[temp_state][3];
                if (actionString == std::string(""))
                    throw std::runtime_error("reduceException ->" + w.top());
                //移进
                if (actionString[0] == 'S') {
                    output(step, actionString);
                    state.push(std::atoi(actionString.c_str() + 1));
                    symbol.push(w.top());
                    w.pop();
                }
                //规约
                else if (actionString[0] == 'R') {
                    output(step, actionString);
                    reduce(actionString);
                }
            }
            else if (w.top() == "*") {
                actionString = action[temp_state][4];
                if (actionString == std::string(""))
                    throw std::runtime_error("reduceException ->" + w.top());
                //移进
                if (actionString[0] == 'S') {
                    output(step, actionString);
                    state.push(std::atoi(actionString.c_str() + 1));
                    symbol.push(w.top());
                    w.pop();
                }
                //规约
                else if (actionString[0] == 'R') {
                    output(step, actionString);
                    reduce(actionString);
                }
            }
            else if (w.top() == "/") {
                actionString = action[temp_state][5];
                if (actionString == std::string(""))
                    throw std::runtime_error("reduceException ->" + w.top());
                //移进
                if (actionString[0] == 'S') {
                    output(step, actionString);
                    state.push(std::atoi(actionString.c_str() + 1));
                    symbol.push(w.top());
                    w.pop();
                }
                //规约
                else if (actionString[0] == 'R') {
                    output(step, actionString);
                    reduce(actionString);
                }
            }
            else if (w.top() == std::string("num")) {
                //遇到“num”时的动作
                actionString = action[temp_state][6];
                if (actionString == std::string(""))
                    throw std::runtime_error("reduceException ->" + w.top());
                //移进
                if (actionString[0] == 'S') {
                    output(step, actionString);
                    state.push(std::atoi(actionString.c_str() + 1));
                    symbol.push(w.top());
                    w.pop();
                }
                //规约
                else if (actionString[0] == 'R') {
                    output(step, actionString);
                    reduce(actionString);
                }
            }
            else if (w.top() == std::string("$")) {
                //遇到“$”时的动作
                actionString = action[temp_state][7];
                if (actionString == std::string(""))
                    throw std::runtime_error("reduceException ->" + w.top());
                //接受符号串
                if (actionString == "ACC") {
                    output(step, actionString);
                    state.pop();
                    symbol.pop();
                    w.pop();
                }
                //移进
                if (actionString[0] == 'S') {
                    output(step, actionString);
                    state.push(std::atoi(actionString.c_str() + 1));
                    symbol.push(w.top());
                    w.pop();
                }
                //规约
                else if (actionString[0] == 'R') {
                    output(step, actionString);
                    reduce(actionString);
                }
            }
            else throw std::runtime_error("reduceException ->" + w.top());
        }
    }
};

int main() {
    std::string syna;
    std::cin >> syna;
    SynAna synatax = SynAna();
    synatax.analyse(syna);
    return 0;
}