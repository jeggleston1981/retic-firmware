// Copyright (c) 2022 by James Eggleston
// Control sequence for 4 station system with master valve

#include <ReticSchedule.h>

ReticSchedule::ReticSchedule(int master_pin, int s1pin, int s2pin, int s3pin, int s4pin)
{

	_masterpin = master_pin;
	_pin1 = s1pin;
	_pin2 = s2pin;
	_pin3 = s3pin;
	_pin4 = s4pin;
	// Runtime of stations in minutes
	_s1time = 0;
	_s2time = 0;
	_s3time = 0;
	_s4time = 0;

	pinMode(_masterpin, OUTPUT);
	pinMode(_pin1, OUTPUT);
	pinMode(_pin2, OUTPUT);
	pinMode(_pin3, OUTPUT);
	pinMode(_pin4, OUTPUT);

	_previousTime = 0;
	_startFlag = false;
	_currentStation = 0;
	_program = false;
	_minute = 60000UL;
	_mode = 0; // We might not need this variable
}
/*************** Public methods **************/

void ReticSchedule::loop(void)
{

	if (_startFlag && _currentStation == 1)
		_s1();
	if (_startFlag && _currentStation == 2)
		_s2();
	if (_startFlag && _currentStation == 3)
		_s3();
	if (_startFlag && _currentStation == 4)
		_s4();
}
// To start program enter runtimes in minutes
void ReticSchedule::startProgram(int s1, int s2, int s3, int s4)
{

	_program = true;
	_startFlag = true;
	_currentStation = 1;
	_s1time = s1;
	_s2time = s2;
	_s3time = s3;
	_s4time = s4;
}
void ReticSchedule::startStation(int station, int time)
{

	_currentStation = station;
	_startFlag = true;
	_program = false;
	_previousTime = 0;
	if (_currentStation == 1)
		_s1time = time;
	if (_currentStation == 2)
		_s2time = time;
	if (_currentStation == 3)
		_s3time = time;
	if (_currentStation == 4)
		_s4time = time;
}

int ReticSchedule::checkStatus(void) // This will need to be modified to track program 1 and 2
{
	if (!_startFlag)
		return 0;
	if (_currentStation == 1 && !_program)
		return 1;
	if (_currentStation == 2 && !_program)
		return 2;
	if (_currentStation == 3 && !_program)
		return 3;
	if (_currentStation == 4 && !_program)
		return 4;
	return -1;
}
bool ReticSchedule::checkStationStatus(int station)
{
	if (_currentStation == station && _startFlag)
		return true;
	else
		return false;
}
bool ReticSchedule::step(void)
{
	if (_currentStation == 0)
		return false;
	if (!_program)
		return false;
	if (_currentStation == 1)
		_previousTime = millis() + _s1time;
	if (_currentStation == 2)
		_previousTime = millis() + _s2time;
	if (_currentStation == 3)
		_previousTime = millis() + _s3time;
	if (_currentStation == 4)
		_previousTime = millis() + _s4time;
	return true;
}
void ReticSchedule::toggle(int station, int time)
{
	if (_startFlag)
	{
		stop();
	}
	else
	{
		startStation(station, time);
	}
}
void ReticSchedule::stop(void)
{
	_currentStation = 0;
	_previousTime = 0;
	_startFlag = false;
	_program = false;
	digitalWrite(_masterpin, LOW);
	digitalWrite(_pin1, LOW);
	digitalWrite(_pin2, LOW);
	digitalWrite(_pin3, LOW);
	digitalWrite(_pin4, LOW);
}
int ReticSchedule::returnMode(void){
	return _mode;
}
void ReticSchedule::resetMode(void){
	_mode = 0;
}

void ReticSchedule::_s1(void)
{

	if (digitalRead(_pin1) == LOW)
	{
		digitalWrite(_masterpin, HIGH);
		digitalWrite(_pin1, HIGH);
		_previousTime = millis();
		_mode = 1;
	}
	else if (_previousTime + (_s1time * _minute) <= millis())
	{
		if (!_program)
		{
			digitalWrite(_masterpin, LOW);
			digitalWrite(_pin1, LOW);
			_currentStation = 0;
			_mode = 1;
			_startFlag = false;
			_previousTime = 0;
		}
		else
		{
			_currentStation = 2;
			digitalWrite(_pin1, LOW);
			_previousTime = 0;
			_mode = 1;
		}
	}
}

void ReticSchedule::_s2(void)
{

	if (!digitalRead(_masterpin))
		digitalWrite(_masterpin, HIGH);
	if (digitalRead(_pin2) == LOW)
	{
		digitalWrite(_pin2, HIGH);
		_previousTime = millis();
		_mode = 1;
	}
	else if (_previousTime + (_s2time * _minute) <= millis())
	{
		if (!_program)
		{
			digitalWrite(_masterpin, LOW);
			digitalWrite(_pin2, LOW);
			_currentStation = 0;
			_mode = 1;
			_startFlag = false;
			_previousTime = 0;
		}
		else
		{
			_currentStation = 3;
			digitalWrite(_pin2, LOW);
			_previousTime = 0;
			_mode = 1;
		}
	}
}

void ReticSchedule::_s3(void)
{

	if (!digitalRead(_masterpin))
		digitalWrite(_masterpin, HIGH);
	if (digitalRead(_pin3) == LOW)
	{
		digitalWrite(_pin3, HIGH);
		_previousTime = millis();
		_mode = 1;
	}
	else if (_previousTime + (_s3time * _minute) <= millis())
	{
		if (!_program)
		{
			digitalWrite(_masterpin, LOW);
			digitalWrite(_pin3, LOW);
			_currentStation = 0;
			_mode = 1;
			_startFlag = false;
			_previousTime = 0;
		}
		else
		{
			_currentStation = 4;
			digitalWrite(_pin3, LOW);
			_previousTime = 0;
			_mode = 1;
		}
	}
}

void ReticSchedule::_s4(void)
{

	if (!digitalRead(_masterpin))
		digitalWrite(_masterpin, HIGH);
	if (digitalRead(_pin4) == LOW && _mode == 1)
	{
		digitalWrite(_pin4, HIGH);
		_previousTime = millis();
		_mode = 1;
	}
	else if (_previousTime + (_s4time * _minute) <= millis())
	{

		digitalWrite(_masterpin, LOW);
		digitalWrite(_pin4, LOW);
		_currentStation = 0;
		_mode = 1;
		_startFlag = false;
		_program = false;
		_previousTime = 0;
	}
}
