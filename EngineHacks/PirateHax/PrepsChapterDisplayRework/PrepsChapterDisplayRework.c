#include "gbafe.h"

#include "PrepsChapterDisplayRework.h"

//respectively: preps chapter thing for a gameplay chapter and for a tower chapter (which I'll repurpose for Hubs)

void PutObj8A18582_WithConfigs(int xOam1, int yOam0, int a3, u16 oam2)
{
    ObjInsert(4, xOam1, yOam0, obj_8A18582, oam2);       // chapter image itself: that's fine

    PutChapterNumber(xOam1, yOam0, a3, oam2);
}

void PutObj8A18590_WithConfigs(int xOam1, int yOam0, int a3, u16 oam2)
{
    ObjInsert(4, xOam1, yOam0, obj_8A18590, oam2);

    PutHubChapterNumber(xOam1, yOam0, a3, oam2);
}

void PutChapterNumber(int xOam1, int yOam0, int a3, u16 oam2)
{
    u8 val;
    val = a3; //got rid of the divide by 2 thing, unneeded
    if (val < 10)
        ObjInsert(4, xOam1 + 0x30, yOam0, objs_8A18614[0xB], oam2);
    else
        ObjInsert(4, xOam1 + 0x30, yOam0, objs_8A18614[val >> 4], oam2); // tens digit

    val = val << 4; //this shifts the tens digit out of the byte?

    val = val >> 4; //then shifts the ones value back down
    
    ObjInsert(4, xOam1 + 0x38, yOam0, objs_8A18614[val], oam2); // ones digit: example: 1s digit is 3, so it should be at 0 1 2 3
}

void PutHubChapterNumber(int xOam1, int yOam0, int a3, int oam2)
{
    u8 val;
    val = a3; //got rid of the divide by 2 thing, unneeded

    //no need for tens digit addition

    val = val << 4; //this shifts the tens digit out of the byte?

    val = val >> 4; //then shifts the ones value back down
    
    ObjInsert(4, xOam1 + 0x24, yOam0, objs_8A18614[val], oam2); // ones digit: example: 1s digit is 3, so it should be at 0 1 2 3
}

void PutObj8A185AC(int xOam1, int yOam0, u16 oam2) //hijacking "ex map" for Finale
{
    ObjInsert(4, xOam1, yOam0, obj_8A185AC, oam2);
}
