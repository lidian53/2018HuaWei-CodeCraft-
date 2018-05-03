#ifndef __ROUTE_H__
#define __ROUTE_H__

#include"predict.h"

//#include"train_interval_time.h"
#include"least_squre.h"
#include"lib_io.h"
#include<stdio.h>
#include<iostream>
#include <sstream>
#include<vector>
#include<string>
#include<cstring>

using namespace std;

int day_num(string &str)
{
	int endyear = 0, endmonth = 0, endday = 0;
	sscanf(str.c_str(), "%d-%d-%d", &endyear, &endmonth, &endday);
	int sum = 0;
	switch (endmonth) {
	case 1:sum = 0; break;
	case 2:sum = 31; break;
	case 3:sum = 59; break;
	case 4:sum = 90; break;
	case 5:sum = 120; break;
	case 6:sum = 151; break;
	case 7:sum = 181; break;
	case 8:sum = 212; break;
	case 9:sum = 243; break;
	case 10:sum = 273; break;
	case 11:sum = 304; break;
	case 12:sum = 334; break;
	//default:cout << "data error!" << endl; break;
	}
	sum = sum + endday;
	return sum;

}

int interval_day(string str1, string str2)
{
	return (day_num(str2) - day_num(str1))+1;
}
//你要完成的功能总入口
void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename,int &train_data_length)
{
	//filename = "output.txt";

	int CPU_NUM;
	int MemSize;
	int HardDisk;
	int VM_NUM;
	string Optimization;
	string BeginTime[2];
	string EndTime[2];

	istringstream is(info[0]);



	is >> CPU_NUM >> MemSize >> HardDisk;
	istringstream is2(info[2]);
	is2 >> VM_NUM;

	string * VMname = new string[VM_NUM];
	//int * VMcpu = new int[VM_NUM];
	//int * VMmem = new int[VM_NUM];
	int VMcpu[15] = { 0 };
	int VMmem[15] = { 0 };
	int jj = 0;
	for (int i = 3; i < 3 + VM_NUM; i++)
	{
		istringstream is3(info[i]);
		is3 >> VMname[jj] >> VMcpu[jj] >> VMmem[jj];
		jj++;
	}


	int z = 3 + VM_NUM;
	istringstream is4(info[z + 1]);
	is4 >> Optimization;
	istringstream is5(info[z + 3]);
	is5 >> BeginTime[0] >> BeginTime[1];
	istringstream is6(info[z + 4]);
	is6 >> EndTime[0] >> EndTime[1];
	
	//char *data_file = "da.txt";

	 
	//int train_data_length =read_file(data, MAX_DATA_NUM, data_file);
	//读入训练数据保存
	string *VMId = new string[train_data_length];
	string *VM_flavor = new string[train_data_length];
	string *VM_useday = new string[train_data_length];
	string *VM_usetime = new string[train_data_length];
	int line_num = 0;
	
	for (int i = 0; i <train_data_length; i++)
	{
		istringstream is_train(data[i]);
		is_train >> VMId[line_num] >> VM_flavor[line_num] >> VM_useday[line_num] >> VM_usetime[line_num];
		line_num++;
	}
	//string tem = VM_flavor[1];
	
	

	int count[15][100] = { 0 };
	int yindex = 0;
	for (int i = 0; i < VM_NUM; i++)
	{
		for (int j = 0; j < train_data_length; j++)
		{
			if ((VMname[i])== (VM_flavor[j]))
			{
				
				yindex =interval_day(VM_useday[0], VM_useday[j]);
				count[i][yindex]+=1;
			}
			
		}
	}
	int interday = interval_day(VM_useday[0], VM_useday[train_data_length - 1]);
	VM_Num_Point vmpoint[15][100];
	for (int i = 0; i < VM_NUM; i++)
	{
		for (int j = 0; j < interday; j++)
		{
			vmpoint[i][j].day = j;
			vmpoint[i][j].vm_num = count[i][j];
		}
	}
	// 需要输出的内容
	//char  *result_file2 = (char*)"17\n\n0 8 0 8";
	int predict_day = interval_day(BeginTime[0],EndTime[0]);
	LeastSquare* ls=new LeastSquare[VM_NUM];
	double lin_B[15] = {0.0};
	double lin_A[15]={0.0};
	int result[15][20] = {0};
	int vm_sum[15] = { 0 };
	int total_vm_sum = 0;
	//int vm_cpu_sum[15] = { 0 };
	//int vm_mem_sum[15] = { 0 };
	//double tes=ls->getA();
	for (int i = 0;i<VM_NUM ;i++) {
		for (int j =0; j <predict_day; j++) {
			ls[i].Linearreg(*(vmpoint + i), interday,10);
			lin_B[i] = ls[i].getB();
			lin_A[i] = ls[i].getA();
			result[i][j] = lin_A[i] * (interday+j+1) + lin_B[i];
			//if (result[i][j] <2) result[i][j] = 3;
			//if(1<result[i][j]&&result[i][j]<5) result[i][j]=3;
			vm_sum[i] += (result[i][j]+2);

		}
		//vm_cpu_sum[i] = VMcpu[i] * vm_sum[i];
		//vm_mem_sum[i] = (VMmem[i]/1024) * vm_sum[i];
		total_vm_sum += vm_sum[i];
	}

	
	/*
	double kk = ls->getA();
	double kk1 = ls->getB();
	unsigned int y;
	y = kk1 * 59 + kk;
	*/
	char  result_file[1000];
	
	string result_file1;
	string result_mac;
	string nn = "\n";
	string tt = "\t";
	//string num = "1";
	result_file1 = result_file1 + to_string(total_vm_sum) + nn;
	for (int i = 0; i < VM_NUM; i++)
	{
	result_file1 = result_file1 + VMname[i] + " " + to_string(vm_sum[i]) + nn;
	}
	//输出分配方案
	int Mac_sum=1;
	int Mac_cpu_current_sum=0;
	int Mac_mem_current_sum=0;
	int each_line_vmi_num[15]={0};//each line mac,No.i vm's number
	int current_bag_load_vm_start=0;
	
	for(int i=0;i<VM_NUM;i++)
	{
	    for(int j=0;j<vm_sum[i];j++)
		{
		    Mac_cpu_current_sum+=VMcpu[i];
		    Mac_mem_current_sum+=(VMmem[i]/1024);
		    if(i<=VM_NUM-1&&Mac_cpu_current_sum <=56 && Mac_mem_current_sum <=128)
			{
			    cout << "第" << Mac_sum << "个背包还没有装满" << endl;
			    ++each_line_vmi_num[i];
			    --total_vm_sum;
			    
			    cout << "第" << i + 1 << "种规格的虚拟机已经装了" << each_line_vmi_num[i] << "个" << endl;
			    cout<<"vm remain number is:"<<total_vm_sum<<endl;
			    
			}
		    
		    if(i<=VM_NUM-1&&(Mac_cpu_current_sum>56||Mac_mem_current_sum>128))
			{
			    j=-1;
			    cout << "第" << Mac_sum << "个背包已经装满" << endl;
			    result_mac = result_mac +to_string(Mac_sum);
			    Mac_sum++;

			    for(int cursta=current_bag_load_vm_start;cursta<=i;cursta++)
			    {
			        result_mac = result_mac + " " + VMname[cursta] + " " + to_string(each_line_vmi_num[cursta]);
			    }
			    Mac_cpu_current_sum=0;
			    Mac_mem_current_sum=0;
			    result_mac = result_mac + nn;
			    vm_sum[i]=vm_sum[i]-each_line_vmi_num[i];
			    cout << "第"<<i+1<<"种规格的虚拟机还剩下" << vm_sum[i]<<"个"<< endl;
			    each_line_vmi_num[i] = 0;
				
			    current_bag_load_vm_start= i;
			    cout << "第"<<Mac_sum<<"个背包从还有数量可分的"<< current_bag_load_vm_start+1<<"规格虚拟机继续装"<< endl;
			    
			}
			
		    if(i==VM_NUM-1&&total_vm_sum==0&&Mac_sum==1&&Mac_cpu_current_sum<=56&&Mac_mem_current_sum<=128)
		    {
		        current_bag_load_vm_start=0;
			result_mac = result_mac +to_string(Mac_sum);
			for(int cursta=current_bag_load_vm_start;cursta<=i;cursta++)
			{

			    result_mac = result_mac + " " + VMname[cursta] + " " + to_string(each_line_vmi_num[cursta]);
			}
		    }
			
		    if(i<=VM_NUM-1&&total_vm_sum==0&&Mac_sum>1&&Mac_cpu_current_sum<=56&&Mac_mem_current_sum<=128)
		    {
			result_mac=result_mac+to_string(Mac_sum);
			for(int cursta=current_bag_load_vm_start;cursta<=(VM_NUM-1);cursta++)
			{
			    result_mac = result_mac + " " + VMname[cursta] + " " + to_string(each_line_vmi_num[cursta]);
			}
		    }

		    
		}	
	}
	result_file1=result_file1+nn+to_string(Mac_sum)+nn+result_mac;
	//cout << result_file1 << endl;
	strcpy(result_file, result_file1.c_str());
	//strcpy(result_file, result_file1.c_str());
	
	//输出待预测的每一天里面，每种规格虚拟机的个数
	
	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(result_file, filename);
	
	
}



#endif

