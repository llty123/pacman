#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "recording.h"

void Record::getlocaltime(char* localtime)//获取系统时间
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	sprintf(localtime, "%d-%02d-%02d %02d:%02d:%02d\t", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
}

void Record::Output_Recording(char* ch_buffer,DWORD *dwReadSize)//将游戏记录输出到窗口中
{
	
	HANDLE hFile = CreateFile(_T("recording.txt"), GENERIC_WRITE | GENERIC_READ,
		0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);//打开文件，并保存文件指针
	if (hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(_T("CreateFile fail!/r/n"));
	}
	LONG IDistance = 0;//设置文件读取位置距离文件起始的偏移量
	DWORD dwPtr = SetFilePointer(hFile, IDistance, NULL, FILE_BEGIN);
	*dwReadSize = GetFileSize(hFile, NULL);//获得recording的文件大小
	BOOL bRet=::ReadFile(hFile, ch_buffer, *dwReadSize, dwReadSize, NULL);//将文件内容读到缓冲区中
	ch_buffer[*dwReadSize] = '\0';//‘0’标记文件结束
	if (bRet)
	{
		//
		OutputDebugString(_T("ReadFile 读文件成功 /r/n"));
	}
	else
	{
		// 获取出错码。
		DWORD dwError = GetLastError();
		// 处理出错。
		TCHAR chErrorBuf[1024];
		wsprintf(chErrorBuf, _T("GetLastError()=%d/r/n"), dwError);
		OutputDebugString(chErrorBuf);
	}
	CloseHandle(hFile);
	

}

void Record::Write_Recording(char* ch_buffer, DWORD* dwWrittenSize,Record *record)//将游戏记录写入文件
{
	HANDLE hFile = CreateFile(_T("recording.txt"), GENERIC_WRITE | GENERIC_READ,
		0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);//打开文件，并保存文件指针
	if (hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(_T("CreateFile fail!/r/n"));
	}
	LONG IDistance = 0;//设置文件读取位置距离文件起始的偏移量
	DWORD dwPtr = SetFilePointer(hFile, IDistance, NULL, FILE_BEGIN);
	*dwWrittenSize = GetFileSize(hFile, NULL);//获得recording的文件大小
	BOOL bRet = ::ReadFile(hFile, ch_buffer, *dwWrittenSize, dwWrittenSize, NULL);//将文件内容读到缓冲区中
	int i = 0,j=0;
	//for (; ch_buffer[i] != '\0'; i++);//找到文件结束位置
	for (; j < 19; j++)//记录时间
	{
		ch_buffer[i] = record->localtime[j];
		i++;
	}
	for (j = 0; j < 4; j++)//输入四个空格
	{
		ch_buffer[i] = ' ';
		i++;
	}
	for (j = 0; j < 3; j++)//用户名
	{
		ch_buffer[i] = record->user[j];
		i++;
	}
	for (j = 0; j < 4; j++)//输入四个空格
	{
		ch_buffer[i] = ' ';
		i++;
	}
	int tmp=record->score;
	for (j = 3; j >0; j--)//输入得分
	{
		ch_buffer[i]=(tmp / (int)pow(10, j - 1))+48;
		tmp = tmp % (int)pow(10, j - 1);
		i++;
	}
	for (j = 0; j < 7; j++)//输入七个空格
	{
		ch_buffer[i] = ' ';
		i++;
	}
	tmp = record->time_use;
	for (j = 4; j > 0; j--)//输入游戏时长
	{
		ch_buffer[i] = (tmp / (int)pow(10, j - 1))+48;
		tmp = tmp % (int)pow(10, j - 1);
		i++;
	}
	ch_buffer[i++] = '\r';
	ch_buffer[i++] = '\n';
	ch_buffer[i++] = '\0';//标记文件结束
	
	//*dwWrittenSize +=43;//获得recording的文件大小
	dwPtr = SetFilePointer(hFile, *dwWrittenSize, NULL, FILE_BEGIN);
	bRet = ::WriteFile(hFile, ch_buffer, 46, dwWrittenSize, NULL);

	FlushFileBuffers(hFile);//将写文件缓冲区的数据强制写入磁盘。
	CloseHandle(hFile);
}