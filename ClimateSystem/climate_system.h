#ifndef CLIMATE_SYSTEM
#define CLIMATE_SYSTEM

#include "convector.h"
#include "heater.h"
#include "conditioner.h"

enum ClimateSystemRegime
{
	  Ventilation = 0
	, Cooling
	, Heating
	, CLIMATE_SYSTEM_N_REGIMES
};

struct ClimateSystem
{
	enum ClimateSystemRegime regime;
	struct Convector convector;
	struct Heater heater;
	struct Conditioner conditioner;
};

struct ClimateSystemTransitionTableColumn
{
	enum ClimateSystemRegime if_dt_less_minus_1degree;
	enum ClimateSystemRegime if_dt_equal_minus_1degree;
	enum ClimateSystemRegime if_dt_equal_0degree;
	enum ClimateSystemRegime if_dt_equal_1degree;
	enum ClimateSystemRegime if_dt_greater_1degree;
};

struct ClimateSystemTransitionTable
{
	struct ClimateSystemTransitionTableColumn
		transitions[CLIMATE_SYSTEM_N_REGIMES];
};

#endif // CLIMATE_SYSTEM
