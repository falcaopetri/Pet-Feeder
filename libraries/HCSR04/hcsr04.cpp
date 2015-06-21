#include "hcsr04.h"

HCSR04::HCSR04 (int pinTrigger, int pinEcho) 
	: _pinTrigger(pinTrigger), _pinEcho(pinEcho)
{
	setRange(0, 200);
}

void HCSR04::setup() {
	pinMode(_pinTrigger, OUTPUT);
	pinMode(_pinEcho, INPUT);
}

void HCSR04::setRange (int lower, int upper) {
	_rangeLower = lower;
	_rangeUpper = upper;
}

long HCSR04::getDistance () {
	digitalWrite(_pinTrigger, LOW);
	delayMicroseconds(2);
	digitalWrite(_pinTrigger, HIGH);
	delayMicroseconds(10);
	digitalWrite(_pinTrigger, LOW);
	long duration = pulseIn(_pinEcho, HIGH);
	long distance = (duration/2.0) / 29.1;

	if (distance < _rangeLower || distance > _rangeUpper)
		distance = -1;
  	return distance;
}