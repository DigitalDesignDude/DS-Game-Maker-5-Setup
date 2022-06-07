/*
 *
 *  A JPEG decompressor, targeted for the GameBoy Advance (although there
 * should be no machine-specific aspects if you disable JPEG_USE_IWRAM
 * and JPEG_MARK_TIME).  On the GBA it consumes, all with slight potential
 * variance:
 *
 * 3348 bytes of IWRAM, temporary
 * 7756 bytes of ROM
 * 4720 bytes of stack space, usually in IWRAM
 * 350 milliseconds for decompressing a representative image
 *
 * Unlike before when IWRAM was permanently used, it's now loaded in just
 * before decompression, allowing you to spend IWRAM on more tools called
 * constantly rather than one you call only once in awhile.  There is no
 * permanent IWRAM usage with this library.
 * 
 * It has a low capacitance for unusual JPEGs.  They cannot be progressive,
 * use arithmetic coding, have more than 4 components in a scan, and must be
 * 8-bit.  They can be colour or grayscale, and any component scaling factors
 * are valid (unless if JPEG_HANDLE_ANY_FACTORS is reset, in which case only
 * 2:1:1 is allowed).  The maximum component scale factors cannot be three.  In
 * general, you'll be all right, but if it doesn't like your input it will not
 * react sensibly in embedded.
 * 
 * This code is in the public domain.  JPEG is used for both its standard
 * meaning and for JFIF.
 * 
 * Revision 1: Inflicted stricter warnings, fixed C99-using code, and reduced
 *     allocation footprint (6144 bytes less).
 * Revision 2: Reduced ROM usage by 276 bytes, with the body going to 832 bytes
 *     of IWRAM.  I made it more configurable, particularly in YCbCr->RGB
 *     conversion.  Some brute force ROM usage reduction.
 * Revision 3: Removed all memset, malloc, and free dependencies.  This
 *     increases stack use drastically but also makes it completely
 *     self-sufficient.
 * Revision 4: Saved 6176 bytes of JPEG_Decoder state by exploiting an
 *     allowance of baseline JPEG decoding.  This requires 3088 more bytes of
 *     stack space, however.
 * Revision 5: Made the fixed-point shift configurable.  Can now be compiled
 *     with -ansi -pedantic, and fixed stack usage so that it is always
 *     predictable by exploiting a JPEG restriction.
 * Revision 6: A fixed type has been added and is configurable.  16-bit
 *     fixed is valid if you reduce JPEG_FIXSHIFT to 4 or lower.
 * Revision 7: Inserted assertions for when you're not on the embedded
 *     environment; good for confirming that a given file is compatible with
 *     the decompressor.  "this" is no longer used as a variable name.  Added
 *     necessary fluff for dealing with C++.
 *
 * - Burton Radons (loth@users.sourceforge.net)
 */

#ifndef GBA_JPEG_DECODE_H
#define GBA_IMAGE_JPEG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef JPEG_DEBUG
#define JPEG_DEBUG 0
    /**< Enable assertion checks for input.  This is useful if you want to use
     * the library in non-embedded environments, such as to confirm that a
     * JPEG file will be compliant.
     * 
     * Enabling this will define JPEG_Assert(TEST) if it's not predefined.
     * The default prints some information on the error to stderr and then
     * returns zero.
     */
#endif /* JPEG_DEBUG */
    
#ifndef JPEG_HANDLE_ANY_FACTORS
#define JPEG_HANDLE_ANY_FACTORS 1
    /**< If this is set, any component factors are valid.  Otherwise
     * it will only handle 2:1:1 (the typical form that sacrifices colour
     * resolution).  Note that Photoshop will only generate such files if you
     * use Save for Web.  Resetting this saves 508 bytes of IWRAM.
     */
#endif /* JPEG_HANDLE_ANY_FACTORS */
     
#ifndef JPEG_FASTER_M211
#define JPEG_FASTER_M211 1
    /**< If this is set, then the most common JPEG format is not given
     * special, faster treatment.  You must set JPEG_HANDLE_ANY_FACTORS
     * in this case, or you will not see anything.  Resetting this saves
     * 532 bytes of IWRAM, at the cost of speed.
     */
#endif /* JPEG_FASTER_M211 */
     
#ifndef JPEG_USE_IWRAM
#define JPEG_USE_IWRAM 0
    /**< If this is set, the JPEG decompressor will use IWRAM for huge
     * benefits to decompression speed (249% faster than reset).  Resetting
     * this saves up to 3348 bytes of IWRAM, depending upon
     * JPEG_HANDLE_ANY_FACTORS and JPEG_FASTER_M211.
     */
