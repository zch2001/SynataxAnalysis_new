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

	//��ʼ��
	//�������е���ʱ��
	vector<vector<int>>* call_arrive_time = generate_call_arrive_time(0.5, 0.5, 5, 5);

	//��������ʵ��
	vector<vector<call>>* call_XY = generate_call(*call_arrive_time);

	//��XY����ʵ��ϲ����õ�һ��ʵ���modeΪ�ϲ�����
	vector<call> call_all = merge(&(*call_XY)[0], &(*call_XY)[1], 2); //��ʱ�Ѿ�����ʱ��˳�����к���

	router router[N_router];  //����·����ʵ��
	for (int i = 0; i < N_router; i++)
	{
		router[i].state = 0;
		router[i].call_now = { 0,'Z',0 ,0 };
	}

	receiver receiver1[N_receiver_X];  //��������Ա1ʵ��: ����X�����
	for (int i = 0; i < N_receiver_X; i++)
	{
		receiver1[i].state = 0;
		receiver1[i].call_now = { 0,'Z',0 ,0 };
	}

	receiver receiver2[N_receiver_Y];  //��������Ա2ʵ��: ����Y�����
	for (int i = 0; i < N_receiver_Y; i++)
	{
		receiver2[i].state = 0;
		receiver2[i].call_now = { 0,'Z',0 ,0 };
	}

	//��ʼ����
	int clock = 0;

	//��ʼ��
	vector<vector<call>> router_list(N_router);  //·�ɶ���
	vector<vector<call>> receiver_X_list(N_receiver_X);  //����Ա1����
	vector<vector<call>> receiver_Y_list(N_receiver_Y);  //����Ա2����

	//ͳ������
	int number_finish_X = 0;  //��ɵ�X���н�����
	int number_finish_Y = 0;  //��ɵ�Y���н�����
	int sum_wait_time = 0;  //���к��еȴ�����ʱ��

	//����B�¼��б�
	struct Node h; //���������ͷ��㣨��ֹ֮�����NULL��
	h.call = { 0,'Z',0 ,0 };
	h.next = NULL;
	h.NType = 0;
	h.Occurtime = 0;
	h.receiver_num = 0;
	h.router_num = 0;
	head = &h;

	struct Node* p;//�����ڵ�ָ��
	p = head;

	int n = call_all.size();  //������
	for (int i = 0; i < n; i++)  //ѭ���������
	{
		struct Node* s = (struct Node*)malloc(sizeof(struct Node));//����s��㣬�������ڴ�
		//��s��㸳ֵ
		s->call = call_all[i];
		if (call_all[i].type == 'X')	s->NType = 1;   //X���е��ﲢ����·�ɶ���
		if (call_all[i].type == 'Y')	s->NType = 2;   //Y���е��ﲢ����·�ɶ���
		s->Occurtime = call_all[i].time_arrive;  //����ʱ�伴�¼�����ʱ��
		s->router_num = 0;
		s->receiver_num = 0;
		//β�巨��������
		p->next = s;  //��ͷ���������ոճ�ʼ���Ľ��
		s->next = NULL;  //�ò���β����s����ָ��ָ��NULL
		p = s;  //p��㱣��ղŵ�s��㣬�Ա�֤pʼ��Ϊ��������һ���ڵ�
	}

	struct Node end;   //������������¼���β���
	end.call = { 0,'Z',0 ,0 };
	end.next = NULL;
	end.NType = 6;
	end.receiver_num = 0;
	end.router_num = 0;
	end.Occurtime = time_end; //�������

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

	cout << "-------��ʼ��-------" << endl;
	print_state(router, receiver1, receiver2, &router_list, &receiver_X_list, &receiver_Y_list);
	cout << endl;
	cout << endl;


	//��ʼB�׶�
	struct Node* s;
	s = head->next;  //ͷ�����һ�����
	while (s != NULL && s->Occurtime <= time_end)
	{
		clock = s->Occurtime;  //�ƽ������Ҫ�������¼���ʱ��
		cout << "��ǰʱ�䣺  " << clock << endl;
		//�γ����ʱ��Ҫ������B�¼��б�����ͷ�������¼��ķ���ʱ��
		while (s != NULL && s->Occurtime == clock)
		{
			if (s->NType == 1 || s->NType == 2)  //B1�¼���X��Y���е���·��
			{
				int k = 0; //kΪѡ���·��
				int min = 0;  //minΪ�ҵ��ĵȴ�������С��·��
				int flag = 0;
				for (int i = 0; i < N_router; i++)
				{
					if (router_list[i].size() <= router_list[min].size()) //�ȿ��ȴ����еĴ�С
					{
						min = i;
						if (router[i].state == 0)
						{
							flag = 1;  //�ٿ�������ȴ��������ڵȴ���·���Ƿ��ڹ����������У���k�������ˣ�
							k = i;
						}
					}
					if (flag == 0)		k = min;  //�������æ���ͻ�����æ�Ļ�����ѡһ��������С�İɣ�
				}  //�ҳ�k�Ƕ�������С�ģ�������ͬ�ģ�ȡ�����·��
				router_list[k].push_back(s->call);
			}
			if (s->NType == 3)  //B3�¼���·����ɹ��������X������Ա1���У�Y������Ա2����
			{
				router[s->router_num].state = 0;
				if (s->call.type == 'X')
				{
					int k = 0; //kΪѡ��Ľ���Ա1
					int min = 0;  //minΪ�ҵ��ĵȴ�������С�Ľ���Ա1
					int flag = 0;
					for (int i = 0; i < N_receiver_X; i++)
					{
						if (receiver_X_list[i].size() <= receiver_X_list[min].size())   //�ȿ��ȴ����еĴ�С
						{
							min = i;
							if (receiver1[i].state == 0)
							{
								flag = 1;  //�ٿ�������ȴ��������ڵȴ���·���Ƿ��ڹ����������У���k�������ˣ�
								k = i;
							}
						}
						if (flag == 0)		k = min;  //�������æ���ͻ�����æ�Ļ�����ѡһ��������С�İɣ�

					}  //�ҳ�k�Ƕ�������С�ģ�������ͬ�ģ�ȡ�����·��
					receiver_X_list[k].push_back(s->call);
				}
				if (s->call.type == 'Y')
				{
					int k = 0; //kΪѡ��Ľ���Ա2
					int min = 0;  //minΪ�ҵ��ĵȴ�������С�Ľ���Ա2
					int flag = 0;
					for (int i = 0; i < N_receiver_Y; i++)
					{
						if (receiver_Y_list[i].size() <= receiver_Y_list[min].size())
						{
							min = i;
							if (receiver2[i].state == 0)
							{
								flag = 1;  //�ٿ�������ȴ��������ڵȴ���·���Ƿ��ڹ����������У���k�������ˣ�
								k = i;
							}
						}
						if (flag == 0)		k = min;  //�������æ���ͻ�����æ�Ļ�����ѡһ��������С�İɣ�
					}  //�ҳ�k�Ƕ�������С�ģ�������ͬ�ģ�ȡ�����·��
					receiver_Y_list[k].push_back(s->call);
				}
			}
			if (s->NType == 4)  //B4�¼�,����Ա1��ɹ�������ɽ�������1��
			{
				receiver1[s->receiver_num].state = 0;
				number_finish_X++;
			}
			if (s->NType == 5)  //B5�¼�,����Ա2��ɹ�������ɽ�������1��
			{
				receiver2[s->receiver_num].state = 0;
				number_finish_Y++;
			}
			if (s->NType == 6)  //B6�¼����������
			{
				cout << "-------�������-------" << endl;
				if (number_finish_X + number_finish_Y == 0)
					cout << "ƽ��ÿ�����еȴ�ʱ��Ϊ��" << "δ���" << endl;
				else
				{
					double average_wait_time = (double)sum_wait_time / ((double)number_finish_X + (double)number_finish_Y);
					cout << "ƽ��ÿ�����еȴ�ʱ��Ϊ��" << average_wait_time << endl;
				}
				exit(1);
			}

			//�ı�״̬���ɾ�������㣬ʹsָ���µ�ͷ���
			head->next = s->next;
			free(s);
			s = head->next;
		}
		cout << "-------B�׶�-------" << endl;
		print_state(router, receiver1, receiver2, &router_list, &receiver_X_list, &receiver_Y_list);
		//��ʼִ��C�¼�

		//ÿ��·�ɰѵ绰��·�ɶ�����ȡ������ʼ��������C1
		for (int t = 0; t < N_router; t++)
		{
			if (!router_list[t].empty())
			{
				int work_time = generate_work_time(average_router, 1);
				int nr = router_list[t].size();
				for (int i = 0; i < nr; i++)
				{
					if (router[t].state == 0)  //���·�ɿ���
					{
						router[t].call_now = router_list[t][i];  //�Ѹú��н���·�ɴ���
						//�޸ĺ��С�·�ɵĲ���
						router_list[t][i].state = 1;
						router[t].state = 1;

						//������һ��B�¼�����B3
						struct Node* in = (struct Node*)malloc(sizeof(struct Node));
						in->NType = 3;
						in->call = router_list[t][i];
						in->Occurtime = clock + work_time;
						in->router_num = t;
						in->next = NULL;

						//����Ҫ����Ľ���occurtime�Ѹý���������ĺ���λ��
						struct Node* tmp = (struct Node*)malloc(sizeof(struct Node));//����tmp��㣬�������ڴ�
						tmp = head; // ָ��ָ��
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
						router_list[t].erase(router_list[t].begin());  //����Ѿ�������ˣ��Ͱ�������д�·�ɵȴ�������ɾ��
					}
					else //���·��æµ��,δ�ɹ����������call��Ҫ�����ڵȴ�������
					{
						sum_wait_time = sum_wait_time + work_time;
					}
				}
			}
		}


		//����Ա1�ѵ绰�ӽ���Ա1�Ķ�����ȡ������ʼ��������C2
		for (int t = 0; t < N_receiver_X; t++)
		{
			if (!receiver_X_list[t].empty())
			{
				int work_time = generate_work_time(average_receiver_X, 1);
				int nx = receiver_X_list[t].size();
				for (int i = 0; i < nx; i++)
				{
					if (receiver1[t].state == 0)  //�������Ա����
					{
						receiver1[t].call_now = receiver_X_list[t][i];  //�Ѹú��н�������Ա����
						//�޸Ľ���Ա�Ĳ���
						receiver_X_list[t][i].state = 1;
						receiver1[t].state = 1;

						//������һ��B�¼�����B4
						struct Node* in = (struct Node*)malloc(sizeof(struct Node));
						in->NType = 4;
						in->call = receiver_X_list[t][i];
						in->Occurtime = clock + work_time;
						in->receiver_num = t;
						in->next = NULL;
						//����Ҫ����Ľ���occurtime�Ѹý���������ĺ���λ��
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
						receiver_X_list[t].erase(receiver_X_list[t].begin());  //����Ѿ�������ˣ��Ͱ�������дӽ���Ա1�ȴ�������ɾ��
					}
					else //�������Աæµ��,δ�ɹ�����������ȴ����л�Ҫ�����ڵȴ�������
					{
						sum_wait_time = sum_wait_time + work_time;
					}
				}
			}
		}

		//����Ա2�ѵ绰�ӽ���Ա2�Ķ�����ȡ������ʼ��������C3
		for (int t = 0; t < N_receiver_Y; t++)
		{
			if (!receiver_Y_list[t].empty())
			{
				int work_time = generate_work_time(average_receiver_Y, 1);
				int ny = receiver_Y_list[t].size();
				for (int i = 0; i < ny; i++)
				{
					if (receiver2[t].state == 0)  //�������Ա2����
					{
						receiver2[t].call_now = receiver_Y_list[t][i];  //�Ѹú��н�������Ա2����
						//�޸ĺ��С�����Ա2�Ĳ���
						receiver_Y_list[t][i].state = 1;
						receiver2[t].state = 1;

						//������һ��B�¼�����B5
						struct Node* in = (struct Node*)malloc(sizeof(struct Node));
						in->NType = 5;
						in->call = receiver_Y_list[t][i];
						in->Occurtime = clock + work_time;
						in->receiver_num = t;
						in->next = NULL;
						//����Ҫ����Ľ���occurtime�Ѹý���������ĺ���λ��
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
						receiver_Y_list[t].erase(receiver_Y_list[t].begin());  //����Ѿ�������ˣ��Ͱ�������дӽ���Ա2�ȴ�������ɾ��
					}
					else //���·��æµ��,δ�ɹ����������call��Ҫ�����ڵȴ�������
					{
						sum_wait_time = sum_wait_time + work_time;
					}
				}
			}
		}
		cout << "-------C�׶�-------" << endl;
		print_state(router, receiver1, receiver2, &router_list, &receiver_X_list, &receiver_Y_list);

		cout << "��ǰ�����X���и�����" << number_finish_X << endl;
		cout << "��ǰ�����Y���и�����" << number_finish_Y << endl;
		cout << "��ǰ�ܵȴ�ʱ�䣺" << sum_wait_time << endl;
		cout << endl;
		cout << endl;

		s = head->next;
	}
}

