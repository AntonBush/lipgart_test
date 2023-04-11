#ifndef CONVECTOR
#define CONVECTOR

#include "degree_t.h"

/*
    Cигнал ШИМ, если PWM = 30%:

    /|\
     |
     |  |<PWM>|
     |  |     |
    1|  +-----+             +-----+
     |  |     |             |     |
    0|--+     +-------------+     +-------------
     |  |                   |
     |  |<------100%------->|
     |
       ----------------------------------------------> Время

    Cигнал ШИМ, если PWM = 70%:

    /|\
     |
     |  |<----PWM---->|
     |  |             |
    1|  +-------------+     +-------------+
     |  |             |     |             |
    0|--+             +-----+             +-----
     |  |                   |
     |  |<------100%------->|
     |
       ----------------------------------------------> Время
*/
enum Pwm
{
      Pwm0
    , Pwm33
    , Pwm66
    , Pwm100
    , PWM_COUNT
};

inline enum Pwm pwmFromInt(int i)
{
    switch (i)
    {
    case 0   : return Pwm0;
    case 33  : return Pwm33;
    case 66  : return Pwm66;
    case 100 : return Pwm100;
    default  : return PWM_COUNT;
    }
}

inline int pwmToInt(enum Pwm pwm)
{
    switch (pwm)
    {
    case Pwm0   : return 0;
    case Pwm33  : return 33;
    case Pwm66  : return 66;
    case Pwm100 : return 100;
    case PWM_COUNT:
    default     : return -1;
    }
}

struct ConvectorTransitionTableColumn
{
    enum Pwm if_abs_dt_equal_0degree;
    enum Pwm if_abs_dt_equal_1degree;
    enum Pwm if_abs_dt_equal_2degree;
    enum Pwm if_abs_dt_equal_3degree;
    enum Pwm if_abs_dt_greater_3degree;
};

struct Convector
{
    enum Pwm regime;
    struct ConvectorTransitionTableColumn
        transition_table[PWM_COUNT];
};

struct Convector * transitConvector(
      struct Convector * convector
    , degrees_t dt                 );

#endif // CONVECTOR
