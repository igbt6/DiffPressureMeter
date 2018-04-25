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
extern uint8_t arial_bold[];

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
    , _updateTime(false)
{
    tft.InitLCD(LANDSCAPE);
    touch.InitTouch(SettingsMemory::instance().appSettings().calibCoeffs, tft.getOrientation());
    DEBUG("--- Hello! System here is living! ---");
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
                    DEBUG("Measurement started ...");
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
                measurement.run();
                currentState = sMainMenu;
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
    tft.clrScr();
    tft.setFont(SmallFont);
    Ticker updateTimeTask;
    updateTimeTask.attach(this, &MainApp::updateTimeTask, 1); // every 1 second
    UTFT_Buttons  buttons(&tft, &touch);

    drawFrame();
    updateTime();
    touch.setPrecision(PREC_MEDIUM);

    buttons.setTextFont(BigFont);
    buttons.setSymbolFont(Dingbats1_XL);

    int butStartMeasurement, butSettings, pressed_button;
    buttons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_RED, VGA_BLUE);
    
    butStartMeasurement = buttons.addButton( 10, 80, 300,  80, "Rozpocznij pomiar");
    butSettings = buttons.addButton(239, 199,  80,  40, "a", BUTTON_SYMBOL);
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
        
        if (_updateTime)
        {
            updateTime();
            _updateTime = false;
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
    butBack = buttons.addButton(  0, 199, 100,  40, "<-");

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
void MainApp::drawFrame()
{
    tft.setColor(VGA_BLUE);
    tft.fillRect(0, 0, tft.getDisplayXSize()-1, 20);
    tft.setColor(VGA_WHITE);
    tft.setBackColor(VGA_BLUE);
    tft.drawLine(0, 21, tft.getDisplayXSize()-1, 21);
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
            DEBUG_ERROR("updateTime - getTime ERROR occured !");
            tft.print("----------------", CENTER, 5);
        }
        else
        {
            tft.setFont(arial_bold);
            tft.setColor(VGA_WHITE);
            tft.setBackColor(VGA_BLUE);
            tft.print(ExternalRTC::getTimeDateString(currentTime), CENTER, 4);
        }
    }
}

//-----------------------------------------------------------------------------
void MainApp::updateTimeTask()
{
    _updateTime = true;
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