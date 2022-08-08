// Arduino Scheduling Sequence for a reticulation program
// https://github.com/jeggleston81
// Copyright (C) 2022 by James Eggleston
// ADD LICENCE TYPE HERE
//
// Helper library for Electricians

#ifndef RETICSCHEDULE_H
#define RETICSCHEDULE_H

#include <Arduino.h>

class ReticSchedule
{
private:
	int _masterpin;
	int _pin1;
	int _pin2;
	int _pin3;
	int _pin4;

	int _s1time;
	int _s2time;
	int _s3time;
	int _s4time;
	struct Times
	{
		int s1;
		int s2;
		int s3;
		int s4;
	};

	unsigned long _previousTime;
	unsigned long _minute;
	bool _program;
	bool _startFlag;
	int _currentStation;
	int _mode;

	void _s1(void);
	void _s2(void);
	void _s3(void);
	void _s4(void);

public:
	ReticSchedule(int master_pin, int s1pin, int s2pin, int s3pin, int s4pin);
	void loop(void);
	void startProgram(int s1, int s2, int s3, int s4);
	void startStation(int station, int time);
	bool step(void);
	void stop(void);
	int checkStatus(void);
	void toggle(int station, struct Times *savedTimes);
	bool checkStationStatus(int station);
};
#endif