#
# Warning: you may need more libraries than are included here on the
# build line.  The agent frequently needs various libraries in order
# to compile pieces of it, but is OS dependent and we can't list all
# the combinations here.  Instead, look at the libraries that were
# used when linking the snmpd master agent and copy those to this
# file.
#

CC=gcc

TARGETS=example-demon main asyncapp

NET_SNMP_CONFIG=net-snmp-config
CFLAGS=`$(NET_SNMP_CONFIG) --cflags` -Wall -Wextra -Werror \
	-Wno-unused-parameter
BUILDLIBS=`$(NET_SNMP_CONFIG) --libs`
BUILDAGENTLIBS=`$(NET_SNMP_CONFIG) --agent-libs`

# shared library flags (assumes gcc)
DLFLAGS=-fPIC -shared

all: $(TARGETS)

main: main.o
	$(CC) -o $@ $@.o $(BUILDLIBS)

asyncapp: asyncapp.o
	$(CC) -o $@ $@.o $(BUILDLIBS)

example-demon: example-demon.o nstAgentSubagentObject.o
	$(CC) -o $@ $@.o nstAgentSubagentObject.o $(BUILDAGENTLIBS)

clean:
	rm -f -- *.o $(TARGETS)

nstAgentPluginObject.o: nstAgentPluginObject.c Makefile
	$(CC) $(CFLAGS) $(DLFLAGS) -c -o $@ nstAgentPluginObject.c

nstAgentPluginObject.so: nstAgentPluginObject.o Makefile
	$(CC) $(CFLAGS) $(DLFLAGS) -o $@ nstAgentPluginObject.o
