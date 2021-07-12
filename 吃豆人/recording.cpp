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

	

}