#ifndef ZDA_H
#define ZDA_H

// 52 byte header for each file
typedef struct {
   char filename[40];
   int  size_uncompressed;
   int  size_compressed;
   int  start_offset;
} ZDAINDEX;

// 12 bytes index
typedef struct {
   char name[4];
   int  records;
   int  size;
} ZDAHEAD;


#define ZLIB_VERSION "1.2.1"

#define gp_uncompressZDA(a,b,c,d) uncompress(a,b,c,d)
extern int gp_uncompressZDA (char *dest, unsigned int *destLen,const char *source,unsigned int sourceLen);
/*
   Decompresses the source buffer into the destination buffer.  sourceLen is
   the byte length of the source buffer. Upon entry, destLen is the total
   size of the destination buffer, which must be large enough to hold the
   entire uncompressed data. (The size of the uncompressed data must have
   been saved previously by the compressor and transmitted to the decompressor
   by some mechanism outside the scope of this compression library.)
   Upon exit, destLen is the actual size of the compressed buffer.
   This function can be used to decompress a whole file at once.

   gp_uncompressZDA returns 0 if success, >0 on any error.
*/

#endif
