#include "conditioner.h"

#include <stddef.h>

// Процедура переключения конденсера
static void transitCondenser( struct Conditioner * conditioner
                            , degrees_t dt                     );
// Процедура переключения компрессора
static void transitCompressor(struct Conditioner * conditioner);


// Основная процедура переключения
struct Conditioner * transitConditioner( struct Conditioner * conditioner
                                       , degrees_t dt                     )
{
    if (conditioner == NULL)
    {
        return NULL;
    }
    
    transitCondenser(conditioner, dt);
    transitCompressor(conditioner);

    return conditioner;
}

// Псевдоним для типа столбца таблицы переходов
typedef struct CondenserTransitionTableColumn condenser_column_t;

// Функция получения столюца таблицы переходов,
// соответствующего текущему режимы работы
static condenser_column_t * nextCondenserStateColumn(struct Condenser * condenser)
{
    return &(condenser->transition_table[condenser->regime]);
}

// Процедура переключения конденсера
void transitCondenser( struct Conditioner * conditioner
                     , degrees_t dt                     )
{
    struct Condenser * condenser = &(conditioner->condenser);
    condenser_column_t * column = nextCondenserStateColumn(condenser);

    if (dt < -3)
    {
        condenser->regime = column->if_dt_less_minus_3degree;
    }
    else if (dt == -3)
    {
        condenser->regime = column->if_dt_equal_minus_3degree;
    }
    else if (dt == -2)
    {
        condenser->regime = column->if_dt_equal_minus_2degree;
    }
    else if (dt == -1)
    {
        condenser->regime = column->if_dt_equal_minus_1degree;
    }
    else
    {
        condenser->regime = column->if_dt_greater_minus_1degree;
    }
}

// Процедура переключения компрессора
void transitCompressor(struct Conditioner * conditioner)
{
    struct Condenser * condenser = &(conditioner->condenser);
    struct Compressor * compressor = &(conditioner->compressor);

    compressor->regime
        = compressor->transition_table[compressor->regime][condenser->regime];
}
