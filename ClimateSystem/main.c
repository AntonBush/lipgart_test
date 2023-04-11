// ClimateSystem.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "string_utility.h"
#include "load_climate_system.h"

#include <stdio.h>

#define INPUT_FILE "in.txt"
#define OUTPUT_FILE "out.txt"

int main(void)
{
    puts("Test task \"Climate management\"");

    struct ClimateSystem system;
    int error = loadClimateSystem(&system);
    if (error != 0)
    {
        puterr("Error while loading climate system.");
        return error;
    }

    FILE * in = NULL;
    FILE * out = NULL;

    error = fopen_s(&in, INPUT_FILE, "r");
    error |= fopen_s(&out, OUTPUT_FILE, "a");

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

    // work

    fclose(in);
    fclose(out);

    puts("Application was completed.");
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
