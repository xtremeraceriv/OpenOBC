/*
    Copyright (c) 2014 <benemorius@gmail.com>

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

#include "OilPressureSensor.h"

OilPressureSensor::OilPressureSensor(AnalogIn& analogIn, float calibrationScale) : analogIn(analogIn), calibrationScale(calibrationScale)
{

}

float OilPressureSensor::getPsi()
{
	return getPsiFromVoltage(analogIn.read()) * calibrationScale;
}

float OilPressureSensor::getBar()
{
	return getPsi() / 14.504;
}

float OilPressureSensor::getPsiFromVoltage(float voltage)
{
	if(voltage < 0.01)
		return -1;
	if(voltage >= 0.01 && voltage < 0.02)
		return 0;
	if(voltage > 0.5)
		return 999;

	float currentPsi = ((voltage)/2,852827656)-10; //for VDO 360-023, 150 psi 10-180 ohm sensor
	//float currentPsi = (float)pressureLookupTable[(unsigned int)(voltage * 100)] / 10; //Original
	//float currentPsi = ((voltage)-1.36)/0.02; // Para el sensor MD modificado

	return currentPsi;
}
