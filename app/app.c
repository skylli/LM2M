#include "../include/lm2m.h"
#include "../include/lm2m_api.h"
#include "../include/lm2m.h"
#include "../include/lm2m_log.h"
#include "../include/util.h"

#include "../config/config.h"

#define TST_GETOTKEN "TST_GETOTKEN"
#define TST_GEGPIO "TST_GEGPIO"
int hastoken = 0;
Lm2m_session_T session;

// session 
int app_recv_handle(Lm2m_local_T    *p_local, Lm2m_session_T *p_s, Lm2m_pkt_T *p_recv){
	Lm2m_session_T *p_new_s = NULL;
	LM2M_Address_T addr_src;
	m2m_log_debug("receive data %s", p_recv->p_payload);

	if( p_recv->len && p_recv->p_payload){
		m2m_bytes_dump("receive bytes:", p_recv->p_payload, p_recv->len );	
	}
	switch(p_recv->cmd){
		case LM2M_CMD_GET_TOKEN_ACK:
			printf("receive cmd LM2M_CMD_GET_TOKEN_ACK\n");
			m2m_bytes_dump("receive token is ", p_recv->p_payload, p_recv->len);
			p_s->token = p_recv->token;
			hastoken = 1;
			break;
		case LM2M_CMD_SCAN_ACK:
			printf("receive cmd CMD_LAN_SCAN_ACK\n");
			memcpy(&session.dst_idh,p_recv->p_payload,2* sizeof(u32));
			break;
		case LM2M_CMD_GPIO_SET_ACK:
			
			printf("receive cmd LM2M_CMD_GPIO_SET_ACK\n");
			break;
	}
	return 0;
}
int main(int argc, char **argv)
{
	char strcmd[512];
	u8 lid[ID_LEN],id_tmp[2*ID_LEN];
	Lm2m_local_T local;
	u8 *p_id = &session.dst_idh;
	int i=0, ret = 0;
	mmemset(&local, 0, sizeof(Lm2m_local_T));
	mmemset(&session, 0, sizeof(Lm2m_session_T));
	mmemset(lid, 0, ID_LEN);	
	mmemset(id_tmp, 0, 2*ID_LEN);
	if(argc < 4){
       m2m_printf("Need more parameter/n please input : %s <device's id> <device's port> <device's ip>\n",argv[0]);
       return -1;
    }

	m2m_printf("remote device id is: %s ; ip: %s; port: %s\n", argv[1], argv[3],argv[2]);
	STR_2_INT_ARRAY_ORDER(id_tmp, argv[1], M_MIN( strlen(argv[1]), 2*ID_LEN) );
	STR_2_HEX_ARRAY(lid, id_tmp,  M_MIN( strlen(argv[1]), 2*ID_LEN));
	INVERT_CPY(p_id, lid, ID_LEN);

	session.addr.port =  atoi(argv[2]);
	local.port = 0;
	m2m_bytes_dump("remote device id: ",  &session.dst_idh, 2*sizeof(u32));
	if( lm2m_upd_server_bind( &local) < 0){
		m2m_log_error("creat socket failt!!");
		return -1;
	}
	session.flag = 1;
	m2m_gethostbyname(&session.addr, argv[3]);

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
			printf("Input pin and value(eg, 20, set 2 pin to low): ");
			scanf("%s", strcmd);
			if(strlen(strcmd) < 2  || strcmd[0] < '0' || strcmd[0] > '9' || strcmd[1] < '0' || strcmd[1] > '1')
						continue;
				   io.pin = strcmd[0] - '0';
				   io.val = strcmd[1] - '0';
				   lm2m_send(&local, &session,LM2M_CMD_GPIO_SET, sizeof(UP2P_IO), &io);
				   printf("setting pin %d to value %d:\n", io.pin, io.val);
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
