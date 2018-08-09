rootdir := $(shell pwd)/../..
project ?= ubuntu-base

include $(rootdir)/build/project/$(project).mk
include $(rootdir)/build/common/common.mk

TARGET_ARCHIVE = liboncommunication.a
MODULENAME = liboncommunication
MODULEVERSION = 0.1.0

COMMON_INCLUDE_DIRS += $(rootdir)/source/$(MODULENAME)/include              \
                       $(incdir)/libonplatform $(incdir)/libonevent
COMMON_SRC_FILES := $(rootdir)/source/$(MODULENAME)/src/base_socket.c       \
                    $(rootdir)/source/$(MODULENAME)/src/tcp_socket.c        \
                    $(rootdir)/source/$(MODULENAME)/src/tcp_connection.c    \
                    $(rootdir)/source/$(MODULENAME)/src/udp_socket.c
COMMON_INST_HEADER_DIRS += $(rootdir)/source/$(MODULENAME)/include

include $(rootdir)/build/utils/archive.mk

.PHONY : sync
