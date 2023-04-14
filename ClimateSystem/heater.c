#include "heater.h"

#include <stddef.h>

// Псевдоним для типа столбца таблицы переходов
typedef struct HeaterTransitionTableColumn column_t;

// Функция получения столюца таблицы переходов,
// соответствующего текущему режимы работы
static column_t * nextStateColumn(struct Heater * heater)
{
    return &(heater->transition_table[heater->regime]);
}

// Основная процедура переключения
struct Heater * transitHeater( struct Heater * heater
                             , degrees_t dt           )
{
    if (heater == NULL)
    {
        return NULL;
    }

    column_t * column = nextStateColumn(heater);

    if (dt < 1)
    {
        heater->regime = column->if_dt_less_1degree;
    }
    else if (dt == 1)
    {
        heater->regime = column->if_dt_equal_1degree;
    }
    else if (dt == 2)
    {
        heater->regime = column->if_dt_equal_2degree;
    }
    else if (dt == 3)
    {
        heater->regime = column->if_dt_equal_3degree;
    }
    else
    {
        heater->regime = column->if_dt_greater_3degree;
    }

    return heater;
}
