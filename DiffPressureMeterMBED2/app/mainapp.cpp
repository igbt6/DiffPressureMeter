#include "mainapp.h"

#include "UTFT_Buttons.h"
//-----------------------------------------------------------------------------
using namespace rtctime;

// fonts for linker
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t Dingbats1_XL[];

//-----------------------------------------------------------------------------
MainApp &MainApp::instance()
{
    static MainApp app;
    return app;
}

//-----------------------------------------------------------------------------
MainApp::MainApp()
    : tft(HwInterfaces::instance().getHwInterface<SPI__1>().handle())
    , touch(HwInterfaces::instance().getHwInterface<SPI__1>().handle())
    , settingsDateTime(tft, touch)
    , settingsMeasurement(tft, touch)
    , measurement(tft, touch)
    , currentState(sMainMenu)
{
    tft.InitLCD(LANDSCAPE);
    touch.InitTouch(SettingsMemory::instance().appSettings().calibCoeffs, tft.getOrientation());
    printf("--- Hello! System here is living! ---");
}

//-----------------------------------------------------------------------------
void MainApp::run()
{
    while(true)
    {
        switch(currentState)
        {
            case sIntro:
            {
                break;
            }
            case sMainMenu:
            {
                mainMenu();
                break;
            }
            case sMeasurement:
            {
                break;
            }
            case sSettingsTime:
            {
                break;
            }
            case sSettingsMeas:
            {
                break;
            }
            case sInfo:
            {
                break;
            }

            default:
                break;
        }
    }
}

//-----------------------------------------------------------------------------
void MainApp::mainMenu()
{
    UTFT_Buttons  buttons(&tft, &touch);

    tft.clrScr();
    tft.setFont(SmallFont);

    touch.setPrecision(PREC_MEDIUM);

    buttons.setTextFont(BigFont);
    buttons.setSymbolFont(Dingbats1_XL);

    int but1, but2, but3, but4, butX, butY, pressed_button;
    bool default_colors = true;
    
    but1 = buttons.addButton( 10,  20, 300,  30, "Button 1");
    but2 = buttons.addButton( 10,  60, 300,  30, "Button 2");
    but3 = buttons.addButton( 10, 100, 300,  30, "Button 3");
    but4 = buttons.addButton( 10, 140, 300,  30, "Button 4", BUTTON_DISABLED);
    butX = buttons.addButton(279, 199,  40,  40, "a", BUTTON_SYMBOL);
    butY = buttons.addButton(  0, 199, 100,  40, "I", BUTTON_SYMBOL | BUTTON_SYMBOL_REP_3X);
    buttons.drawButtons();

    tft.print("You pressed:", 110, 205);
    tft.setColor(VGA_BLACK);
    tft.setBackColor(VGA_WHITE);
    tft.print("None    ", 110, 220);

    while(1) 
    {
        if (touch.dataAvailable() == true)
        {
        pressed_button = buttons.checkButtons();

        if (pressed_button==butX)
        {
            if (buttons.buttonEnabled(but4))
            buttons.disableButton(but4, true);
            else
            buttons.enableButton(but4, true);
        }
        else if (pressed_button==butY)
        {
            if (default_colors)
            {
            buttons.setButtonColors(VGA_YELLOW, VGA_RED, VGA_YELLOW, VGA_BLUE, VGA_GRAY);
            buttons.relabelButton(butY, "_");
            buttons.drawButtons();
            default_colors=false;
            }
            else
            {
            buttons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_RED, VGA_BLUE);
            buttons.relabelButton(butY, "I");
            buttons.drawButtons();
            default_colors=true;
            }
        }
        if (pressed_button==but1)
            tft.print("Button 1", 110, 220);
        if (pressed_button==but2)
            tft.print("Button 2", 110, 220);
        if (pressed_button==but3)
            tft.print("Button 3", 110, 220);
        if (pressed_button==but4)
        {
            tft.print("Button 4", 110, 220);
            break;
        }
        if (pressed_button==-1)
            tft.print("None    ", 110, 220);
        }
    }   
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------