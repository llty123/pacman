#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "recording.h"

void Record::getlocaltime(char* localtime)//��ȡϵͳʱ��
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	sprintf(localtime, "%d-%02d-%02d %02d:%02d:%02d\t", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
}

void Record::Output_Recording(char* ch_buffer,DWORD *dwReadSize)//����Ϸ��¼�����������
{
	
	HANDLE hFile = CreateFile(_T("recording.txt"), GENERIC_WRITE | GENERIC_READ,
		0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);//���ļ����������ļ�ָ��
	if (hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(_T("CreateFile fail!/r/n"));
	}
	LONG IDistance = 0;//�����ļ���ȡλ�þ����ļ���ʼ��ƫ����
	DWORD dwPtr = SetFilePointer(hFile, IDistance, NULL, FILE_BEGIN);
	*dwReadSize = GetFileSize(hFile, NULL);//���recording���ļ���С
	BOOL bRet=::ReadFile(hFile, ch_buffer, *dwReadSize, dwReadSize, NULL);//���ļ����ݶ�����������
	if (bRet)
	{
		//
		OutputDebugString(_T("ReadFile ���ļ��ɹ� /r/n"));
	}
	else
	{
		// ��ȡ�����롣
		DWORD dwError = GetLastError();
		// �������
		TCHAR chErrorBuf[1024];
		wsprintf(chErrorBuf, _T("GetLastError()=%d/r/n"), dwError);
		OutputDebugString(chErrorBuf);
	}

	

}