CFLAGS	+= -Wall -Wno-unused-result
CFLAGS	+= -O3
CFLAGS	+= -I.

SRCS	= readHex.cc crc16.cc uart.cc boot.cc main.cc
HDRS	= readHex.h  crc16.h  uart.h  boot.h

pic32upload: $(SRCS) $(HDRS)
	g++ -o pic32upload $(CFLAGS) $(SRCS)

clean::
	$(RM) $(OBJS)
