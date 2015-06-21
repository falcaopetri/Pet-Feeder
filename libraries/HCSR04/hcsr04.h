#pragma once
#include <Arduino.h>

class HCSR04 {
private:
	int _pinTrigger;
	int _pinEcho;

	int _rangeLower;
	int _rangeUpper;

public:
	HCSR04 (int pinTrigger, int _pinEcho);
	void setup ();
	void setRange (int lower, int upper);
	long getDistance ();

};