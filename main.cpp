#include "main.h"
#include <math.h>
#include <iostream>
#include <algorithm>
using namespace std;

const int average_router = 1;
const int average_receiver_X = 4;
const int average_receiver_Y = 7;

int main()
{
	int time_end = 500;

	//初始化
	//创建呼叫到达时间
	vector<vector<int>>* call_arrive_time = generate_call_arrive_time(0.5, 0.5, 5, 5);

	//创建呼叫实体
	vector<vector<call>>* call_XY = generate_call(*call_arrive_time);

	//把XY呼叫实体合并，得到一个实体表，mode为合并规则
	vector<call> call_all = merge(&(*call_XY)[0], &(*call_XY)[1], 2); //这时已经按照时间顺序排列好了

	router router[N_router];  //创建路由器实体
	for (int i = 0; i < N_router; i++)
	{
		router[i].state = 0;
		router[i].call_now = { 0,'Z',0 ,0 };
	}

	receiver receiver1[N_receiver_X];  //创建接线员1实体: 负责X类呼叫
	for (int i = 0; i < N_receiver_X; i++)
	{
		receiver1[i].state = 0;
		receiver1[i].call_now = { 0,'Z',0 ,0 };
	}

	receiver receiver2[N_receiver_Y];  //创建接线员2实体: 负责Y类呼叫
	for (int i = 0; i < N_receiver_Y; i++)
	{
		receiver2[i].state = 0;
		receiver2[i].call_now = { 0,'Z',0 ,0 };
	}

	//开始仿真
	int clock = 0;

	//初始化
	vector<vector<call>> router_list(N_router);  //路由队列
	vector<vector<call>> receiver_X_list(N_receiver_X);  //接线员1队列
	vector<vector<call>> receiver_Y_list(N_receiver_Y);  //接线员2队列

	//统计数据
	int number_finish_X = 0;  //完成的X呼叫接线数
	int number_finish_Y = 0;  //完成的Y呼叫接线数
	int sum_wait_time = 0;  //所有呼叫等待的总时间

	//创建B事件列表
	struct Node h; //创建链表的头结点（防止之后出现NULL）
	h.call = { 0,'Z',0 ,0 };
	h.next = NULL;
	h.NType = 0;
	h.Occurtime = 0;
	h.receiver_num = 0;
	h.router_num = 0;
	head = &h;

	struct Node* p;//创建节点指针
	p = head;

	int n = call_all.size();  //链表长度
	for (int i = 0; i < n; i++)  //循环创建结点
	{
		struct Node* s = (struct Node*)malloc(sizeof(struct Node));//创建s结点，并分配内存
		//给s结点赋值
		s->call = call_all[i];
		if (call_all[i].type == 'X')	s->NType = 1;   //X呼叫到达并进入路由队列
		if (call_all[i].type == 'Y')	s->NType = 2;   //Y呼叫到达并进入路由队列
		s->Occurtime = call_all[i].time_arrive;  //到达时间即事件发生时间
		s->router_num = 0;
		s->receiver_num = 0;
		//尾插法建立链表
		p->next = s;  //在头结点后面插入刚刚初始化的结点
		s->next = NULL;  //让插在尾部的s结点的指针指向NULL
		p = s;  //p结点保存刚才的s结点，以保证p始终为链表的最后一个节点
	}

	struct Node end;   //创建仿真结束事件的尾结点
	end.call = { 0,'Z',0 ,0 };
	end.next = NULL;
	end.NType = 6;
	end.receiver_num = 0;
	end.router_num = 0;
	end.Occurtime = time_end; //仿真结束

	struct Node* tmp;
	tmp = head;
	while (tmp != NULL)
	{
		if (tmp->next == NULL || tmp->next->Occurtime > end.Occurtime)
		{
			end.next = tmp->next;
			tmp->next = &end;
			break;
		}
		tmp = tmp->next;
	}

	cout << "-------初始化-------" << endl;
	print_state(router, receiver1, receiver2, &router_list, &receiver_X_list, &receiver_Y_list);
	cout << endl;
	cout << endl;


	//开始B阶段
	struct Node* s;
	s = head->next;  //头结点后第一个结点
	while (s != NULL && s->Occurtime <= time_end)
	{
		clock = s->Occurtime;  //推进到最近要发生的事件的时间
		cout << "当前时间：  " << clock << endl;
		//形成这个时刻要发生的B事件列表，即从头检查各个事件的发生时间
		while (s != NULL && s->Occurtime == clock)
		{
			if (s->NType == 1 || s->NType == 2)  //B1事件，X、Y呼叫到达路由
			{
				int k = 0; //k为选择的路由
				int min = 0;  //min为找到的等待队列最小的路由
				int flag = 0;
				for (int i = 0; i < N_router; i++)
				{
					if (router_list[i].size() <= router_list[min].size()) //先看等待队列的大小
					{
						min = i;
						if (router[i].state == 0)
						{
							flag = 1;  //再看看这个等待队列正在等待的路由是否在工作，若空闲，那k就是它了！
							k = i;
						}
					}
					if (flag == 0)		k = min;  //如果都在忙，就还是在忙的基础上选一个队列最小的吧！
				}  //找出k是队列数最小的，若有相同的，取靠后的路由
				router_list[k].push_back(s->call);
			}
			if (s->NType == 3)  //B3事件，路由完成工作并输出X到接线员1队列，Y到接线员2队列
			{
				router[s->router_num].state = 0;
				if (s->call.type == 'X')
				{
					int k = 0; //k为选择的接线员1
					int min = 0;  //min为找到的等待队列最小的接线员1
					int flag = 0;
					for (int i = 0; i < N_receiver_X; i++)
					{
						if (receiver_X_list[i].size() <= receiver_X_list[min].size())   //先看等待队列的大小
						{
							min = i;
							if (receiver1[i].state == 0)
							{
								flag = 1;  //再看看这个等待队列正在等待的路由是否在工作，若空闲，那k就是它了！
								k = i;
							}
						}
						if (flag == 0)		k = min;  //如果都在忙，就还是在忙的基础上选一个队列最小的吧！

					}  //找出k是队列数最小的，若有相同的，取靠后的路由
					receiver_X_list[k].push_back(s->call);
				}
				if (s->call.type == 'Y')
				{
					int k = 0; //k为选择的接线员2
					int min = 0;  //min为找到的等待队列最小的接线员2
					int flag = 0;
					for (int i = 0; i < N_receiver_Y; i++)
					{
						if (receiver_Y_list[i].size() <= receiver_Y_list[min].size())
						{
							min = i;
							if (receiver2[i].state == 0)
							{
								flag = 1;  //再看看这个等待队列正在等待的路由是否在工作，若空闲，那k就是它了！
								k = i;
							}
						}
						if (flag == 0)		k = min;  //如果都在忙，就还是在忙的基础上选一个队列最小的吧！
					}  //找出k是队列数最小的，若有相同的，取靠后的路由
					receiver_Y_list[k].push_back(s->call);
				}
			}
			if (s->NType == 4)  //B4事件,接线员1完成工作（完成接线数加1）
			{
				receiver1[s->receiver_num].state = 0;
				number_finish_X++;
			}
			if (s->NType == 5)  //B5事件,接线员2完成工作（完成接线数加1）
			{
				receiver2[s->receiver_num].state = 0;
				number_finish_Y++;
			}
			if (s->NType == 6)  //B6事件，仿真结束
			{
				cout << "-------仿真结束-------" << endl;
				if (number_finish_X + number_finish_Y == 0)
					cout << "平均每个呼叫等待时间为：" << "未完成" << endl;
				else
				{
					double average_wait_time = (double)sum_wait_time / ((double)number_finish_X + (double)number_finish_Y);
					cout << "平均每个呼叫等待时间为：" << average_wait_time << endl;
				}
				exit(1);
			}

			//改变状态后就删除这个结点，使s指向新的头结点
			head->next = s->next;
			free(s);
			s = head->next;
		}
		cout << "-------B阶段-------" << endl;
		print_state(router, receiver1, receiver2, &router_list, &receiver_X_list, &receiver_Y_list);
		//开始执行C事件

		//每个路由把电话从路由队列中取出并开始工作——C1
		for (int t = 0; t < N_router; t++)
		{
			if (!router_list[t].empty())
			{
				int work_time = generate_work_time(average_router, 1);
				int nr = router_list[t].size();
				for (int i = 0; i < nr; i++)
				{
					if (router[t].state == 0)  //如果路由空闲
					{
						router[t].call_now = router_list[t][i];  //把该呼叫交给路由处理
						//修改呼叫、路由的参数
						router_list[t][i].state = 1;
						router[t].state = 1;

						//产生下一个B事件——B3
						struct Node* in = (struct Node*)malloc(sizeof(struct Node));
						in->NType = 3;
						in->call = router_list[t][i];
						in->Occurtime = clock + work_time;
						in->router_num = t;
						in->next = NULL;

						//根据要插入的结点的occurtime把该结点插入链表的合适位置
						struct Node* tmp = (struct Node*)malloc(sizeof(struct Node));//创建tmp结点，并分配内存
						tmp = head; // 指向指针
						while (tmp != NULL)
						{
							if (tmp->next == NULL || tmp->next->Occurtime > in->Occurtime)
							{
								in->next = tmp->next;
								tmp->next = in;
								break;
							}
							tmp = tmp->next;
						}
						router_list[t].erase(router_list[t].begin());  //如果已经处理过了，就把这个呼叫从路由等待队列中删除
					}
					else //如果路由忙碌中,未成功处理，则这个call还要继续在等待队列中
					{
						sum_wait_time = sum_wait_time + work_time;
					}
				}
			}
		}


		//接线员1把电话从接线员1的队列中取出并开始工作——C2
		for (int t = 0; t < N_receiver_X; t++)
		{
			if (!receiver_X_list[t].empty())
			{
				int work_time = generate_work_time(average_receiver_X, 1);
				int nx = receiver_X_list[t].size();
				for (int i = 0; i < nx; i++)
				{
					if (receiver1[t].state == 0)  //如果接线员空闲
					{
						receiver1[t].call_now = receiver_X_list[t][i];  //把该呼叫交给接线员处理
						//修改接线员的参数
						receiver_X_list[t][i].state = 1;
						receiver1[t].state = 1;

						//产生下一个B事件——B4
						struct Node* in = (struct Node*)malloc(sizeof(struct Node));
						in->NType = 4;
						in->call = receiver_X_list[t][i];
						in->Occurtime = clock + work_time;
						in->receiver_num = t;
						in->next = NULL;
						//根据要插入的结点的occurtime把该结点插入链表的合适位置
						struct Node* tmp = (struct Node*)malloc(sizeof(struct Node));
						tmp = head;
						while (tmp != NULL)
						{
							if (tmp->next == NULL || tmp->next->Occurtime > in->Occurtime)
							{
								in->next = tmp->next;
								tmp->next = in;
								break;
							}
							tmp = tmp->next;
						}
						receiver_X_list[t].erase(receiver_X_list[t].begin());  //如果已经处理过了，就把这个呼叫从接线员1等待队列中删除
					}
					else //如果接线员忙碌中,未成功处理，则这个等待队列还要继续在等待队列中
					{
						sum_wait_time = sum_wait_time + work_time;
					}
				}
			}
		}

		//接线员2把电话从接线员2的队列中取出并开始工作——C3
		for (int t = 0; t < N_receiver_Y; t++)
		{
			if (!receiver_Y_list[t].empty())
			{
				int work_time = generate_work_time(average_receiver_Y, 1);
				int ny = receiver_Y_list[t].size();
				for (int i = 0; i < ny; i++)
				{
					if (receiver2[t].state == 0)  //如果接线员2空闲
					{
						receiver2[t].call_now = receiver_Y_list[t][i];  //把该呼叫交给接线员2处理
						//修改呼叫、接线员2的参数
						receiver_Y_list[t][i].state = 1;
						receiver2[t].state = 1;

						//产生下一个B事件——B5
						struct Node* in = (struct Node*)malloc(sizeof(struct Node));
						in->NType = 5;
						in->call = receiver_Y_list[t][i];
						in->Occurtime = clock + work_time;
						in->receiver_num = t;
						in->next = NULL;
						//根据要插入的结点的occurtime把该结点插入链表的合适位置
						struct Node* tmp = (struct Node*)malloc(sizeof(struct Node));
						tmp = head;
						while (tmp != NULL)
						{
							if (tmp->next == NULL || tmp->next->Occurtime > in->Occurtime)
							{
								in->next = tmp->next;
								tmp->next = in;
								break;
							}
							tmp = tmp->next;
						}
						receiver_Y_list[t].erase(receiver_Y_list[t].begin());  //如果已经处理过了，就把这个呼叫从接线员2等待队列中删除
					}
					else //如果路由忙碌中,未成功处理，则这个call还要继续在等待队列中
					{
						sum_wait_time = sum_wait_time + work_time;
					}
				}
			}
		}
		cout << "-------C阶段-------" << endl;
		print_state(router, receiver1, receiver2, &router_list, &receiver_X_list, &receiver_Y_list);

		cout << "当前已完成X呼叫个数：" << number_finish_X << endl;
		cout << "当前已完成Y呼叫个数：" << number_finish_Y << endl;
		cout << "当前总等待时间：" << sum_wait_time << endl;
		cout << endl;
		cout << endl;

		s = head->next;
	}
}

