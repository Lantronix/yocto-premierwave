
#ifndef __LTRX_IMAGE_H__
#define __LTRX_IMAGE_H__

#ifdef LTRX_BOOTSTRAP
#include "_types.h"
#else
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#endif

/* invalid type */
#define IMAGE_TYPE_INVALID                         0
/* primary boot loader (loaded by cpu) */
#define IMAGE_TYPE_BOOTSTRAP                       1
/* secondary boot loader (u-boot, etc.) with rom header */
#define IMAGE_TYPE_BOOTLOADER                      2
/* boot environment */
#define IMAGE_TYPE_BOOT_ENV                        3
/* firmware kernel image */
#define IMAGE_TYPE_FIRMWARE_KERNEL                 4
/* firmware root file system in jffs2 or ext2 format */
#define IMAGE_TYPE_FIRMWARE_ROOTFS_JFFS2_OR_EXT2   5
/* production loader image */
#define IMAGE_TYPE_PRODUCTION_LOADER               6
/* recovery loader image */
#define IMAGE_TYPE_RECOVERY_LOADER                 7
/* configuration loader image */
#define IMAGE_TYPE_CONFIG_LOADER                   8
/* reserved for now */
#define IMAGE_TYPE_MFG_TEST_LOADER                 9
/* firmware root file system in ubifs format with 16k erase block size */
#define IMAGE_TYPE_FIRMWARE_ROOTFS_UBIFS_16KEB     10
/* OEM defaults file */
#define IMAGE_TYPE_OEM_DEFAULTS                    11
/* firmware root filesystem in lzma-compressed tar format */
#define IMAGE_TYPE_FIRMWARE_ROOTFS_TAR_LZMA        12
/* firmware root filesystem in ubifs format with 128k erase block size */
#define IMAGE_TYPE_FIRMWARE_ROOTFS_UBIFS_128KEB    13
/* python setup files */
#define IMAGE_TYPE_PYTHON_USERFS                   14
/* python setup files */
#define IMAGE_TYPE_ECDSA_PUBLIC_KEY                15

/* maximum type range*/
#define IMAGE_TYPE_MAX                             16

#if 0
#define IMAGE_TYPE_INVALID             0
#define IMAGE_TYPE_BOOTLOADER          1
#define IMAGE_TYPE_OTA                 2
#define IMAGE_TYPE_FIRMWARE            3
#define IMAGE_TYPE_RADIO_FW            4
#define IMAGE_TYPE_PRODUCTION_CONFIG   5
#define IMAGE_TYPE_RESOURCE_BUNDLE     6
#define IMAGE_TYPE_AUTOLOADER          7
#define IMAGE_TYPE_MAX                 7

#define IMAGE_SPACE_RAM                      1
#define IMAGE_SPACE_FLASH                    2
#define IMAGE_SPACE_SPI_FLASH                3
#define IMAGE_SPACE_PERIPHERAL               4
#endif

#define IMAGE_OLD_VERSION                    0
#define IMAGE_VERSION                        1

/* Mangle Types */
#define IMAGE_MANGLE_TYPE_NONE               0
#define IMAGE_MANGLE_TYPE_LZSS               1
#define IMAGE_MANGLE_TYPE_MAX                1

/* Passcode Types */
#define IMAGE_OPTIONAL_CHECKSUM_TYPE_NONE    0
#define IMAGE_PASSCODE_TYPE_NONE             0

/* Options bit codes */
#define IMAGE_LOADOPT_ADDRESS_REVERSE               0x1
#define IMAGE_LOADOPT_ADDRESS_LOADER_DETERMINED     0x2
#define IMAGE_LOADOPT_REBOOT                        0x4
#define IMAGE_LOADOPT_NO_REBOOT                     0x8

#define n32(x)                  htonl(x)
#define n16(x)                  htons(x)

#define STRINGIFY(v)            __STRINGIFY(v)
#define __STRINGIFY(v)          #v

# define    IMAGE_HEADER_ID                     "NUEVO-2"
# define    SB_OTP_MAGIC_SIGNED_ECDSA_2048   1
# define    SB_OTP_MAGIC_ECDSA_PRIMARY       2
# define    SB_OTP_MAGIC_ECDSA_OPTIONAL      3


static const char header_id[] = IMAGE_HEADER_ID;

#ifndef LTRX_BOOTSTRAP
uint32_t opt_verbose;
#endif

/*
 *  256 bytes. All values are BIGENDIAN.
 */
