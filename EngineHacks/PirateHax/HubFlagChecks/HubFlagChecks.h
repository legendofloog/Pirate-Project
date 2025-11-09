bool CheckEventId(int eventId);
void SetEventId(int eventId);
void UnsetEventId(int eventId);
void SetTalkNumber(int number);

enum
{
    CHAPTER_FLAG_0          = (1 << 0),
    CHAPTER_FLAG_1          = (1 << 1),
    CHAPTER_FLAG_POSTGAME   = (1 << 2),
    CHAPTER_FLAG_3          = (1 << 3),
    CHAPTER_FLAG_PREPSCREEN = (1 << 4),
    CHAPTER_FLAG_5          = (1 << 5),
    CHAPTER_FLAG_DIFFICULT  = (1 << 6),
    CHAPTER_FLAG_7          = (1 << 7)
};

const char * GetChapterTitleName(unsigned chIndex);

const struct ROMChapterData* GetChapterDefinition(unsigned chIndex);
void sub_80896FC(u16 * tm, int pal, int c);
void sub_80895B4(int config, int palId);

void LoadChapterTitleGfx(int chr, u32 titleId);
void ChapterIntro_DrawChapterTitleMaybe(void);