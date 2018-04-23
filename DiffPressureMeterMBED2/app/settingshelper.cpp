#include "settingshelper.h"

//-----------------------------------------------------------------------------
SettingsHelper::SettingsHelper(LUTFT &tft, LUTouch &touch)
  : tft(tft)
  , touch(touch)
{

}

//-----------------------------------------------------------------------------
void SettingsHelper::drawUpButton(int x, int y)
{
  tft.setColor(64, 64, 128);
  tft.fillRoundRect(x, y, x+32, y+25);
  tft.setColor(255, 255, 255);
  tft.drawRoundRect(x, y, x+32, y+25);
  tft.setColor(128, 128, 255);
  for (int i=0; i<15; i++)
  {
    tft.drawLine(x+6+(i/1.5), y+20-i, x+27-(i/1.5), y+20-i);
  }
}  

//-----------------------------------------------------------------------------
void SettingsHelper::drawDownButton(int x, int y)
{
  tft.setColor(64, 64, 128);
  tft.fillRoundRect(x, y, x+32, y+25);
  tft.setColor(255, 255, 255);
  tft.drawRoundRect(x, y, x+32, y+25);
  tft.setColor(128, 128, 255);
  for (int i=0; i<15; i++)
  {
    tft.drawLine(x+6+(i/1.5), y+5+i, x+27-(i/1.5), y+5+i);
  }
}

//-----------------------------------------------------------------------------
void SettingsHelper::drawCancelButton()
{
  tft.setColor(64, 64, 128);
  tft.fillRoundRect(0, 200, 154, 239);
  tft.setColor(255, 255, 255);
  tft.drawRoundRect(0, 200, 154, 239);
  tft.setBackColor(64, 64, 128);
  tft.print("Cancel", 29, 212);
  tft.setBackColor(0, 0, 0);
}

//-----------------------------------------------------------------------------
void SettingsHelper::drawSaveButton()
{
  tft.setColor(64, 64, 128);
  tft.fillRoundRect(165, 200, 319, 239);
  tft.setColor(255, 255, 255);
  tft.drawRoundRect(165, 200, 319, 239);
  tft.setBackColor(64, 64, 128);
  tft.print("Save", 210, 212);
  tft.setBackColor(0, 0, 0);
}

//-----------------------------------------------------------------------------
void SettingsHelper::waitForTouchRelease()
{
  // Wait for release
  while (touch.dataAvailable()==true)
  {
    touch.read();
  }
}

//-----------------------------------------------------------------------------
void SettingsHelper::buttonWait(int x, int y)
{
  tft.setColor(255, 0, 0);
  tft.drawRoundRect(x, y, x+32, y+25);
  waitForTouchRelease();
  tft.setColor(255, 255, 255);
  tft.drawRoundRect(x, y, x+32, y+25);
}
//-----------------------------------------------------------------------------