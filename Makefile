# To make all of the targets, just run
#
#    make
#
# To make the single target foo, run
#
#    make foo
#
# 'make clean' will remove _all_ targets and any lingering .o files.
# You've been warned.
#

# Targets to be made.
TARGETS = dance follow lasers

CFLAGS=-fPIC -g -Wall
ARIA_INCLUDE=-I/usr/local/Aria/include
ARIA_LINK=-L/usr/local/Aria/lib -lAria -lpthread -ldl -lrt

all:
	make $(TARGETS)

# The "-" preceding the command line below will cause make to keep processing
# targets if the compilation of a target fails.  If this is a problem, then
# remove the "-".

%: %.cpp $@
	-$(CXX) $(CFLAGS) $(ARIA_INCLUDE) $< -o $@ $(ARIA_LINK)

clean:
	rm -f $(TARGETS) *.o
