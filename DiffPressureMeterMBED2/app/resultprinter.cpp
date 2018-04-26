#include "resultprinter.h"

//-----------------------------------------------------------------------------
ResultPrinter::ResultPrinter(PinName uartRxPin, PinName uartTxPin)
    : printer(uartRxPin, uartTxPin)
{
    printer.begin();   
}



//-----------------------------------------------------------------------------
void ResultPrinter::runTest(AdafruitThermal& printer)
{
    printer.setDefault();

    char *Testing_Foo = "Hello World!\n";
    printer.print(Testing_Foo);
  
    printer.justify('C');
    char *Text_Out11 = "normal\nline\nspacing\n";
    printer.print(Text_Out11);
    printer.setLineHeight(50);
    char *Text_Out12 = "Taller\nline\nspacing\n";
    printer.print(Text_Out12);
    printer.setLineHeight();
    printer.justify('L');

    printer.setSize('L');
    char *Text_Out8 = "Large\n";
    printer.print(Text_Out8);

    printer.setSize('M');
    char *Text_Out9 = "Medium\n";
    printer.print(Text_Out9);

    printer.setSize('S');
    char *Text_Out10 = "Small\n";
    printer.print(Text_Out10);

    printer.doubleHeightOn();
    char *Text_Out2 = "Double Height ON\n";
    printer.print(Text_Out2);
    printer.doubleHeightOff();

    printer.justify('R');
    char *Text_Out3 = "Right Justified\n";
    printer.print(Text_Out3);

    printer.justify('C');
    char *Text_Out4 = "Center Justified\n";
    printer.print(Text_Out4);

    printer.justify('L');
    char *Text_Out5 = "Left Justified\n";
    printer.print(Text_Out5);

    printer.boldOn();
    char *Text_Out6 = "Bold Text\n";
    printer.print(Text_Out6);
    printer.boldOff();
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