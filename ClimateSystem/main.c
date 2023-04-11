// ClimateSystem.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "string_utility.h"
#include "climate_system.h"

#include <stdio.h>

FILE * open_file(string_t file_name)
{
    FILE * file;
    fopen_s(&file, file_name, "r");
    return file;
}

int main(void)
{
    FILE * climate_system_transition_table_file
        = open_file("climate_system_transition_table.txt");

    FILE * convector_transition_table_file
        = open_file("convector_transition_table.txt");

    FILE * heater_transition_table_file
        = open_file("heater_transition_table.txt");

    FILE * condenser_transition_table_file
        = open_file("condenser_transition_table.txt");
    FILE * compressor_transition_table_file
        = open_file("compressor_transition_table.txt");

    if (climate_system_transition_table_file == NULL)
    {
        puts("Climate system transition table not found.");
        return -1;
    }

    if (convector_transition_table_file == NULL)
    {
        puts("Convector transition table not found.");
        return -1;
    }

    if (heater_transition_table_file == NULL)
    {
        puts("Heater transition table not found.");
        return -1;
    }

    if (condenser_transition_table_file == NULL)
    {
        puts("Condenser transition table not found.");
        return -1;
    }

    if (compressor_transition_table_file == NULL)
    {
        puts("Compressor transition table not found.");
        return -1;
    }

    struct ClimateSystem system;

    system.transition_table;
    system.convector.transition_table;
    system.heater.transition_table;
    system.conditioner.condenser.transition_table;
    system.conditioner.compressor.transition_table;

    puts("End");
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
