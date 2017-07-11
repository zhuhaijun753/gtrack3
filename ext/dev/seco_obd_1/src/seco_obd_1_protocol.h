#ifndef __SECO_OBD_1_PROTOCOL_H__
#define __SECO_OBD_1_PROTOCOL_H__


#define OBD_ERROR_CODE__DEFAULT_ERR				0
#define OBD_ERROR_CODE__PACKET_TYPE_ERR			1
#define OBD_ERROR_CODE__UNKOWN_CMD				2
#define OBD_ERROR_CODE__PKT_CHECKSUM_ERR		3
#define OBD_ERROR_CODE__UNSUPPORT_FUNC			4
#define OBD_ERROR_CODE__NOT_INIT_VECHICLE		100
#define OBD_ERROR_CODE__NOT_INIT_OBD			101
#define OBD_ERROR_CODE__DETECT_RESTART			102
#define OBD_ERROR_CODE__FIRM_UP_FAIL_CAR_ON		200

#define OBD_ERROR_CODE__NOT_TRANS_DATA_ADDR		300
#define OBD_ERROR_CODE__NOT_TRANS_DATA			400
#define OBD_ERROR_CODE__FLASH_ERASE				500
#define OBD_ERROR_CODE__SET_GENDER_SPEC			600
#define OBD_ERROR_CODE__GET_GENDER_SPEC			700
#define OBD_ERROR_CODE__GET_TIME				800
#define OBD_ERROR_CODE__GET_DTC_CODE			900

#define OBD_ERROR_CODE__SUCCESS					-99
#define OBD_ERROR_CODE__UNKOWN_CODE				-1
#define OBD_ERROR_CODE__NOT_VAILD_CMD_RET		-2
#define OBD_ERROR_CODE__UART_READ_TIMEOUT		-3
#define OBD_ERROR_CODE__NO_DATA_RET				-4
#define OBD_ERROR_CODE__KNOWN_CODE              -5


#define MAX_RET_BUFF_SIZE   2048

int _seco_obd_1_device_argument(char* buff, int buff_len, char* argv[]);
int _seco_obd_uart_chk();

#endif
