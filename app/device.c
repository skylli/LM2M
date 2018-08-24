#include "../include/lm2m.h"
#include "../include/lm2m_api.h"
#include "../include/lm2m.h"
#include "../include/lm2m_log.h"
#include "../config/config.h"
#include "../include/util.h"

#define PIN_VALUE_STR(v)	(v>0)?"HIGH":"LOW"
// session 
Lm2m_session_T local_session[SESSION_MAX];
u8 token_index = 0;
Lm2m_session_T *session_creat(Lm2m_pkt_T *p_recv,LM2M_Address_T *p_addr_src){
	int i = 0;
	u32 cur_tm  =  m2m_current_time_get();
	u8 tmp_randm = (u8)m2m_get_random();
	for(i=0; i<SESSION_MAX; i++){
		m2m_log("session alive time %d, current time %d",local_session[i].active_tm, cur_tm );
		if(  local_session[i].flag == 0 || DIFF_(local_session[i].active_tm, cur_tm) > SESSION_LIFTE_TIME){
			local_session[i].dst_idh = p_recv->src_idh;
			local_session[i].dst_idl = p_recv->src_idl;
			local_session[i].msgid = p_recv->msgid;
			local_session[i].token = ((tmp_randm << 8) |token_index++);		
			mcpy(&local_session[i].addr, p_addr_src, sizeof(LM2M_Address_T));
			local_session[i].flag = 1;
			
			return &local_session[i];
		}
	}
	return NULL;
}
int recv_handle(Lm2m_local_T    *p_local, Lm2m_session_T *p_s, Lm2m_pkt_T *p_recv,LM2M_Address_T *p_addr_src){
	Lm2m_session_T *p_new_s = NULL;
	Lm2m_session_T tmp_ps ;

	switch(p_recv->cmd){
		case LM2M_CMD_GET_TOKEN:
			printf("receive token request");
			p_new_s = session_creat(p_recv, p_addr_src);
		
			p_s->active_tm = m2m_current_time_get();
			lm2m_ack_send(p_local, p_new_s,LM2M_CMD_GET_TOKEN_ACK, sizeof(u16), &p_new_s->token);
			return 0;
		case LM2M_CMD_GPIO_SET:
			//p_new_s = session_creat(p_recv, p_addr_src);
			// digitalWrite(p_recv->p_payload[0],p_recv->p_payload[1]);
			SESSION_CPY(tmp_ps, p_recv,p_addr_src);
			p_s->active_tm = m2m_current_time_get();
			lm2m_ack_send(p_local, &tmp_ps,LM2M_CMD_GPIO_SET_ACK, 2* sizeof(u32), p_local->idh);
			printf("----->setting pin %d to %s \n",p_recv->p_payload[0],PIN_VALUE_STR(p_recv->p_payload[1]));
			return 0;
		case LM2M_CMD_SCAN:
			printf("receive cmd LM2M_CMD_SCAN");
			SESSION_CPY(tmp_ps, p_recv,p_addr_src);
			lm2m_ack_send(p_local, &tmp_ps,LM2M_CMD_SCAN_ACK, 2 * sizeof(u32), &p_local->idh);			
			return 0;
		case CMD_SEND_SERIAL:
			printf("receive cmd CMD_SEND_SERIAL");
			p_s->active_tm = m2m_current_time_get();
			return 0;
	}
}

int main(int argc, char **argv)
{
	Lm2m_local_T local;
	int ret = 0;
	u8 lid[ID_LEN],id_tmp[2*ID_LEN];
	u8 *p_id = &local.idh;
	mmemset(&local, 0, sizeof(Lm2m_local_T));
	mmemset(lid, 0, ID_LEN);	
	mmemset(id_tmp, 0, 2*ID_LEN);
	if(argc < 3){
		m2m_printf("Need more parameter/n please input : %s <device's id> <device's port>\n",argv[0]);
       return -1;
    }
	m2m_printf("local device id is: %s ; port: %s\n", argv[1], argv[2]);
	STR_2_INT_ARRAY_ORDER(id_tmp, argv[1], M_MIN( strlen(argv[1]), 2*ID_LEN) );
	STR_2_HEX_ARRAY(lid, id_tmp,  M_MIN( strlen(argv[1]), 2*ID_LEN));
	INVERT_CPY(p_id, lid, ID_LEN);

	local.port = atoi(argv[2]);
	if( lm2m_upd_server_bind( &local) < 0){
		m2m_log_error("creat socket failt!!");
		return -1;
	}
	m2m_log("Listing udp port %d,device ready to used ....", local.port);
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
