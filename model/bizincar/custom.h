#ifndef __MODEL_CUSTOM_H__
#define __MODEL_CUSTOM_H__

typedef enum msg_ev_code_custom msg_ev_code_custom_t;
enum msg_ev_code_custom{
	eKIT_DEFINE_1_EVT			= 90,
	eKIT_DEFINE_2_EVT			= 93,

	eMAX_EVENT_CODE                 = 94, //LAST_EVENT_CODE + 1
};

#define MILEAGE_FILE	"/data/mds/data/mdt800_model_mileage.dat"

#endif