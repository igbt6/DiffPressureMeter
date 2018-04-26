#include "measurement.h"
#include "settingsmemory.h"
#include "UTFT_Buttons.h"

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
            char buf [4] = {'\0'};
            sprintf (buf, "%d", time);
            tft.print(buf, CENTER, 80);
            time--;
        }
        if (touch.dataAvailable() == true)
        {
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
    while (time > 0)
    {
        // measurement
        {
            pressureSensor.performMeasurement();
            char buf [15] = {'\0'};
            sprintf (buf, "%3.3f", pressureSensor.getLastResult());
            tft.setFont(BigFont);
            tft.setBackColor(VGA_BLUE);
            tft.print(buf, CENTER, 120);
            DEBUG("Last_result: %f", pressureSensor.getLastResult());
        }
        if (lastSeconds < measurementTimer.read())
        {
            lastSeconds++;
            char buf [15] = {'\0'};
            sprintf (buf, "  %d  ", time);
            tft.setFont(SmallFont);
            tft.setBackColor(VGA_BLACK);
            tft.print(buf, 240, 50);
            time--;
        }
        if (touch.dataAvailable() == true)
        {
            if (pressed_button == butBack)
            {
                return true;
            }
        }
    }
    relayPin.write(HIGH); 
    return false;
}
//-----------------------------------------------------------------------------