#ifndef TYPEDEFINE_H

#define TYPEDEFINE_H

#include<stdio.h>

//#define DATAVIEWBUFFERLENTH		8192 /* (1024*8)*/
#define INVALID_VERSION				0xFF /* */

#define VALUE_MAX_LENGTH_IN_BYTE   8

//#define STOP_WHEN_ERROR

//#define NULL                                ((void *)0)
#define SHOW(fmt, args...) \
        printf(fmt , ##args)


#ifdef _NODEBUG
#define DBG(fmt, args...)
#else
#define DBG(fmt, args...) \
        printf("<DBG:%s:%d> " fmt "\n", __FUNCTION__, __LINE__, ##args)
#endif

#ifdef _NOERROR
#define ERR(fmt, args...)
#else
#ifdef STOP_WHEN_ERROR
#define ERR(fmt, args...) ({\
                printf("<ERR:%s:%s:%d> " fmt "\n", __FILE__,__FUNCTION__, __LINE__, ##args);\
                while(1);\
                })
#else
#define ERR(fmt, args...) \
                printf("<ERR:%s:%s:%d> " fmt "\n", __FILE__,__FUNCTION__, __LINE__, ##args)
#endif
#endif

#ifdef _NOINFOM
#define INF(fmt, args...)
#else
#define INF(fmt, args...) \
        printf(fmt "\n", ##args)
#endif

#define ASSERT(x) ({ \
    if (!(x)) { \
        printf("<assertion failed :%s:%s:%d> \n", __FILE__,__FUNCTION__,  __LINE__);\
        while(1); \
    } \
})

//#define     SYSTEM_WITH_ES_OPERATORION

typedef unsigned char                           	u8;
typedef  char                           	            s8;

typedef unsigned short                          	u16;
typedef short                          	        s16;

//typedef unsigned long                            u32;
//typedef long	                           		 s32;
typedef unsigned int                            u32;
typedef int	                           		 s32;

typedef unsigned long long                           u64;
typedef long long                     		 s64;

#define TestDataLenth() { \
        printf("u8 lenth is %d . \n",sizeof(u8));\
        printf("s8 lenth is %d . \n",sizeof(s8));\
                                                            \
        printf("u16 lenth is %d . \n",sizeof(u16));\
        printf("s16 lenth is %d . \n",sizeof(s16));\
                                                            \
        printf("u32 lenth is %d . \n",sizeof(u32));\
        printf("s32 lenth is %d . \n",sizeof(s32));\
                                                            \
        printf("u64 lenth is %d . \n",sizeof(u64));\
        printf("s64 lenth is %d . \n",sizeof(s64));\
}


typedef enum
{
	FIELD_FMT_BIN,
	FIELD_FMT_DEC,
	FIELD_FMT_HEX,
	FIELD_FMT_BCD,
	FIELD_FMT_MJD,
	FIELD_FMT_STR,
	FIELD_FMT_INVALID
}  Format_e;


#endif
