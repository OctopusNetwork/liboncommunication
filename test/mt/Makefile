rootdir := $(shell pwd)/../../..
project := ubuntu-base

include $(rootdir)/build/common/inc.mk
ifeq ($(hostmaketype), windows)
rootdir := ../../..
endif

####
##  Application targets
####
DEP_HEADERS +=  $(incdir)/libonplatform $(rootdir)/liboncommunication/include  \
                $(rootdir)/liboncommunication/src $(incdir)/libonevent

apps = mt_udp_server mt_udp_client mt_tcp_server mt_tcp_client mt_tcp_server_select \
       mt_tcp_conn_server mt_tcp_conn_client

mt_udp_server_SRCS = $(rootdir)/liboncommunication/test/mt/udp_server_main.c
mt_udp_server_ARCHIVES = libonplatform liboncommunication libonevent
mt_udp_server_LDLIBS =
mt_udp_server_DISTSOLIBS =

mt_udp_client_SRCS = $(rootdir)/liboncommunication/test/mt/udp_client_main.c
mt_udp_client_ARCHIVES = libonplatform liboncommunication libonevent
mt_udp_client_LDLIBS =
mt_udp_client_DISTSOLIBS =

mt_tcp_client_SRCS = $(rootdir)/liboncommunication/test/mt/tcp_client_main.c
mt_tcp_client_ARCHIVES = libonplatform liboncommunication libonevent
mt_tcp_client_LDLIBS =
mt_tcp_client_DISTSOLIBS =

mt_tcp_server_SRCS = $(rootdir)/liboncommunication/test/mt/tcp_server_main.c
mt_tcp_server_ARCHIVES = libonplatform liboncommunication libonevent
mt_tcp_server_LDLIBS =
mt_tcp_server_DISTSOLIBS =

mt_tcp_server_select_SRCS = $(rootdir)/liboncommunication/test/mt/tcp_server_select_main.c
mt_tcp_server_select_ARCHIVES = libonplatform liboncommunication libonevent
mt_tcp_server_select_LDLIBS =
mt_tcp_server_select_DISTSOLIBS =

mt_tcp_conn_server_SRCS = $(rootdir)/liboncommunication/test/mt/tcp_conn_server_main.c
mt_tcp_conn_server_ARCHIVES = libonplatform liboncommunication libonevent
mt_tcp_conn_server_LDLIBS =
mt_tcp_conn_server_DISTSOLIBS =

mt_tcp_conn_client_SRCS = $(rootdir)/liboncommunication/test/mt/tcp_conn_client_main.c
mt_tcp_conn_client_ARCHIVES = libonplatform liboncommunication libonevent
mt_tcp_conn_client_LDLIBS =
mt_tcp_conn_client_DISTSOLIBS =

include $(rootdir)/build/utils/app.mk
include $(rootdir)/build/utils/app.mk
