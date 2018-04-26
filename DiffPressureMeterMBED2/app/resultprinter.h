#ifndef RESULTPRINTER_H
#define RESULTPRINTER_H

#include "system.h"
#include "AdafruitThermal.h"
#include "externalrtc.h"

using namespace rtctime;

typedef struct
{
    Time rtcTime;
    float temperature;
    float pressure;
} ResultData;


class ResultPrinter
{

public:
    ResultPrinter(PinName uartRxPin, PinName uartTxPin);
    bool printResult();
    void setResultData(const ResultData &resultData);
    static void runTest(ResultPrinter& resultPrinter);

protected:
    AdafruitThermal printer;
private:
    ResultData resultData;
};

#endif