#ifndef GP32STDIO_H
#define GP32STDIO_H


/*#define  fopen(f,m)      smc_fopen(f,m)
#define  fread(p,s,n,f)  smc_fread(p,s,n,f)
#define  fwrite(a,b,c,d) smc_fwrite(a,b,c,d)
#define  fclose(f)       smc_fclose(f)
#define  ftell(f)        smc_ftell(f)
#define  rewind(f)       smc_rewind(f)
#define  fseek(a,b,c)    smc_fseek(a,b,c)
#define  FILE            GPFILE
#define  malloc(a)       calloc(1,a)
*/

typedef struct {
   char name[128][16];    //  128 entrys,16 Bytes long
   int size[128];
} DIR;


typedef struct {
  char path[256];
  char mode;
  unsigned int pos;
  unsigned int size;
  char *wbuffer;
} GPFILE;

/*# define   SEEK_SET   0
# define   SEEK_CUR   1
# define   SEEK_END   2
*/

#endif
