VSRC_DIR = $(NPC_HOME)/vsrc
CSRC_DIR = $(NPC_HOME)/csrc
BUILD_DIR = $(NPC_HOME)/build

VSRCS = $(wildcard $(VSRC_DIR)/*.v)
CSRCS = $(shell find $(CSRC_DIR) -name "*.cpp" -o -name "*.c" -o -name "*.cc" -o -name "*.cxx")

GUEST_ISA ?= riscv32

TOP_MODULE ?= top

TIME_UNIT_ns ?= 1
TIME_PRECISION_ps ?= 10

TARGET = $(BUILD_DIR)/V$(TOP_MODULE)

LIBCAPSTONE = $(NEMU_HOME)/tools/capstone/repo/libcapstone.so.5


INCFLAGS = -I$(CSRC_DIR)/include -I$(CSRC_DIR)/src/isa/$(GUEST_ISA)/include
INCFLAGS += -I$(NEMU_HOME)/tools/capstone/repo/include

CXXFLAGS = -Wall -O2 -mcmodel=medium
CXXFLAGS += $(shell pkg-config --cflags verilator)
CXXFLAGS += $(INCFLAGS)
CXXFLAGS += -DTOP_MODULE=V$(TOP_MODULE)
CXXFLAGS += -DTOP_MODULE_HEADER=V$(TOP_MODULE).h
CXXFLAGS += -D__GUEST_ISA__=$(GUEST_ISA) -DTIME_PRECISION_ps=$(TIME_PRECISION_ps) -DCAPSTONE_SO_PATH=$(LIBCAPSTONE)

LDFLAGS = -lreadline -ldl

VERILATOR = verilator
VERILATOR_FLAGS = --cc --exe --build 
VERILATOR_FLAGS += -Wall -Wno-DECLFILENAME
VERILATOR_FLAGS += --top-module $(TOP_MODULE)
VERILATOR_FLAGS += -Mdir $(BUILD_DIR)
VERILATOR_FLAGS += -I$(VSRC_DIR)
VERILATOR_FLAGS += --timescale-override $(TIME_UNIT_ns)ns/$(TIME_PRECISION_ps)ps
#VERILATOR_FLAGS += -CFLAGS "$(CXXFLAGS)"

LINT_FLAGS = --lint-only -Wall
LINT_FLAGS += --top-module $(TOP_MODULE)
LINT_FLAGS += -I$(VSRC_DIR)
#LINT_FLAGS += -CFLAGS "$(CXXFLAGS)"






all: $(TARGET)

$(TARGET): $(VSRCS) $(CSRCS) $(SO)
	@echo "Building Verilator simulation..."
	@mkdir -p $(BUILD_DIR)
	$(VERILATOR) $(VERILATOR_FLAGS) $(VSRCS) $(CSRCS) $(addprefix -CFLAGS , $(CXXFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS))

$(SO): $(LIBCAPSTONE)

$(LIBCAPSTONE):
	$(MAKE) -C $(NEMU_HOME)/tools/capstone

sim: $(TARGET)
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Running simulation..."
	@echo "Args : $(ARGS)"
	$(TARGET) $(ARGS) $(IMG)

sim-trace: VERILATOR_FLAGS += --trace 
sim-trace: CXXFLAGS += -DWAVE
sim-trace: clean $(TARGET)
	@echo "Running simulation with trace..."
	$(TARGET) $(ARGS) $(IMG)

sim-trace-fst: VERILATOR_FLAGS += --trace-fst 
sim-trace-fst: CXXFLAGS += -DWAVE_FST
sim-trace-fst: clean $(TARGET)
	@echo "Running simulation with trace..."
	$(TARGET) $(ARGS) $(IMG)

debug: CXXFLAGS += -g
debug: clean $(TARGET)
	@echo "Running simulation with debug info..."
	gdb  --args $(TARGET) $(ARGS) $(IMG)

lint: $(VSRCS)
	@echo "Running Verilator lint check..."
	$(VERILATOR) $(LINT_FLAGS) $(VSRCS)

clean:
	rm -rf $(BUILD_DIR)

include ../Makefile


.PHONY: all sim sim-trace clean lint