#include "string_utility.h"
// Загрузчик таблиц переходов климатической системы
#include "load_climate_system_tt.h"

#include <stdio.h>

// Пути файлов для ввода-вывода
#define INPUT_FILE "in.txt"
#define OUTPUT_FILE "out.txt"

// Сообщение для вывода результата итерации
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

// Установка начального состояния климатической системы
static void initClimateSystemRegimes(struct ClimateSystem * system)
{
    system->regime = Ventilation;
    system->convector.regime = Pwm0;
    system->heater.regime = Heater0;
    system->conditioner.condenser.regime = Condenser0;
    system->conditioner.compressor.regime = Compressor0;
}

// Структура для работы с вводом-выводом
struct Io
{
    FILE * in;
    FILE * out;
};

// Установка начального состояния структуры Io
static void initIo(struct Io* io)
{
    io->in  = NULL;
    io->out = NULL;
}

// Открытие файлов для ввода-вывода
static int openIo(struct Io * io);
// Закрытие файлов для ввода-вывода
static void closeIo(struct Io * io);
// Процедура эмуляции системы
static void processInput(struct Io * io, struct ClimateSystem * system);

int main(void)
{
    puts("Test task \"Climate management\"");

    struct ClimateSystem system;
    initClimateSystemRegimes(&system);

    // В климатическую систему загружаются таблицы
    // переходов из текстовых файлов
    int error = loadClimateSystemTransitionTables(&system);
    if (error != 0)
    {
        puterr("Error while loading climate system.");
        return error;
    }

    // Подготовка файлов ввода-вывода
    struct Io io;
    initIo(&io);
    error = openIo(&io);
    if (error != 0)
    {
        if (io.in == NULL)
        {
            puterr("Can not open input file ("INPUT_FILE").");
        }

        if (io.out == NULL)
        {
            puterr("Can not open output file ("OUTPUT_FILE").");
        }

        closeIo(&io);

        return error;
    }

    processInput(&io, &system);

    closeIo(&io);

    puts("Application was completed successfully.");
}

int openIo(struct Io * io)
{
    int error = fopen_s(&(io->in), INPUT_FILE, "r");
    // параметр css=UTF-8 необходим, потому что формат выходных
    // данных содержит кириллицу.
    // Так же необходимо добавлять флаг /utf-8 компилятору,
    // чтобы он воспринимал исходный текст программы как UNICODE.
    error |= fopen_s(&(io->out), OUTPUT_FILE, "w, ccs=UTF-8");

    if (io->in == NULL || io->out == NULL)
    {
        puterr("Error while openning input-output files.");
        return -1;
    }

    return 0;
}

void closeIo(struct Io * io)
{
    if (io->in != NULL)
    {
        fclose(io->in);
    }

    if (io->out != NULL)
    {
        fclose(io->out);
    }
}

// Процедура заполнения сообщения на основе
// информации о системе для дальнейшего вывода
static void fillMessage(struct Message* message, struct ClimateSystem* system);
// Процедура вывода сообщения
static void printMessage(FILE* stream, struct Message message);

// Основная процедура эмуляции системы
void processInput(struct Io* io, struct ClimateSystem* system)
{
    struct Message message;
    // Сообщение также использует для хранения
    // номера текущей итерации
    message.iteration_no = 0;
    // Читаем из файла пока можем прочитать два
    // целых числа
    while (fscanf_s(io->in
        , "%d %d"
        , &(message.target_t)
        , &(message.current_t)) == 2)
    {
        // Процедура переключения автомата климатической системы
        transitClimateSystem(system
            , message.target_t - message.current_t);

        message.iteration_no += 1;
        fillMessage(&message, system);

        printMessage(io->out, message);
    }
}

void fillMessage(struct Message* message, struct ClimateSystem* system)
{
    // Используются функции преобразования режимов в
    // целочисленный формат
    message->fan        = pwmToInt(system->convector.regime);
    message->heater     = heaterRegimeToInt(system->heater.regime);
    message->compressor = compressorRegimeToInt(system->conditioner.compressor.regime);
    message->condenser  = condenserRegimeToInt(system->conditioner.condenser.regime);
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
