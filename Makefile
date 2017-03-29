##############################################################################
# Path

SRCROOT := $(PWD)
DESTDIR := $(SRCROOT)/out
BASE_DIR := /system/mds/system
BINDIR	:= $(BASE_DIR)/bin
USER_DATA_DIR := /data/mds/data

CONFIG_FILE	:= config.ini
CONFIG_USER_FILE	:= user.ini
DM_FILE	:= dm.ini

CONFIG_USR_MON_INI := mon.ini

PATHRUN_BIN := $(DESTDIR)/system/sbin/pathrun3
RSSH_BIN := $(DESTDIR)/system/sbin/rssh3
DLPKG_BIN := $(DESTDIR)/system/sbin/dlpkg3
EMER_BIN := $(DESTDIR)/system/sbin/emer3
MONCHK_BIN := $(DESTDIR)/system/sbin/monchk3
CHKPROG_BIN := $(DESTDIR)/system/sbin/chkprog3
WDPROG_BIN := $(DESTDIR)/system/sbin/wdprog3
###############################################################################
# Compile

CC := $(CROSS_COMPILE)gcc
AR := $(CROSS_COMPILE)ar
RANLIB := $(CROSS_COMPILE)ranlib

CFLAGS	:= $(EXTRA_CFLAGS)
LDFLAGS	:= $(EXTRA_LDFLAGS)

###############################################################################
# Options

###############################################################################
# common src package
###############################################################################
COMMON_SRC_PATH = ../common-src3
COMMON_SRC_PKG = logd mon
###############################################################################
# Board

#CFLAGS	+= -DBOARD_$(BOARD) -DDEBUG
CFLAGS	+= -DBOARD_$(BOARD) 


###############################################################################
# PACKAGE

# ********** GPS FEATURE *************
USE_GPS_MODEL=y
# ********** BUTTON THREAD FEATURE *************
USE_BUTTON_THREAD=y
# ************************************

## MODEM CFG #####################
ifeq ($(BOARD),TX501S)
MODEM := TX501S
USE_GPS_MODEL=n
USE_BUTTON_THREAD=n
else ifeq ($(BOARD),TL500S)
MODEM := TL500S
USE_GPS_MODEL=y
USE_BUTTON_THREAD=y
else
$(error BOARD is not correct, please define correct BOARD)
endif
ifeq ($(MODEM),)
$$(error MODEM is not found, please define MODEM)
endif

## SVR CFG #####################
ifeq ($(SERVER),skel)
SERVER_ABBR := SKEL
else ifeq ($(SERVER),app1)
SERVER_ABBR := APP1
else ifeq ($(SERVER),asn)
SERVER_ABBR := ASN
else ifeq ($(SERVER),mds)
SERVER_ABBR := MDS
else ifeq ($(SERVER),cl-thermal)
SERVER_ABBR := CLT
else ifeq ($(SERVER),etrace)
SERVER_ABBR	:=	ETR
else ifeq ($(SERVER),bizincar)
SERVER_ABBR := BIC
else ifeq ($(SERVER),dtg-skel)
SERVER_ABBR := DSKL
else
$(error SERVER is not registerd in Makefile, please input registred server)
endif
ifeq ($(SERVER),)
$(error SERVER is not found, please define SERVER)
endif

## CORP CFG #####################
ifeq ($(CORP),mds)
CORP_ABBR := MDS
else ifeq ($(CORP),asn)
CORP_ABBR := ASN
else ifeq ($(CORP),cl)
CORP_ABBR := CL
else ifeq ($(CORP),corp)
CORP_ABBR := CORP
else ifeq ($(CORP),etrace)
CORP_ABBR     :=      ETR
else ifeq ($(CORP),bizincar)
CORP_ABBR := BIC
else
$(error CORP is not registerd in Makefile, please input registred corporation)
endif

