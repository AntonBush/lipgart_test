#include "conditioner.h"

#include <stddef.h>

static void transitCondenser(
      struct Conditioner * conditioner
    , degrees_t dt                     );
static void transitCompressor(struct Conditioner * conditioner);


struct Conditioner * transitConditioner(
      struct Conditioner * conditioner
    , degrees_t dt)
{
    if (conditioner == NULL)
    {
        return NULL;
    }
    
    transitCondenser(conditioner, dt);
    transitCompressor(conditioner);

    return conditioner;
}

typedef struct CondenserTransitionTableColumn condenser_column_t;

static condenser_column_t * nextCondenserStateColumn(
    struct Condenser * condenser)
{
    return &(condenser->transition_table[condenser->regime]);
}

void transitCondenser(
      struct Conditioner * conditioner
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

void transitCompressor(struct Conditioner * conditioner)
{
    struct Condenser * condenser = &(conditioner->condenser);
    struct Compressor * compressor = &(conditioner->compressor);

    enum CompressorRegime ** transition_table
        = compressor->transition_table;

    compressor->regime
        = transition_table[condenser->regime][compressor->regime];
}
