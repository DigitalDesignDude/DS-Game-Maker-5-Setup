/* <dir.h>

	provides prototypes for custom directory functions in libsysbase

*/
#ifndef _dir_h_
#define _dir_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/stat.h>

/* Directory iterator for mantaining state between dir* calls */
typedef struct {
    int device;
    void *dirStruct;
} DIR_ITER;

DIR_ITER* diropen (const char *path);
int dirreset (DIR_ITER *dirState);
int dirnext (DIR_ITER *dirState, char *filename, struct stat *filestat);
int dirclose (DIR_ITER *dirState);



#ifdef __cplusplus
}
#endif

#endif // _dir_h_
