COMMON_INCLUDE_DIRS += $(rootdir)/$(MODULE)/include             \
                       $(incdir)/libonplatform                    \
                       $(incdir)/libonlog                         \
                       $(incdir)/libonevent

COMMON_SRC_FILES := $(rootdir)/$(MODULE)/src/udp_socket.c       \
                    $(rootdir)/$(MODULE)/src/tcp_socket.c       \
                    $(rootdir)/$(MODULE)/src/tcp_connection.c   \
                    $(rootdir)/$(MODULE)/src/base_socket.c

COMMON_INST_HEADER_DIRS += $(rootdir)/$(MODULE)/include
