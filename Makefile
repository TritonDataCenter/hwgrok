#
# Copyright (c) 2018, Joyent, Inc.
#
PROG=		hwgrok
CC=		/opt/local/bin/cc
CFLAGS=		-g -std=gnu99
LDFLAGS=	-lnvpair -L/usr/lib/fm -ltopo -R/usr/lib/fm
CTFCONVERT=	/opt/onbld/bin/i386/ctfconvert
CTFMERGE=	/opt/onbld/bin/i386/ctfmerge

SRCS= hwgrok.c llist.c dump_json.c

OBJS = $(SRCS:%.c=%.o)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
	$(CTFCONVERT) -l 0 $@

$(PROG): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)
	$(CTFMERGE) -l 0 -o $@ $(OBJS)

all: $(PROG)

clean clobber:
	$(RM) $(PROG) $(OBJS)
