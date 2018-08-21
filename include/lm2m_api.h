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
#ifndef _LM2M_API_H
#define _LM2M_API_H

int lm2m_upd_server_bind(Lm2m_local_T *p_local);
Lm2m_ret_T lm2m_send(Lm2m_local_T *p_local, Lm2m_session_T *p_s, Lm2m_cmd_T cmd,int len, u8 *p_data);
Lm2m_pkt_T *lm2m_receive_match(Lm2m_local_T *p_local, Lm2m_session_T *p_s, int len, u8 *p_recv);

#endif // _LM2M_API_H

