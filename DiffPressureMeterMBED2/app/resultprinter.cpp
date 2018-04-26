#include "resultprinter.h"

//-----------------------------------------------------------------------------
ResultPrinter::ResultPrinter(PinName uartRxPin, PinName uartTxPin)
    : printer(uartRxPin, uartTxPin)
{
    printer.begin();   
}



//-----------------------------------------------------------------------------
void ResultPrinter::runTest(ResultPrinter& resultPrinter)
{
    resultPrinter.printer.setDefault();

    char *Testing_Foo = "Hello World!\n";
    resultPrinter.printer.print(Testing_Foo);
  
    resultPrinter.printer.justify('C');
    char *Text_Out11 = "normal\nline\nspacing\n";
    resultPrinter.printer.print(Text_Out11);
    resultPrinter.printer.setLineHeight(50);
    char *Text_Out12 = "Taller\nline\nspacing\n";
    resultPrinter.printer.print(Text_Out12);
    resultPrinter.printer.setLineHeight();
    resultPrinter.printer.justify('L');

    resultPrinter.printer.setSize('L');
    char *Text_Out8 = "Large\n";
    resultPrinter.printer.print(Text_Out8);

    resultPrinter.printer.setSize('M');
    char *Text_Out9 = "Medium\n";
    resultPrinter.printer.print(Text_Out9);

    resultPrinter.printer.setSize('S');
    char *Text_Out10 = "Small\n";
    resultPrinter.printer.print(Text_Out10);

    resultPrinter.printer.doubleHeightOn();
    char *Text_Out2 = "Double Height ON\n";
    resultPrinter.printer.print(Text_Out2);
    resultPrinter.printer.doubleHeightOff();

    resultPrinter.printer.justify('R');
    char *Text_Out3 = "Right Justified\n";
    resultPrinter.printer.print(Text_Out3);

    resultPrinter.printer.justify('C');
    char *Text_Out4 = "Center Justified\n";
    resultPrinter.printer.print(Text_Out4);

    resultPrinter.printer.justify('L');
    char *Text_Out5 = "Left Justified\n";
    resultPrinter.printer.print(Text_Out5);

    resultPrinter.printer.boldOn();
    char *Text_Out6 = "Bold Text\n";
    resultPrinter.printer.print(Text_Out6);
    resultPrinter.printer.boldOff();
}
//-----------------------------------------------------------------------------
bool ResultPrinter::printResult()
{
   return true;
}

//-----------------------------------------------------------------------------
void ResultPrinter::setResultData(const ResultData &resultData)
{
      this->resultData = resultData;
}
//-----------------------------------------------------------------------------