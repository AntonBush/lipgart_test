#include "string_utility.h"
#include "load_climate_system_tt.h"

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

static void initClimateSystemRegimes(struct ClimateSystem * system)
{
    system->regime = Ventilation;
    system->convector.regime = Pwm0;
    system->heater.regime = Heater0;
    system->conditioner.condenser.regime = Condenser0;
    system->conditioner.compressor.regime = Compressor0;
}

struct Io
{
    FILE * in;
    FILE * out;
};

static void initIo(struct Io* io)
{
    io->in  = NULL;
    io->out = NULL;
}

static int openIo(struct Io * io);
static void closeIo(struct Io * io);
static void processInput(struct Io * io, struct ClimateSystem * system);

int main(void)
{
    puts("Test task \"Climate management\"");

    struct ClimateSystem system;
    initClimateSystemRegimes(&system);

    int error = loadClimateSystemTransitionTables(&system);
    if (error != 0)
    {
        puterr("Error while loading climate system.");
        return error;
    }

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

static void fillMessage(struct Message* message, struct ClimateSystem* system);
static void printMessage(FILE* stream, struct Message message);

void processInput(struct Io* io, struct ClimateSystem* system)
{
    struct Message message;
    message.iteration_no = 0;
    while (fscanf_s(io->in
        , "%d %d"
        , &(message.target_t)
        , &(message.current_t)) == 2)
    {
        transitClimateSystem(system
            , message.target_t - message.current_t);

        message.iteration_no += 1;
        fillMessage(&message, system);

        printMessage(io->out, message);
    }
}

void fillMessage(struct Message* message, struct ClimateSystem* system)
{
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
