#pragma once
#include<iostream>

class Record
{
public:
	char localtime[30] = { 0 };//��Ҿ�����Ϸʱ��
	char user[4] = { 'a','b','c','\0' };//����ǳ�
	int score;//��ҷ���
	int time_use;//�����Ϸʱ��

	Record() {

	}
	void getlocaltime(char *localtime);
	void Output_Recording(char* ch_buffer, DWORD* dwReadSize);
	void Write_Recording(char* wr_buffer, DWORD* dwWrittenSize, Record* record);
	

};
