#include "stdafx.h"
#include "SimpleStrategy.h"
#include <iostream>
#include <cstring>

enum POSITION_DIRECTION :int {
	SHORT_POSITION = -1,
	EMPTY_POSITION = 0,
	LONG_POSITION = 1
};

CSimpleStrategy::CSimpleStrategy(CSimpleTraderSpi *pTraderSpi,
	std::string strTradeInstrument,
	double dAtrRatio)
	: m_pTraderSpi(pTraderSpi)
	, m_strTradeInstrument(strTradeInstrument)
	, m_dAtrRatio(dAtrRatio)
	, m_dBreakHighPrice(1000000.0)
	, m_dBreakLowPrice(-1000000.0)
	, m_nPositionDriection(EMPTY_POSITION)
{
	memset(&m_CurrentTick, 0, sizeof(m_CurrentTick));
	memset(&m_PreviousTick, 0, sizeof(m_PreviousTick));
}


CSimpleStrategy::~CSimpleStrategy()
{
}


//Tick��ʵ�ֲ��Ե����߼�
//�Ϲ� = ���̼۸� * ��1 + AtrRatio��
//�¹� = ���̼۸� * ��1 - AtrRatio��	// AtrRatio Ϊ�������
//
//A = Tick��LastPrice
//�޲�λ��Aͻ���Ϲ졣�����
//�޲�λ��Aͻ���¹졣���ղ�
//
//��֣�Aͻ���Ϲ졣�޲���
//��֣�Aͻ���¹졣ƽ��֣����ղ�
//
//�ղ֣�Aͻ���Ϲ졣ƽ�ղ֣����ղ�
//�ղ֣�Aͻ���¹졣�޲���
void CSimpleStrategy::OnTick(Tick *pTick)
{
	//���˷ǽ��׺�Լ������
	if(strcmp(m_strTradeInstrument.c_str(),pTick->InstrumentID))
	{ 
		return;
	}

	m_PreviousTick = m_CurrentTick;
	m_CurrentTick = *pTick;

	if (m_PreviousTick.LastPrice < 0.001 || trading_day_not_same(&m_CurrentTick, &m_PreviousTick))
	{
		//����
		OnDayOpen();
	}

	if (EMPTY_POSITION == m_nPositionDriection)
	{
		if (m_CurrentTick.LastPrice > m_dBreakHighPrice)
		{
			//�޲�λ��Aͻ���Ϲ졣�����
			std::cout << "�ղ֣�Aͻ���Ϲ졣�����" << std::endl;
			
			m_nPositionDriection = LONG_POSITION;
			m_pTraderSpi->reqOrderInsert((char *)m_strTradeInstrument.c_str(),
										m_CurrentTick.AskPrice1, 
										1, 
										THOST_FTDC_DEN_Buy, //��
										THOST_FTDC_OF_Open);//��
		}

		if (m_CurrentTick.LastPrice < m_dBreakLowPrice)
		{
			//�޲�λ��ͻ���¹졣���ղ�
			std::cout << "�ղ֣�ͻ���¹졣���ղ�" << std::endl;

			m_nPositionDriection = SHORT_POSITION;
			m_pTraderSpi->reqOrderInsert((char *)m_strTradeInstrument.c_str(),
										m_CurrentTick.BidPrice1,
										1,
										THOST_FTDC_DEN_Sell,//��
										THOST_FTDC_OF_Open);//��
		}

	}
	
	if (LONG_POSITION == m_nPositionDriection)
	{
		if (m_CurrentTick.LastPrice < m_dBreakLowPrice)
		{
			//��֣�Aͻ���¹졣ƽ��֣����ղ�
			std::cout << "��֣�Aͻ���¹졣ƽ��֣����ղ�" << std::endl;

			m_nPositionDriection = SHORT_POSITION;
			//ƽ���
			m_pTraderSpi->reqOrderInsert((char *)m_strTradeInstrument.c_str(),
										m_CurrentTick.BidPrice1,
										1,
										THOST_FTDC_DEN_Sell,//��
										THOST_FTDC_OF_Close);//ƽ
			//���ղ�
			m_pTraderSpi->reqOrderInsert((char *)m_strTradeInstrument.c_str(),
										m_CurrentTick.BidPrice1,
										1,
										THOST_FTDC_DEN_Sell,//��
										THOST_FTDC_OF_Open);//��

		}
	}

	if (SHORT_POSITION == m_nPositionDriection)
	{
		if (m_CurrentTick.LastPrice > m_dBreakHighPrice)
		{
			//�ղ֣�Aͻ���Ϲ졣ƽ�ղ֣������
			std::cout << "�ղ֣�Aͻ���Ϲ졣ƽ�ղ֣����ղ�" << std::endl;

			m_nPositionDriection = LONG_POSITION;
			//ƽ�ղ�
			m_pTraderSpi->reqOrderInsert((char *)m_strTradeInstrument.c_str(),
				m_CurrentTick.AskPrice1,
				1,
				THOST_FTDC_D_Buy,//��
				THOST_FTDC_OF_Close);//ƽ
			//�����
			m_pTraderSpi->reqOrderInsert((char *)m_strTradeInstrument.c_str(),
				m_CurrentTick.AskPrice1,
				1,
				THOST_FTDC_D_Buy,//��
				THOST_FTDC_OF_Open);//��

		}

	}

}

void CSimpleStrategy::OnDayOpen()
{
	//�������¹�
	m_dBreakHighPrice = m_CurrentTick.LastPrice * (1.0 + m_dAtrRatio);
	std::cout << "�Ϲ�: " << m_dBreakHighPrice << std::endl;

	m_dBreakLowPrice = m_CurrentTick.LastPrice * (1.0 - m_dAtrRatio);
	std::cout << "�¹�: " << m_dBreakLowPrice << std::endl;
}

bool CSimpleStrategy::trading_day_not_same(Tick *pTick1, Tick *pTick2)
{
	if (strcmp(pTick1->TradingDay, pTick2->TradingDay) == 0)
	{
		return false;
	}

	return true;
}
