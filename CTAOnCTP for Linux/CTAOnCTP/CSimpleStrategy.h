#pragma once
#include <vector>
#include <string>
#include "CSimpleTraderSpi.h"
#include "CSimpleMdSpi.h"

using Tick = CThostFtdcDepthMarketDataField;

class CSimpleStrategy
{
public:
    CSimpleStrategy(CSimpleTraderSpi *pTraderSpi,
                    std::string strTradeInstrument,
                    double dAtrRatio);
    virtual ~CSimpleStrategy();
    void OnTick(Tick *pTick);
    void OnDayOpen();
//inner function 内部函数 小写
private:
     bool trading_day_not_same(Tick *pTick1,Tick *pTick2);
private:
    CSimpleTraderSpi *m_pTraderSpi;
    Tick m_CurrentTick;
    Tick m_PreviousTick;
    std::vector<Tick> m_vecTicks;
    double m_dBreakHighPrice;
    double m_dBreakLowPrice;
    double m_dAtrRatio;
    double m_nPositionDriection;
    std::string m_strTradeInstrument;   //jiaoyi heyue

};

