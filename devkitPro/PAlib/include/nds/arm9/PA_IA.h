#ifndef _PA_IA
#define _PA_IA

#ifdef __cplusplus
extern "C" {
#endif

#include <nds.h>
extern u16 *maze[255];

static inline void PA_InitAstar(u16 lx, u16 ly){
	int i;
	lx = lx << 1; // u16 take up 2 spaces...
	
	for (i = 0; i < ly; i++){
		free(maze[i]);
		maze[i] = (u16*)malloc(lx);
	}
}

s8 PA_Astar(u16 width, u16 height);

#ifdef __cplusplus
}
#endif
#endif
