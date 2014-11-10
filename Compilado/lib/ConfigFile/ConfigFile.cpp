/*
 *  Copyright (c) 2012 <benemorius@gmail.com>
 * 
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation
 *  files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use,
 *  copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following
 *  conditions:
 * 
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *  OTHER DEALINGS IN THE SOFTWARE.
 */

#include "ConfigFile.h"
#include <string.h>
#include <cstdio>
#include <errno.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdarg.h>
#include <debugpretty.h>

using namespace std;

ConfigFile::ConfigFile(const char* filename)
{
	this->filename = filename;
}

int32_t ConfigFile::readConfig()
{
	FILE* file = fopen(filename.c_str(), "r");
	if(file == NULL)
	{
		DEBUG("failed to open config file: %s (%i)\r\n", filename.c_str(), errno);
		return -1;
	}

// 	DEBUG("parsing config file: %s\r\n", filename.c_str());

	//TODO needs better validation
	int32_t parametersRead = 0;
	char* line = new char[256];
	while(fgets(line, 256, file) != NULL)
	{
		if(line[0] == '#') //comment
			continue;
		if(line[0] == '\n' || line[0] == '\r') //blank line
			continue;
		
		char* tok;
		tok = strtok(line, "=\r\n");
		if(tok == NULL)
			continue;
		//strip leading spaces
		while(*tok == ' ')
			++tok;
		if(*tok == '\0')
			continue;
		//strip trailing spaces
		char* end = tok + strlen(tok) - 1;
		while(end > tok && *end == ' ')
			--end;
		*(end+1) = '\0';
		string key = tok;
		tok = strtok(NULL, "=\r\n");
		if(tok == NULL)
			continue;
		//strip leading spaces
		while(*tok == ' ')
			++tok;
		if(*tok == '\0')
			continue;
		//strip trailing spaces
		end = tok + strlen(tok) - 1;
		while(end > tok && *end == ' ')
			--end;
		*(end+1) = '\0';
		string value = tok;
		
		parameters[key] = value;
		parametersRead++;
		printf("<%s> = <%s>\r\n", key.c_str(), value.c_str());
	}

	delete[] line;
	fclose(file);
	if(parametersRead == 0)
	{
		DEBUG("config file read; no parameters found\r\n");
	}
	return parametersRead;
}

int32_t ConfigFile::writeConfig(bool overwriteExistingFile)
{
	int fd = open(filename.c_str(), O_CREAT | O_WRONLY | (overwriteExistingFile ? 0 : O_EXCL));
	if(fd < 0)
	{
		DEBUG("error opening %s for writing: %s\r\n", filename.c_str(), strerror(errno));
		return -1;
	}
	FILE* file = fdopen(fd, "w");
	if(file == NULL)
	{
		DEBUG("error opening %s for writing: %s\r\n", filename.c_str(), strerror(errno));
		return -1;
	}
	
	int32_t parametersWritten = 0;
	map<string, string>::iterator parameter = parameters.begin();
	while(parameter != parameters.end())
	{
		fprintf(file, "%s = %s\n", parameter->first.c_str(), parameter->second.c_str());
		++parameter;
		parametersWritten++;
	}
	fclose(file);
	return parametersWritten;
}

string ConfigFile::getValueByName(const string& name)
{
	return parameters[name];
}

string ConfigFile::getValueByNameWithDefault(const string& name, const char* defaultValue, ... )
{
	if(isSet(name))
		return parameters[name];
	
	char* buf = new char[128];
	va_list args;
	va_start(args, defaultValue);
	vsnprintf(buf, 128, defaultValue, args);
	va_end(args);
	string value = buf;
	delete[] buf;

	setValueByName(name, value);
	return value;
}

void ConfigFile::setValueByName(const string& name, const string& value)
{
	if(parameters[name] == value)
		return;
	parameters[name] = value;

	string newfilename = filename + "~";

	FILE* originalFile = fopen(filename.c_str(), "r");
	if(originalFile == NULL)
	{
		return;
	}
	FILE* newfile = fopen(newfilename.c_str(), "w");
	if(newfile == NULL)
	{
		return;
	}

	char* line = new char[256];
	bool foundKey = false;
	
	while(fgets(line, 256, originalFile) != NULL)
	{
		//keep comments or blank lines unchanged
		if(line[0] == '#' || line[0] == '\r' || line[0] == '\n')
		{
			fputs(line, newfile);
			continue;
		}

		//get the key and value from the line
		char* tok;
		tok = strtok(line, "=\r\n");
		if(tok == NULL)
			continue;
		//strip leading spaces
		while(*tok == ' ')
			++tok;
		if(*tok == '\0')
			continue;
		//strip trailing spaces
		char* end = tok + strlen(tok) - 1;
		while(end > tok && *end == ' ')
			--end;
		*(end+1) = '\0';
		string key = tok;
		tok = strtok(NULL, "=\r\n");
		if(tok == NULL)
			continue;
		//strip leading spaces
		while(*tok == ' ')
			++tok;
		if(*tok == '\0')
			continue;
		//strip trailing spaces
		end = tok + strlen(tok) - 1;
		while(end > tok && *end == ' ')
			--end;
		*(end+1) = '\0';
		string oldValue = tok;

		//keep lines unchanged if the parameter name doesn't match
		if(key != name)
		{
			fprintf(newfile, "%s = %s\n", key.c_str(), oldValue.c_str());
		}
		//if the name matches for the first time, replace this line with the new value; omit further matches
		else if(!foundKey)
		{
			fprintf(newfile, "%s = %s\n", name.c_str(), value.c_str());
			foundKey = true;
		}
	}
	//if the parameter didn't already exist, add it to the end of the file
	if(!foundKey)
	{
		fprintf(newfile, "%s = %s\n", name.c_str(), value.c_str());
	}

	delete[] line;
	fclose(originalFile);
	fclose(newfile);

	if(remove(filename.c_str()) != 0)
	{
		perror(filename.c_str());
		return;
	}
	if(rename(newfilename.c_str(), filename.c_str()) != 0)
	{
		perror(newfilename.c_str());
		return;
	}
	return;
}

void ConfigFile::setValueByName(const string& name, const char* format, ...)
{
	char* buf = new char[128];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, 128, format, args);
	setValueByName(name, string(buf));
	delete[] buf;
	va_end(args);
}

bool ConfigFile::isSet(const string& name)
{
	return(getValueByName(name) != "");
}
