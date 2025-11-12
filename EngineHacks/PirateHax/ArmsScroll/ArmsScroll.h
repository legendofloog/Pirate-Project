#define NO_WEXP 0
#define D_WEXP 1
#define C_WEXP 26
#define B_WEXP 76
#define A_WEXP 151
#define S_WEXP 251

void NewPopup2_PlanA(Proc* parent, int IconIndex, char *str);

extern u16 ArmsScrollMessageReturnLink;

extern u8 ArmsScrollBoostLink;

extern u8 ArmsScrollIDLink;


bool ArmsScrollUsability(struct Unit* unit, int item);
bool ArmsScrollPrepUsability(struct Unit* unit, int item);
void ArmsScrollEffect(Proc* proc);
int ArmsScrollPrepsEffect(struct Unit* unit, int itemSlot);
void DoArmsScrollEffect(struct Unit* unit, int itemSlot);
u8 ReturnArmsScrollWeaponType(struct Unit* unit);

extern const u32 ArmsScrollPopup[];
extern u16 ArmsScrollPopupTextWordLink;