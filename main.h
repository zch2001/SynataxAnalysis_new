#include <algorithm>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include "mt19937ar.h"
using namespace std;
#define PI 3.1415926

#define N_router 2 //路由器数量
#define N_receiver_X 2 //接线员1（处理X类呼叫的）数量
#define N_receiver_Y 2 //接线员2（处理Y类呼叫的）数量

struct call {   //呼叫
	//该实体状态量
	int number;
	char type;
	int state;   //0-空闲,1-处理中
	int time_arrive;
};

struct router {   //路由
	//该实体状态量
	int number;  //序号
	int state;   //0-空闲,1-处理中
	call call_now;
};

struct receiver {   //接线员
	//该实体状态量
	int number;  //序号
	int state;   //0-空闲,1-处理中
	call call_now;
};


struct Node {
	int NType;  //事件类型
	int Occurtime;  //事件发生时间
	//事件发生的对象
	call call;
	int router_num;
	int receiver_num;
	struct Node* next;
};

extern struct Node* head = (struct Node*)malloc(sizeof(struct Node));  //创建头结点

class Table {
private:
	//列属性
	vector<string> column_content;
	//列宽
	vector<int> column_size;
	//记录
	vector<vector<string>> record;
public:
	//默认构造函数
	Table() {

	}
	//构造函数1:无转换构造
	Table(vector<string> cc, vector<int> cs, vector<vector<string>> rc) {
		column_content = cc;
		column_size = cs;
		record = rc;
	}
	//构造函数2:自动生成列宽
	Table(vector<string> cc, vector<vector<string>> rc) {
		int cols = cc.size(), rows = rc.size();
		//生成列宽数组
		for (int col = 0; col < cols; col++) {
			int max = cc[col].size();
			for (int row = 0; row < rows; row++) {
				max = rc[row][col].size() > max ? rc[row][col].size() : max;
			}
			column_size.push_back(max);
		}
		column_content = cc;
		record = rc;

	}
	//画分隔线
	void Print_Line() {
		for (int col = 0; col < column_size.size(); col++) {
			cout << "+-";
			for (int i = 0; i <= column_size[col]; i++) {
				cout << '-';
			}
		}
		cout << '+' << endl;
	}
	//打印表
	void Print_Table() {
		Print_Line();
		//表头
		for (int col = 0; col < column_content.size(); col++) {
			cout << "| " << setw(column_size[col]) << setiosflags(ios::left) << setfill(' ') << column_content[col] << ' ';
		}
		cout << '|' << endl;
		//内容
		Print_Line();
		for (int row = 0; row < record.size(); row++) {
			for (int col = 0; col < column_content.size(); col++) {
				cout << "| " << setw(column_size[col]) << setiosflags(ios::left) << setfill(' ');
				cout << record[row][col] << ' ';
			}
			cout << '|' << endl;
		}
		Print_Line();
	}
	~Table() {

	}
};


vector<vector<call>>* generate_call(vector<vector<int>> call_arrive_time) //根据到达时间数组产生的呼叫实体数组
{
	static vector<vector<call>> call_list;   //产生call数组
	int N_X = (call_arrive_time[0]).size();
	int N_Y = (call_arrive_time[1]).size();
	vector<call> call_X_list;
	vector<call> call_Y_list;
	for (int i = 0; i < N_X; i++)
	{
		call call;
		call.number = i;
		call.state = 0;  //空闲
		call.time_arrive = (call_arrive_time[0])[i];
		call.type = 'X';
		call_X_list.push_back(call);
	}
	for (int i = 0; i < N_Y; i++)
	{
		call call;
		call.number = i;
		call.state = 0;  //空闲
		call.time_arrive = (call_arrive_time[1])[i];
		call.type = 'Y';
		call_Y_list.push_back(call);
	}
	call_list.push_back(call_X_list);
	call_list.push_back(call_Y_list);
	return &call_list;
};