#endif /* JPEG_USE_IWRAM */
     
#define JPEG_DCTSIZE 8
    /**< The number of samples across and down a JPEG DCT.  This cannot be
     * configured, as the inverse DCT only handles 8x8.
     */

#define JPEG_DCTSIZE2 (JPEG_DCTSIZE * JPEG_DCTSIZE)
    /**< The number of samples in a full 2-D DCT. */
    
#ifndef JPEG_MAXIMUM_COMPONENTS
#define JPEG_MAXIMUM_COMPONENTS 4
    /**< The maximum number of components that can be involved in an image.
      * Each value costs 8 bytes of stack space and 8 bytes of allocations.
      */
#endif /* JPEG_MAXIMUM_SCAN_COMPONENTS */

#ifndef JPEG_FIXSHIFT
#define JPEG_FIXSHIFT 8
    /**< The shift used for converting to and from fixed point.  A higher value
      * here (up to 10 for 32-bit) results in better quality; a lower value
      * (down to 2) results in lesser quality.  Lower values can be somewhat
      * faster depending upon the hardware's clockings for multiplication.
      */
#endif /* JPEG_FIXSHIFT */

#ifndef JPEG_MAXIMUM_SCAN_COMPONENT_FACTORS
#define JPEG_MAXIMUM_SCAN_COMPONENT_FACTORS 10
    /**< The limit of the sum of the multiplied horizontal scaling factors in
      * the components.  For example, if Y is 1x1, Cb is 2x2, and Cr is 2x2,
      * that comes out to (1 * 1 + 2 * 2 + 2 * 2), or 9.  The limit here is
      * what is specified in the standard (B.2.3).
      */
#endif /* JPEG_MAXIMUM_SCAN_COMPONENT_FACTORS */

#ifndef JPEG_FIXED_TYPE
#define JPEG_FIXED_TYPE long int
    /**< The fixed data type.  This requires a minimum size of
      * JPEG_FIXSHIFT plus 12.
      */
#endif /* JPEG_FIXED_TYPE */

/* If this value is defined as 1, then it outputs to RGB in 32-bit words, with
 * red in the first eight bits, green in the second eight bits, and blue in the
 * third eight bits.
 */
 
#if JPEG_OUTPUT_RGB8
    #define JPEG_OUTPUT_TYPE unsigned int
    
    #define JPEG_Convert_Limit(VALUE) ((VALUE) < 0 ? 0 : (VALUE) > 255 ? 255 : (VALUE))
    
    #define JPEG_Convert(OUT, Y, Cb, Cr) \
        do { \
            int eY = (Y) + 63; \
            int R = ((eY) + ((Cr) * 359 >> 8)) * 2; \
            int G = ((eY) - ((Cb) * 88 >> 8) - ((Cr) * 183 >> 8)) * 2; \
            int B = ((eY) + ((Cb) * 454 >> 8)) * 2; \
            \
            R = JPEG_Convert_Limit (R); \
            G = JPEG_Convert_Limit (G) << 8; \
            B = JPEG_Convert_Limit (B) << 16; \
            (OUT) = R | G | B; \
        } while (0)
        
    #define JPEG_Convert_From(IN, Y, Cb, Cr) \
        do { \
            int R = IN & 255; \
            int G = (IN >> 8) & 255; \
            int B = (IN >> 16) & 255; \
            \
            Y = (((R * 66 >> 8) + (G * 129 >> 8) + (B * 25 >> 8)) >> 1) - 63; \
            Cb = ((R * -38 >> 8) + (G * -74 >> 8) + (B * 112 >> 8)) >> 1; \
            Cr = ((R * 112 >> 8) + (G * -94 >> 8) + (B * 18 >> 8)) >> 1; \
        } while (0)
#endif /* JPEG_OUTPUT_RGB8 */
    
#ifndef JPEG_OUTPUT_TYPE
#define JPEG_OUTPUT_TYPE unsigned short
    /**< This is the data type that JPEG outputs to.  The interpretation of
     * this type is dependent upon JPEG_Convert.
     */
#endif /* JPEG_OUTPUT_TYPE */

#ifndef JPEG_Convert
/** Convert YCbCr values (each in the nominal range -64 to 63) to RGB and store
  * in the output value (of type JPEG_OUTPUT_TYPE).  By default this stores to
  * 15-bit RGB.
  */
  
