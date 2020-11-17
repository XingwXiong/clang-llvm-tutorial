#include <stdint.h>
#include <stdlib.h>

#include <iostream>
#include <map>

#include "llvm/IR/Instruction.h"

std::map<unsigned, unsigned> instr_map;
int branch_count[2];

// For section 2
// num: the number of unique llvm::Instructions in the basic block. It is the length of keys and values.
// keys: the array of the opcodes of the llvm::Instructions
// values: the array of the counts of the llvm::Instructions
extern "C" __attribute__((visibility("default"))) void updateInstrInfo(unsigned num, uint32_t* keys, uint32_t* values) {
    int i;
    uint32_t key;
    uint32_t value;

    for (i = 0; i < num; i++) {
        key = keys[i];
        value = values[i];
        if (instr_map.count(key) == 0)
            instr_map.insert(std::pair<uint32_t, uint32_t>(key, value));
        else
            instr_map[key] = instr_map[key] + value;
    }
    return;
}

// For section 3
// If taken is true, then a conditional branch is taken;
// If taken is false, then a conditional branch is not taken.
extern "C" __attribute__((visibility("default"))) void updateBranchInfo(bool taken) {
    if (taken) branch_count[0]++;
    branch_count[1]++;
    return;
}

const char* getOpcodeType(unsigned OpCode) {
    switch (OpCode) {
        // Terminators
        case llvm::Instruction::Ret:
        case llvm::Instruction::Br:
        case llvm::Instruction::Switch:
        case llvm::Instruction::IndirectBr:
        case llvm::Instruction::Invoke:
        case llvm::Instruction::Resume:
        case llvm::Instruction::Unreachable:
        case llvm::Instruction::CleanupRet:
        case llvm::Instruction::CatchRet:
        case llvm::Instruction::CatchPad:
        case llvm::Instruction::CatchSwitch:
        case llvm::Instruction::CallBr:
        case llvm::Instruction::Call:
            return "branch";

        // Floating-point Operators...
        case llvm::Instruction::FNeg:
        case llvm::Instruction::FAdd:
        case llvm::Instruction::FSub:
        case llvm::Instruction::FMul:
        case llvm::Instruction::FDiv:
        case llvm::Instruction::FRem:
        case llvm::Instruction::FCmp:
        case llvm::Instruction::FPTrunc:
        case llvm::Instruction::FPExt:
        case llvm::Instruction::UIToFP:
        case llvm::Instruction::SIToFP:
            return "fp";

        // Integer operators...
        case llvm::Instruction::Add:
        case llvm::Instruction::Sub:
        case llvm::Instruction::Mul:
        case llvm::Instruction::UDiv:
        case llvm::Instruction::SDiv:
        case llvm::Instruction::URem:
        case llvm::Instruction::SRem:
        // Logical operators...
        case llvm::Instruction::And:
        case llvm::Instruction::Or:
        case llvm::Instruction::Xor:
        case llvm::Instruction::ICmp:
        case llvm::Instruction::Trunc:
        case llvm::Instruction::ZExt:
        case llvm::Instruction::SExt:
        case llvm::Instruction::FPToUI:
        case llvm::Instruction::FPToSI:
        case llvm::Instruction::IntToPtr:
        case llvm::Instruction::PtrToInt:
        case llvm::Instruction::BitCast:
        case llvm::Instruction::AddrSpaceCast:
        case llvm::Instruction::Shl:
        case llvm::Instruction::LShr:
        case llvm::Instruction::AShr:
        case llvm::Instruction::GetElementPtr:
            return "int";

        // Memory instructions...
        case llvm::Instruction::Load:
            return "load";
        
        case llvm::Instruction::Store:
            return "store";
        
        case llvm::Instruction::Fence:
        case llvm::Instruction::AtomicRMW:
        case llvm::Instruction::AtomicCmpXchg:
        case llvm::Instruction::Alloca:
        case llvm::Instruction::PHI:
        case llvm::Instruction::Select:
        case llvm::Instruction::VAArg:
        case llvm::Instruction::ExtractElement:
        case llvm::Instruction::InsertElement:
        case llvm::Instruction::ShuffleVector:
        case llvm::Instruction::ExtractValue:
        case llvm::Instruction::InsertValue:
        case llvm::Instruction::LandingPad:
        case llvm::Instruction::CleanupPad:
            return "unknown";
        default:
            return "<Invalid operator>";
    }
}

// For section 2
extern "C" __attribute__((visibility("default"))) void printOutInstrInfo() {
    // REF: https://github.com/llvm-mirror/llvm/blob/master/lib/IR/llvm::Instruction.cpp#L290-L371
    std::map<std::string, uint32_t>  instype_map;
    for (std::map<uint32_t, uint32_t>::iterator it = instr_map.begin(); it != instr_map.end(); ++it) {
        printf("%s\t%u\n", llvm::Instruction::getOpcodeName(it->first), it->second);
        instype_map[getOpcodeType(it->first)] += it->second;
    }
    printf("\n");
    
    for (std::map<std::string, uint32_t>::iterator it = instype_map.begin(); it != instype_map.end(); ++it) {
        printf("[%s\t%u]\n", it->first.c_str(), it->second);
    }
    instr_map.clear();

    return;
}

// For section 3
extern "C" __attribute__((visibility("default"))) void printOutBranchInfo() {
    std::cerr << "taken\t" << branch_count[0] << '\n';
    std::cerr << "total\t" << branch_count[1] << '\n';

    branch_count[0] = 0;
    branch_count[1] = 0;

    return;
}
