# lm2m lib

## usege
```shell
sh automake.sh
cd build
cmake ..
make 

```
# Lm2m 协议

## 简介
Lm2m 协议是给基于 udp 包的传输，内部通过 messageid 对包顺序进行管理，没有加密，没有重传机制，只能用于局域网的控制。

## 协议结构
```c
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

```

	- token： 由设备由设备端生成，开启一个会话时必须先向设备端获取 token， 后续的数据包必须携带 token。 
	- msgid: 会话id，用于区别会话中的每一请求，会话开启端产生，并在发送新命令的 cmd 时，自动加1.
	- id 标示： 各个端应该有各个端的id， 设备端的id= 随机数 + mac地址。
	- src_idh，src_idl: 包的源id， src_idh 的高位，src_idl 的低位。
	- dst_idh，dst_idl：包的目标 id。

## 命令

//cmd命令包
typedef enum LM2M_CMD_T{
	LM2M_CMD_NONE = 0,
    LM2M_CMD_GET_TOKEN = 1,   
    LM2M_CMD_GET_TOKEN_ACK = 2,
	LM2M_CMD_GPIO_SET = 3,
	LM2M_CMD_GPIO_SET_ACK = 4,
	LM2M_CMD_SCAN = 5,
	LM2M_CMD_SCAN_ACK = 6,
	CMD_SEND_SERIAL = 7,
	CMD_SEND_SERIAL_ACK = 8,
	
    LM2M_CMD_MAX
}Lm2m_cmd_T;

	- 设备联网后，可以通过发送 LM2M_CMD_SCAN，向整个网段发送数据包，设备端会回应 LM2M_CMD_GET_TOKEN_ACK  并在 payload 里携带 该设备的  id。
	-要跟设备交互数据时，必须先获取 设备的 token，即发送 LM2M_CMD_GET_TOKEN， 获取到  token 之后再发送控制命令，如 LM2M_CMD_GPIO_SET。
	
###  获取 token

客户端 发送 LM2M_CMD_GET_TOKEN, 设备端回应 LM2M_CMD_GET_TOKEN_ACK，并把 token 放到 payload里， 客户端获取 payload，并用作后续所有包的 token 字段。


