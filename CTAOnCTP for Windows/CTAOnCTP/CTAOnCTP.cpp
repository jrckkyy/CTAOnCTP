// CTAOnCTP.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "CTP\include\ThostFtdcMdApi.h"
#include "SimpleMdSpi.h"

// ���ӿ�
#pragma comment (lib, "CTP/lib/thostmduserapi.lib")
#pragma comment (lib, "CTP/lib/thosttraderapi.lib")

// ---- ȫ�ֱ��� ---- //
// ��������
TThostFtdcBrokerIDType gBrokerID = "9999";                         // ģ�⾭���̴���
TThostFtdcInvestorIDType gInvesterID = "";                         // Ͷ�����˻���
TThostFtdcPasswordType gInvesterPassword = "";                     // Ͷ��������


// ���ײ���
TThostFtdcInstrumentIDType g_pTradeInstrumentID = "rb1801";        // �����׵ĺ�Լ����
TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Sell;       // ��������
TThostFtdcPriceType gLimitPrice = 3571;                           // ���׼۸�


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