#pragma pack(1)
struct image_header
{
    uint8_t  HeaderID[8];
    uint32_t HeaderVersion;                 /* Header version. */
    uint32_t HeaderLength;
    uint32_t HeaderChecksum32;              /* 32-bit 1's complement big endian checksum of the entire header.  This field assume zero during checksuming */
    uint32_t Options;                       /* bitmap */
    uint32_t ContentsType;                  /* IMAGE_TYPE_* */
    uint32_t ContentsMangleType;            /* IMAGE_MANGLE_TYPE_. Specified if to use compression (or even encrpytion) on image */
    uint8_t  ContentsVersion[4];            /* Version in binary:  0x01020304 would be version 1.2.3.4 */
    uint8_t  ContentsBuild[4];              /* Build indicator and release number in ascii: 0x52050000 would be R5 */
    uint8_t  ContentsProductID[16];         /* TBD. Null termintated string */
    uint32_t ContentsLength;                /* Length of image following header.  Length used to compute checksums */
    uint32_t ContentsFinalLength;           /* Length of final image.  If image is compressed then this is the length of the uncompressed image. */
    uint32_t ContentsDestinationAddress;    /* Address where image should be copied to */
    uint32_t ContentsChecksum32;            /* 32-bit 1's complement checksum of the image following this header.  ALWAYS COMPUTED AND TESTED*/
    uint32_t ContentsFinalChecksum32;       /* 32-bit 1's complememt big endian checkum of the final/uncompress/unencryped/unmodified image. ALWAYS COMPUTED AND TESTED*/
    uint32_t ContentsLoadOffset;

    uint32_t ContentsOptionalChecksumType;  /* IMAGE_OPTIONAL_CHECKSUM_TYPE_* */
    uint8_t  ContentsOptionalChecksum[32];  /* If there is an optional checksum defined, this is the value of that checksum on the contents of the image following the header.  If one is not defined this is zero. */
    uint32_t ContentsFinalOptionalChecksumType; /* IMAGE_OPTIONAL_CHECKSUM_TYPE_ */
    uint8_t  ContentsFinalOptionalChecksum[32];
    uint32_t ContentsPasscodeType;          /* IMAGE_PASSCODE_TYPE_*  Specified type of pass code, if used, for image */
    uint8_t  ContentsPasscode[64];          /* Can be used to support passcoding of images.  If not used should be all null(0) */
    uint32_t OffsetToNextHeader;            /* Offset in bytes from the start of this header to the next header in the image file.  If there is not a 'next header' this should be all zeros */
    uint32_t ChainedImageLengthRemaining;   /* Total length from the start of this header to the end of a chained .rom image file/blob */

    uint32_t ImageIndex;
    uint32_t ImagePartitionSizeKBytes;
    uint8_t reserved[20];
};
#pragma pack()



#if defined(LITTLE) || defined(LITTLE_ENDIAN)
#  ifndef htonl
#    define __SWAP32__(val) \
       ((((val) & 0xFF000000) >> 24 ) | (((val) & 0x00FF0000) >> 8) \
       | (((val) & 0x0000FF00) << 8) | (((val) & 0x000000FF) << 24))

#    define __SWAP16__(val) \
       ((((val) & 0xFF00) >> 8) | (((val) & 0x00FF) << 8))

#    define htonl(val)  __SWAP32__(val)
#    define ntohl(val)  __SWAP32__(val)
#    define htons(val)  __SWAP16__(val)
#    define ntohs(val)  __SWAP16__(val)
#  endif
#else
#  ifndef htonl
#    define htons(val) (val)
#    define ntohs(val) (val)
#    define ntohl(val) (val)
#    define htonl(val) (val)
#  endif
#endif


#define image_is_type_bootstrap(_h) ( \
    ntohl((_h)->ContentsType) == IMAGE_TYPE_BOOTSTRAP \
)

#define image_is_type_kernel(_h)    ( \
    ntohl((_h)->ContentsType) == IMAGE_TYPE_FIRMWARE_KERNEL \
)

#define image_is_type_root_fs(_h)   ( \
    ntohl((_h)->ContentsType) == IMAGE_TYPE_FIRMWARE_ROOTFS_JFFS2_OR_EXT2 || \
    ntohl((_h)->ContentsType) == IMAGE_TYPE_FIRMWARE_ROOTFS_UBIFS_16KEB || \
    ntohl((_h)->ContentsType) == IMAGE_TYPE_FIRMWARE_ROOTFS_UBIFS_128KEB || \
    ntohl((_h)->ContentsType) == IMAGE_TYPE_FIRMWARE_ROOTFS_TAR_LZMA \
)

struct image_header_checksum
{
    uint64_t ihcs_sum;
    uint32_t ihcs_value;
    uint32_t ihcs_count;
};


void image_header_checksum_init(struct image_header_checksum *);
void image_header_checksum_next(struct image_header_checksum *, int32_t);
void image_header_checksum_buffer(
    struct image_header_checksum *, const void *, size_t
);
uint32_t image_header_checksum_done(struct image_header_checksum *);
uint32_t image_checksum(const uint8_t *, uint32_t);

bool image_contents_verify(
    const struct image_header *header, const uint8_t *contents
);
bool image_header_verify(
    const struct image_header *header, struct image_header_checksum *ihcs
);

void getHeaderDefaults(struct image_header *ihdr);

bool parseImageVersion(char *version, struct image_header *ihdr);

void verbose_printf(uint32_t level, const char *fmt, ...);
 
void printHeader(struct image_header *ihdr);

#endif /* __LTRX_IMAGE_H__ */
