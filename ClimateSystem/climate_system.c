#include "climate_system.h"

#include <stddef.h>

// Псевдоним для типа столбца таблицы переходов
typedef struct ClimateSystemTransitionTableColumn column_t;

// Функция получения столюца таблицы переходов,
// соответствующего текущему режимы работы
static column_t * nextStateColumn(struct ClimateSystem * system)
{
    return &(system->transition_table[system->regime]);
}

// Основная процедура переключения
struct ClimateSystem * transitClimateSystem( struct ClimateSystem * system
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

    // Конвектор работает вне зависимости
    // от режима климатической системы
    transitConvector(&(system->convector), dt);

    if (system->regime == Cooling)
    {
        transitConditioner(&(system->conditioner), dt);
    }
    // В общем случае необходимо отключать
    // кондиционер, когда нет охлаждения
    else
    {
        transitConditioner(&(system->conditioner), 0);
    }

    if (system->regime == Heating)
    {
        transitHeater(&(system->heater), dt);
    }
    // В общем случае необходимо отключать
    // отопитель, когда нет отопления
    else
    {
        transitHeater(&(system->heater), 0);
    }

    return system;
}
