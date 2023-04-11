#include "climate_system.h"

#include <stddef.h>

typedef struct ClimateSystemTransitionTableColumn column_t;

static column_t * nextStateColumn(struct ClimateSystem * system)
{
    return &(system->transition_table[system->regime]);
}

struct ClimateSystem * transitClimateSystem(
      struct ClimateSystem * system
    , degrees_t dt                  )
{
    if (system == NULL)
    {
        return NULL;
    }

    column_t * column = nextStateColumn(system);

    if (dt < -1)
    {
        system->regime = column->if_dt_less_minus_1degree;
    }
    else if (dt == -1)
    {
        system->regime = column->if_dt_equal_minus_1degree;
    }
    else if (dt == 0)
    {
        system->regime = column->if_dt_equal_0degree;
    }
    else if (dt == 1)
    {
        system->regime = column->if_dt_equal_1degree;
    }
    else
    {
        system->regime = column->if_dt_greater_1degree;
    }

    transitConvector(&(system->convector), dt);

    if (system->regime == Cooling)
    {
        transitConditioner(&(system->conditioner), dt);
    }
    else
    {
        transitConditioner(&(system->conditioner), 0);
    }

    if (system->regime == Heating)
    {
        transitHeater(&(system->heater), dt);
    }
    else
    {
        transitHeater(&(system->heater), 0);
    }

    return system;
}