ifeq ($(SUB),)
else ifeq ($(SUB),hnr0)
SERVER_ABBR   :=      HNR0
else ifeq ($(SUB),hnr3)
SERVER_ABBR   :=      HNR3
else ifeq ($(SUB),hnr4)
SERVER_ABBR   :=      HNR4
else ifeq ($(SUB),hnrf)
SERVER_ABBR   :=      HNRF
else ifeq ($(SUB),etrg)
SERVER_ABBR   :=      ETRG
else ifeq ($(SUB),etrf)
SERVER_ABBR   :=      ETRF
else ifeq ($(SUB),neognp)
SERVER_ABBR   :=      NEO
else
$(error SUB is not registerd in Makefile, please input registred sub-model)
endif

ifeq ($(CORP),)
$(error CORP is not found, please define CORP)
endif

## VERSION CFG #####################
define checkver
ifeq ($$(VER),0)
$$(error VER is not found, please define VER)
endif
endef

DEBUG_MODE  ?= 0
VER   ?= 0

WORK_PATH    := /system/$(VER)
SOCK_PATH    := /var/log
MODEL_PATH   := model/$(SERVER)
PACKAGE_FILE = $(BINDIR)/PACKAGE

###############################################################################
# Target rules

DATE        := `date +%F`
COMMIT_NUM  = `cat .git/refs/heads/master`
REPO_NAME	= `git rev-parse --abbrev-ref HEAD`
REPO_REV	= `git log --pretty=format:'' | wc -l`

CFLAGS	+= -Wall -g -rdynamic
CFLAGS	+= -I./ -Imodel/$(SERVER) -I../common-src3/
CFLAGS	+= -D_REENTRANT -DCONFIG_FILE_PATH=\"$(BINDIR)/$(CONFIG_FILE)\" -DDM_FILE_PATH=\"$(BINDIR)/$(DM_FILE)\"
CFLAGS	+= -DCONFIG_USER_FILE_PATH=\"$(USER_DATA_DIR)/$(CONFIG_USER_FILE)\" -DCONFIG_USER_ORG_FILE_PATH=\"$(BINDIR)/$(CONFIG_USER_FILE)\" 
CFLAGS	+= -DDATE=\"$(DATE)\" -DCOMMIT_NUM=\"$(COMMIT_NUM)\" -DREPO_NAME=\"$(REPO_NAME)\" -DREPO_REV=\"$(REPO_REV)\"
CFLAGS	+= -DPRG_VER=\"$(VER)\" -DPACKAGE_NAME=\"$(CORP_ABBR)-MDT.$(SERVER_ABBR)-$(MODEM)-$(VER)\"
CFLAGS	+= -DPACKAGE_FILE=\"$(PACKAGE_FILE)\"
CFLAGS	+= -DBOARD_$(BOARD)
CFLAGS	+= -DCORP_ABBR_$(CORP_ABBR) -DSERVER_ABBR_$(SERVER_ABBR)
CFLAGS  += -DUSE_NET_THREAD2
LIBS	= -lpthread -liniparser -ljansson -lm -lrt -ldm -lcurl -lz -lat3  -llogd -g -rdynamic -lmdsapi


CFLAGS  += -DMDS_FEATURE_USE_NMEA_UDP_IPC 
# net-kti model don't use USE_NET_THREAD2 
#ifeq ($(SERVER_ABBR), NKTI)
#CFLAGS  :=$(filter-out -DUSE_NET_THREAD2 ,$(CFLAGS))
#endif

-include model/${SERVER}/build.mk

OBJS	:= base/config.o base/main.o base/sender.o 
OBJS	+= base/thread-network.o base/devel.o base/error.o base/thread.o base/dmmgr.o base/watchdog.o
OBJS	+= base/thermtool.o
OBJS	+= board/gpio.o board/power.o board/led.o board/battery.o board/uart.o board/modem-time.o board/app-ver.o board/crit-data.o
OBJS	+= board/rfidtool.o board/thermometer.o
OBJS	+= util/stackdump.o util/list.o util/debug.o util/poweroff.o util/pipe.o
OBJS	+= util/nettool.o util/tools.o util/transfer.o util/validation.o util/crc16.o util/storage.o


OBJS	+= $(OBJS_MODEL)

