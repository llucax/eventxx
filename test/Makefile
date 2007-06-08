
CXXFLAGS=-I.. -g -Wall

# Uncomment this if you have a libevent version previous to 1.3b.
#CXXFLAGS+=-DEVENTXX_NO_EVENT_BASE_FREE

LDFLAGS=-levent

targets=bench test-eof test-time test-weof trivial c-way functor-way \
	wrapped-functor-way prio-test mixed-way

all: $(targets)

test: all
	@echo Running tests...
	@echo
	@for f in $(targets); do \
		echo -------------------------------------------------------; \
		echo runnig $$f; \
		echo -------------------------------------------------------; \
		./$$f; \
		echo; \
	done

clean:
	$(RM) -fv *.o $(targets)

.PHONY: clean all

