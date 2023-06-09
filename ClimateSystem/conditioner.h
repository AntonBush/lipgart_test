#ifndef CONDITIONER
#define CONDITIONER

#include "degree_t.h"

// ����� ������ ����������
enum CondenserRegime
{
      Condenser0
    , Condenser33
    , Condenser66
    , Condenser100
    , CONDENSER_N_REGIMES
};

// ��������� ������ �� �����
inline enum CondenserRegime condenserRegimeFromInt(int i)
{
    switch (i)
    {
    case 0   : return Condenser0;
    case 33  : return Condenser33;
    case 66  : return Condenser66;
    case 100 : return Condenser100;
    default  : return CONDENSER_N_REGIMES;
    }
}

// �������������� ������ � �����
inline int condenserRegimeToInt(enum CondenserRegime regime)
{
    switch (regime)
    {
    case Condenser0   : return 0;
    case Condenser33  : return 33;
    case Condenser66  : return 66;
    case Condenser100 : return 100;
    case CONDENSER_N_REGIMES:
    default           : return -1;
    }
}

// dt = delta_temperature = target_t - current_t
// dt - ���������� �� �������
#define CONDENSER_TRANSITION_TABLE_COLUMN_SIZE 5
struct CondenserTransitionTableColumn
{
    enum CondenserRegime if_dt_less_minus_3degree;
    enum CondenserRegime if_dt_equal_minus_3degree;
    enum CondenserRegime if_dt_equal_minus_2degree;
    enum CondenserRegime if_dt_equal_minus_1degree;
    enum CondenserRegime if_dt_greater_minus_1degree;
};

// ��������� - ��� �������� �������
struct Condenser
{
    enum CondenserRegime regime;
    struct CondenserTransitionTableColumn
        transition_table[CONDENSER_N_REGIMES];
};


// ����� ������ �����������
enum CompressorRegime
{
      Compressor0
    , Compressor50
    , Compressor100
    , COMPRESSOR_N_REGIMES
};

// ��������� ������ �� �����
inline enum CompressorRegime compressorRegimeFromInt(int i)
{
    switch (i)
    {
    case 0   : return Compressor0;
    case 50  : return Compressor50;
    case 100 : return Compressor100;
    default  : return COMPRESSOR_N_REGIMES;
    }
}

// �������������� ������ � �����
inline int compressorRegimeToInt(enum CompressorRegime regime)
{
    switch (regime)
    {
    case Compressor0   : return 0;
    case Compressor50  : return 50;
    case Compressor100 : return 100;
    case COMPRESSOR_N_REGIMES:
    default            : return -1;
    }
}

// ���������� - ��� �������� �������
struct Compressor
{
    enum CompressorRegime regime;
    enum CompressorRegime
        transition_table[COMPRESSOR_N_REGIMES][CONDENSER_N_REGIMES];
};


// ����������� ������� ��
// * ����������
// * �����������
struct Conditioner
{
    struct Condenser condenser;
    struct Compressor compressor;
};

// �������� ��������� ������������ ������������
struct Conditioner * transitConditioner( struct Conditioner * conditioner
                                       , degrees_t dt                     );

#endif // CONDITIONER
