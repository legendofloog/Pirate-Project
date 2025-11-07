extern ProcInstruction* gSoundRoomUIProc; // FE8U! 0x8A21338

// for PROC_BODY
enum{
	CURRENT_SONG_INDEX = 0xC
};

typedef struct SoundRoomSongEntry SoundRoomSongEntry;

struct SoundRoomSongEntry{
	/* 00 */ u32 number;
	/* 04 */ u32 length;
	/* 08 */ void* displayFunction;
	/* 0C */ u32 nameTextId;
};

extern SoundRoomSongEntry SoundRoomSongTable[];

typedef struct SoundRoomSourceEntry SoundRoomSourceEntry;

struct SoundRoomSourceEntry{
	/* 00 */ u32 number;
	/* 04 */ u16 sourceTextId;
	/* 06 */ u16 unk6;
};

extern SoundRoomSourceEntry SoundRoomSourceTable[];

struct SoundRoomProc
{
    /* 00 */ PROC_HEADER;

    /* 29 */ u8 unk_29; // maybe padding?
    /* 2A */ u16 bgYOffset;
    /* 2C */ u16 currentSongTime;
    /* 2E */ u8 unk_2e;
    /* 2F */ u8 unk_2f;
    /* 30 */ s8 isSongPlaying;
    /* 31 */ u8 shuffleIndex;
    /* 32 */ s8 currentSongIdx;
    /* 33 */ u8 playableSongs;
    /* 34 */ u8 completionPercent;
    /* 35 */ u8 curIndex;
    /* 36 */ u8 totalSongs;
    /* 37 */ s8 unk_37;
    /* 38 */ u8 unk_38[3];
    /* 39 */ u8 unk_39; // pad?
    /* 3A */ u8 unk_3a;
    /* 3B */ u8 unk_3b;
    /* 3C */ s8 unk_3c;
    /* 3D */ s8 unk_3d;
    /* 3E */ s8 unk_3e;
    /* 3F */ u8 unk_3f;
    /* 40 */ u32 flags[4];
};