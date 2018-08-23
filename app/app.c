#include "../include/lm2m.h"
#include "../include/lm2m_api.h"
#include "../include/lm2m.h"
#include "../include/lm2m_log.h"
#include "../config/config.h"

#define TST_GETOTKEN "TST_GETOTKEN"
#define TST_GEGPIO "TST_GEGPIO"
int hastoken = 0;
// session 
int app_recv_handle(Lm2m_local_T    *p_local, Lm2m_session_T *p_s, Lm2m_pkt_T *p_recv){
	Lm2m_session_T *p_new_s = NULL;
	Lm2m_session_T session;
	LM2M_Address_T addr_src;
	m2m_log("receive data %s", p_recv->p_payload);

	if( p_recv->len && p_recv->p_payload){
		m2m_bytes_dump("receive bytes:", p_recv->p_payload, p_recv->len );	
	}
	switch(p_recv->cmd){
		case LM2M_CMD_GET_TOKEN_ACK:
			m2m_log("receive cmd LM2M_CMD_GET_TOKEN_ACK");
			m2m_log("receive data %s", p_recv->p_payload);

			p_s->token = p_recv->token;
			hastoken = 1;
			return 0;
		case LM2M_CMD_SCAN_ACK:
			m2m_log("receive cmd CMD_LAN_SCAN_ACK");
			m2m_log("receive data %s", p_recv->p_payload);
			//SESSION_CPY(session,p_recv,&addr_src);
			memcpy(&session.dst_idh,p_recv->p_payload, sizeof(u32));
			return 0;
	}
}

int main(void)
{
	char strcmd[512];

	Lm2m_local_T local;
	Lm2m_session_T session;
	int ret = 0;
	mmemset(&local, 0, sizeof(Lm2m_local_T));
	mmemset(&session, 0, sizeof(Lm2m_session_T));

	local.idl = 2;
	local.port = 9528;
	if( lm2m_upd_server_bind( &local) < 0){
		m2m_log_error("creat socket failt!!");
		return -1;
	}
	
	session.dst_idl = 1;
	session.flag = 1;
	m2m_gethostbyname(&session.addr, "127.0.0.1");
	session.addr.port = 9527;


	lm2m_send(&local, &session,LM2M_CMD_SCAN,0, NULL);
	while(1){
		LM2M_Address_T addr_recv;
		int len_recv = PKT_MAX_BYTE,i;
		u8 buf_recv[PKT_MAX_BYTE];
		Lm2m_pkt_T *p_pkt = NULL;
		len_recv = m2m_receive(local.fd, &addr_recv,buf_recv, PKT_MAX_BYTE, RECV_TIMEOUT);
		p_pkt = lm2m_receive_match(&local, &session, len_recv, buf_recv);
		if(p_pkt){
			ret = app_recv_handle(&local, &session,p_pkt);
			break;
		}
	}
		lm2m_send(&local, &session,LM2M_CMD_GET_TOKEN, strlen(TST_GETOTKEN), TST_GETOTKEN);	
	while(1){
		LM2M_Address_T addr_recv;
		int len_recv = PKT_MAX_BYTE,i;
		u8 buf_recv[PKT_MAX_BYTE];
		Lm2m_pkt_T *p_pkt = NULL;
		UP2P_IO io;
		mmemset(strcmd, 0, 512);
		mmemset(buf_recv, 0, PKT_MAX_BYTE);
		mmemset(&addr_recv, 0, sizeof(LM2M_Address_T));
		if(hastoken){
			scanf("%s", strcmd);
			printf("input: %s\n", strcmd);
			printf("\n input string to be send out:\t");
			if(strlen(strcmd) < 2  || strcmd[0] < '0' || strcmd[0] > '9' || strcmd[1] < '0' || strcmd[1] > '1')
						continue;
				   io.pin = strcmd[0] - '0';
				   io.val = strcmd[1] - '0';
				   lm2m_send(&local, &session,LM2M_CMD_GPIO_SET, sizeof(UP2P_IO), &io);
		}	
		len_recv = m2m_receive(local.fd, &addr_recv,buf_recv, PKT_MAX_BYTE, RECV_TIMEOUT);
		p_pkt = lm2m_receive_match(&local, &session, len_recv, buf_recv);
		if(p_pkt){
			ret = app_recv_handle(&local, &session,p_pkt);
			//break;
		}
		
	}
	#if 0
	lm2m_send(&local, &session,LM2M_CMD_GPIO_SET, strlen(TST_GEGPIO), TST_GEGPIO);
		

	#endif
}
