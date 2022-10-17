
CC=gcc

EXE = exe
IMGUI_DIR = ../
SOURCES = main.cpp
SOURCES += $(IMGUI_DIR)/imgui/imgui.cpp $(IMGUI_DIR)/imgui/imgui_draw.cpp $(IMGUI_DIR)/imgui/imgui_tables.cpp $(IMGUI_DIR)/imgui/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

CXXFLAGS = -std=c++11 -I$(IMGUI_DIR)/imgui -I$(IMGUI_DIR)/backends -I$(IMGUI_DIR)/inc
CXXFLAGS += -g -Wall -Wformat
LIBS =

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
endif

NET_SNMP_CONFIG=net-snmp-config
CFLAGS=`$(NET_SNMP_CONFIG) --cflags` -Wall -Wextra -Werror \
	-Wno-unused-parameter
BUILDLIBS=`$(NET_SNMP_CONFIG) --libs`
BUILDAGENTLIBS=`$(NET_SNMP_CONFIG) --agent-libs`

DLFLAGS=-fPIC -shared

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/imgui/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/inc/%.c
	$(CC) -o $@ $@.o $(BUILDLIBS)

%.o:$(IMGUI_DIR)/inc/%.c
	$(CC) $(CFLAGS) $(DLFLAGS) -c -o $@ nstAgentPluginObject.c

%.o:$(IMGUI_DIR)/inc/%.c
	$(CC) $(CFLAGS) $(DLFLAGS) -o $@ nstAgentPluginObject.o

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ snmpGet.o `net-snmp-config --libs` $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
