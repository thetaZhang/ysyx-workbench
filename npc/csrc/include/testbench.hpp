#ifndef TESTBENCH_HPP
#define TESTBENCH_HPP

#include <verilated.h>


#ifdef WAVE
#include <verilated_vcd_c.h>
#elif defined (WAVE_FST)
#include <verilated_fst_c.h>
#endif


#define RESET_TIME 10
#define CLOCK_PERIOD_ns 10 
#define CLOCK_STEP (10 * 1000)/(2 * TIME_PRECISION_ps)


#if defined(WAVE) || defined(WAVE_FST)
  #define TB_TRACE_DUMP() do { tracePtr->dump(contextPtr->time()); } while (0)
#else
  #define TB_TRACE_DUMP() ((void)0)
#endif

#define POSEDGE(...) \
  do {   \
    contextPtr->timeInc(CLOCK_STEP); \
    __VA_ARGS__ \
    modulePtr->clk = 1; \
    modulePtr->eval(); \
    TB_TRACE_DUMP(); \
  } while (0)

#define NEGEDGE(...) \
  do {   \
    contextPtr->timeInc(CLOCK_STEP); \
    __VA_ARGS__ \
    modulePtr->clk = 0; \
    modulePtr->eval(); \
    TB_TRACE_DUMP(); \
  } while (0)

#define HALF_PERIOD(...) \
  do {   \
    contextPtr->timeInc(CLOCK_STEP); \
    __VA_ARGS__ \
    modulePtr->eval(); \
    TB_TRACE_DUMP(); \
  } while (0)

template <class MODULE>
class TestBench {
  private:
    VerilatedContext* contextPtr;
#ifdef WAVE
    VerilatedVcdC* tracePtr;
#elif defined (WAVE_FST)
    VerilatedFstC* tracePtr;
#endif
  
  public:
    MODULE* modulePtr;
    TestBench(int argc, char** argv);
    ~TestBench(){
        delete modulePtr;
        delete contextPtr;
#if defined (WAVE) || defined (WAVE_FST)
        tracePtr->close();
        delete tracePtr;
#endif
    };
    
    void reset();

   

    template <class F>
    void posedge(F&& body);

    void posedge();

    template <class F>
    void negedge(F&& body);

    void negedge();

    template <class F>
    void tick(F&& body);

    void tick();
    
};

template <class MODULE>
TestBench<MODULE>::TestBench(int argc, char** argv) {
    contextPtr = new VerilatedContext;
    contextPtr->commandArgs(argc, argv);
    modulePtr = new MODULE{contextPtr};
#ifdef WAVE
    Verilated::traceEverOn(true);
    tracePtr = new VerilatedVcdC;
    modulePtr->trace(tracePtr, 0);
    tracePtr->open("build/waveform.vcd");
#elif defined (WAVE_FST)
    Verilated::traceEverOn(true);
    tracePtr = new VerilatedFstC;
    modulePtr->trace(tracePtr, 0);
    tracePtr->open("build/waveform.fst");
#endif
}

template <class MODULE>
template <class F>
void TestBench<MODULE>::posedge(F&& body) {
  POSEDGE(std::forward<F>(body)(*modulePtr););
}

template <class MODULE>
void TestBench<MODULE>::posedge() {
  POSEDGE();
}

template <class MODULE>
template <class F>
void TestBench<MODULE>::negedge(F&& body) {
  NEGEDGE(std::forward<F>(body)(*modulePtr););
}

template <class MODULE>
void TestBench<MODULE>::negedge() {
  NEGEDGE();
}

template <class MODULE>
template <class F>
void TestBench<MODULE>::tick(F&& body) {
  auto&& f = std::forward<F>(body);
  posedge(f);
  negedge();
}

template <class MODULE>
void TestBench<MODULE>::tick() {
  posedge();
  negedge();
}

template <class MODULE>
void TestBench<MODULE>::reset() {
  // init state
  modulePtr->rst_n = 1;
  modulePtr->clk = 0;
  modulePtr->eval();
  TB_TRACE_DUMP();

  HALF_PERIOD(modulePtr->rst_n = 0;);
  HALF_PERIOD(modulePtr->rst_n = 1;);


// reset cirkuit
  // posedge([](auto& dut){ dut.rst_n = 1; });
  // negedge();
}


#endif // TESTBENCH_HPP
