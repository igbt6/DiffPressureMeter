#include "resultprinter.h"

//-----------------------------------------------------------------------------
ResultPrinter::ResultPrinter(PinName uartRxPin, PinName uartTxPin)
    : printer(uartRxPin, uartTxPin)
{
    printer.begin();   
}

//-----------------------------------------------------------------------------
bool ResultPrinter::printResult()
{
    printer.setDefault();
    char buf[100];

    char *header = "**********************\n";
    printer.justify('C');
    printer.print(header);
    
    printer.setSize('L');
    char *measResult = "-Wyniki pomiaru-\n\n";
    printer.print(measResult);

    // date and time
    printer.setSize('S');
    printer.justify('L');
    memset(buf, '\0', sizeof(buf));
    sprintf(buf, "Data:  %s\n", ExternalRTC::getDateString(resultData.rtcTime));
    printer.print(buf);
    memset(buf, '\0', sizeof(buf));
    sprintf(buf, "Czas:  %s\n", ExternalRTC::getTimeString(resultData.rtcTime));
    printer.print(buf);
    
    // temperature
    memset(buf, '\0', sizeof(buf));
    sprintf(buf, "Temperatura:  %3.1f [C]\n", resultData.temperature);
    printer.print(buf);

    // pressure
    printer.setSize('M');
    memset(buf, '\0', sizeof(buf));
    sprintf(buf, "Cisnienie:  %3.3f \n", resultData.pressure);
    printer.underlineOn();
    printer.print(buf);
    printer.underlineOff();
    printer.setSize('S');
    memset(buf, '\0', sizeof(buf));
    sprintf(buf, "Jednostka:  [kPa]\n\n");
    printer.print(buf);

    printer.justify('C');
    printer.print(header);

    printer.offline();
    return true;
}

//-----------------------------------------------------------------------------
void ResultPrinter::setResultData(const ResultData &resultData)
{
    this->resultData = resultData;
}


//-----------------------------------------------------------------------------
// TEST METHOD
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
    resultPrinter.printer.offline();
}
//-----------------------------------------------------------------------------