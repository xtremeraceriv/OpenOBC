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

//MODULO DE TABLERO DE INSTRUMENTOS

#include "E36Kombi.h"

#define CMD_QUERY {0x00}
#define CMD_READ_STATUS {0x08}

#define STATUS_BYTE_COOLANT_TEMPERATURE (5)

E36Kombi::E36Kombi(DS2& diagnosticInterface) : diag(diagnosticInterface)
{
	address = 0x0d;
	packetType = DS2_16BIT;
}

bool E36Kombi::query()
{
	const uint8_t cmd[] = CMD_QUERY;
	DS2Packet query(address, cmd, sizeof(cmd), packetType);
	DS2Packet* reply = diag.query(query);
	if(reply != NULL)
	{
		delete reply;
		return true;
	}
	return false;
}

float E36Kombi::getCoolantTemperature()
{
	const uint8_t cmd[] = CMD_READ_STATUS;
	DS2Packet query(address, cmd, sizeof(cmd), packetType);
	DS2Packet* reply = diag.query(query, DS2_L);
	if(reply != NULL)
	{
		uint8_t* statusData = reply->getData();
		uint8_t index = STATUS_BYTE_COOLANT_TEMPERATURE;
		if(index >= reply->getDataLength())
			return -273.15f;
		
		uint8_t rawTemp = statusData[index];
		delete reply;
		float temperature = coolant_temp_table[rawTemp];
		return temperature;
	}
	return -273.15f;
}
