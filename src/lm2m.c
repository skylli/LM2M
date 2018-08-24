/*
 * network.h
 * description: handle connetion,socket
 *  Created on: 2018-08-21
 *      Author: skylli
 */
#include "../config/config.h"
#include "../include/lm2m.h"
#include "../include/lm2m_log.h"

#include "../include/util.h"

#include <string.h>

/** package thing *****/

/** network thing .*****/
int lm2m_upd_server_bind(Lm2m_local_T *p_local){
	int ret = m2m_openSocket( &p_local->fd, p_local->port);
	return ret;
}
Lm2m_ret_T lm2m_ack_send(Lm2m_local_T *p_local, Lm2m_session_T *p_s, Lm2m_cmd_T cmd,int len, u8 *p_data){
	u8 data_send[PKT_MAX_BYTE];
	Lm2m_pkt_T *p_pkt = data_send;
	mmemset(data_send, 0, PKT_MAX_BYTE);

	if( !p_local || !p_s || cmd >= LM2M_CMD_MAX   )
		return LM2M_RET_ILL;
	/* full protocoll thing.*/
	p_pkt->ver = PROT_VER;
	p_pkt->src_idh = p_local->idh;
	p_pkt->src_idl = p_local->idl;
	// session data
	p_pkt->msgid = p_s->msgid;
	p_pkt->token = p_s->token;
	p_pkt->dst_idh = p_s->dst_idh;
	p_pkt->dst_idl = p_s->dst_idl;
	p_pkt->cmd = cmd;
	// payload
	if(len > 0 && p_data){
		p_pkt->len = len;
		m2m_bytes_dump("send data: ", p_data, len);
		mcpy(p_pkt->p_payload, p_data,len);
	}

	return m2m_send(p_local->fd, &p_s->addr, data_send, (sizeof(Lm2m_pkt_T) + len ) );
}

Lm2m_ret_T lm2m_send(Lm2m_local_T *p_local, Lm2m_session_T *p_s, Lm2m_cmd_T cmd,int len, u8 *p_data){
	u8 data_send[PKT_MAX_BYTE];
	Lm2m_pkt_T *p_pkt = data_send;
	mmemset(data_send, 0, PKT_MAX_BYTE);

	if( !p_local || !p_s || cmd >= LM2M_CMD_MAX   )
		return LM2M_RET_ILL;
	/* full protocoll thing.*/
	p_pkt->ver = PROT_VER;
	p_pkt->src_idh = p_local->idh;
	p_pkt->src_idl = p_local->idl;
	// session data
	p_s->msgid++;
	p_pkt->msgid = p_s->msgid;
	p_pkt->token = p_s->token;
	p_pkt->dst_idh = p_s->dst_idh;
	p_pkt->dst_idl = p_s->dst_idl;
	p_pkt->cmd = cmd;
	// payload
	if(len > 0 && p_data){
		p_pkt->len = len;
		m2m_bytes_dump("send data: ", p_data, len);
		mcpy(p_pkt->p_payload, p_data,len);
	}

	return m2m_send(p_local->fd, &p_s->addr, data_send, (sizeof(Lm2m_pkt_T) + len ) );
}
Lm2m_pkt_T *lm2m_receive_match(Lm2m_local_T *p_local, Lm2m_session_T *p_s, int len, u8 *p_recv){
	Lm2m_pkt_T *p_pkt = NULL;

	if( !p_local || !p_s || len < sizeof(Lm2m_pkt_T) || !p_recv){
		
		return NULL;

		}
	p_pkt = (Lm2m_pkt_T*)p_recv;
	// protocol version
	// token
	// message id
	// id
	if( p_pkt->cmd == LM2M_CMD_SCAN && p_pkt->ver == PROT_VER)
		return p_pkt;
		
	if( p_pkt->ver != PROT_VER  || \
		p_pkt->dst_idh != p_local->idh || p_pkt->dst_idl != p_local->idl ){
		m2m_log_warn(" prot not match !");
		return NULL;
			}
	m2m_log_debug("cmd %x", p_pkt->cmd);
	if(p_pkt->cmd == LM2M_CMD_GET_TOKEN || p_pkt->cmd == LM2M_CMD_GET_TOKEN_ACK){
		m2m_log_debug("get token request."); 
		return p_pkt;
		} 
	if( p_s->flag == 0 || p_pkt->src_idh != p_s->dst_idh || p_pkt->src_idl != p_s->dst_idl || \
		p_pkt->token !=  p_s->token || ( !A_BIGER_U8(p_pkt->msgid, p_s->msgid ) && p_pkt->msgid != p_s->msgid ) ){
		//m2m_log_error("token or msg not match");
		return NULL;
			}
	p_s->msgid = p_pkt->msgid;
	return p_pkt;
}



