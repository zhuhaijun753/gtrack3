/**
* @file parsing.c
* @brief 
* @author Jinwook Hong
* @version 
* @date 2013-11-08
*/
#include <stdlib.h>
#include <string.h>

#include <wrapper/dtg_log.h>
#include <wrapper/dtg_convtools.h>
#include <standard_protocol.h>
#include <dtg_data_manage.h>
#include <wrapper/dtg_atcmd.h>
#include <wrapper/dtg_version.h>
#include <board/modem-time.h>
#include <time.h>

void print_msg(char *title, char *msg, int len)
{
	char dmsg[256];
	memset(dmsg, 0x00, 256);
	memcpy(dmsg, msg, len);
	DTG_LOGI("%s : %s", title, dmsg);
}

//int etrs_dtg_parsing(unsigned char *std_buff, int std_buff_len, unsigned char *pack_buf, int data_id)
int bulk_dtg_parsing(unsigned char *std_buff, int std_buff_len, unsigned char *pack_buf)
{
	int src_idx = 0;
	int dest_idx = 0;
	struct tm cur_time;
	time_t system_time;
	struct tm *timeinfo;
	int data_size = 0;
	unsigned short val_crc16 = 0;
	tacom_std_hdr_t *p_std_hdr;
	tacom_std_data_t *p_std_data;

	//sample : structure for etrace model
	//etrace_packet_hdr_t *p_etr_pck_hdr;
	//etrace_dtg_hdr_t *p_etr_dtg_hdr;
	//etrace_dtg_body_t *p_etr_dtg_data;

	char *phonenum = atcmd_get_phonenum(); 

/*	
	p_etr_pck_hdr = (etrace_packet_hdr_t *)&pack_buf[dest_idx];
	dest_idx += sizeof(etrace_packet_hdr_t);

	p_etr_pck_hdr->prot_id = 0x45;
	p_etr_pck_hdr->data_id = data_id;//0x44; //DTG DATA
	memset(p_etr_pck_hdr->terminal_id, 0x20, sizeof(p_etr_pck_hdr->terminal_id));
	if(phonenum != NULL) {
		strncpy(p_etr_pck_hdr->terminal_id, phonenum, strlen(phonenum));
	}

	if(get_modem_time_tm(&cur_time) != MODEM_TIME_RET_SUCCESS) {
		time(&system_time);
		timeinfo = localtime ( &system_time );
	}
	else {
		timeinfo = (struct tm *)&cur_time;
	}

	p_etr_pck_hdr->date.year = timeinfo->tm_year+1900;
	p_etr_pck_hdr->date.mon = timeinfo->tm_mon+1;
	p_etr_pck_hdr->date.day = timeinfo->tm_mday;
	p_etr_pck_hdr->date.hour = timeinfo->tm_hour;
	p_etr_pck_hdr->date.min = timeinfo->tm_min;
	p_etr_pck_hdr->date.sec = timeinfo->tm_sec;

	p_etr_pck_hdr->dtg_id = 567;


	p_std_hdr = (tacom_std_hdr_t *)&std_buff[src_idx];
	p_etr_dtg_hdr = (etrace_dtg_hdr_t *)&pack_buf[dest_idx];
	src_idx += sizeof(tacom_std_hdr_t);
	dest_idx += sizeof(etrace_dtg_hdr_t);


	memset(p_etr_dtg_hdr, 0x00, sizeof(etrace_dtg_hdr_t));
	strncpy(p_etr_dtg_hdr->model, p_std_hdr->vehicle_model, sizeof(p_etr_dtg_hdr->model));
	memset(p_etr_dtg_hdr->vin, 0x20, sizeof(p_etr_dtg_hdr->vin));
	strncpy(p_etr_dtg_hdr->vin, p_std_hdr->vehicle_id_num, sizeof(p_std_hdr->vehicle_id_num));
	p_etr_dtg_hdr->vechicle_type = (p_std_hdr->vehicle_type[0]-0x30)*10 + (p_std_hdr->vehicle_type[1] - 0x30);
	
	
	memset(p_etr_dtg_hdr->vrn, 0x20, sizeof(p_etr_dtg_hdr->vrn));
	if (!strncmp(p_std_hdr->registration_num, "####",4)) {
		memcpy(p_etr_dtg_hdr->vrn, &p_std_hdr->registration_num[4], 8);
	} else if (!strncmp(p_std_hdr->registration_num, "0000",4)) {
		memcpy(p_etr_dtg_hdr->vrn, &p_std_hdr->registration_num[4], 8);
	} else if (!strncmp(p_std_hdr->registration_num, "����",4)) {
		memcpy(p_etr_dtg_hdr->vrn, &p_std_hdr->registration_num[4], 8);
	} else {
		memcpy(p_etr_dtg_hdr->vrn, p_std_hdr->registration_num, 12);
	}

	print_msg("������ȣ", p_etr_dtg_hdr->vin, sizeof(p_etr_dtg_hdr->vin));
	print_msg("������ȣ", p_etr_dtg_hdr->vrn, sizeof(p_etr_dtg_hdr->vrn));

	strncpy(p_etr_dtg_hdr->brn, p_std_hdr->business_license_num, sizeof(p_etr_dtg_hdr->brn));
	strncpy(p_etr_dtg_hdr->driver_code, p_std_hdr->driver_code, sizeof(p_etr_dtg_hdr->driver_code));

	data_size += sizeof(etrace_dtg_hdr_t);
	
	printf("start src_idx [%d]\n", src_idx);
	p_etr_dtg_hdr->record_count = 0;
	while ((std_buff_len - src_idx) >= sizeof(tacom_std_data_t)) {
		p_std_data = (tacom_std_data_t *)&std_buff[src_idx];
		p_etr_dtg_data = (etrace_dtg_body_t *)&pack_buf[dest_idx];
	
		p_etr_dtg_data->distance_a_day = char_mbtol(p_std_data->day_run_dist, 4) * 1000;
		p_etr_dtg_data->distance_all = char_mbtol(p_std_data->cumul_run_dist, 7) * 1000;

		//int i;
		//printf("std data time : ");
		//for(i = 0; i < 14; i++)
		//	printf("[%02x]", p_std_data->date_time[i]);
		//printf("\n");


		p_etr_dtg_data->date[0] = char_mbtol(p_std_data->date_time,    2); //year
		p_etr_dtg_data->date[1] = char_mbtol(p_std_data->date_time+ 2, 2); //month
		p_etr_dtg_data->date[2] = char_mbtol(p_std_data->date_time+ 4, 2); //day

		p_etr_dtg_data->date[3] = char_mbtol(p_std_data->date_time+ 6, 2); //hour
		p_etr_dtg_data->date[4] = char_mbtol(p_std_data->date_time+ 8, 2); //min
		p_etr_dtg_data->date[5] = char_mbtol(p_std_data->date_time+10, 2); //sec
		p_etr_dtg_data->date[6] = char_mbtol(p_std_data->date_time+12, 2);

		//printf("etrace data time : ");
		//for(i = 0; i < 7; i++)
		//	printf("[%02x]", p_etr_dtg_data->date[i]);
		//printf("\n");

		p_etr_dtg_data->speed = char_mbtol(p_std_data->speed, 3);
		p_etr_dtg_data->rpm = char_mbtol(p_std_data->rpm, 4);
		p_etr_dtg_data->bs = (p_std_data->bs << 7);
		p_etr_dtg_data->gps_x = char_mbtol(p_std_data->gps_x, 9);
		p_etr_dtg_data->gps_y = char_mbtol(p_std_data->gps_y, 9);
		p_etr_dtg_data->azimuth = char_mbtol(p_std_data->azimuth, 3);
		p_etr_dtg_data->accelation_x = (short)(char_mbtod(p_std_data->accelation_x, 6) * 10);
		p_etr_dtg_data->accelation_y = (short)(char_mbtod(p_std_data->accelation_y, 6) * 10);
		p_etr_dtg_data->status_code = char_mbtol(p_std_data->status, 2);
//printf("gps_x, gps_y = [%d] [%d]\n", p_etr_dtg_data->gps_x, p_etr_dtg_data->gps_y);
//printf("azimuth, speed, rpm = [%d] [%d] [%d]\n", p_etr_dtg_data->azimuth, p_etr_dtg_data->speed, p_etr_dtg_data->rpm);

		p_etr_dtg_data->rtUsedFuelAday = char_mbtol(p_std_data->day_oil_usage, 9);
		p_etr_dtg_data->rtUsedFuelAll = char_mbtol(p_std_data->cumulative_oil_usage, 9);
		p_etr_dtg_data->rtRemainFuelAmount = char_mbtol(p_std_data->residual_oil, 7);

		p_etr_dtg_data->temp1 = char_mbtol(p_std_data->temperature_A, 5);
		p_etr_dtg_data->temp2 = char_mbtol(p_std_data->temperature_B, 5);
		p_etr_dtg_data->temp3 = -5555;	


		data_size += sizeof(etrace_dtg_body_t);
		p_etr_dtg_hdr->record_count += 1;
		dest_idx += sizeof(etrace_dtg_body_t);
		src_idx += sizeof(tacom_std_data_t);
	}

	p_etr_pck_hdr->data_size = data_size; //data size
	printf("date size = [%d]\n", p_etr_pck_hdr->data_size);
	printf("record count = [%d]\n", p_etr_dtg_hdr->record_count);
	val_crc16 = etrs_crc16(pack_buf, sizeof(etrace_packet_hdr_t) + data_size);

	memcpy(&pack_buf[dest_idx], &val_crc16, 2);
	dest_idx += 2;
*/	
	return dest_idx;

}


