#pragma once

#define CONCAT_STR(X, Y)				X Y
#define MAKE_CMD_STR(X, Y)				X " " Y
#define SYSTEM_DIR						"/system/mds/system"
#define USER_DATA_DIR					"/data/mds/data"
#define GPIO_ROOT_DIR					"/sys/class/gpio"
#define NETTOOL_NETIF					"rmnet_data0"
#define EVENT0_DEV_NAME					"/dev/input/event1"
// #define EVENT1_DEV_NAME					"/dev/input/event1" // external gpio input dev is not support yet.

#define UART0_DEV_NAME					"/dev/ttyHSL2"
#define UART1_DEV_NAME					"/dev/ttyHSL2"


#define GPIO_VALUE						CONCAT_STR(GPIO_ROOT_DIR, "/gpio%d/value")
#define GPIO_DIRECTION					CONCAT_STR(GPIO_ROOT_DIR, "/gpio%d/direction")


#define GPIO_NO_MIN						9
#define GPIO_NO_MAX						66

#define GPIO_SRC_NUM_POWER				6
#define POWER_SRC_DC				(1)
#define POWER_SRC_BATTERY			(0)

#define GPIO_SRC_NUM_IGNITION			30
#define POWER_IGNITION_ON		(0)
#define POWER_IGNITION_OFF		(1)

#define ALIVE_NOTIFIER_DEV_NUM			255
#define ALIVE_NOTIFIER_DEV_NAME			"/dev/alive"
#define MDS_DEVICE_UTIL_MAJOR_NUMBER	255
#define ALIVE_END_CMD					CONCAT_STR(SYSTEM_DIR, "/sbin/alive2.notifier end &")

#define LOG_PWR_PATH					CONCAT_STR(SYSTEM_DIR, "/log/power.log")
#define LOG_ERR_PATH					CONCAT_STR(SYSTEM_DIR, "/log/error.log")
#define LOG_CRITICAL_PATH				CONCAT_STR(SYSTEM_DIR, "/log/critical.log")
#define DAT_ERR_PATH					CONCAT_STR(SYSTEM_DIR, "/log/error.dat")
#define FLG_ERR_PATH					"/tmp/.nomon"
#define CALLSTACK_LOG_PATH				CONCAT_STR(SYSTEM_DIR, "/log/abort.log")

#define MILEAGE_PATH					CONCAT_STR(USER_DATA_DIR, "/mileage.dat")
#define MKDIR_USER_DIR					MAKE_CMD_STR("mkdir -p", USER_DATA_DIR)
#ifdef USE_EXTGPIO_EVT
	#define INPUT_KEY_MODULE_NAME		CONCAT_STR(SYSTEM_DIR, "/bin/gpio_input_keys.ko")
	#define INSMODE_CMD					"/sbin/insmod"
#endif


#define AT_LIB_TARGET_DEV   e_DEV_TX501_BASE

#define LED_NAME_PWR	"power"
#define LED_NAME_WCDMA	"wcdma"
#define LED_NAME_GPS	"gps"

#define LED_COLOR_GREEN		"green"
#define LED_COLOR_RED		"red"
#define LED_COLOR_YELLO		"yellow"
#define LED_COLOR_BLUE		"blue"
#define LED_COLOR_MAGENTA	"magenta"
#define LED_COLOR_CYAN		"cyan"
#define LED_COLOR_WHITE		"white"
