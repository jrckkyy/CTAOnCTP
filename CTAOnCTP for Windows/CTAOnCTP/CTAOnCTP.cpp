// CTAOnCTP.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "CTP\include\ThostFtdcMdApi.h"
#include "SimpleMdSpi.h"
#include "CTP\include\ThostFtdcTraderApi.h"
#include "SimpleTraderSpi.h"

// ���ӿ�
#pragma comment (lib, "CTP/lib/thostmduserapi.lib")
#pragma comment (lib, "CTP/lib/thosttraderapi.lib")

// ---- ȫ�ֱ��� ---- //
// ��������
TThostFtdcBrokerIDType gBrokerID = "9999";                         // ģ�⾭���̴���
TThostFtdcInvestorIDType gInvesterID = "044390";                         // Ͷ�����˻���
TThostFtdcPasswordType gInvesterPassword = "123456";                     // Ͷ��������


// ���ײ���
TThostFtdcInstrumentIDType g_pTradeInstrumentID = "rb1801";        // �����׵ĺ�Լ����
TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Sell;       // ��������
TThostFtdcPriceType gLimitPrice = 3671;                           // ���׼۸�


int main()
{
	// �������
	char gMdFrontAddr[] = "tcp://180.168.146.187:10010";               // ģ������ǰ�õ�ַ

	CThostFtdcMdApi * pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	CSimpleMdSpi simpleMdSpi(pMdUserApi);
	pMdUserApi->RegisterSpi(&simpleMdSpi);
	pMdUserApi->RegisterFront(gMdFrontAddr);
	pMdUserApi->Init();
	//pMdUserApi->Join();

	//���ײ���
	char strTradeFrontAddr[] = "tcp://180.168.146.187:10001";            // ģ�⽻��ǰ�õ�ַ

	CThostFtdcTraderApi *api = CThostFtdcTraderApi::CreateFtdcTraderApi();
	CSimpleTraderSpi simpleTraderSpi(api);
	api->RegisterSpi(&simpleTraderSpi);
	api->RegisterFront(strTradeFrontAddr);
	api->SubscribePrivateTopic(THOST_TERT_QUICK);
	api->SubscribePublicTopic(THOST_TERT_QUICK);
	api->Init();
	api->Join();

    return 0;
}

