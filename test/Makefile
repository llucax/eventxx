
CXXFLAGS=-I.. -g -Wall
# Uncomment this if you have a fixed event_base_free().
# See http://www.mail-archive.com/libevent-users@monkey.org/msg00112.html
#CXXFLAGS+=-DEVENT_BASE_FREE_FIX
LDFLAGS=-levent

targets=bench test-eof test-time test-weof trivial c-way functor-way \
	prio-test mixed-way

all: $(targets)

clean:
	$(RM) -fv *.o $(targets)

.PHONY: clean all

