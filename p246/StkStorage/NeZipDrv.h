#pragma once
#include "NeZipDrv/Stockdrv.h"

#define		 WM_OEM_CLINENT				(WM_APP + 0X168)			//�ͻ��˿����Զ���������ݴ��ھ����

//�������ݴ��ھ������Ϣ�ţ��汾�ţ����ʷ��������IP��ַ�Ͷ˿ںţ��ص�����
typedef BOOL (WINAPI *_InitStockDrv)(HWND hWnd, UINT message, OEM_VERSION dataVersion, LPCSTR serverAddr, LPCSTR serverPort, OEM_STOCKDRV_CALLBACK callBack);
typedef BOOL (WINAPI *_AskData)(TCP_DATA_HEAD* pTcpHead);	//�����ʷ�ӿڳ���������ṩ����


class NeZipDrv
{
public:
	NeZipDrv(void);
	~NeZipDrv(void);
public:
	bool load(const char *dllfile);
	bool querydata(LPCSTR stkLabel, KLINETYPE klineType, bool sameDay = false, bool isSplit = false, char splitLeft = 1, bool askServer = false);
protected:
	HINSTANCE m_stockdrvDll;	//��̬��ģ����
	OEM_VERSION	m_dataVersion;
	_InitStockDrv InitStockDrv_;			//��ʼ�����
	_AskData	AskData_;						//����������ʷ��ṩ����
	static BOOL m_initialized;		//��ʼ�����
	UINT m_traceCount;	//ģ��ֱ�����������
	char		m_clientPackInfo[40];	//�ڲ�ʶ�����ݣ����û���д��������ʱԭ��ֱ�ӷ���

	static BOOL OnCallBack(TCP_DATA_HEAD *pTcpHead);

private:
	static void handle_daydata(TCP_DATA_HEAD *pTcpHead);
	static void handle_mindata(TCP_DATA_HEAD *pTcpHead,bool ismin1);

	static void handle_reportdata(TCP_DATA_HEAD *pTcpHead);
	static void handle_tick(TCP_DATA_HEAD *pTcpHead);
};