//int term_info_parsing(unsigned char *std_buff, int std_buff_len, unsigned char *dest, eTrace_vehicle_status_t vs)
int current_dtg_parsing(unsigned char *std_buff, int std_buff_len, unsigned char *dest)
{
	
	tacom_std_hdr_t *p_std_hdr;
	tacom_std_data_t *p_std_data;
	char *phonenum;
	struct tm cur_time;
	time_t system_time;
	struct tm *timeinfo;
	unsigned short val_crc16 = 0;

	return 0;
	
/*
	etrace_packet_hdr_t *p_etr_pck_hdr;
	etrace_ternminal_info_t *p_term_info;

	p_std_hdr = (tacom_std_hdr_t *)std_buff;
	p_std_data = (tacom_std_data_t *)&std_buff[sizeof(tacom_std_hdr_t)];

	p_etr_pck_hdr = (etrace_packet_hdr_t *)dest;
	p_term_info = (etrace_ternminal_info_t *)&dest[sizeof(etrace_packet_hdr_t)];

	phonenum = atcmd_get_phonenum(); 
	p_etr_pck_hdr->prot_id = 0x45;
	p_etr_pck_hdr->data_id = 0x45; //status report (key on/off, breakdown etc)
	memset(p_etr_pck_hdr->terminal_id, 0x20, sizeof(p_etr_pck_hdr->terminal_id));
	if(phonenum != NULL) {
		strncpy(p_etr_pck_hdr->terminal_id, phonenum, strlen(phonenum));
	}

	if(get_modem_time_tm(&cur_time) != MODEM_TIME_RET_SUCCESS) {
		time(&system_time);
		timeinfo = localtime ( &system_time );
	}
	else {
		timeinfo = (struct tm *)&cur_time;
	}

	p_etr_pck_hdr->date.year = timeinfo->tm_year+1900;
	p_etr_pck_hdr->date.mon = timeinfo->tm_mon+1;
	p_etr_pck_hdr->date.day = timeinfo->tm_mday;
	p_etr_pck_hdr->date.hour = timeinfo->tm_hour;
	p_etr_pck_hdr->date.min = timeinfo->tm_min;
	p_etr_pck_hdr->date.sec = timeinfo->tm_sec;

	p_etr_pck_hdr->dtg_id = 567;
	p_etr_pck_hdr->data_size = sizeof(etrace_ternminal_info_t);



	memset(p_term_info, 0x20, sizeof(etrace_ternminal_info_t));
	strncpy(p_term_info->model, p_std_hdr->vehicle_model, sizeof(p_term_info->model));
	strncpy(p_term_info->vin, p_std_hdr->vehicle_id_num, sizeof(p_term_info->vin));
	
	p_term_info->vechicle_type = (p_std_hdr->vehicle_type[0]-0x30)*10 + (p_std_hdr->vehicle_type[1] - 0x30);
	
	memset(p_term_info->vrn, 0x20, sizeof(p_term_info->vrn));
	if (!strncmp(p_std_hdr->registration_num, "####",4)) {
		memcpy(p_term_info->vrn, &p_std_hdr->registration_num[4], 8);
	} else if (!strncmp(p_std_hdr->registration_num, "0000",4)) {
		memcpy(p_term_info->vrn, &p_std_hdr->registration_num[4], 8);
	} else if (!strncmp(p_std_hdr->registration_num, "����",4)) {
		memcpy(p_term_info->vrn, &p_std_hdr->registration_num[4], 8);
	} else {
		memcpy(p_term_info->vrn, p_std_hdr->registration_num, 12);
	}

	strncpy(p_term_info->brn, p_std_hdr->business_license_num, sizeof(p_term_info->brn));
	strncpy(p_term_info->driver_code, p_std_hdr->driver_code, sizeof(p_term_info->driver_code));
	p_term_info->distance_all = char_mbtol(p_std_data->cumul_run_dist, 7) * 1000;
	p_term_info->gps_x = char_mbtol(p_std_data->gps_x, 9) * 10;
	p_term_info->gps_y = char_mbtol(p_std_data->gps_y, 9) * 10;
	p_term_info->report_period = get_dtg_report_period();
	p_term_info->vs = vs;

	
	//val_crc16 = etrs_crc16(&dest[1], sizeof(etrace_packet_hdr_t)-1 + sizeof(etrace_ternminal_info_t));
	val_crc16 = etrs_crc16(dest, sizeof(etrace_packet_hdr_t) + sizeof(etrace_ternminal_info_t));
	memcpy(&dest[sizeof(etrace_packet_hdr_t) + sizeof(etrace_ternminal_info_t)], &val_crc16, 2);

	return sizeof(etrace_packet_hdr_t) + sizeof(etrace_ternminal_info_t)+2;
*/
}