ifeq ($(USE_GPS_MODEL),y)
CFLAGS  += -DUSE_GPS_MODEL
OBJS	+= base/gpstool.o base/mileage.o base/thread-gps.o
endif

ifeq ($(USE_BUTTON_THREAD),y)
CFLAGS  += -DUSE_BUTTON_THREAD
OBJS	+= base/thread-btn-pwr.o
endif

ifndef BOARD
$(error BOARD is not correct, please define correct BOARD)
endif

export CROSS_COMPILE CC AR RANLIB BOARD CFLAGS LDFLAGS DESTDIR WORK_PATH SOCK_PATH MODEL_PATH

APP	:= gtrack3

# webdm pathrun rssh dmlib dlpkg 
all: check common-src $(APP)
#subdirs
check:
	$(Q)$(call check_install_dir, $(DESTDIR)$(WORK_PATH))
	$(eval $(call checkver))
	
common-src :
	@echo [Compile...]-------------------------------------------
	for dir in $(COMMON_SRC_PKG) ; do \
		make -C $(COMMON_SRC_PATH)/$$dir || exit $?; \
		make -C $(COMMON_SRC_PATH)/$$dir install || exit $?; \
	done

dmlib :
	make -C $(PWD)/../dmlib	|| exit $?; \
	make -C $(PWD)/../dmlib install || exit $?;	
	
# webdm compile

pathrun :
	make -C $(PWD)/../pathrun	|| exit $?; \
	make -C $(PWD)/../pathrun install || exit $?; \

rssh :
	make -C $(PWD)/../rssh	|| exit $?; \
	make -C $(PWD)/../rssh install || exit $?; \

dlpkg :
	make -C $(PWD)/../dlpkg3	|| exit $?; \
	make -C $(PWD)/../dlpkg3 install || exit $?; \

$(APP):		$(OBJS)
	$(Q)$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

install-binary:	$(APP)
	$(Q)$(call check_install_dir, $(DESTDIR)$(WORK_PATH))
	$(Q)fakeroot cp -v $(APP) $(DESTDIR)$(WORK_PATH)

install-script:	$(CONFIG_FILE) $(CONFIG_USER_FILE) base/$(DM_FILE) $(CONFIG_USR_MON_INI)
	$(Q)$(call check_install_dir, $(DESTDIR)$(WORK_PATH))
	$(Q)fakeroot cp -v $(CONFIG_FILE) $(DESTDIR)$(WORK_PATH)
	$(Q)fakeroot cp -v $(CONFIG_USER_FILE) $(DESTDIR)$(WORK_PATH)
	$(Q)fakeroot cp -v $(CONFIG_USR_MON_INI) $(DESTDIR)$(WORK_PATH)
	$(Q)fakeroot cp -v base/$(DM_FILE) $(DESTDIR)$(WORK_PATH)


ifneq ($(SUB),)
	@if [ -d "$(MODEL_PATH)/sub/$(SUB)" ]; then \
	@echo Copy Submodule $(SUB) of $(SERVER).; \
	cp -f `find $(MODEL_PATH)/sub/$(SUB) -maxdepth 1 -type f` $(DESTDIR)$(WORK_PATH); \
	cp -f `find $(MODEL_PATH)/sub/$(SUB) -maxdepth 1 -type f` $(SRCROOT); \
	fi
endif

# pathrun copy to "bin" dir	
install-pathrun : $(PATHRUN_BIN)
	$(Q)fakeroot cp -v $(PATHRUN_BIN) $(DESTDIR)$(WORK_PATH)

# rssh copy to "bin" dir	
install-rssh : $(RSSH_BIN)
	$(Q)fakeroot cp -v $(RSSH_BIN) $(DESTDIR)$(WORK_PATH)

install-dlpkg : $(DLPKG_BIN)
	$(Q)fakeroot cp -v $(DLPKG_BIN) $(DESTDIR)$(WORK_PATH)

install-emerkg : $(EMER_BIN)
	$(Q)fakeroot cp -v $(EMER_BIN) $(DESTDIR)$(WORK_PATH)

install-monchkg : $(MONCHK_BIN)
	$(Q)fakeroot cp -v $(MONCHK_BIN) $(DESTDIR)$(WORK_PATH)

