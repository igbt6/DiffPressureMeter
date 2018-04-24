#include "mainapp.h"

#include "UTFT_Buttons.h"
#include "TFT_Extension.h"
//-----------------------------------------------------------------------------
using namespace rtctime;

// fonts for linker
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
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
    millisStart();
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
                int res = mainMenu();
                if (res == 1)
                {
                    printf("Measurement started ...");
                    currentState = sMeasurement;
                }
                else
                {
                    currentState = sSettings;   
                }
                break;
            }
            case sMeasurement:
            {
                break;
            }
            case sSettings:
            {
                settingsMenu();
                currentState = sMainMenu;
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
int MainApp::mainMenu()
{
    // KEYBOARD DEMO
    /*
    TFT_Extension tftExt(&tft, &touch);
    tftExt.ExtSetup();
    tftExt.SetupMobileKB(0,150); //NEEDED TO WORK!
    while(1)
    {
        tftExt.ReceiveMsg(0,0,YELLOW); // X,Y(position on screen), Color
        char* msg = tftExt.Mobile_KeyBoard(BLUE);
        if(msg != NULL)
            tft.print(msg,0,50);
    }
    */
    Ticker updateTime;
    updateTime.attach(this, &MainApp::updateTime, 1); // every 1 second
    UTFT_Buttons  buttons(&tft, &touch);
    tft.clrScr();
    tft.setFont(SmallFont);

    touch.setPrecision(PREC_MEDIUM);

    buttons.setTextFont(BigFont);
    buttons.setSymbolFont(Dingbats1_XL);

    int butStartMeasurement, butSettings, pressed_button;
    buttons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_RED, VGA_BLUE);
    
    butStartMeasurement = buttons.addButton( 10, 80, 300,  80, "Rozpocznij pomiar");
    butSettings = buttons.addButton(239, 199,  80,  40, "e", BUTTON_SYMBOL);
    buttons.drawButtons();

    while(1) 
    {
        if (touch.dataAvailable() == true)
        {
            pressed_button = buttons.checkButtons();
            if (pressed_button == butStartMeasurement)
            {
                return 1;
            }
            if (pressed_button == butSettings)
            {
                return 2;
            }
        }
    }
    return 0;  
}

//-----------------------------------------------------------------------------
int MainApp::settingsMenu()
{
    UTFT_Buttons  buttons(&tft, &touch);
    tft.setFont(SmallFont);
    touch.setPrecision(PREC_MEDIUM);
    buttons.setTextFont(BigFont);
    buttons.setSymbolFont(Dingbats1_XL);

    int butDateTime, butMeasurement, butOther, butBack, pressed_button;
    buttons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_RED, VGA_BLUE);

    butDateTime = buttons.addButton( 10,  60, 300,  30, "Ustawienia czasu");
    butMeasurement = buttons.addButton( 10, 100, 300,  30, "Ustawienia pomiaru");
    butOther = buttons.addButton( 10, 140, 300,  30, "Inne");
    butBack = buttons.addButton(  0, 199, 100,  40, "i", BUTTON_SYMBOL);

clean:    
    tft.clrScr();
    buttons.drawButtons();

    while(1) 
    {
        if (touch.dataAvailable() == true)
        {
            pressed_button = buttons.checkButtons();
            if (pressed_button == butDateTime)
            {
                settingsDateTime.setDateTime();
                goto clean;
            }
            else if (pressed_button == butMeasurement)
            {
                settingsMeasurement.setMeasurement();
                goto clean;
            }
            else if (pressed_button == butOther)
            {
                goto clean;
            }
            else if (pressed_button == butBack)
            {
                return 0;
            }
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
void MainApp::drawFrame(const char* txt)
{
    tft.setColor(VGA_BLUE);
    tft.fillRect(0, 0, tft.getDisplayXSize()-1, 20);
    tft.setColor(VGA_WHITE);
    tft.setBackColor(VGA_BLUE);
    tft.drawLine(0, 21, tft.getDisplayXSize()-1, 21);
    tft.print(txt, CENTER, 1);
    tft.setBackColor(VGA_BLACK);
}
//-----------------------------------------------------------------------------
void MainApp::updateTime()
{
    if (currentState == sMainMenu)
    {
        Time currentTime;
        if (ExternalRTC::instance().getTime(currentTime))
        {
            printf("\r getTime ERROR occured !\t\n");
            drawFrame("----------------"); 
        }
        else
        {
            printf("\rCurrent time: %s,  %s, %s, %s\t\n",
                    ExternalRTC::getDateString(currentTime),       
                    ExternalRTC::getTimeString(currentTime),
                    ExternalRTC::getDayOfWeekString(currentTime),
                    ExternalRTC::getMonthString(currentTime));         
            drawFrame(ExternalRTC::getTimeDateString(currentTime));
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