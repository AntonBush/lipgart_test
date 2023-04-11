// ClimateSystem.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "string_utility.h"
#include "climate_system.h"

#include <stdio.h>

void puterr(string_t s)
{
    fputs(s, stderr);
}

struct TableTransitionFiles
{
    FILE * climate_system;
    FILE * convector;
    FILE * heater;
    FILE * condenser;
    FILE * compressor;
};

static int openTransitionTableFiles(struct TableTransitionFiles* files);
static void closeTransitionTableFiles(struct TableTransitionFiles * files);

static int fillTransitionTables( struct ClimateSystem * system
                               , struct TableTransitionFiles * files );

int main(void)
{
    puts("Test task \"Climate management\"");

    struct TableTransitionFiles files;
    files.climate_system = NULL;

    int error = openTransitionTableFiles(&files);
    if (error != 0)
    {
        puterr("Error while opening transition table files.");
        closeTransitionTableFiles(&files);
        return error;
    }

    struct ClimateSystem system;
    error = fillTransitionTables(&system, &files);
    closeTransitionTableFiles(&files);
    if (error != 0)
    {
        puterr("Error while filling transition tables.");
        return error;
    }

    // work

    puts("Application was completed.");
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
        = open_file("climate_system_transition_table.txt");

    files->convector
        = open_file("convector_transition_table.txt");

    files->heater
        = open_file("heater_transition_table.txt");

    files->condenser
        = open_file("condenser_transition_table.txt");
    files->compressor
        = open_file("compressor_transition_table.txt");

    if (files->climate_system == NULL)
    {
        puterr("Climate system transition table not found.");
        return -1;
    }

    if (files->convector == NULL)
    {
        puterr("Convector transition table not found.");
        return -1;
    }

    if (files->heater == NULL)
    {
        puterr("Heater transition table not found.");
        return -1;
    }

    if (files->condenser == NULL)
    {
        puterr("Condenser transition table not found.");
        return -1;
    }

    if (files->compressor == NULL)
    {
        puterr("Compressor transition table not found.");
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


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