install-progchkg : $(CHKPROG_BIN)
	$(Q)fakeroot cp -v $(CHKPROG_BIN) $(DESTDIR)$(WORK_PATH)

install-wdprogkg : $(WDPROG_BIN)
	$(Q)fakeroot cp -v $(WDPROG_BIN) $(DESTDIR)$(WORK_PATH)
	

install: check install-binary install-script install-pathrun install-rssh install-dlpkg install-emerkg install-monchkg install-progchkg install-wdprogkg $(WDPROG_BIN)
		@echo CORP=$(CORP)
		@echo DEVICE=$(DEVICE)
		@echo SERVER=$(SERVER_ABBR)
		@echo MODEM=$(MODEM)
		@echo VERSION=$(VER)
		@echo SUB=$(SUB)
		@echo DEBUG=$(DEBUG_MODE)
		@echo Press any key to continue
		@read input
		@echo [Install...]-------------------------------------------
		@if [ ! -d $(DESTDIR)$(WORK_PATH) ]; then mkdir -p $(DESTDIR)$(WORK_PATH); fi
		@for dir in $(SUBDIRS) ; do \
			make -C $$dir install || exit $?; \
		done

		@echo ""
		@rm -f $(DESTDIR)/system/bin
		@ln -sf $(BASE_DIR)/$(VER) $(DESTDIR)/system/bin
		@echo -e '\033[1;36m'
		@echo [Filesystem]-------------------------------------------
		@tree $(DESTDIR)/system
		@echo ""
		@echo [Repository]-------------------------------------------
		@echo "Current Branch Name : $(REPO_NAME)"
		@echo "Current Commit Rev  : $(REPO_REV)"
		@echo ""
		@echo [Package]----------------------------------------------
		@echo "PACKAGE FILE: $(PACKAGE_FILE)"
		@echo "UP_VER: $(CORP_ABBR)-MDT.$(SERVER_ABBR)-$(MODEM)-$(VER)"    >  $(DESTDIR)$(WORK_PATH)/PACKAGE
		@echo "Install date: $(DATE)"                       >> $(DESTDIR)$(WORK_PATH)/PACKAGE
		@echo "Commit: $(COMMIT_NUM)"                       >> $(DESTDIR)$(WORK_PATH)/PACKAGE
		@echo ""
		@cat $(DESTDIR)$(WORK_PATH)/PACKAGE
		@echo -e '\033[0m'

clean:
	@for dir in $(SUBDIRS) ; do \
		make -C $$dir clean ; \
	done
	$(Q)rm -vrf $(APP) $(OBJS) \
		$(CONFIG_FILE) $(CONFIG_USER_FILE)
	$(Q)rm -vrf $(CONFIG_FILE_KT_FOTA)

uninstall:
	@echo [Uninstall.....]-------------------------------------------
	rm -vrf $(DESTDIR)/system/*.??
	rm -vrf $(DESTDIR)/system/bin

###############################################################################
# Extra rules

$(CONFIG_FILE):		$(MODEL_PATH)/$(CONFIG_FILE).in
	$(Q)echo Create $@ ...
	$(Q)cp -v $^ $@
	$(Q)#sed "s,@app.path@,$(BINDIR)/$(APP),g" $^ > $@
	$(Q)#chmod 755 $@
	
$(CONFIG_USER_FILE):		$(MODEL_PATH)/$(CONFIG_USER_FILE).in
	$(Q)echo Create $@ ...
	$(Q)cp -v $^ $@
	$(Q)#sed "s,@app.path@,$(BINDIR)/$(APP),g" $^ > $@
	$(Q)#chmod 755 $@

$(CONFIG_USR_MON_INI):		$(MODEL_PATH)/$(CONFIG_USR_MON_INI).in
	$(Q)echo Create $@ ...
	$(Q)cp -v $^ $@
	$(Q)#chmod 755 $@



###############################################################################
# Functions

define check_install_dir
	if [ ! -d "$1" ]; then mkdir -p $1; fi
endef
