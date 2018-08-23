/*
 * m2m projuct
 *
 * FileName: m2m.h
 *
 * Description: m2m sdk and the 3rd party Application need to be included.
 *
 * Author: skylli
 */
#include "../config/config.h"
#include "lm2m_type.h"
#ifndef _M2M_H_
#define _M2M_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define M2M_LOG_ALL    (0)
#define M2M_LOG_DEBUG  (1)
#define M2M_LOG        (2)
#define M2M_LOG_WARN   (3)
#define M2M_LOG_ERROR  (4)

#ifndef M2M_VERSION_MAJOR
#include "BuildValue.h"
#endif

#define m2m_printf  printf

typedef enum LM2M_CMD_T{
	LM2M_CMD_NONE = 0,
	LM2M_CMD_SCAN = 1,
	LM2M_CMD_SCAN_ACK = 2,
    LM2M_CMD_GET_TOKEN = 3,
    LM2M_CMD_GET_TOKEN_ACK = 4,
	LM2M_CMD_GPIO_SET = 5,
	LM2M_CMD_GPIO_SET_ACK = 6,
	CMD_SEND_SERIAL = 7,
	CMD_SEND_SERIAL_ACK = 8,
	CMD_ERR,
	
    LM2M_CMD_MAX
}Lm2m_cmd_T;
typedef enum LM2M_RET_T{
	LM2M_RET_OK = 0,
	LM2M_RET_NULL = -1,	 // 内存 错误
	LM2M_RET_ILL	= -2,   // 非法值
	LM2M_RET_SOCKET = -3,	// socket 
	LM2M_RET_NOMATCH = -4,
	LM2M_RET_MAX
}Lm2m_ret_T;

typedef struct LM2M_PKT_T{
	u8 ver;  //版本号
	u8 msgid;	// session index 
	u16 token;  // 回话令牌   设备端生成
	u32 src_idh; // 包 源id
	u32 src_idl;
	u32 dst_idh; // 包 目标id
	u32 dst_idl;
	u16 cmd;	// 命令
	u16 len;	// payload 长度
	u8  p_payload[0];
}Lm2m_pkt_T;
typedef struct LM2M_ADDRESS_T
{
    u8 len;
    u8 ip[16];
    u16 port;
} LM2M_Address_T;

typedef struct LM2M_LOCAL_T{
	int fd;
	u16 port;
	u32 idh;
	u32 idl;
}Lm2m_local_T;
typedef struct LM2M_SESSION_T{
	u8 flag;
	u8 msgid;
	u16 token;
	u32 dst_idh;
	u32 dst_idl;
	LM2M_Address_T addr;
}Lm2m_session_T;

typedef struct LM2M_PACKET_T{
    u32 len;
    u8* p_data;
}LM2M_packet_T;
typedef struct LM2M_PAYLOAD_T{
    u32 len;
    u8 p_data[0];
}LM2M_payload_T;

typedef struct{
	u8 pin;
	u8 val;
}UP2P_IO;

#ifdef __cplusplus
}
#endif

#endif/*_M2M_H_*/

