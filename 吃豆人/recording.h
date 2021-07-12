#pragma once
#include<iostream>

class Record
{
public:
	char localtime[30] = { 0 };//玩家具体游戏时间
	char user[4] = { 'a','b','c','\0' };//玩家昵称
	int score;//玩家分数
	int time_use;//玩家游戏时长

	Record() {

	}
	void getlocaltime(char *localtime);
	void Output_Recording(char* ch_buffer, DWORD* dwReadSize);
	void Write_Recording(char* wr_buffer, DWORD* dwWrittenSize, Record* record);
	

};
