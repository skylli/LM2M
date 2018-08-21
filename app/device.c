#include "../include/lm2m.h"
#include "../include/lm2m_api.h"
#include "../include/lm2m.h"
#include "../include/lm2m_log.h"
#include "../config/config.h"

// session 
Lm2m_session_T local_session[SESSION_MAX];
u16 token_index = 0;
Lm2m_session_T *session_creat(Lm2m_pkt_T *p_recv,LM2M_Address_T *p_addr_src){
	int i = 0;
	for(i=0; i<SESSION_MAX; i++){
		if(local_session[i].flag == 0){
			local_session[i].dst_idh = p_recv->src_idh;
			local_session[i].dst_idl = p_recv->src_idl;
			local_session[i].msgid = p_recv->msgid;
			local_session[i].token = token_index++;		
			mcpy(&local_session[i].addr, p_addr_src, sizeof(LM2M_Address_T));
			local_session[i].flag = 1;
			
			return &local_session[i];
		}
	}
	return NULL;
}
int recv_handle(Lm2m_local_T    *p_local, Lm2m_session_T *p_s, Lm2m_pkt_T *p_recv,LM2M_Address_T *p_addr_src){
	Lm2m_session_T *p_new_s = NULL;

	switch(p_recv->cmd){
		case LM2M_CMD_GET_TOKEN:
			m2m_log("receive token request");
			p_new_s = session_creat(p_recv, p_addr_src);
			lm2m_send(p_local, p_new_s,LM2M_CMD_GET_TOKEN_ACK, sizeof(u16), &p_new_s->token);
			return 0;
			// todo 
	}
}

int main(void)
{
	Lm2m_local_T local;
	int ret = 0;
	mmemset(&local, 0, sizeof(Lm2m_local_T));

	local.idl = 1;
	local.port = 9527;
	if( lm2m_upd_server_bind( &local) < 0){
		m2m_log_error("creat socket failt!!");
		return -1;
	}
	m2m_log("listing ....");
	while(1){
		LM2M_Address_T addr_src;
		int len_recv = PKT_MAX_BYTE,i;
		u8 buf_recv[PKT_MAX_BYTE];
		Lm2m_pkt_T *p_pkt = NULL;
		
		mmemset(buf_recv, 0, PKT_MAX_BYTE);
		mmemset(&addr_src, 0, sizeof(LM2M_Address_T));

		len_recv = m2m_receive(local.fd, &addr_src, buf_recv, PKT_MAX_BYTE, RECV_TIMEOUT);
		for(i=0; i< SESSION_MAX; i++){
			
			p_pkt = lm2m_receive_match(&local, &local_session[i], len_recv, buf_recv);
			if(p_pkt){
				ret = recv_handle(&local, &local_session[i],p_pkt, &addr_src);
				break;
			}
		}
	}	
}
