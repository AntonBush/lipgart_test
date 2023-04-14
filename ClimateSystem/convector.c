#include "convector.h"

#include <stddef.h>

// Псевдоним для типа столбца таблицы переходов
typedef struct ConvectorTransitionTableColumn column_t;

// Функция получения столюца таблицы переходов,
// соответствующего текущему режимы работы
static column_t * nextStateColumn(struct Convector * convector)
{
    return &(convector->transition_table[convector->regime]);
}

// Основная процедура переключения
struct Convector * transitConvector( struct Convector * convector
                                   , degrees_t dt                 )
{
    if (convector == NULL)
    {
        return NULL;
    }

    column_t * column = nextStateColumn(convector);

    // Получение модуля отклонения от уставки
    degrees_t abs_dt = (dt < 0) ? -dt : dt;

    if (abs_dt == 0)
    {
        convector->regime = column->if_abs_dt_equal_0degree;
    }
    else if (abs_dt == 1)
    {
        convector->regime = column->if_abs_dt_equal_1degree;
    }
    else if (abs_dt == 2)
    {
        convector->regime = column->if_abs_dt_equal_2degree;
    }
    else if (abs_dt == 3)
    {
        convector->regime = column->if_abs_dt_equal_3degree;
    }
    else
    {
        convector->regime = column->if_abs_dt_greater_3degree;
    }

    return convector;
}
