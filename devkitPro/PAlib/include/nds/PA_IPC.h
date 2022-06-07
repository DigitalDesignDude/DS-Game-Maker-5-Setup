#ifndef _PA_IPC
#define _PA_IPC

#ifdef __cplusplus
extern "C" {
#endif

#define PAIPC_PLAY  0
#define PAIPC_PAUSE 1
#define PAIPC_STOP  2

typedef struct {
	// fincs edit: rearranged the items to conserve space (*cough* word alignement *cough*...)
	void* Data;
	u8 Command;
	u8 Busy;
	u8 Volume; // |(1<<7) to change Volume, &127 to get value
	u8 Pan; // |(1<<7) to change Pan, &127 to get value
	s32 Rate;
	u32 Length;
	u8 Format;
	u8 Padding;
	u8 Repeat;
	u8 Duty;
	u32 RepeatPoint;
} PA_IPCSound;

typedef struct{
	PA_IPCSound Sound[17];
} PA_IPCType;

#ifdef __cplusplus
}
#endif

#endif
