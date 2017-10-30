#include "stdafx.h"
#include "SimpleMdSpi.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <functional>

using namespace std;
using Tick = CThostFtdcDepthMarketDataField;

//ȫ�ֱ�������
extern TThostFtdcBrokerIDType gBrokerID;         // ģ�⾭���̴���
extern TThostFtdcInvestorIDType gInvesterID;     // Ͷ�����˻���
extern TThostFtdcPasswordType gInvesterPassword; // Ͷ��������
extern std::function<void(Tick *)> g_pfunMDEventHandlerOnTick;

//ȫ�ֱ���
char *g_pInstrumentID[] = {"rb1801" };				   // �����Լ�����б�
int g_nInstrumentNum = 1;                                          // �����Լ��������


CSimpleMdSpi::~CSimpleMdSpi()
{
}

int CSimpleMdSpi::UserLogin(char *pBrokerID, char *pInverstID, char *pPassWord)
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, pBrokerID);
	strcpy(req.UserID, pInverstID);
	strcpy(req.Password, pPassWord);

	int ret = m_pMdUserApi->ReqUserLogin(&req, 0);

	if (!ret)
		cout << ">>>>>>���͵�¼����ɹ�" << endl;
	else
		cerr << "--->>>���͵�¼����ʧ��" << endl;

	return ret;
}

// ---- ctp_api�ص����� ---- //
// ���ӳɹ�Ӧ��
void CSimpleMdSpi::OnFrontConnected()
{
	cout << "=====�����������ӳɹ�=====" << endl;

	//�û���¼
	UserLogin(gBrokerID, gInvesterID, gInvesterPassword);
}

// �Ͽ�����֪ͨ
void CSimpleMdSpi::OnFrontDisconnected(int nReason)
{
	cerr << "=====�������ӶϿ�=====" << endl;
	cerr << "�����룺 " << nReason << endl;
}

// ������ʱ����
void CSimpleMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "=====����������ʱ=====" << endl;
	cerr << "���ϴ�����ʱ�䣺 " << nTimeLapse << endl;
}

// ��¼Ӧ��
void CSimpleMdSpi::OnRspUserLogin(
	CThostFtdcRspUserLoginField *pRspUserLogin,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		cout << "=====�˻���¼�ɹ�=====" << endl;
		cout << "�����գ� " << pRspUserLogin->TradingDay << endl;
		cout << "��¼ʱ�䣺 " << pRspUserLogin->LoginTime << endl;
		cout << "�����̣� " << pRspUserLogin->BrokerID << endl;
		cout << "�ʻ����� " << pRspUserLogin->UserID << endl;
		// ��ʼ��������
		int rt = m_pMdUserApi->SubscribeMarketData(g_pInstrumentID, g_nInstrumentNum);
		if (!rt)
			cout << ">>>>>>���Ͷ�����������ɹ�" << endl;
		else
			cerr << "--->>>���Ͷ�����������ʧ��" << endl;
	}
	else
		cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
}

// �ǳ�Ӧ��
void CSimpleMdSpi::OnRspUserLogout(
	CThostFtdcUserLogoutField *pUserLogout,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		cout << "=====�˻��ǳ��ɹ�=====" << endl;
		cout << "�����̣� " << pUserLogout->BrokerID << endl;
		cout << "�ʻ����� " << pUserLogout->UserID << endl;
	}
	else
		cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
}

// ����֪ͨ
void CSimpleMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (bResult)
		cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
}

// ��������Ӧ��
void CSimpleMdSpi::OnRspSubMarketData(
	CThostFtdcSpecificInstrumentField *pSpecificInstrument,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		cout << "=====��������ɹ�=====" << endl;
		cout << "��Լ���룺 " << pSpecificInstrument->InstrumentID << endl;
		cout << "��Լ����" << ","
			<< "����ʱ��" << ","
			<< "���¼�" << ","
			<< "�ɽ���" << ","
			<< "���һ" << ","
			<< "����һ" << ","
			<< "����һ" << ","
			<< "����һ" << ","
			<< "�ֲ���" << ","
			<< "������"
			<< endl;
	}
	else
		cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
}

// ȡ����������Ӧ��
void CSimpleMdSpi::OnRspUnSubMarketData(
	CThostFtdcSpecificInstrumentField *pSpecificInstrument,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		cout << "=====ȡ����������ɹ�=====" << endl;
		cout << "��Լ���룺 " << pSpecificInstrument->InstrumentID << endl;
	}
	else
		cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
}

// ����ѯ��Ӧ��
void CSimpleMdSpi::OnRspSubForQuoteRsp(
	CThostFtdcSpecificInstrumentField *pSpecificInstrument,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		cout << "=====����ѯ�۳ɹ�=====" << endl;
		cout << "��Լ���룺 " << pSpecificInstrument->InstrumentID << endl;
	}
	else
		cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
}

// ȡ������ѯ��Ӧ��
void CSimpleMdSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		cout << "=====ȡ������ѯ�۳ɹ�=====" << endl;
		cout << "��Լ���룺 " << pSpecificInstrument->InstrumentID << endl;
	}
	else
		cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
}

// ��������֪ͨ
void CSimpleMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	// ��ӡ����
	cout << "=====����������=====" << endl;
	cout << "�����գ� " << pDepthMarketData->TradingDay << endl;
	cout << "���������룺 " << pDepthMarketData->ExchangeID << endl;
	cout << "��Լ���룺 " << pDepthMarketData->InstrumentID << endl;
	cout << "��Լ�ڽ������Ĵ��룺 " << pDepthMarketData->ExchangeInstID << endl;
	cout << "���¼ۣ� " << pDepthMarketData->LastPrice << endl;
	cout << "������ " << pDepthMarketData->Volume << endl;
	
	if (g_pfunMDEventHandlerOnTick)
	{
		if (pDepthMarketData)
		{
			g_pfunMDEventHandlerOnTick(pDepthMarketData);
		}

	}



	// ȡ����������
	//int rt = m_pMdUserApi->UnSubscribeMarketData(g_pInstrumentID, g_nInstrumentNum);
	//if (!rt)
	//	cout << ">>>>>>����ȡ��������������ɹ�" << endl;
	//else
	//	cerr << "--->>>����ȡ��������������ʧ��" << endl;
}

// ѯ������֪ͨ
void CSimpleMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
	// ����ѯ�۽��
	cout << "=====���ѯ�۽��=====" << endl;
	cout << "�����գ� " << pForQuoteRsp->TradingDay << endl;
	cout << "���������룺 " << pForQuoteRsp->ExchangeID << endl;
	cout << "��Լ���룺 " << pForQuoteRsp->InstrumentID << endl;
	cout << "ѯ�۱�ţ� " << pForQuoteRsp->ForQuoteSysID << endl;
}
