
#ifndef __OS_GLOBAL_H__
#define  __OS_GLOBAL_H__

#include "GlobalDef.h"


#define  FLASH_SAVE 
#define  RO_DATA_IN_FLASH
#define  ALL_STRING_SAVE_IN_FLASH  1
#define SEC(sec)    ((sec) * (1000L / OS_PER_TICK_MS))   // 设置多少秒

typedef enum
{
   OS_OK = 0,
   OS_ERROR = 1,
}OS_RESULT;

typedef enum
{
   E_PROTOCOL_VER = 0,
   E_PRIVATE_VER,
   E_HARD_VER,
   E_SOFT_VER,
   E_PRODUCT_KEY,
}E_VERION_TYPE;

#include <string.h>
#include <stdio.h>
#define  os_memset(src, c, len)         memset(src, c, len)
#define  os_strncpy(dsc, src, len)      strncpy((char *)dsc, (const char *)src, len)
#define  os_strlen(str)                  strlen((char *)str)
#define  os_memcpy(dsc, src, len)       memcpy(dsc, src, len)
#define  os_snprintf(str, size, fmt, ...)        snprintf((char *)str, size, fmt, ##__VA_ARGS__)
#define  os_strchr(s, c)                      strchr((const char *)s, c)  // 查找指定字符在字符串中的第一次出现位置
#define  os_strrchr(s, c)                     strrchr((const char *)s, c)  // 查找指定字符在字符串中的最后一次出现位置


extern E_BOOL gCpuIsBigEndian;  // 是否大端
#define   OS_IsMcuBigEndian()   gCpuIsBigEndian    // mcu 是否大端
void FLASH_SAVE BSP_Init(void);

// 交换long型的字节序
#define swap_long(hdw) (\
	( ((hdw) & 0xFF) << 24 ) |\
	( ((hdw) & 0xFF00) << 8) |\
	( ((hdw) & 0xFF0000) >> 8) |\
	( ((hdw) & 0xFF000000) >> 24)\
	)

// 交换short 型的字节序
#define swap_short(hw) ( ((hw) >> 8) | ((hw) << 8) )

// 将主机上的long 型转换为网络字节序(大端序)
#define htonl(hostlong) ( OS_IsMcuBigEndian() ? hostlong : swap_long(hostlong) )

// 将主机上的 short 型转换为网络字节序(大端序)
#define htons(hw)       ( OS_IsMcuBigEndian() ? hw : swap_short(hw) )

// 将网络long型的字节序(大端序)转换为主机的字节序
#define ntohl(netlong) ( OS_IsMcuBigEndian() ? netlong : swap_long(netlong))

// 将网络short型的字节序(大端序)转换为主机的字节序
#define ntohs(nw) ( OS_IsMcuBigEndian() ? nw : swap_short(nw) )

#if (GIZ_BYTE_ORDER == LITTLE_ENDIAN)  // GIZ 传输的数据字节序为小端模式
#define giz_ntohl(netlong)  ( OS_IsMcuBigEndian() ? swap_long(netlong) : netlong)
#define giz_ntohs(netshort) ( OS_IsMcuBigEndian() ? swap_short(netshort) : netshort)

#define giz_htonl(hostlong) ( OS_IsMcuBigEndian() ? swap_long(hostlong) : hostlong)
#define giz_htons(hw)        ( OS_IsMcuBigEndian() ? swap_short(hw) : hw)

#else  // GIZ 传输采用大端序
#define giz_ntohl(netlong)  ntohl(netlong)
#define giz_ntohs(nw)        ntohs(nw)

#define giz_htonl(hostlong) htonl(hostlong)
#define giz_htons(hw)        htons(hw)
#endif

// 通用字节序转换, 主机 -> 网络, 1, 2, 4字节
#define giz_hton(host_bytes) (\
	(1 == sizeof(host_bytes) ) ? host_bytes : (\
	(2 == sizeof(host_bytes) ) ? giz_htons(host_bytes) : giz_htonl(host_bytes) ) )

// 通用字节序转换, 网络 -> 主机, 1, 2, 4字节
#define giz_ntoh(net_bytes) (\
	(1 == sizeof(net_bytes) ) ? net_bytes : (\
	(2 == sizeof(net_bytes) ) ? giz_ntohs(net_bytes) : giz_ntohl(net_bytes) ) )

#define net2host(net_bytes)   giz_ntoh(net_bytes)
#define host2net(host_bytes)  giz_hton(host_bytes)

uint32_t OS_GetSysTick(void);
uint32_t OS_SetTimeout(uint32_t tick);
E_BOOL OS_IsTimeout(uint32_t timetick);

//uint32_t FLASH_SAVE Sys_GetRunTime(void);
#define Sys_GetRunTime()   OS_GetSysTick()
void FLASH_SAVE Sys_DelayMs(uint32_t ms);
E_BOOL FLASH_SAVE Sys_IsBigEndian(void);
E_RESULT Sys_GetVersion(uint8_t *outString, int16_t stringSize, E_VERION_TYPE index);
void JumpToBootloader(void);
uint32_t Sys_GenSum32(void * data, uint32_t length);


/******************************** 工具函数****************************************************/
uint8_t Util_NumToString(uint32_t val, uint8_t * out_buf, uint8_t placeholder_size);


#endif

