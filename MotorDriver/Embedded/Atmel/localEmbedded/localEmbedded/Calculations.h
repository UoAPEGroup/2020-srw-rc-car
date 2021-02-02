 /*
 * Calculations.h
 *
 * Created: 18/12/2020 12:06:08 pm
 *  Author: yxin683
 */ 


#ifndef CALCULATIONS_H_
#define CALCULATIONS_H_

#include <stdint.h>
#include <stdbool.h>

//variables
static volatile uint8_t finalOnTime;
static volatile uint8_t leftOnTime;
static volatile uint8_t rightOnTime;

static volatile uint16_t inputV;
static volatile uint16_t inputI;
static volatile uint16_t averageV;
static volatile uint16_t averageI;
static volatile uint32_t averagePower;
static volatile uint16_t speedGrade;
static volatile uint16_t requiredSpeedGrade; 
static volatile bool forward;

static volatile uint16_t voltageValues;
static volatile uint16_t currentValues;

static volatile uint8_t ADCReadingsCount = 0;

//FLAGS
extern volatile bool arrayFull;
extern volatile bool sendData;

//functions
uint8_t returnLeftOnTime();
uint8_t returnRightOnTime();
uint8_t returnFinalPeriod();
uint16_t returnSpeedGrade();
uint16_t returnInputI();
uint16_t returnInputV();
uint16_t returnAvgV();
uint16_t returnAvgI();
uint32_t returnAvgP();
bool returnDirection();

void ramp();
void updateDutyCycle();
void setInputV(uint16_t vinD);
void setSpeedGrade(uint16_t speed);
void setRequiredSpeedGrade(uint16_t newSpeed);
void setDirection(bool forward);
void addCurrent(uint16_t adcCurrentReading);
void addVoltage(uint16_t adcVoltageReading);
void convertVoltageAndCurrent();
void averageVoltageAndCurrent();

uint16_t adcConvert(uint16_t adcValue);


#endif /* CALCULATIONS_H_ */