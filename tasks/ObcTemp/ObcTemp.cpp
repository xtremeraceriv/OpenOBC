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

#include "ObcTemp.h"
#include <ObcUI.h>
#include <cmath>
#include <cstdlib>

using namespace ObcTempState;

ObcTemp::ObcTemp(OpenOBC& obc) : ObcUITask(obc)
{
	setDisplay("ObcTemp");

	static bool StatusSet = true; //Inicializo selector
	
	std::string configState = obc.config->getValueByNameWithDefault("ObcTempState", "TempExt");
	if(configState == "TempCoolant")
		state = TempCoolant;
	else if(configState == "TempExt")
		state = TempExt;
	else if(configState == "TempPressOil") //Agrego estado de temperatura y presion de aceite
		state = TempPressOil;
	
	coolantWarningTemp = strtoul(obc.config->getValueByNameWithDefault("ObcTempCoolantWarningTemp", "100").c_str(), NULL, 0);
	OilWarningTemp = strtoul(obc.config->getValueByNameWithDefault("ObcTempOilWarningTemp", "140").c_str(), NULL, 0);		//Agrego warning de temp oil
	OilWarningPress = strtoul(obc.config->getValueByNameWithDefault("ObcPressOilWarningPress", "10").c_str(), NULL, 0);		//Agrego warning de press oil
}

ObcTemp::~ObcTemp()
{

}

void ObcTemp::wake()
{
	runTask();
}

void ObcTemp::sleep()
{
	obc.config->setValueByName("ObcTempCoolantWarningTemp", "%i", coolantWarningTemp);
	obc.config->setValueByName("ObcTempOilWarningTemp", "%i", OilWarningTemp);			//Nuevo Warning
	obc.config->setValueByName("ObcPressOilWarningPress", "%i", OilWarningPress);		//Nuevo Warning
	if(state == TempCoolant)
		obc.config->setValueByName("ObcTempState", "TempCoolant");
	else if(state == TempExt)
		obc.config->setValueByName("ObcTempState", "TempExt");
	else if(state == TempPressOil)										//Nuevo estado
		obc.config->setValueByName("ObcTempState", "TempPressOil");
}

void ObcTemp::runTask()
{
	if(state == TempExt)
	{
		float voltage = obc.temperature->read();
		float resistance = (10000 * voltage) / (REFERENCE_VOLTAGE - voltage);
		float temperature = 1.0 / ((1.0 / 298.15) + (1.0/3950) * log(resistance / 4700)) - 273.15f;
		if(obc.ui->getMeasurementSystem() == ObcUIMeasurementSystem::Both)
			setDisplay("ext. % 2.1fC % 3.0fF", temperature, temperature * 1.78 + 32);
		if(obc.ui->getMeasurementSystem() == ObcUIMeasurementSystem::Metric)
			setDisplay("Exterior % 2.1fC", temperature);								//Modifico a Español
		if(obc.ui->getMeasurementSystem() == ObcUIMeasurementSystem::Imperial)
			setDisplay("exterior % 3.0fF", temperature * 1.78 + 32);
	}
	else if(state == TempCoolant)
	{
		if(obc.ui->getMeasurementSystem() == ObcUIMeasurementSystem::Both)
			setDisplay("coolant % 2.0fC %.0fF", obc.coolantTemperature, obc.coolantTemperature * 1.78 + 32);
		if(obc.ui->getMeasurementSystem() == ObcUIMeasurementSystem::Metric)
			setDisplay("Agua % 2.0fC", obc.coolantTemperature);							//Modifico a Español
		if(obc.ui->getMeasurementSystem() == ObcUIMeasurementSystem::Imperial)
			setDisplay("coolant temp % 3.0fF", obc.coolantTemperature * 1.78 + 32);
	}
	else if(state == TempPressOil)
	{
		setDisplay("Aceite % 2.0f psi % 2.0fC",  obc.oilPressure->getPsi(), obc.analogIn2->read());	//Agrego estado nuevo
	}
	else if(state == TempCoolantWarningSet)
	{
		setDisplay("set warning % 3iC", coolantWarningTempSet);
	}
	else if(state == TempOilWarningSet)								//Nuevo warning
	{
		setDisplay("set warning % 3iC", OilWarningTempSet);
	}
	else if(state == PressOilWarningSet)							//Nuevo warning
	{
		setDisplay("set warning % 3i psi", OilWarningPressSet);
	}

	//Warning Temperatura de Agua
	static Timer coolantWarningTimer;
	static bool hasWarned;
	if(obc.coolantTemperature >= coolantWarningTemp && !hasWarned)
	{
		coolantWarningTimer.start();
		hasWarned = true;
		obc.ui->setActiveTask(this, 5);
		state = TempCoolant;
		obc.ccmLight->on();
		obc.ui->callback.addCallback(obc.ccmLight, &IO::off, 4000);
		obc.chime1->on();
		obc.ui->callback.addCallback(obc.chime1, &IO::off, 100);
	}
	else if(obc.coolantTemperature >= coolantWarningTemp)
	{
		if(coolantWarningTimer.read_ms() >= 5000)
		{
			coolantWarningTimer.start();
			obc.ccmLight->on();
			obc.ui->callback.addCallback(obc.ccmLight, &IO::off, 4000);
			obc.chime1->on();
			obc.ui->callback.addCallback(obc.chime1, &IO::off, 100);
		}
	}
	else
	{
		hasWarned = false;
	}

	//Warning Temperatura de Aceite
	static Timer OTWarningTimer;
	static bool OThasWarned;
	if(obc.analogIn2->read() >= OilWarningTemp && !OThasWarned)
	{
		OTWarningTimer.start();
		OThasWarned = true;
		obc.ui->setActiveTask(this, 5);
		state = TempPressOil;
		obc.ccmLight->on();
		obc.ui->callback.addCallback(obc.ccmLight, &IO::off, 4000);
		obc.chime1->on();
		obc.ui->callback.addCallback(obc.chime1, &IO::off, 100);
	}
	else if((obc.analogIn2->read()) >= OilWarningTemp)
	{
		if(OTWarningTimer.read_ms() >= 5000)
		{
			OTWarningTimer.start();
			obc.ccmLight->on();
			obc.ui->callback.addCallback(obc.ccmLight, &IO::off, 4000);
			obc.chime1->on();
			obc.ui->callback.addCallback(obc.chime1, &IO::off, 100);
		}
	}
	else
	{
		OThasWarned = false;
	}

	//Warning Presion de aceite
	static Timer OPWarningTimer;
	static bool OPhasWarned;
	if((obc.oilPressure->getPsi()) >= OilWarningPress && !OPhasWarned)
	{
		OPWarningTimer.start();
		OPhasWarned = true;
		obc.ui->setActiveTask(this, 5);
		state = TempPressOil;
		obc.ccmLight->on();
		obc.ui->callback.addCallback(obc.ccmLight, &IO::off, 4000);
		obc.chime1->on();
		obc.ui->callback.addCallback(obc.chime1, &IO::off, 100);
	}
	else if((obc.oilPressure->getPsi()) >= OilWarningPress)
	{
		if(OPWarningTimer.read_ms() >= 5000)
		{
			OPWarningTimer.start();
			obc.ccmLight->on();
			obc.ui->callback.addCallback(obc.ccmLight, &IO::off, 4000);
			obc.chime1->on();
			obc.ui->callback.addCallback(obc.chime1, &IO::off, 100);
		}
	}
	else
	{
		OPhasWarned = false;
	}
}