#define JPEG_Convert(OUT, Y, Cb, Cr) \
    do { \
        int eY = (Y) + 63; \
        int R = (eY) + ((Cr) * 359 >> 8); \
        int G = (eY) - ((Cb) * 88 >> 8) - ((Cr) * 183 >> 8); \
        int B = (eY) + ((Cb) * 454 >> 8); \
        \
        R = ComponentRange [R >> 2]; \
        G = ComponentRange [G >> 2] << 5; \
        B = ComponentRange [B >> 2] << 10; \
        (OUT) = R | G | B | (1 << 15); \
    } while (0)
    
#endif /* JPEG_Convert  */

#ifndef JPEG_Assert
    #if JPEG_DEBUG
        #include <stdio.h>
        #include <stdlib.h>
        #define JPEG_Assert(TEST) \
            do { \
                if (TEST) \
                    break; \
                fprintf (stderr, __FILE__ "(%d): " #TEST "\n", __LINE__); \
                return 0; \
            } while (0)
    #else
        #define JPEG_Assert(TEST) do { } while (0)
    #endif /* JPEG_DEBUG */
#endif /* JPEG_Assert */

/** The markers that can appear in a JPEG stream. */
enum JPEG_Marker
{
    JPEG_Marker_APP0 = 0xFFE0, /**< Reserved application segment 0. */
    JPEG_Marker_APP1 = 0xFFE1, /**< Reserved application segment 1. */
    JPEG_Marker_APP2 = 0xFFE2, /**< Reserved application segment 2. */
    JPEG_Marker_APP3 = 0xFFE3, /**< Reserved application segment 3. */
    JPEG_Marker_APP4 = 0xFFE4, /**< Reserved application segment 4. */
    JPEG_Marker_APP5 = 0xFFE5, /**< Reserved application segment 5. */
    JPEG_Marker_APP6 = 0xFFE6, /**< Reserved application segment 6. */
    JPEG_Marker_APP7 = 0xFFE7, /**< Reserved application segment 7. */
    JPEG_Marker_APP8 = 0xFFE8, /**< Reserved application segment 8. */
    JPEG_Marker_APP9 = 0xFFE9, /**< Reserved application segment 9. */
    JPEG_Marker_APP10 = 0xFFEA, /**< Reserved application segment 10. */
    JPEG_Marker_APP11 = 0xFFEB, /**< Reserved application segment 11. */
    JPEG_Marker_APP12 = 0xFFEC, /**< Reserved application segment 12. */
    JPEG_Marker_APP13 = 0xFFED, /**< Reserved application segment 13. */
    JPEG_Marker_APP14 = 0xFFEE, /**< Reserved application segment 14. */
    JPEG_Marker_APP15 = 0xFFEF, /**< Reserved application segment 15. */
    JPEG_Marker_COM = 0xFFFE, /**< Comment. */
    JPEG_Marker_DHT = 0xFFC4, /**< Define huffman table. */
    JPEG_Marker_DQT = 0xFFDB, /**< Define quantization table(s). */
    JPEG_Marker_DRI = 0xFFDD, /**< Define restart interval. */
    JPEG_Marker_EOI = 0xFFD9, /**< End of image. */
    JPEG_Marker_SOF0 = 0xFFC0, /**< Start of Frame, non-differential, Huffman coding, baseline DCT. */
    JPEG_Marker_SOI = 0xFFD8, /**< Start of image. */
    JPEG_Marker_SOS = 0xFFDA /**< Start of scan. */
};

typedef enum JPEG_Marker JPEG_Marker;
typedef JPEG_FIXED_TYPE JPEG_QuantizationTable [JPEG_DCTSIZE2]; /**< Quantization table elements, in zigzag order, fixed. */

/** Compute the multiplication of two fixed-point values. */
#define JPEG_FIXMUL(A, B) ((A) * (B) >> JPEG_FIXSHIFT)

/** Convert a fixed-point value to an integer. */
#define JPEG_FIXTOI(A) ((A) >> JPEG_FIXSHIFT)

/** Convert an integer to a fixed-point value. */
#define JPEG_ITOFIX(A) ((A) << JPEG_FIXSHIFT)

/** Convert a floating-point value to fixed-point. */
#define JPEG_FTOFIX(A) ((int) ((A) * JPEG_ITOFIX (1)))

/** Convert a fixed-point value to floating-point. */
#define JPEG_FIXTOF(A) ((A) / (float) JPEG_ITOFIX (1))

typedef struct JPEG_HuffmanTable JPEG_HuffmanTable;
typedef struct JPEG_Decoder JPEG_Decoder;
typedef struct JPEG_FrameHeader JPEG_FrameHeader;
typedef struct JPEG_FrameHeader_Component JPEG_FrameHeader_Component;
typedef struct JPEG_ScanHeader JPEG_ScanHeader;
typedef struct JPEG_ScanHeader_Component JPEG_ScanHeader_Component;

/** A huffman table. */
struct JPEG_HuffmanTable
{
    const unsigned char *huffval; /**< Pointer to values in the table (256 entries). */
    int maxcode [16]; /**< The maximum code for each length - 1. */
    const unsigned char *valptr [16]; /**< Items are subtracted by mincode and then indexed into huffval. */
    
    unsigned char look_nbits [256]; /**< The lookahead buffer lengths. */
    unsigned char look_sym [256]; /**< The lookahead buffer values. */
};

/** An image component in the frame. */
struct JPEG_FrameHeader_Component
{
    unsigned char selector; /**< Component identifier, must be unique amongst the identifiers (C). */
    unsigned char horzFactor; /**< Horizontal sampling factor. */
    unsigned char vertFactor; /**< Vertical sampling factor. */
    unsigned char quantTable; /**< Quantization table destination selector. */
};

/** The frame header state. */
struct JPEG_FrameHeader
{
    JPEG_Marker marker; /**< The marker that began this frame header, one of JPEG_Marker_SOFn. */
    int encoding; /**< 0 for Huffman coding, 1 for arithmetic coding. */
    char differential; /**< Differential (1) or non-differential (0). */
    
    unsigned char precision; /**< Sample precision - precision in bits for the samples of the components in the frame. */
    unsigned short height; /**< Maximum number of lines in the source image, equal to the number of lines in the component with the maximum number of vertical samples.  0 indicates that the number of lines shall be defined by the DNL marker and parameters at the end of the first scan. */
    unsigned short width; /**< Number of samples per line in the source image, equal to the number of samples per line in the component with the maximum number of horizontal samples. */
    JPEG_FrameHeader_Component componentList [JPEG_MAXIMUM_COMPONENTS]; /**< Components. */
    int componentCount; /**< Number of components. */
};

/** A component involved in this scan. */
struct JPEG_ScanHeader_Component
{
    unsigned char selector; /**< Selector index corresponding to one specified in the frame header (Csj). */
    unsigned char dcTable; /**< DC entropy coding table destination selector (Tdj). */
    unsigned char acTable; /**< AC entropy coding table destination selector (Taj). */
};

/** Scan header state. */
struct JPEG_ScanHeader
{
    JPEG_ScanHeader_Component componentList [JPEG_MAXIMUM_COMPONENTS]; /**< Components involved in this scan. */
    int componentCount; /**< Number of components involved in this scan. */
    unsigned char spectralStart; /**< In DCT modes of operation, the first DCT coefficient in each block in zig-zag order which shall be coded in the scan (Ss).  For sequential DCT this is zero. */
    unsigned char spectralEnd; /**< Specify the last DCT coefficient in each block in zig-zag order which shall be coded in the scan. */
    unsigned char successiveApproximationBitPositionHigh; /**< (Ah). */
    unsigned char successiveApproximationBitPositionLow; /**< (Al). */
};

/** The complete decoder state. */
struct JPEG_Decoder
{
    const unsigned char *acTables [4]; /**< The AC huffman table slots. */
    const unsigned char *dcTables [4]; /**< The DC huffman table slots. */
    JPEG_QuantizationTable quantTables [4]; /**< The quantization table slots. */
    unsigned int restartInterval; /**< Number of MCU in the restart interval (Ri). */
    JPEG_FrameHeader frame; /**< Current frame. */
    JPEG_ScanHeader scan; /**< Current scan. */
};

/** Start reading bits. */
#define JPEG_BITS_START() \
    unsigned int bits_left = 0; \
    unsigned long int bits_data = 0
    
/** Rewind any bytes that have not been read from and reset the state. */
#define JPEG_BITS_REWIND() \
    do { \
        int count = bits_left >> 3; \
        \
        while (count --) \
        { \
            data --; \
            if (data [-1] == 0xFF) \
                data --; \
        } \
        \
        bits_left = 0; \
        bits_data = 0; \
    } while (0)
    
/** Fill the buffer. */
#define JPEG_BITS_CHECK() \
    do { \
        while (bits_left < 32 - 7) \
        { \
            bits_data = (bits_data << 8) | (*data ++); \
            if (data [-1] == 0xFF) \
                data ++; \
            bits_left += 8; \
        } \
    } while (0)
   
/** Return and consume a number of bits. */
#define JPEG_BITS_GET(COUNT) \
    ((bits_data >> (bits_left -= (COUNT))) & ((1 << (COUNT)) - 1))
    
/** Return a number of bits without consuming them. */
#define JPEG_BITS_PEEK(COUNT) \
    ((bits_data >> (bits_left - (COUNT))) & ((1 << (COUNT)) - 1))
    
/** Drop a number of bits from the stream. */
#define JPEG_BITS_DROP(COUNT) \
    (bits_left -= (COUNT))

/** Read a single unsigned char from the current bit-stream by using the provided table. */
#define JPEG_HuffmanTable_Decode(TABLE, OUT) \
    do { \
        int bitcount, result; \
        \
        result = JPEG_BITS_PEEK (8); \
        \
        if ((bitcount = (TABLE)->look_nbits [result]) != 0) \
        { \
            JPEG_BITS_DROP (bitcount); \
            result = (TABLE)->look_sym [result]; \
        } \
        else \
        { \
            int i = 7; \
            \
            JPEG_BITS_DROP (8); \
            do result = (result << 1) | JPEG_BITS_GET (1); \
            while (result > (TABLE)->maxcode [++ i]); \
            \
            result = (TABLE)->valptr [i] [result]; \
        } \
        \
        (OUT) = result; \
    } while (0)

//extern const unsigned char JPEG_ToZigZag [JPEG_DCTSIZE2]; /* Converts row-major indices to zig-zagged order. */
//extern const unsigned char JPEG_FromZigZag [JPEG_DCTSIZE2]; /* Converts zig-zagged indices to row-major order. */
//extern const JPEG_FIXED_TYPE JPEG_AANScaleFactor [JPEG_DCTSIZE2]; /* AA&N scaling factors for quantisation in fixed point. */
//extern const unsigned char JPEG_ComponentRange [32 * 3]; /* A limited component clamp that keeps values in the 0..31 range if incremented by 32. */

/** Return whether this data matches as a JPEG input stream.  You only need
  * to read four bytes.
  */
  
extern int JPEG_Match (const unsigned char *data, int length);

/** Read a FrameHeader segment (SOFn) and store the new data pointer in
  * *data.  Returns true on success and false on failure (failure isn't
  * possible if JPEG_DEBUG is reset).
  */

extern int JPEG_FrameHeader_Read (JPEG_FrameHeader *frame, const unsigned char **data, JPEG_Marker marker);

/** Read a HuffmanTable segment (DHT) and store the new data pointer in
  * *data.  Returns true on success and false on failure (failure isn't
  * possible if JPEG_DEBUG is reset).
  */
  
extern int JPEG_HuffmanTable_Read (JPEG_HuffmanTable *table, const unsigned char **data);

/** Skip a HuffmanTable segment (DHT) and store the new data pointer in
  * *data on success.  Returns true on success and false on failure (failure
  * isn't possible if JPEG_DEBUG is reset).
  */
  
extern int JPEG_HuffmanTable_Skip (const unsigned char **data);

/** Read a ScanHeader segment (SOS) and store the new data pointer in
  * *data.  Returns true on success and false on failure (failure isn't
  * possible if JPEG_DEBUG is reset).
  */
  
extern int JPEG_ScanHeader_Read (JPEG_ScanHeader *scan, const unsigned char **data);

/** Read all headers up to the start of the image and store the new data
  * pointer in *data.  Returns true on success and false on failure (failure
  * isn't possible if JPEG_DEBUG is reset).
  */
  
extern int JPEG_Decoder_ReadHeaders (JPEG_Decoder *decoder, const unsigned char **data);

/** Read the entire image from the *data value and then store the new data pointer.
  * Returns true on success and false on failure (failure isn't possible if
  * JPEG_DEBUG is reset).
  */
  
extern int JPEG_Decoder_ReadImage (JPEG_Decoder *decoder, const unsigned char **data, volatile JPEG_OUTPUT_TYPE *out, int outWidth, int outHeight);

/** Perform a 2D inverse DCT computation on the input.
  *
  * @param zz The coefficients to process, JPEG_DCTSIZE2 in length.  The
  *     contents will be destroyed in the computations.
  * @param chunk The chunk to store the results in, nominally from -64 to 63,
  *     although some error is expected.
  * @param chunkStride The number of values in a row for the chunk array.
  */

extern void JPEG_IDCT (JPEG_FIXED_TYPE *zz, signed char *chunk, int chunkStride);

/** Create a decompressor, read the headers from the provided data, and then
  * read the image into the buffer given.  Returns true on success and false on
  * failure (failure isn't possible if JPEG_DEBUG is reset).
  */
  
extern int JPEG_DecompressImage (const unsigned char *data, volatile JPEG_OUTPUT_TYPE *out, int outWidth, int outHeight);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GBA_IMAGE_JPEG_H */