int generate_work_time(int average, int d)
{
	srand(time(NULL));
	unsigned long init[4] = { rand(), 0x534, 0x345, 0x456 }, length = 4;
	init_by_array(init, length);

	double a = genrand_real3();
	double b = genrand_real3();
	double k = average + (sqrt((-2) * log(a)) * cos(2 * PI * b)) * d;
	return (int)(k * 5);
}; //产生一个路由或接线员的处理时间


vector<vector<int>>* generate_call_arrive_time(double lambda1, double lambda2, int numX, int numY)
{//产生服从参数为lambda的指数分布的呼叫到达时间数组
	srand(time(NULL));
	unsigned long init[4] = { rand(), 0x534, 0x345, 0x456 }, length = 4;
	init_by_array(init, length);

	vector<int> arrive_time_X;
	vector<int> arrive_time_Y;
	static vector<vector<int>> arrive_time_all;
	int x_t = 0;
	int y_t = 0;
	for (int i = 0; i < numX; i++) //产生numX个样本
	{
		double u = genrand_real3();
		double x = -(1.0 / (double)lambda1) * log(u);
		x_t = x_t + (int)(x * 20);
		arrive_time_X.push_back(x_t);
	}
	for (int i = 0; i < numY; i++) //产生numY个样本
	{
		double u = genrand_real3();
		double x = -(1.0 / (double)lambda2) * log(u);
		y_t = y_t + (int)(x * 20);
		arrive_time_Y.push_back(y_t);
	}
	arrive_time_all.push_back(arrive_time_X);
	arrive_time_all.push_back(arrive_time_Y);
	return &arrive_time_all;
};

//把a和b两个call容器按照到达时间arrive_time按照一定规则排序
//1.a始终在前面
//2.b始终在前面
bool t_comp1(const call& a, const call& b) {
	return a.time_arrive < b.time_arrive;
}

bool t_comp2(const call& a, const call& b) {
	if (a.time_arrive == b.time_arrive) return a.type > b.type;
	else return a.time_arrive < b.time_arrive;
}

vector<call> merge(vector<call>* a, vector<call>* b, int mode)
{
	int numA = a->size();
	int numB = b->size();
	vector<call> temp;
	for (int i = 0; i < numA; i++) {
		temp.push_back((*a)[i]);
	}
	for (int j = 0; j < numB; j++) {
		temp.push_back((*b)[j]);
	}
	switch (mode) {
	case 1:
		sort(temp.begin(), temp.end(), t_comp1);
		break;
	case 2:
		sort(temp.begin(), temp.end(), t_comp2);
		break;
	default: break;
	}
	return temp;
};


