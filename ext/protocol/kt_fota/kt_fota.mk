
KT_FOTA_CONFIG_PATH = ext/protocol/kt_fota/kt_fota_svc
KT_FOTA_CONFIG_FILE = kt_fota.ini
KT_FOTA_CONFIG_FILE_TEST = kt_fota.ini.test

KT_FOTA_CFLAGS = -Iext/protocol/kt_fota
KT_FOTA_CFLAGS += -DKT_FOTA_FILE_PATH=\"/system/mds/system/bin/$(KT_FOTA_CONFIG_FILE)\" 
KT_FOTA_CFLAGS += -DKT_FOTA_ENABLE

OBJ_KT_FOTA = ext/protocol/kt_fota/kt_fota.o 
OBJ_KT_FOTA += ext/protocol/kt_fota/kt_fota_config.o 
OBJ_KT_FOTA += ext/protocol/kt_fota/kt_fota_svc/kt_dmc_proc.o 
OBJ_KT_FOTA += ext/protocol/kt_fota/kt_fota_svc/kt_fs_parser.o 
OBJ_KT_FOTA += ext/protocol/kt_fota/kt_fota_svc/kt_fs_send.o 
OBJ_KT_FOTA += ext/protocol/kt_fota/kt_fota_svc/kt_fs_udf.o

ifeq ($(KT_FOTA_TEST_SVR),1)
KT_FOTA_CFLAGS += -DKT_FOTA_TEST_SVR
endif