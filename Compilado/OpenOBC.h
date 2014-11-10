/*
    Copyright (c) 2012 <benemorius@gmail.com>

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

#ifndef OPENOBC_H
#define OPENOBC_H

#include "FunctionPointer.h"
#include "Debug.h"
#include "RTC.h"
#include "PWM.h"
#include "SPI.h"
#include "ObcLcd.h"
#include "InterruptManager.h"
#include "Input.h"
#include "ObcKeypad.h"
#include "CheckControlModule.h"
#include "FuelLevel.h"
#include "SDFS.h"
#include "ConfigFile.h"
#include <AnalogIn.h>
#include "SpeedInput.h"
#include "E36ZKE4.h"
#include "E36SRS.h"
#include "E36IHKR.h"
#include "E36Kombi.h"
#include "E36MK4.h"
#include <Callback.h>
#include <Watchdog.h>
#include <I2C.h>
#include <PCA95xx.h>
#include <MMA845x.h>
#include <AnalogOut.h>
#include "FuelConsumption.h"
#include <OilPressureSensor.h>

class ObcUI;

typedef enum
{
	DISPLAY_CONSUM1,
	DISPLAY_CONSUM2,
	DISPLAY_CONSUM3,
	DISPLAY_CONSUM4,
	DISPLAY_TEMP,
	DISPLAY_TEMP1,
	DISPLAY_TEMP1SET,
	DISPLAY_SPEED,
	DISPLAY_VOLTAGE,
	DISPLAY_OPENOBC,
	DISPLAY_CHECK,
	DISPLAY_FREEMEM,
	DISPLAY_RANGE1,
	DISPLAY_RANGE2,
	DISPLAY_ACCELEROMETER,
	DISPLAY_CLOCKSET,
	DISPLAY_DATESET,
	DISPLAY_CALIBRATE
} DisplayMode_Type;

typedef enum
{
	CLOCKDISPLAY_CLOCK,
	CLOCKDISPLAY_DATE
} ClockDisplayMode_Type;

namespace ObcBootupText {
	enum mode {GitHash, Custom, None};
}

class OpenOBC : public InterruptManagerOwner
{
	
public:
	OpenOBC();
	void mainloop();
	void button1000();
	void button100();
	void button10();
	void button1();
	void uartHandler();
	void hardwareTest();

	Callback* callback;
	SPI* spi0;
	SPI* spi1;
	PWM* lcdLight;
	PWM* clockLight;
	PWM* auxLight;
	PWM* keypadLight;
	IO* codeLed;
	IO* limitLed;
	IO* timerLed;
	IO* ccmLight;
	IO* chime0;
	IO* chime1;
	IO* ventilation;
	IO* antitheftHorn;
	IO* ews;
	IO* out0;
	IO* out1;
	IO* out2;
	IO* out3;
	IO* io0;
	IO* io1;
	Input* run;
	ObcKeypad* keypad;
	Input* sdcardDetect;
	FuelLevel* fuelLevel;
	Input* stalkButton;
	Input* illumination;
	Input* ambientLight;
	Input* brakeSwitch;
	Input* brakeCheck;
	SDFS* sd;
	ConfigFile* config;
	DS2* diag;
	E36ZKE4* zke;
	E36SRS* srs;
	E36IHKR* ihkr;
	E36Kombi* kombi;
	E36MK4* mk4;
	I2C* i2c0;
	I2C* i2c1;
	PCA95xx* io;
	MMA845x* accel;
	ObcUI* ui;
	CheckControlModule* ccm;
	SpeedInput* speed;
	FuelConsumption* fuelCons;
	float averageLitresPer100km;
	AnalogIn* temperature;
	AnalogIn* batteryVoltage;
	AnalogIn* vstart;
	Uart* kline;
	Uart* lline;
    IO* klWake;
	RTC* rtc;
// 	PWM* lcdBacklight;
// 	PWM* clockBacklight;
// 	PWM* auxBacklight;
// 	PWM* keypadBacklight;
	ObcLcd* lcd;
	IO* lcdReset;
	IO* lcdBiasEn;
	AnalogIn* analogIn1;
	AnalogIn* analogIn2;
	AnalogOut* analogOut;
	float coolantTemperature; //coolant temperature in degrees celsius from KOMBI via diagnostic bus
	float averageKmh;
	float currentKm;
	OilPressureSensor* oilPressure;
	//OilTempSensor* oilTemp;
	float illuminationDutycycle;
	float ambientLightDutycycle;

protected:
	void sleep();
	void wake();
	void printDS2Packet();
	void writeConfigData();
	
	Debug* debug;
	PWM* lcdBiasClock;
	volatile DisplayMode_Type& displayMode;
// 	volatile DisplayMode_Type& displayModeNonvolatile;
	ClockDisplayMode_Type clockDisplayMode;
	bool useMetricSystem;
	bool useMetricSystemBoth; //both metric and imperial
	volatile uint32_t& averageFuelConsumptionSeconds;
    DisplayMode_Type lastDisplayMode;
	Watchdog wdt;
	float batteryVoltageCalibration;
	bool disableComms;
    IO* vrefEn;
	bool doHardwareTest;
	
};



#endif //OPENOBC_H
