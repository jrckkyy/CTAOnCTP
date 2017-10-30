// CTAOnCTP.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <functional>
#include <iostream>
#include "CTP\include\ThostFtdcMdApi.h"
#include "SimpleMdSpi.h"
#include "CTP\include\ThostFtdcTraderApi.h"
#include "SimpleTraderSpi.h"
#include "SimpleStrategy.h"

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
TThostFtdcPriceType gLimitPrice = 3671;                           // ���׼۸�

std::function<void(Tick *)> g_pfunMDEventHandlerOnTick = nullptr;




int main()
{

	// �˺�����
	std::cout << "�������˺ţ� ";
	scanf("%s", gInvesterID);
	std::cout << "���������룺 ";
	scanf("%s", gInvesterPassword);

	//���ײ���
	char strTradeFrontAddr[] = "tcp://180.168.146.187:10001";            // ģ�⽻��ǰ�õ�ַ

	CThostFtdcTraderApi *api = CThostFtdcTraderApi::CreateFtdcTraderApi();
	CSimpleTraderSpi simpleTraderSpi(api);
	api->RegisterSpi(&simpleTraderSpi);
	api->RegisterFront(strTradeFrontAddr);
	api->SubscribePrivateTopic(THOST_TERT_QUICK);
	api->SubscribePublicTopic(THOST_TERT_QUICK);
	api->Init();
	//api->Join();

	CSimpleStrategy simpleStrategy(&simpleTraderSpi,g_pTradeInstrumentID,0.0004);

	using namespace std::placeholders;
	g_pfunMDEventHandlerOnTick = std::bind(std::mem_fn(&CSimpleStrategy::OnTick), &simpleStrategy, _1); //bind event callback function

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

