#ifndef _PA_Main
#define _PA_Main

#ifdef __cplusplus
extern "C" {
#endif

#include <nds.h>
#include <maxmod7.h>
#include <dswifi7.h>

#include <stdlib.h>

#include "PA_IPC.h"
#include "PA_FifoMessages.h"
#include "PA_Transfer.h"

#include "arm7/as_lib7.h"

void PA_Init(bool sound);

void PA_InputGetAndSend();
void PA_LegacyIPCManage();

#ifdef __cplusplus
}
#endif

#endif
