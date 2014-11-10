/*
    Copyright (c) 2013 <benemorius@gmail.com>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
*/


#include "Watchdog.h"

Watchdog::Watchdog()
{
	WDT_Init(WDT_CLKSRC_IRC, WDT_MODE_RESET);
}

Watchdog::~Watchdog()
{
	//FIXME deinit wdt
}

void Watchdog::start(float seconds)
{
	WDT_Start(seconds * 1000 * 1000);
}

void Watchdog::stop()
{
	LPC_WDT->WDMOD &= ~WDT_WDMOD_WDEN;
}

void Watchdog::feed()
{
	WDT_Feed();
}

bool Watchdog::wasReset()
{
	if(WDT_ReadTimeOutFlag())
	{
		WDT_ClrTimeOutFlag();
		return true;
	}
	return false;
}
