/*
 * TimeDelayConfig.cpp
 *
 *  Created on: 3 Apr 2018
 *      Author: pi
 */

#include "global.h"

TimeDelayConfig globalTimeDelayConfig;

void timeDelayInit()
{
	globalTimeDelayConfig.resetPulseLength = 10;
	globalTimeDelayConfig.stepPulseLength = 2;
	globalTimeDelayConfig.waitAfterEachMovement = 0;
}


const TimeDelayConfig& getTimeDelayConfig()
{
	return globalTimeDelayConfig;
}


void setTimeDelayConfig(TimeDelayConfig &config)
{
	globalTimeDelayConfig = config;
}
