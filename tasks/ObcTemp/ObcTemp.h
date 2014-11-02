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

#ifndef OBCTEMP_H
#define OBCTEMP_H

#include <ObcUITask.h>

namespace ObcTempState 
{
	enum state {TempExt, TempCoolant, TempPressOil, TempCoolantWarningSet, TempOilWarningSet, PressOilWarningSet}; //Agrego estados del OBC
}

class ObcTemp : public ObcUITask
{

public:
	ObcTemp(OpenOBC& obc);
	~ObcTemp();
	
	virtual void runTask();
	virtual void buttonHandler(ObcUITaskFocus::type focus, uint32_t buttonMask);
	
	virtual void wake();
	virtual void sleep();
	
private:
	ObcTempState::state state;
	uint32_t coolantWarningTemp;
	uint32_t coolantWarningTempSet;
	uint32_t OilWarningTemp;			//Agrego warnings de temperatura y presion de aceite
	uint32_t OilWarningTempSet;
	uint32_t OilWarningPress;
	uint32_t OilWarningPressSet;
	static bool StatusSet;
};

#endif // OBCTEMP_H
