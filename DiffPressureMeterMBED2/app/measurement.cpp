#include "measurement.h"
#include "settingsmemory.h"
#include "UTFT_Buttons.h"
#include "externalrtc.h"

using namespace rtctime;

// fonts for linker
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
extern uint8_t Dingbats1_XL[];


//-----------------------------------------------------------------------------
Measurement::Measurement(LUTFT &tft, LUTouch &touch)
    : tft(tft)
    , touch(touch)
    , pressureSensor()
    , relayPin(RELAY_OUT_PIN, HIGH)
    , resultPrinter(CSN_A2L_RX_PIN, CSN_A2L_TX_PIN)
    , pressureResultAverage(0.0f)
{
}

//-----------------------------------------------------------------------------
void Measurement::run()
{
    if (start())
    {
        return;
    }
    if (measure())
    {
        return;
    }
    postProcess();
}

//-----------------------------------------------------------------------------
bool Measurement::start()
{
    UTFT_Buttons  buttons(&tft, &touch);
    touch.setPrecision(PREC_MEDIUM);
    buttons.setTextFont(BigFont);
    buttons.setSymbolFont(Dingbats1_XL);

    int butBack, pressed_button;
    buttons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_RED, VGA_BLUE);
    butBack = buttons.addButton(  0, 199, 100,  40, "<-");

    tft.clrScr();
    tft.setColor(VGA_WHITE);
    tft.setBackColor(VGA_BLACK);
    tft.setFont(BigFont);
    tft.print("Pomiar rozpocznie", CENTER, 20);
    tft.print("sie za:          ", CENTER, 52);
    tft.print("sekund", CENTER, 130);
    buttons.drawButtons();
    tft.setFont(SevenSegNumFont);
    Timer delayTimer;
    delayTimer.start();
    int lastSeconds = delayTimer.read();
    tft.setBackColor(VGA_BLUE);
    int time = SettingsMemory::instance().appSettings().measurementStartDelaySecs;
    while(time > 0) 
    {
        if (lastSeconds < delayTimer.read())
        {
            lastSeconds++;
            char buf [10] = {'\0'};
            sprintf (buf, "%3d", time);
            tft.print(buf, CENTER, 80);
            time--;
        }
        if (touch.dataAvailable() == true)
        {
            pressed_button = buttons.checkButtons();
            if (pressed_button == butBack)
            {
                return true;
            }
        }
    }
    return false;
}

//-----------------------------------------------------------------------------
bool Measurement::measure()
{
    UTFT_Buttons  buttons(&tft, &touch);
    touch.setPrecision(PREC_MEDIUM);
    buttons.setTextFont(BigFont);
    buttons.setSymbolFont(Dingbats1_XL);

    int butBack, pressed_button;
    buttons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_RED, VGA_BLUE);
    butBack = buttons.addButton(  0, 199, 100,  40, "<-");

    tft.clrScr();
    tft.setColor(VGA_WHITE);
    tft.setBackColor(VGA_BLACK);
    tft.setFont(BigFont);
    tft.print("Pomiar w trakcie...", CENTER, 20);
    tft.print("Wynik: ", LEFT, 120);
    tft.print("Jednostka: [kPa]", LEFT, 150); // TODO
    tft.setFont(SmallFont);
    tft.print("Czas do konca pomiaru [s]:", 10, 50);
    buttons.drawButtons();
    Timer measurementTimer;
    measurementTimer.start();
    int lastSeconds = measurementTimer.read();
    relayPin.write(LOW);
    int time = SettingsMemory::instance().appSettings().measurementTimeSecs;
    int measCounter = 0;
    pressureResultAverage = 0.0f;
    while (time > 0)
    {
        // measurement
        pressureSensor.performMeasurement();
        pressureResultAverage += pressureSensor.getLastResult();
        measCounter++;
        char buf [15] = {'\0'};
        sprintf (buf, "%3.3f", pressureSensor.getLastResult());
        tft.setFont(BigFont);
        tft.setBackColor(VGA_BLUE);
        tft.print(buf, CENTER, 120);
        DEBUG("Last_result: %3.3f", pressureSensor.getLastResult());
        if (lastSeconds < measurementTimer.read())
        {
            lastSeconds++;
            char buf [15] = {'\0'};
            sprintf (buf, "  %d  ", time);
            tft.setFont(SmallFont);
            tft.setBackColor(VGA_BLACK);
            tft.print(buf, 250, 50);
            time--;
        }
        if (touch.dataAvailable() == true)
        {
            pressed_button = buttons.checkButtons();
            if (pressed_button == butBack)
            {
                relayPin.write(HIGH);
                return true;
            }
        }
    }
    relayPin.write(HIGH);
    // average the result
    pressureResultAverage = (float)pressureResultAverage / measCounter;
    return false;
}
//-----------------------------------------------------------------------------

bool Measurement::postProcess()
{
    UTFT_Buttons  buttons(&tft, &touch);
    touch.setPrecision(PREC_MEDIUM);
    buttons.setTextFont(BigFont);
    buttons.setSymbolFont(Dingbats1_XL);

    int butBack, butPrint, butSave, pressed_button;
    buttons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_RED, VGA_BLUE);
    butBack = buttons.addButton(0, 199, 100,  40, "<-");
    butPrint = buttons.addButton(109, 199, 100, 40, "z", BUTTON_SYMBOL);
    butSave = buttons.addButton(219, 199, 100,  40, "l", BUTTON_SYMBOL);
    

    tft.clrScr();
    tft.setColor(VGA_WHITE);
    tft.setBackColor(VGA_BLACK);
    tft.setFont(BigFont);
    tft.print("Pomiar zakonczony!", CENTER, 20);
    tft.print("Wynik: ", 10, 70);
    char buf [15] = {'\0'};
    sprintf (buf, "%3.3f", pressureResultAverage);
    Time currentTime;
    if (ExternalRTC::instance().getTime(currentTime))
    {
        DEBUG_ERROR("Print result - get time failed!");
    }
    float temperature = ExternalRTC::instance().getTemperature();
    ResultData resultData = {currentTime, temperature, pressureResultAverage};
    DEBUG("-Wynik pomiaru-::: Data: %s, Czas: %s, Temperatura: %3.1f [C], Cisnienie: %3.3f", \
            ExternalRTC::getDateString(resultData.rtcTime),
            ExternalRTC::getTimeString(resultData.rtcTime),
            resultData.temperature,
            resultData.pressure);
    tft.setBackColor(VGA_BLUE);
    tft.print(buf, 130, 70);
    tft.setBackColor(VGA_BLACK);
    tft.print("Jednostka:", 10, 105);
    tft.setBackColor(VGA_BLUE);
    tft.print("[kPa]", 170, 105); // TODO
    tft.setBackColor(VGA_BLACK);

    buttons.drawButtons();
    Timer printTimer;
    printTimer.start();
    int time = 0;
    int lastSeconds = printTimer.read();
    while (true)
    {
        if (touch.dataAvailable() == true)
        {
            pressed_button = buttons.checkButtons();
            if (pressed_button == butBack)
            {
                return true;
            }
            else if (pressed_button == butPrint)
            {
                resultPrinter.setResultData(resultData);
                DEBUG("priniting ... ");
                resultPrinter.printResult();
                DEBUG("priniting finished! ");
            }
            else if (pressed_button == butSave)
            {

            }
        }
    }
    return false;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------