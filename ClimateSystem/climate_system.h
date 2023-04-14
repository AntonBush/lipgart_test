#ifndef CLIMATE_SYSTEM
#define CLIMATE_SYSTEM

#include "convector.h"
#include "heater.h"
#include "conditioner.h"

// Для обозначения режима работы используется перечисление
enum ClimateSystemRegime
{
      Ventilation = 0
    , Cooling
    , Heating
    // Число режимов работы климатической системы
    , CLIMATE_SYSTEM_N_REGIMES
};

// Получение режима из символа
inline enum ClimateSystemRegime climateSystemRegimeFromChar(char ch)
{
    switch (ch)
    {
    case 'v' : return Ventilation;
    case 'c' : return Cooling;
    case 'h' : return Heating;
    default  : return CLIMATE_SYSTEM_N_REGIMES;
    }
}

// Преобразование режима в символ
inline char climateSystemRegimeToChar(enum ClimateSystemRegime regime)
{
    switch (regime)
    {
    case Ventilation : return 'v';
    case Cooling     : return 'c';
    case Heating     : return 'h';
    case CLIMATE_SYSTEM_N_REGIMES:
    default          : return '?';
    }
}

// dt = delta_temperature = target_t - current_t
// dt - отклонение от уставки
#define CLIMATE_SYSTEM_TRANSITION_TABLE_COLUMN_SIZE 5
struct ClimateSystemTransitionTableColumn
{
    enum ClimateSystemRegime if_dt_less_minus_1degree;
    enum ClimateSystemRegime if_dt_equal_minus_1degree;
    enum ClimateSystemRegime if_dt_equal_0degree;
    enum ClimateSystemRegime if_dt_equal_1degree;
    enum ClimateSystemRegime if_dt_greater_1degree;
};

// Климатическая система - это конечный автомат.
// Климатическая система состоит из
// * конвектора
// * отопителя
// * кондиционера
struct ClimateSystem
{
    enum ClimateSystemRegime regime;
    struct ClimateSystemTransitionTableColumn
        transition_table[CLIMATE_SYSTEM_N_REGIMES];

    struct Convector convector;
    struct Heater heater;
    struct Conditioner conditioner;
};

// Основная процедура переключения климатической системы
struct ClimateSystem * transitClimateSystem( struct ClimateSystem * system
                                           , degrees_t dt                  );

#endif // CLIMATE_SYSTEM