void ObcTemp::buttonHandler(ObcUITaskFocus::type focus, uint32_t buttonMask)
{
	if(focus == ObcUITaskFocus::background)
	{
		if(buttonMask == BUTTON_TEMP_MASK)
			obc.ui->setActiveTask(this);
		return;
	}
	
	if(buttonMask == BUTTON_TEMP_MASK)
	{
		if(state == TempExt)
			state = TempCoolant;
		else if(state == TempCoolant)
			state = TempPressOil;
		else if(state == TempPressOil)
			state = TempExt;
	}
	
	else if(buttonMask == BUTTON_SET_MASK)
	{
		if(state == TempCoolant)
		{
			coolantWarningTempSet = coolantWarningTemp;
			state = TempCoolantWarningSet;
		}
		else if((state == TempPressOil) && !StatusSet)
		{
			OilWarningTemp = OilWarningTempSet;
			state = TempOilWarningSet;
			StatusSet = false;
		}
		else if((state == TempPressOil) && StatusSet)
		{
			OilWarningPress = OilWarningPressSet;
			state = PressOilWarningSet;
			StatusSet = true;
		}
		else
		{
			coolantWarningTempSet = coolantWarningTemp;
			state = TempExt;
		}
	}
	
	//Cargo valor de Coolant temp Warning
	else if((state == TempCoolantWarningSet) && (buttonMask & (BUTTON_1000_MASK | BUTTON_100_MASK | BUTTON_10_MASK | BUTTON_1_MASK)))
	{
		if(buttonMask == BUTTON_1000_MASK)
		{
			coolantWarningTempSet += 1000;
		}
		if(buttonMask == BUTTON_100_MASK)
		{
			coolantWarningTempSet += 100;
		}
		if(buttonMask == BUTTON_10_MASK)
		{
			coolantWarningTempSet += 10;
		}
		if(buttonMask == BUTTON_1_MASK)
		{
			coolantWarningTempSet += 1;
		}
		coolantWarningTempSet %= 1000;
	}

	//Cargo valor de Oil temp Warning
	else if((state == TempOilWarningSet) && (buttonMask & (BUTTON_1000_MASK | BUTTON_100_MASK | BUTTON_10_MASK | BUTTON_1_MASK)))
	{
		if(buttonMask == BUTTON_1000_MASK)
		{
			OilWarningTempSet += 1000;
		}
		if(buttonMask == BUTTON_100_MASK)
		{
			OilWarningTempSet += 100;
		}
		if(buttonMask == BUTTON_10_MASK)
		{
			OilWarningTempSet += 10;
		}
		if(buttonMask == BUTTON_1_MASK)
		{
			OilWarningTempSet += 1;
		}
		OilWarningTempSet %= 1000;
	}

	//Cargo valor de Oil press Warning
	else if((state == PressOilWarningSet) && (buttonMask & (BUTTON_1000_MASK | BUTTON_100_MASK | BUTTON_10_MASK | BUTTON_1_MASK)))
	{
		if(buttonMask == BUTTON_1000_MASK)
		{
			OilWarningPressSet += 1000;
		}
		if(buttonMask == BUTTON_100_MASK)
		{
			OilWarningPressSet += 100;
		}
		if(buttonMask == BUTTON_10_MASK)
		{
			OilWarningPressSet += 10;
		}
		if(buttonMask == BUTTON_1_MASK)
		{
			OilWarningPressSet += 1;
		}
		OilWarningPressSet %= 1000;
	}
}
