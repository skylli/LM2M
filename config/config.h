#ifndef _CONFIG_H
#define _CONFIG_H

/**log define ****/
#define CONF_LOG_TIME		1

#define PROT_VER	(0X01)
#define PKT_MAX_BYTE	(1024)
#define SESSION_MAX	(10)

#define SESSION_LIFTE_TIME  ( 3*60*1000)	// session 不活跃时存活时间

/** receive block timeout*************************/
#define RECV_TIMEOUT	(200)
#endif // _CONFIG_H

