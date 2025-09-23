VSRC_DIR = $(NPC_HOME)/vsrc
CSRC_DIR = $(NPC_HOME)/csrc
BUILD_DIR = $(NPC_HOME)/build

VSRCS = $(wildcard $(VSRC_DIR)/*.v)
CSRCS = $(shell find $(CSRC_DIR) -name "*.cpp" -o -name "*.c" -o -name "*.cc" -o -name "*.cxx")


TOP_MODULE ?= top

INCFLAGS = -I$(CSRC_DIR)/include 

CXXFLAGS = -Wall -O2 -g -mcmodel=medium
CXXFLAGS += $(shell pkg-config --cflags verilator)
CXXFLAGS += $(INCFLAGS)
CXXFLAGS += -DTOP_MODULE=V$(TOP_MODULE)
CXXFLAGS += -DTOP_MODULE_HEADER=V$(TOP_MODULE).h
CXXFLAGS += -DCONFIG_DEVICE=1 -DCONFIG_HAS_SERIAL=1 -DCONFIG_HAS_TIMER=1

LD_FLAGS =

VERILATOR = verilator
VERILATOR_FLAGS = --cc --exe --build 
VERILATOR_FLAGS += -Wall -Wno-DECLFILENAME
VERILATOR_FLAGS += --top-module $(TOP_MODULE)
VERILATOR_FLAGS += -Mdir $(BUILD_DIR)
VERILATOR_FLAGS += -I$(VSRC_DIR)
#VERILATOR_FLAGS += -CFLAGS "$(CXXFLAGS)"

LINT_FLAGS = --lint-only -Wall
LINT_FLAGS += --top-module $(TOP_MODULE)
LINT_FLAGS += -I$(VSRC_DIR)
#LINT_FLAGS += -CFLAGS "$(CXXFLAGS)"


TARGET = $(BUILD_DIR)/V$(TOP_MODULE)

all: $(TARGET)

$(TARGET): $(VSRCS) $(CSRCS)
	@echo "Building Verilator simulation..."
	@mkdir -p $(BUILD_DIR)
	$(VERILATOR) $(VERILATOR_FLAGS) $(VSRCS) $(CSRCS) $(addprefix -CFLAGS , $(CXXFLAGS))

sim: $(TARGET)
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Running simulation..."
	$(TARGET) $(IMG)

sim-trace: VERILATOR_FLAGS += --trace
sim-trace: clean $(TARGET)
	@echo "Running simulation with trace..."
	$(TARGET) $(IMG)

lint: $(VSRCS)
	@echo "Running Verilator lint check..."
	$(VERILATOR) $(LINT_FLAGS) $(VSRCS)

clean:
	rm -rf $(BUILD_DIR)

include ../Makefile


.PHONY: all sim sim-trace clean lint