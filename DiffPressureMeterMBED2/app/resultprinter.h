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
      Time rtcTime;
   }ResultData;
public:
    ResultPrinter(PinName uartRxPin, PinName uartTxPin);
    bool printResult();
    void setResultData(const ResultData &resultData);
    static void runTest(AdafruitThermal& printer);
private:
    AdafruitThermal printer;
    ResultData resultData;
};

#endif