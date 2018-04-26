#ifndef RESULTPRINTER_H
#define RESULTPRINTER_H

#include "system.h"
#include "AdafruitThermal.h"
#include "externalrtc.h"

class ResultPrinter
{
public:
   typedef struct
   {
        rtctime::Time rtcTime;
        float temperature;
        float pressure;
   } ResultData;

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