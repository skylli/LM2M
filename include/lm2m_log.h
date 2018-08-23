/*
 * m2m projuct
 *
 * FileName: lm2m_log.h
 *
 * Description: log function
 *      1.provide log function.
 *      2.log information can write to both file and screen.
 *                                                                                                                                                                                                 
 * Author: skylli
 */
 #include <stdio.h>
 #include <string.h>
#include "lm2m.h"
#include "../config/config.h"


#ifndef M2M_LOG_H
#define M2M_LOG_H
/** config ************************/
#define LOG_VERBOSE
/** config end***************************/
static const char *s_debug[] = {
        "[ ALL ]",
        "[DEBUG]",
        "[ LOG ]",
        "[WARN ]",
        "[ERROR]",
    };
typedef struct LOG_T{

	FILE *fp;
	u16 warn_cnt;
	u16 err_cnt;
	u32 file_index;
	u8 level;
	u8 *p_log_path;
	
}Log_T;

extern u8 g_log_level;

extern Log_T g_mlog;
#define __FILENAME__ (strrchr(__FILE__, '/')? strrchr(__FILE__, '/') + 1 : __FILE__) 
#ifdef LOG_VERBOSE

#ifdef CONF_LOG_TIME 
void current_time_printf();
#define m2m_debug_level(level, format,...) do{ if( level >= g_log_level ){ \
		m2m_printf("%s ",s_debug[level]); \
		current_time_printf();\
        m2m_printf("%s func:%s LINE: %d: " format "\r\n",__FILENAME__,__func__, __LINE__, ##__VA_ARGS__); \
        }}while(0)
#define m2m_debug_level_noend(level, format,...) do{ if( level >= g_log_level ){ \
				m2m_printf("%s ",s_debug[level]); \
				current_time_printf();\
                m2m_printf("%s func:%s LINE: %d: " format,__FILENAME__,__func__, __LINE__, ##__VA_ARGS__); \
                }}while(0)


#else //CONF_LOG_TIME
#define m2m_debug_level(level, format,...) do{ if( level >= g_log_level ){ \
        m2m_printf("%s: %s func:%s LINE: %d: " format "\r\n",s_debug[level],__FILENAME__,__func__, __LINE__, ##__VA_ARGS__); \
        }}while(0)
#define m2m_debug_level_noend(level, format,...) do{ if( level >= g_log_level ){ \
                m2m_printf("%s: %s func:%s LINE: %d: " format,s_debug[level],__FILENAME__,__func__, __LINE__, ##__VA_ARGS__); \
                }}while(0)
#endif  //CONF_LOG_TIME
#else
#define m2m_debug_level(level, format,...) do{ if( level >= g_log_level ){ \
            m2m_printf("%s:" format "\r\n",s_debug[level], ##__VA_ARGS__); \
            }}while(0)
#define m2m_debug_level_noend(level, format,...) do{ if( level >= g_log_level ){ \
                m2m_printf("%s:" format,s_debug[level], ##__VA_ARGS__); \
                }}while(0)
#endif


#define m2m_byte_print(p,n) do{int i=0;for( i=0; p && i<n;i++){ m2m_printf("[%x]",p[i]);}}while(0)
#ifdef C_HAS_FILE
/* log api provided */
void m2m_record_init(int level, const char *p_file);
void m2m_record_uninit(void);
void m2m_file_print(int level,const char *fmt, ...);

#define m2m_log_init(l,f)     do{ m2m_record_level_set(l); m2m_record_init(l,f); }while(0)
#define m2m_log_uninit()    do{ m2m_record_uninit(); }while(0)

#define m2m_log(format,...) do{ m2m_file_print( M2M_LOG,format,##__VA_ARGS__); }while(0)
#define m2m_log_debug(format,...) do{m2m_file_print( M2M_LOG_DEBUG,format,##__VA_ARGS__); }while(0)
#define m2m_log_warn(format,...) do{m2m_file_print( M2M_LOG_WARN,format,##__VA_ARGS__);}while(0)
#define m2m_log_error(format,...) do{ m2m_file_print( M2M_LOG_ERROR,format,##__VA_ARGS__);}while(0)
#define m2m_debug(format,...) do{  m2m_file_print( M2M_LOG_DEBUG,format,##__VA_ARGS__);}while(0)
#else
#define m2m_log_init(l,f)     m2m_record_level_set(l)
#define m2m_log_uninit()    

#define m2m_log(format,...)         m2m_debug_level(M2M_LOG, format, ##__VA_ARGS__)
#define m2m_log_debug(format,...)   m2m_debug_level(M2M_LOG_DEBUG, format, ##__VA_ARGS__)
#define m2m_log_warn(format,...)    m2m_debug_level(M2M_LOG_WARN, format, ##__VA_ARGS__)
#define m2m_log_error(format,...)   m2m_debug_level(M2M_LOG_ERROR, format, ##__VA_ARGS__)
#define m2m_debug(format,...)       m2m_debug_level(M2M_LOG_DEBUG, format, ##__VA_ARGS__)


#endif

#define m2m_assert(_arg, _return) do{if((_arg)==0) \
        {m2m_printf("%s %d, assert failed!\r\n",__func__, __LINE__); \
            return (_return);}}while(0)

void m2m_bytes_dump(u8 *p_shd,u8 *p,int len);


#endif
