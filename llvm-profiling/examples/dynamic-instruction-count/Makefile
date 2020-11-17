PROJECT=cdi
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    DBG_FILES := ${PROJECT}.dSYM
endif
ifeq ($(UNAME_S),Linux)
    DBG_FILES :=
endif

COMMON_FLAGS=-Wall -Wextra #-g
SRC_DIR?=$(PWD)
SRC=CountDynamicInstructions.cpp

cdi:
	@echo Compiling $(SRC)
	clang++ $(SRC) $(COMMON_FLAGS) `llvm-config --cxxflags --ldflags --system-libs --libs irreader bitwriter passes core support` -O3 -o $(PROJECT)

fibo: cdi
	clang++ -O0 -S -emit-llvm benchmark/fibo.cpp -o benchmark/fibo.ll
	./cdi benchmark/fibo.ll
	llc -filetype=obj benchmark/fibo.ll
	clang++ benchmark/fibo.o runtime.cpp `llvm-config --cxxflags --ldflags --system-libs --libs` -o fibo
clean:
	rm -rf $(PROJECT) $(DBG_FILES) foo a.out
