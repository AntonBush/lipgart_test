#include "load_climate_system.h"
#include "string_utility.h"

#include <stdio.h>

#define CLIMATE_SYSTEM_TT_FILE "climate_system_transition_table.txt"
#define CONVECTOR_TT_FILE "convector_transition_table.txt"
#define HEATER_TT_FILE "heater_transition_table.txt"
#define CONDENSER_TT_FILE "condenser_transition_table.txt"
#define COMPRESSOR_TT_FILE "compressor_transition_table.txt"

struct TableTransitionFiles
{
    FILE * climate_system;
    FILE * convector;
    FILE * heater;
    FILE * condenser;
    FILE * compressor;
};

static int openTransitionTableFiles(struct TableTransitionFiles * files);
static void closeTransitionTableFiles(struct TableTransitionFiles * files);

static int fillTransitionTables( struct ClimateSystem * system
                               , struct TableTransitionFiles * files );

int loadClimateSystem(struct ClimateSystem * system)
{
    if (system == NULL)
    {
        puterr("loadClimateSystem: ClimateSystem argument is NULL.");
        return -1;
    }

    struct TableTransitionFiles files;
    files.climate_system = NULL;
    files.convector      = NULL;
    files.heater         = NULL;
    files.condenser      = NULL;
    files.compressor     = NULL;

    int error = openTransitionTableFiles(&files);
    if (error != 0)
    {
        puterr("Error while opening transition table files.");
        closeTransitionTableFiles(&files);
        return error;
    }

    error = fillTransitionTables(system, &files);
    closeTransitionTableFiles(&files);
    if (error != 0)
    {
        puterr("Error while filling transition tables.");
        return error;
    }

	return 0;
}

static FILE * open_file(string_t file_name)
{
    FILE * file;
    fopen_s(&file, file_name, "r");
    return file;
}

int openTransitionTableFiles(struct TableTransitionFiles * files)
{
    files->climate_system
        = open_file(CLIMATE_SYSTEM_TT_FILE);

    files->convector
        = open_file(CONVECTOR_TT_FILE);

    files->heater
        = open_file(HEATER_TT_FILE);

    files->condenser
        = open_file(CONDENSER_TT_FILE);
    files->compressor
        = open_file(COMPRESSOR_TT_FILE);

    if (files->climate_system == NULL)
    {
        puterr("Can not open climate system transition table ("CLIMATE_SYSTEM_TT_FILE").");
        return -1;
    }

    if (files->convector == NULL)
    {
        puterr("Can not open convector transition table ("CONVECTOR_TT_FILE").");
        return -1;
    }

    if (files->heater == NULL)
    {
        puterr("Can not open heater transition table ("HEATER_TT_FILE").");
        return -1;
    }

    if (files->condenser == NULL)
    {
        puterr("Can not open condenser transition table ("CONDENSER_TT_FILE").");
        return -1;
    }

    if (files->compressor == NULL)
    {
        puterr("Can not open compressor transition table ("COMPRESSOR_TT_FILE").");
        return -1;
    }

    return 0;
}

void closeTransitionTableFiles(struct TableTransitionFiles * files)
{
    if (files->climate_system != NULL) fclose(files->climate_system);
    if (files->convector != NULL)      fclose(files->convector);
    if (files->heater != NULL)         fclose(files->heater);
    if (files->condenser != NULL)      fclose(files->condenser);
    if (files->compressor != NULL)     fclose(files->compressor);
}

static int fillClimateSystemTransitionTable( struct ClimateSystem * system
                                           , FILE * file                   );
static int fillConvectorTransitionTable( struct Convector * convector
                                       , FILE * file                  );
static int fillHeaterTransitionTable( struct Heater * heater
                                    , FILE * file            );
static int fillCondenserTransitionTable( struct Condenser * condenser
                                       , FILE * file                  );
static int fillCompressorTransitionTable( struct Compressor * compressor
                                        , FILE * file                    );

int fillTransitionTables( struct ClimateSystem * system
                        , struct TableTransitionFiles * files )
{
    int error = fillClimateSystemTransitionTable(system, files->climate_system);
    if (error != 0)
    {
        puterr("Error while filling climate system transitional table.");
        return error;
    }

    error = fillConvectorTransitionTable(&(system->convector), files->convector);
    if (error != 0)
    {
        puterr("Error while filling convector transitional table.");
        return error;
    }

    error = fillHeaterTransitionTable(&(system->heater), files->heater);
    if (error != 0)
    {
        puterr("Error while filling heater transitional table.");
        return error;
    }

    error = fillCondenserTransitionTable(&(system->conditioner.condenser), files->condenser);
    if (error != 0)
    {
        puterr("Error while filling condenser transitional table.");
        return error;
    }

    error = fillCompressorTransitionTable(&(system->conditioner.compressor), files->compressor);
    if (error != 0)
    {
        puterr("Error while filling compressor transitional table.");
        return error;
    }

    return 0;
}