void print_state(router* router, receiver* receiver1, receiver* receiver2, vector<vector<call>>* router_list, vector<vector<call>>* receiver_X_list, vector<vector<call>>* receiver_Y_list)
{
	vector<vector<string>> rc_router;
	for (int i = 0; i < N_router; i++)  //共N_router列，输出路由状态和路由队列
	{
		vector<string> rc_row_router;
		//第一列，输出序号
		string num = to_string(i + 1);
		rc_row_router.push_back(num);

		//第二列，输出路由状态
		if (router[i].state == 0)
			rc_row_router.push_back("空闲");
		else
		{
			string router_call_now;
			string type(1, router[i].call_now.type);
			string number = to_string(router[i].call_now.number + 1);
			router_call_now.append(type);
			router_call_now.append(number);
			rc_row_router.push_back(router_call_now);
		}

		//第三列，输出当前路由的路由队列
		if (!(*router_list)[i].empty())
		{
			string router_list_out = "";
			int N_call = (*router_list)[i].size();
			for (int j = 0; j < N_call; j++)
			{
				if (j != 0) router_list_out.append(",");
				string type(1, (*router_list)[i][j].type);
				string number = to_string((*router_list)[i][j].number + 1);
				router_list_out.append(type);
				router_list_out.append(number);
			}
			rc_row_router.push_back(router_list_out);
		}
		else
			rc_row_router.push_back("无");
		rc_router.push_back(rc_row_router);
	}
	vector<string> cc_router = { "序号","路由状态","路由队列" };  //列属性
	Table my_table_router(cc_router, rc_router);
	my_table_router.Print_Table();


	//共N_receriver_X列，输出接线员1状态和队列
	vector<vector<string>> rc_receiver_X;
	for (int i = 0; i < N_receiver_X; i++)
	{
		vector<string> rc_row_receiver_X;
		//第一列，输出序号
		string num = to_string(i + 1);
		rc_row_receiver_X.push_back(num);

		//第二列，输出状态
		if (receiver1[i].state == 0)
			rc_row_receiver_X.push_back("空闲");
		else
		{
			string receiver_X_call_now;
			string type(1, receiver1[i].call_now.type);
			string number = to_string(receiver1[i].call_now.number + 1);
			receiver_X_call_now.append(type);
			receiver_X_call_now.append(number);
			rc_row_receiver_X.push_back(receiver_X_call_now);
		}

		//第三列，输出当前接线员的队列
		if (!(*receiver_X_list)[i].empty())
		{
			string receiver_X_list_out;
			int N_call = (*receiver_X_list)[i].size();
			for (int j = 0; j < N_call; j++)
			{
				if (j != 0) receiver_X_list_out.append(",");
				string type(1, (*receiver_X_list)[i][j].type);
				string number = to_string((*receiver_X_list)[i][j].number + 1);
				receiver_X_list_out.append(type);
				receiver_X_list_out.append(number);
			}
			rc_row_receiver_X.push_back(receiver_X_list_out);
		}
		else
			rc_row_receiver_X.push_back("无");
		rc_receiver_X.push_back(rc_row_receiver_X);
	}
	vector<string> cc_receiver_X = { "序号","接线员1状态","接线员1队列" };  //列属性
	Table my_table_receiver_X(cc_receiver_X, rc_receiver_X);
	my_table_receiver_X.Print_Table();


	//共N_receriver_Y列，输出接线员1状态和队列
	vector<vector<string>> rc_receiver_Y;
	for (int i = 0; i < N_receiver_Y; i++)
	{
		vector<string> rc_row_receiver_Y;
		//第一列，输出序号
		string num = to_string(i + 1);
		rc_row_receiver_Y.push_back(num);

		//第二列，输出状态
		if (receiver2[i].state == 0)
			rc_row_receiver_Y.push_back("空闲");
		else
		{
			string receiver_Y_call_now;
			string type(1, receiver2[i].call_now.type);
			string number = to_string(receiver2[i].call_now.number + 1);
			receiver_Y_call_now.append(type);
			receiver_Y_call_now.append(number);
			rc_row_receiver_Y.push_back(receiver_Y_call_now);
		}

		//第三列，输出当前接线员的队列
		if (!(*receiver_Y_list)[i].empty())
		{
			string receiver_Y_list_out;
			int N_call = (*receiver_Y_list)[i].size();
			for (int j = 0; j < N_call; j++)
			{
				if (j != 0) receiver_Y_list_out.append(",");
				string type(1, (*receiver_Y_list)[i][j].type);
				string number = to_string((*receiver_Y_list)[i][j].number + 1);
				receiver_Y_list_out.append(type);
				receiver_Y_list_out.append(number);
			}
			rc_row_receiver_Y.push_back(receiver_Y_list_out);
		}
		else
			rc_row_receiver_Y.push_back("无");
		rc_receiver_Y.push_back(rc_row_receiver_Y);
	}
	vector<string> cc_receiver_Y = { "序号","接线员2状态","接线员2队列" };  //列属性
	Table my_table_receiver_Y(cc_receiver_Y, rc_receiver_Y);
	my_table_receiver_Y.Print_Table();
}


