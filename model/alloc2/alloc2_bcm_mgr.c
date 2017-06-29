#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include <base/devel.h>
#include <base/sender.h>
#include <base/thread.h>
#include <base/watchdog.h>
#include <util/tools.h>
#include <util/list.h>
#include <util/transfer.h>
#include <util/poweroff.h>
#include <logd_rpc.h>

#include <netcom.h>
#include <callback.h>
#include <config.h>

#include "alloc2_pkt.h"
#include "alloc2_senario.h"

#include "allkey_bcm_1.h"
#include "alloc2_bcm_mgr.h"


// bcm 에서 오는 이벤트 처리
int mdm_bcm_evt_proc(const int evt_code, const unsigned char stat_1, const unsigned char stat_2, const unsigned char err_list)
{
	printf("%s() - start evt code [%d]\r\n", __func__, evt_code);
	printf("%s() - start evt code [%d]\r\n", __func__, evt_code);
	printf("%s() - start evt code [%d]\r\n", __func__, evt_code);
	int pkt_evt_code = 0;


	switch(evt_code)
	{
		case e_bcm_evt_driver_call:
		{
			break;
		}
		case e_bcm_evt_monitor_off_door_open:
		case e_bcm_evt_monitor_off_door_close:
		{
			if ( allkey_bcm_1_chk_stat(e_bcm_stat1_door_open, stat_1) )
			{
				pkt_evt_code = e_evt_code_door_open;
				LOGT(eSVC_MODEL, " +++ [BCM EVT] - code [%d] ==> door open => pkt evt code [%d] \r\n", evt_code, pkt_evt_code);
			}
			else
			{
				pkt_evt_code = e_evt_code_door_close;
				LOGT(eSVC_MODEL, " +++ [BCM EVT] - code [%d] ==> door close => pkt evt code [%d] \r\n", evt_code, pkt_evt_code);
			}
			
			sender_add_data_to_buffer(e_mdm_stat_evt, &pkt_evt_code, ePIPE_2);
			break;
		}
		case e_bcm_evt_small_shock:
		{
			break;
		}
		case e_bcm_evt_big_shock:
		{
			break;
		}
		case e_bcm_evt_monitor_on_door_stat:
		{
			if ( allkey_bcm_1_chk_stat(e_bcm_stat1_door_open, stat_1) )
			{
				pkt_evt_code = e_evt_code_door_open;
				LOGT(eSVC_MODEL, " +++ [BCM EVT] - code [%d] ==> door open => pkt evt code [%d] \r\n", evt_code, pkt_evt_code);
			}
			else
			{
				pkt_evt_code = e_evt_code_door_close;
				LOGT(eSVC_MODEL, " +++ [BCM EVT] - code [%d] ==> door close => pkt evt code [%d] \r\n", evt_code, pkt_evt_code);
			}

			sender_add_data_to_buffer(e_mdm_stat_evt, &pkt_evt_code, ePIPE_2);
			break;
		}
		case e_bcm_evt_monitor_on_trunk_stat:
		{
			if ( allkey_bcm_1_chk_stat(e_bcm_stat2_trunk_open, stat_2) )
			{
				pkt_evt_code = e_evt_code_trunk_open;
				LOGT(eSVC_MODEL, " +++ [BCM EVT] - code [%d] ==> trunk open => pkt evt code [%d] \r\n", evt_code, pkt_evt_code);
			}
			else
			{
				pkt_evt_code = e_evt_code_trunk_close;
				LOGT(eSVC_MODEL, " +++ [BCM EVT] - code [%d] ==> trunk close => pkt evt code [%d] \r\n", evt_code, pkt_evt_code);
			}

			sender_add_data_to_buffer(e_mdm_stat_evt, &pkt_evt_code, ePIPE_2);
			break;
		}
		case e_bcm_evt_monitor_on_hood_stat:
		{
			allkey_bcm_1_chk_stat(e_bcm_stat1_hood_open, stat_1);
			break;
		}
	}
	
	return ALLKEY_BCM_RET_SUCCESS;
}
#define CHK_ALLKEY_BCM_INTERVAL		5
#define CHK_ALLKEY_BCM_FAIL_CNT		3
// 1초에 한번씩불린다.
void chk_allkey_bcm()
{
	static int chk_cnt = 0;
	static int fail_cnt = 0;

	int cmd_ret = ALLKEY_BCM_RET_FAIL;
//	ALLKEY_BCM_1_INFO_T cur_allkey_bcm_info;

//	memset( &cur_allkey_bcm_info, 0x00, sizeof(ALLKEY_BCM_1_INFO_T) );
	LOGT(eSVC_MODEL, "[ALLKEY MGR] chk cnt [%d]/[%d]\r\n",chk_cnt,CHK_ALLKEY_BCM_INTERVAL);
	
	if ( chk_cnt++ > CHK_ALLKEY_BCM_INTERVAL)
	{
		//allkey_bcm_cmd__get_stat(&cur_allkey_bcm_info);
		cmd_ret = allkey_bcm_cmd__get_stat();
		chk_cnt = 0;
	}
	else
	{
		return;
	}

	if ( cmd_ret == ALLKEY_BCM_RET_SUCCESS )
//	if ( cur_allkey_bcm_info.init_stat == 1 )
	{
		if ( fail_cnt > CHK_ALLKEY_BCM_FAIL_CNT )
			devel_webdm_send_log("[ALLKEY MGR] allkey comm success\r\n");
		fail_cnt = 0;
		return;
	}
	
	fail_cnt++;
	
	LOGE(eSVC_MODEL, "[ALLKEY MGR] chk fail\r\n");

	if ( fail_cnt == CHK_ALLKEY_BCM_FAIL_CNT )
	{
		devel_webdm_send_log("[ALLKEY MGR] allkey comm fail\r\n");
	}

}





