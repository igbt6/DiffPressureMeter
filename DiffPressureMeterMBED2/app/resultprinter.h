#ifndef RESULTPRINTER_H
#define RESULTPRINTER_H

#include "system.h"


class ResultPrinter
{
public:
    ResultPrinter(PinName uartRX, PinNameuartTX);
    bool printResult();

private:
};

#endif