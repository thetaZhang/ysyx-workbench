VSRC_DIR = vsrc
CSRC_DIR = csrc
BUILD_DIR = build

VSRCS = $(wildcard $(VSRC_DIR)/*.v)
CSRCS = $(wildcard $(CSRC_DIR)/*.cpp)

TOP_MODULE ?= top 

VERILATOR = verilator
VERILATOR_FLAGS = --cc --exe --build --trace-fst
VERILATOR_FLAGS += -Wall #-Wno-fatal
VERILATOR_FLAGS += --top-module $(TOP_MODULE)
VERILATOR_FLAGS += -Mdir $(BUILD_DIR)

LINT_FLAGS = --lint-only -Wall
LINT_FLAGS += --top-module $(TOP_MODULE)

CXX_FLAGS = -Wall -O2

TARGET = $(BUILD_DIR)/V$(TOP_MODULE)

all: $(TARGET)

$(TARGET): $(VSRCS) $(CSRCS)
	@echo "Building Verilator simulation..."
	@mkdir -p $(BUILD_DIR)
	$(VERILATOR) $(VERILATOR_FLAGS) $(VSRCS) $(CSRCS)

sim: $(TARGET)
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Running simulation..."
	$(TARGET)

lint: $(VSRCS)
	@echo "Running Verilator lint check..."
	$(VERILATOR) $(LINT_FLAGS) $(VSRCS)

clean:
	rm -rf $(BUILD_DIR)

include ../Makefile


.PHONY: all sim sim-trace clean lint