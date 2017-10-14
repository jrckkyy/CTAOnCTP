// CTAOnCTP.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "CTP\include\ThostFtdcMdApi.h"
#include "SimpleMdSpi.h"

// ���ӿ�
#pragma comment (lib, "thostmduserapi.lib")
#pragma comment (lib, "thosttraderapi.lib")

// ---- ȫ�ֱ��� ---- //
// ��������
TThostFtdcBrokerIDType gBrokerID = "9999";                         // ģ�⾭���̴���
TThostFtdcInvestorIDType gInvesterID = "";                         // Ͷ�����˻���
TThostFtdcPasswordType gInvesterPassword = "";                     // Ͷ��������


int main()
{
	// �������
	char gMdFrontAddr[] = "tcp://180.168.146.187:10010";               // ģ������ǰ�õ�ַ

	CThostFtdcMdApi * pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	CSimpleMdSpi simpleMdSpi(pMdUserApi);
	pMdUserApi->RegisterSpi(&simpleMdSpi);
	pMdUserApi->RegisterFront(gMdFrontAddr);
	pMdUserApi->Init();
	pMdUserApi->Join();

	



    return 0;
}

