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

#include "ObcRange.h"
#include <ObcUI.h>

ObcRange::ObcRange(OpenOBC& obc) : ObcUITask(obc)
{
	setDisplay("ObcRange");
}

ObcRange::~ObcRange()
{

}

void ObcRange::wake()
{
	runTask();
}

void ObcRange::runTask()
{
	float fuelLevelLitres = obc.fuelLevel->getLitres();
	float fuelLevelGallons = obc.fuelLevel->getGallons();
	float rangeKm = fuelLevelLitres / obc.averageLitresPer100km * 100;
			
	if(obc.ui->getMeasurementSystem() == ObcUIMeasurementSystem::Both)
		setDisplay("%.0f km %.0f miles", rangeKm, rangeKm * 0.621371f);
	if(obc.ui->getMeasurementSystem() == ObcUIMeasurementSystem::Metric)
		setDisplay("%.0f km  %.1f L", rangeKm, fuelLevelLitres);
	if(obc.ui->getMeasurementSystem() == ObcUIMeasurementSystem::Imperial)
		setDisplay("%.0f miles  %.2f gal", rangeKm * 0.621371f, fuelLevelGallons);
}

void ObcRange::buttonHandler(ObcUITaskFocus::type focus, uint32_t buttonMask)
{
	if(focus == ObcUITaskFocus::background)
	{
		if(buttonMask == BUTTON_RANGE_MASK)
			obc.ui->setActiveTask(this);
		return;
	}
	
	
}
