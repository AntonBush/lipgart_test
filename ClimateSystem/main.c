// ClimateSystem.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "string_utility.h"
#include "load_climate_system.h"

#include <stdio.h>

#define INPUT_FILE "in.txt"
#define OUTPUT_FILE "out.txt"

struct Message
{
    int iteration_no;
    int target_t;
    int current_t;
    int fan;
    int heater;
    int compressor;
    int condenser;
};

static void printMessage(FILE * stream, struct Message message);

int main(void)
{
    puts("Test task \"Climate management\"");

    struct ClimateSystem system;
    system.regime = Ventilation;
    system.convector.regime = Pwm0;
    system.heater.regime = Heater0;
    system.conditioner.condenser.regime = Condenser0;
    system.conditioner.compressor.regime = Compressor0;

    int error = loadClimateSystem(&system);
    if (error != 0)
    {
        puterr("Error while loading climate system.");
        return error;
    }

    FILE * in = NULL;
    FILE * out = NULL;

    error = fopen_s(&in, INPUT_FILE, "r");
    error |= fopen_s(&out, OUTPUT_FILE, "w, ccs=UTF-8");

    if (in == NULL || out == NULL)
    {
        if (in == NULL)
        {
            puterr("Can not open input file ("INPUT_FILE").");
        }
        else
        {
            fclose(in);
        }

        if (out == NULL)
        {
            puterr("Can not open output file ("OUTPUT_FILE").");
        }
        else
        {
            fclose(out);
        }

        return -1;
    }

    struct Message message;
    message.iteration_no = 0;
    while (fwscanf_s( in
                    , L"%d %d"
                    , &(message.target_t)
                    , &(message.current_t)) == 2 )
    {
        transitClimateSystem( &system
                            , message.target_t - message.current_t );
        
        message.iteration_no += 1;
        message.fan        = pwmToInt(system.convector.regime);
        message.heater     = heaterRegimeToInt(system.heater.regime);
        message.compressor = compressorRegimeToInt(system.conditioner.compressor.regime);
        message.condenser  = condenserRegimeToInt(system.conditioner.condenser.regime);

        printMessage(out, message);
    }

    fclose(in);
    fclose(out);

    puts("Application was completed.");
}

void printMessage(FILE * stream, struct Message message)
{
    fwprintf_s(stream, L"=== Итерация %d ==="ENDL, message.iteration_no);
    fwprintf_s(stream, L"T уставки   = %d"ENDL,    message.target_t);
    fwprintf_s(stream, L"T салона    = %d"ENDL,    message.current_t);
    fwprintf_s(stream, L"Вентилятор  = %d"ENDL,    message.fan);
    fwprintf_s(stream, L"Отопитель   = %d"ENDL,    message.heater);
    fwprintf_s(stream, L"Компрессора = %d"ENDL,    message.compressor);
    fwprintf_s(stream, L"Конденсатор = %d"ENDL,    message.condenser);
}


/*
=== Итерация N ===
T уставки = value
T салона = value
Вентилятор = value
Отопитель = value
Компрессора = value
Конденсор = value
*/

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
