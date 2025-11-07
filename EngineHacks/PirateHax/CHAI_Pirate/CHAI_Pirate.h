#if !defined(MODERN) || !MODERN
#  define STRUCT_PAD(from, to) unsigned char _pad_ ## from[(to) - (from)]
#else
#  define STRUCT_PAD(from, to)
#endif

struct EventEngineProc
{
    /* 00 */ PROC_HEADER;
    /* 2C */ void (*pCallback)(struct EventEngineProc*);

    /* 30 */ const u16* pEventStart;
    /* 34 */ const u16* pEventIdk;
    /* 38 */ const u16* pEventCurrent;

    /* 3C */ u16 evStateBits;
    /* 3E */ u16 evStallTimer;

    /* 40 */ s8 overwrittenTextSpeed;
    /* 41 */ u8 execType;
    /* 42 */ u8 activeTextType;
    /* 43 */ u8 chapterIndex;

    /* 44 */ u16 mapSpritePalIdOverride;

    /* 46 */ STRUCT_PAD(0x46, 0x48);

    /* 48 */ const struct UnitDefinition* pUnitLoadData;
    /* 4C */ s16 unitLoadCount;
    
    /* 4E */ u8  chance;

    /* 4F */ u8 unitLoadParameter : 7;
    /* 4F */ u8 diable_REDA : 1;

    /* 50 */ STRUCT_PAD(0x50, 0x54);
    /* 54 */ struct Unit *unit;
};

enum event_sub_cmd_idx  {
        /* EV_CMD_CHANGEAI */
    EVTSUBCMD_CHAI = 0,
    EVTSUBCMD_CHAI_AT = 1,
};

enum EventCommandReturnCode
{
    EVC_ADVANCE_CONTINUE = 0,
    EVC_STOP_CONTINUE    = 1,

    EVC_ADVANCE_YIELD    = 2,
    EVC_STOP_YIELD       = 3,
    
    EVC_UNK4             = 4,
    EVC_END              = 5,
    EVC_ERROR            = 6
};

u8 Event39_CHAI(struct EventEngineProc * proc);

void ChangeAiForCharacter(u8 pid, u8 ai1, u8 ai2, u8 ai3, u8 ai4);
void ChangeUnitAi(struct Unit * unit, u8 ai1, u8 ai2, u8 ai3, u8 ai4);

#define EVT_SUB_CMD(scr) (*((const u8 *)(scr)) & 0xF)
#define EVT_CMD_ARGV(scr) ((const s16 *)(scr) + 1)

enum
{
    AI_A_00 = 0x00,
    AI_A_01 = 0x01,
    AI_A_02 = 0x02,
    AI_A_03 = 0x03,
    AI_A_04 = 0x04,
    AI_A_05 = 0x05,
    AI_A_06 = 0x06,
    AI_A_07 = 0x07,
    AI_A_08 = 0x08,
    AI_A_09 = 0x09,
    AI_A_0A = 0x0A,
    AI_A_0B = 0x0B,
    AI_A_0C = 0x0C,
    AI_A_0D = 0x0D,
    AI_A_0E = 0x0E,
    AI_A_0F = 0x0F,
    AI_A_10 = 0x10,
    AI_A_11 = 0x11,
    AI_A_12 = 0x12,
    AI_A_13 = 0x13,
    AI_A_14 = 0x14,

    AI_A_INVALID
};

enum
{
    AI_B_00 = 0x00,
    AI_B_01 = 0x01,
    AI_B_02 = 0x02,
    AI_B_03 = 0x03,
    AI_B_04 = 0x04,
    AI_B_05 = 0x05,
    AI_B_06 = 0x06,
    AI_B_07 = 0x07,
    AI_B_08 = 0x08,
    AI_B_09 = 0x09,
    AI_B_0A = 0x0A,
    AI_B_0B = 0x0B,
    AI_B_0C = 0x0C,
    AI_B_0D = 0x0D,
    AI_B_0E = 0x0E,
    AI_B_0F = 0x0F,
    AI_B_10 = 0x10,
    AI_B_11 = 0x11,
    AI_B_12 = 0x12,

    AI_B_INVALID
};

enum
{
    // Unit::aiFlags
    AI_UNIT_FLAG_0 = (1 << 0),
    AI_UNIT_FLAG_1 = (1 << 1),
    AI_UNIT_FLAG_2 = (1 << 2),
    AI_UNIT_FLAG_3 = (1 << 3),
    AI_UNIT_FLAG_4 = (1 << 4),
    AI_UNIT_FLAG_5 = (1 << 5),
    AI_UNIT_FLAG_6 = (1 << 6),
};

enum
{
    // "ai3"

    // 0~2 healing ai theshold select
    AI_UNIT_CONFIG_HEALTHRESHOLD_SHIFT = 0,
    AI_UNIT_CONFIG_HEALTHRESHOLD_BITS = 3,
    AI_UNIT_CONFIG_HEALTHRESHOLD_MASK = ((1 << AI_UNIT_CONFIG_HEALTHRESHOLD_BITS) - 1) << AI_UNIT_CONFIG_HEALTHRESHOLD_SHIFT,

    // 3~7 combat target decide weight set select
    AI_UNIT_CONFIG_COMBATWEIGHT_SHIFT = 3,
    AI_UNIT_CONFIG_COMBATWEIGHT_BITS = 5,
    AI_UNIT_CONFIG_COMBATWEIGHT_MASK = ((1 << AI_UNIT_CONFIG_COMBATWEIGHT_BITS) - 1) << AI_UNIT_CONFIG_COMBATWEIGHT_SHIFT,

    // "ai4"

    // 8+

    // 13
    AI_UNIT_CONFIG_FLAG_STAY = 1 << 13,
};