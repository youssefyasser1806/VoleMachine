#include "voleMachine.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <limits>

// Instruction Constructor
Instruction::Instruction(CPU* c, Memory* m, Registers* r) : cpu(c), mem(m), regs(r) {}

// Memory Class Implementation
Memory::Memory(size_t size) : mem(size, 0) {}

void Memory::store(uint8_t addr, uint8_t val) {
    if (addr < mem.size()) mem[addr] = val;
}

uint8_t Memory::read(uint8_t addr) const {
    return (addr < mem.size()) ? mem[addr] : 0;
}

size_t Memory::size() const {
    return mem.size();
}

// Instruction Execution for LoadFromRAM
void LoadFromRAM::execute(uint16_t instr) {
    uint8_t reg = (instr >> 8) & 0xF;
    uint8_t addr = instr & 0xFF;
    regs->store(reg, mem->read(addr));
}

void LoadImmediate::execute(uint16_t instr) {
    uint8_t reg = (instr >> 8) & 0xF;
    uint8_t val = instr & 0xFF;
    regs->store(reg, val);
}

// CPU Constructor and Destructor
CPU::CPU(MainMemory* mm, Registers* r) : pc(0), ir(0), ram(mm), regs(r) {
    ops.resize(16, nullptr);
    ops[1] = new LoadFromRAM(this, ram, regs);
    ops[2] = new LoadImmediate(this, ram, regs);
    // Initialize other instructions here...
}

CPU::~CPU() {
    for (auto* op : ops) delete op;
}

// Execute a hexadecimal instruction
void CPU::execute(const string& hex_instr) {
    ir = stoul(hex_instr, nullptr, 16);
    uint8_t opcode = (ir >> 12) & 0xF;
    if (opcode > 0 && opcode < ops.size() && ops[opcode]) {
        ops[opcode]->execute(ir);
    }
}

// Display CPU and Memory State
void CPU::dump_state() const {
    cout << "Program Counter (PC): " << static_cast<int>(pc) << endl;
    cout << "\nMemory:\n";
    for (size_t i = 0; i < ram->size(); i++) {
        if (i % 16 == 0)
            cout << "\n" << hex << uppercase << setw(2) << setfill('0') << i/16 << ": ";
        cout << setw(2) << setfill('0') << static_cast<int>(ram->read(i)) << " ";
    }

    cout << "\n\nRegisters:\n";
    for (size_t i = 0; i < regs->size(); i++) {
        cout << "R" << hex << uppercase << i << ": "
             << setw(2) << setfill('0') << static_cast<int>(regs->read(i)) << " ";
    }
    cout << dec << endl;
}

// Utility Function to Validate Hexadecimal Instruction
bool valid_hex_instruction(const string& input) {
    return input.size() == 4 &&(input.begin(), input.end(), ::isxdigit);
}

int main() {
    MainMemory ram;
    Registers regs;
    CPU cpu(&ram, &regs);

    string filename;
    ifstream infile;

    // Requesting file until a valid one is provided
    do {
        cout << "Enter program file (e.g., program.txt): ";
        getline(cin, filename);
        infile.open(filename);
        if (!infile) {
            cerr << "Could not open file: " << filename << "\nPlease try again.\n";
            infile.clear();
        }
    } while (!infile);

    string line;
    while (getline(infile, line)) {
        if (line.empty()) continue;

        while (!valid_hex_instruction(line)) {
            cout << "\nInvalid instruction: " << line;
            cout << "\nEnter a valid 4-digit hex instruction or 'skip' to skip: ";
            getline(cin, line);

            if (line == "skip") {
                cout << "Skipping invalid instruction...\n";
                break;
            }
        }

        if (valid_hex_instruction(line)) {
            try {
                cpu.execute(line);
                cout << "\nExecuted: " << line << endl;
                cpu.dump_state();
            } catch (const exception& e) {
                cout << "\nError executing " << line << ": " << e.what() << "\nRetry? (y/n): ";
                string response;
                getline(cin, response);

                if (response == "y" || response == "Y") {
                    cout << "Enter new instruction: ";
                    getline(cin, line);
                    continue;
                }
            }
        }
    }

    infile.close();
    cout << "\nProgram execution completed.\n";
    return 0;
}
