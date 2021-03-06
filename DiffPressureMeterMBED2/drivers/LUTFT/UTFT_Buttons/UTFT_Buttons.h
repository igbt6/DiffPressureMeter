/*
  UTFT_Buttons.h - Add-on Library for UTFT: Buttons
  Copyright (C)2016 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  This library adds simple but easy to use buttons to extend the use
  of the UTFT and URTouch libraries.

  You can find the latest version of the library at 
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/

#ifndef UTFT_Buttons_h
#define UTFT_Buttons_h

#define UTFT_BUTTONS_VERSION	103

#define bitmapdatatype unsigned short*

#include "LUTft.h"
#include "LUTouch.h"

#define MAX_BUTTONS	20	// Maximum number of buttons available at one time

// Define presets for button status
#define BUTTON_DISABLED			0x0001
#define BUTTON_SYMBOL			0x0002
#define BUTTON_SYMBOL_REP_3X	0x0004
#define BUTTON_BITMAP			0x0008	
#define BUTTON_NO_BORDER		0x0010
#define BUTTON_UNUSED			0x8000

typedef struct
{
  uint16_t			pos_x, pos_y, width, height;
  uint16_t			flags;
  const char				*label;
  bitmapdatatype	data;
} button_type;

class UTFT_Buttons
{
	public:
		UTFT_Buttons(LUTFT *ptrUTFT, LUTouch *ptrURTouch);

		int		addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char *label, uint16_t flags=0);
		int		addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, bitmapdatatype data, uint16_t flags=0);
		void	drawButtons();
		void	drawButton(int buttonID);
		void	enableButton(int buttonID, bool redraw=false);
		void	disableButton(int buttonID, bool redraw=false);
		void	relabelButton(int buttonID, const char *label, bool redraw=false);
		bool	buttonEnabled(int buttonID);
		void	deleteButton(int buttonID);
		void	deleteAllButtons();
		int		checkButtons();
		void	setTextFont(uint8_t* font);
		void	setSymbolFont(uint8_t* font);
		void	setButtonColors(word atxt, word iatxt, word brd, word brdhi, word back);

	protected:
		LUTFT		*_UTFT;
		LUTouch		*_URTouch;
		button_type	buttons[MAX_BUTTONS];
		word		_color_text, _color_text_inactive, _color_background, _color_border, _color_hilite;
		uint8_t		*_font_text, *_font_symbol;
};

#endif
