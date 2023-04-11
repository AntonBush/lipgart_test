#ifndef HEATER
#define HEATER

#include "degree_t.h"

enum HeaterRegime
{
      Heater0
    , Heater33
    , Heater66
    , Heater100
    , HEATER_N_REGIMES
};

inline enum HeaterRegime heaterRegimeFromInt(int i)
{
    switch (i)
    {
    case 0   : return Heater0;
    case 33  : return Heater33;
    case 66  : return Heater66;
    case 100 : return Heater100;
    default  : return HEATER_N_REGIMES;
    }
}

inline int heaterRegimeToInt(enum HeaterRegime regime)
{
    switch (regime)
    {
    case Heater0   : return 0;
    case Heater33  : return 33;
    case Heater66  : return 66;
    case Heater100 : return 100;
    case HEATER_N_REGIMES:
    default        : return -1;
    }
}

struct HeaterTransitionTableColumn
{
    enum CondenserRegime if_dt_less_1degree;
    enum CondenserRegime if_dt_equal_1degree;
    enum CondenserRegime if_dt_equal_2degree;
    enum CondenserRegime if_dt_equal_3degree;
    enum CondenserRegime if_dt_greater_3degree;
};

struct Heater
{
    enum HeaterRegime regime;
    struct HeaterTransitionTableColumn
        transition_table[HEATER_N_REGIMES];
};

struct Heater * transitHeater( struct Heater * heater
                             , degrees_t dt           );

#endif // HEATER