int fillClimateSystemTransitionTable( struct ClimateSystem * system
                                    , FILE * file                   )
{
    char climate_system_tt[5][CLIMATE_SYSTEM_N_REGIMES];
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < CLIMATE_SYSTEM_N_REGIMES; ++j)
        {
            char ch;
            int was_read = fscanf_s(file, " %c", &ch, 1);
            if (was_read == 0)
            {
                puterr("Error while reading transition table file.");
                return -1;
            }
            climate_system_tt[i][j] = ch;
        }
    }

    for (int i = 0; i < CLIMATE_SYSTEM_N_REGIMES; ++i)
    {
        system->transition_table[i].if_dt_less_minus_1degree
            = climateSystemRegimeFromChar(climate_system_tt[0][i]);
        system->transition_table[i].if_dt_equal_minus_1degree
            = climateSystemRegimeFromChar(climate_system_tt[1][i]);
        system->transition_table[i].if_dt_equal_0degree
            = climateSystemRegimeFromChar(climate_system_tt[2][i]);
        system->transition_table[i].if_dt_equal_1degree
            = climateSystemRegimeFromChar(climate_system_tt[3][i]);
        system->transition_table[i].if_dt_greater_1degree
            = climateSystemRegimeFromChar(climate_system_tt[4][i]);
    }

    return 0;
}

int fillConvectorTransitionTable( struct Convector * convector
                                , FILE * file                  )
{
    int convector_tt[5][PWM_COUNT];
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < PWM_COUNT; ++j)
        {
            int d;
            int was_read = fscanf_s(file, "%d", &d);
            if (was_read == 0)
            {
                puterr("Error while reading transition table file.");
                return -1;
            }
            convector_tt[i][j] = d;
        }
    }

    for (int i = 0; i < PWM_COUNT; ++i)
    {
        convector->transition_table[i].if_abs_dt_equal_0degree
            = pwmFromInt(convector_tt[0][i]);
        convector->transition_table[i].if_abs_dt_equal_1degree
            = pwmFromInt(convector_tt[1][i]);
        convector->transition_table[i].if_abs_dt_equal_2degree
            = pwmFromInt(convector_tt[2][i]);
        convector->transition_table[i].if_abs_dt_equal_3degree
            = pwmFromInt(convector_tt[3][i]);
        convector->transition_table[i].if_abs_dt_greater_3degree
            = pwmFromInt(convector_tt[4][i]);
    }

    return 0;
}

int fillHeaterTransitionTable( struct Heater * heater
                             , FILE * file            )
{
    int heater_tt[5][HEATER_N_REGIMES];
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < HEATER_N_REGIMES; ++j)
        {
            int d;
            int was_read = fscanf_s(file, "%d", &d);
            if (was_read == 0)
            {
                puterr("Error while reading transition table file.");
                return -1;
            }
            heater_tt[i][j] = d;
        }
    }

    for (int i = 0; i < HEATER_N_REGIMES; ++i)
    {
        heater->transition_table[i].if_dt_less_1degree
            = heaterRegimeFromInt(heater_tt[0][i]);
        heater->transition_table[i].if_dt_equal_1degree
            = heaterRegimeFromInt(heater_tt[1][i]);
        heater->transition_table[i].if_dt_equal_2degree
            = heaterRegimeFromInt(heater_tt[2][i]);
        heater->transition_table[i].if_dt_equal_3degree
            = heaterRegimeFromInt(heater_tt[3][i]);
        heater->transition_table[i].if_dt_greater_3degree
            = heaterRegimeFromInt(heater_tt[4][i]);
    }

    return 0;
}

int fillCondenserTransitionTable( struct Condenser * condenser
                                , FILE * file                  )
{
    int condenser_tt[5][CONDENSER_N_REGIMES];
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < CONDENSER_N_REGIMES; ++j)
        {
            int d;
            int was_read = fscanf_s(file, "%d", &d);
            if (was_read == 0)
            {
                puterr("Error while reading transition table file.");
                return -1;
            }
            condenser_tt[i][j] = d;
        }
    }

    for (int i = 0; i < CONDENSER_N_REGIMES; ++i)
    {
        condenser->transition_table[i].if_dt_less_minus_3degree
            = condenserRegimeFromInt(condenser_tt[0][i]);
        condenser->transition_table[i].if_dt_equal_minus_3degree
            = condenserRegimeFromInt(condenser_tt[1][i]);
        condenser->transition_table[i].if_dt_equal_minus_2degree
            = condenserRegimeFromInt(condenser_tt[2][i]);
        condenser->transition_table[i].if_dt_equal_minus_1degree
            = condenserRegimeFromInt(condenser_tt[3][i]);
        condenser->transition_table[i].if_dt_greater_minus_1degree
            = condenserRegimeFromInt(condenser_tt[4][i]);
    }

    return 0;
}

int fillCompressorTransitionTable( struct Compressor * compressor
                                 , FILE * file                    )
{
    for (int i = 0; i < CONDENSER_N_REGIMES; ++i)
    {
        for (int j = 0; j < COMPRESSOR_N_REGIMES; ++j)
        {
            int d;
            int was_read = fscanf_s(file, "%d", &d);
            if (was_read == 0)
            {
                puterr("Error while reading transition table file.");
                return -1;
            }
            compressor->transition_table[j][i]
                = compressorRegimeFromInt(d);
        }
    }

    return 0;
}
