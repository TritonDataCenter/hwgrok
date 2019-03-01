#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Copyright (c) 2019, Joyent, Inc.
#
SUBDIRS = i386 amd64

all: TARGET = all
clean: TARGET = clean
clobber: TARGET = clobber

.KEEP_STATE:

all clean clobber: $(SUBDIRS)

$(SUBDIRS): FRC
	@cd $@; pwd; $(MAKE) $(TARGET)

FRC:
