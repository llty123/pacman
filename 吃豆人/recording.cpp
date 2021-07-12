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
	ch_buffer[*dwReadSize] = '\0';//��0������ļ�����
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
	CloseHandle(hFile);
	

}

void Record::Write_Recording(char* ch_buffer, DWORD* dwWrittenSize,Record *record)//����Ϸ��¼д���ļ�
{
	HANDLE hFile = CreateFile(_T("recording.txt"), GENERIC_WRITE | GENERIC_READ,
		0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);//���ļ����������ļ�ָ��
	if (hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(_T("CreateFile fail!/r/n"));
	}
	LONG IDistance = 0;//�����ļ���ȡλ�þ����ļ���ʼ��ƫ����
	DWORD dwPtr = SetFilePointer(hFile, IDistance, NULL, FILE_BEGIN);
	*dwWrittenSize = GetFileSize(hFile, NULL);//���recording���ļ���С
	BOOL bRet = ::ReadFile(hFile, ch_buffer, *dwWrittenSize, dwWrittenSize, NULL);//���ļ����ݶ�����������
	int i = 0,j=0;
	//for (; ch_buffer[i] != '\0'; i++);//�ҵ��ļ�����λ��
	for (; j < 19; j++)//��¼ʱ��
	{
		ch_buffer[i] = record->localtime[j];
		i++;
	}
	for (j = 0; j < 4; j++)//�����ĸ��ո�
	{
		ch_buffer[i] = ' ';
		i++;
	}
	for (j = 0; j < 3; j++)//�û���
	{
		ch_buffer[i] = record->user[j];
		i++;
	}
	for (j = 0; j < 4; j++)//�����ĸ��ո�
	{
		ch_buffer[i] = ' ';
		i++;
	}
	int tmp=record->score;
	for (j = 3; j >0; j--)//����÷�
	{
		ch_buffer[i]=(tmp / (int)pow(10, j - 1))+48;
		tmp = tmp % (int)pow(10, j - 1);
		i++;
	}
	for (j = 0; j < 7; j++)//�����߸��ո�
	{
		ch_buffer[i] = ' ';
		i++;
	}
	tmp = record->time_use;
	for (j = 4; j > 0; j--)//������Ϸʱ��
	{
		ch_buffer[i] = (tmp / (int)pow(10, j - 1))+48;
		tmp = tmp % (int)pow(10, j - 1);
		i++;
	}
	ch_buffer[i++] = '\r';
	ch_buffer[i++] = '\n';
	ch_buffer[i++] = '\0';//����ļ�����
	
	//*dwWrittenSize +=43;//���recording���ļ���С
	dwPtr = SetFilePointer(hFile, *dwWrittenSize, NULL, FILE_BEGIN);
	bRet = ::WriteFile(hFile, ch_buffer, 46, dwWrittenSize, NULL);

	FlushFileBuffers(hFile);//��д�ļ�������������ǿ��д����̡�
	CloseHandle(hFile);
}