#ifndef __MODEL_CONFIG_H__
#define __MODEL_CONFIG_H__

/*===========================================================================================*/
typedef struct configModel configModel_t;
struct configModel
{
	char report_ip[40];
	int report_port;
	char control_ip[40];
	int control_port;
	int report_interval;
	int board_rate;
	int InV_T_collection_interval;

	int tcp_connect_retry_count;
	int tcp_send_retry_count;
	int tcp_receive_retry_count;
	int tcp_timeout_secs;
	char model_name[11];


};

typedef struct configurationModel configurationModel_t;
struct configurationModel
{
	configModel_t model;
};
/*===========================================================================================*/

configurationModel_t* load_config_model(void);
configurationModel_t* get_config_model(void);
configurationModel_t* load_config_user(void);
void load_config_base_default(void);
void load_config_model_default(void);
void load_config_user_default(void);

#endif
