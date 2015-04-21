# Hey Emacs, this is a -*- makefile -*-
#
# ardrone2_sdk.makefile
#
# http://wiki.paparazziuav.org/wiki/AR.Drone_2_-_Specifications
#

BOARD=ardrone
BOARD_VERSION=2
BOARD_TYPE=sdk
BOARD_CFG=\"boards/$(BOARD)$(BOARD_VERSION)_$(BOARD_TYPE).h\"

ARCH=linux
$(TARGET).ARCHDIR = $(ARCH)
# include conf/Makefile.parrot (with specific upload rules) instead of only Makefile.linux:
ap.MAKEFILE = parrot

# -----------------------------------------------------------------------
USER=foobar
HOST=192.168.1.1
SUB_DIR=sdk
FTP_DIR=/data/video
TARGET_DIR=$(FTP_DIR)/$(SUB_DIR)
# -----------------------------------------------------------------------
ARDRONE2_START_PAPARAZZI ?= 0
ARDRONE2_WIFI_MODE ?= 0
ARDRONE2_SSID ?= ardrone2_paparazzi
ARDRONE2_IP_ADDRESS_BASE ?= 192.168.1.
ARDRONE2_IP_ADDRESS_PROBE ?= 1
# -----------------------------------------------------------------------

# The GPS sensor is connected trough USB so we have to define the device
GPS_PORT         ?= UART1
GPS_BAUD         ?= B57600

# The datalink default uses UDP
MODEM_HOST       ?= 192.168.1.255

# for distinction between SDK and RAW version
ap.CFLAGS +=-DARDRONE2_SDK

# -----------------------------------------------------------------------

# default LED configuration
RADIO_CONTROL_LED			?= none
BARO_LED           			?= none
AHRS_ALIGNER_LED   			?= none
GPS_LED            			?= none
SYS_TIME_LED       			?= none
