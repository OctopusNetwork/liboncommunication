rootdir := $(shell pwd)/../../..
project := ubuntu-base

include $(rootdir)/build/common/inc.mk
ifeq ($(hostmaketype), windows)
rootdir := ../../..
endif

TARGETSPEC = liboncommunication
TARGET_LIB = liboncommunication.$(staticlibsuffix)
MODULE = liboncommunication
MODULEVERSION = 0.0.1

include ../source.mk
include $(rootdir)/build/utils/archive.$(hostmaketype).mk
