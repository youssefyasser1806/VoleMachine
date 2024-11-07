#ifndef VOLEMACHINE_H
#define VOLEMACHINE_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Forward declarations
class CPU;
class Memory;
class MainMemory;
class Registers;

// Base instruction class
class Instruction {
protected:
    CPU* cpu;
    Memory* mem;
    Registers* regs;

public:
    Instruction(CPU* c, Memory* m, Registers* r);
    virtual void execute(uint16_t instr) = 0;
    virtual ~Instruction() = default;
};

// Memory management
class Memory {
protected:
    vector<uint8_t> mem;

public:
    Memory(size_t size);
    virtual void store(uint8_t addr, uint8_t val);
    virtual uint8_t read(uint8_t addr) const;
    size_t size() const;
};

// Specific memory types
class MainMemory : public Memory {
public:
    MainMemory();
};

class Registers : public Memory {
public:
    Registers();
};

// Instruction classes
class LoadFromRAM : public Instruction {
public:
    LoadFromRAM(CPU* c, Memory* m, Registers* r);
    void execute(uint16_t instr) override;
};
class LoadImmediate : public Instruction {
public:
    LoadImmediate(CPU* c, Memory* m, Registers* r) ;
    void execute(uint16_t instr) override;
    };

    class StoreToRAM : public Instruction {
public:
    StoreToRAM(CPU* c, Memory* m, Registers* r) ;
    void execute(uint16_t instr) override;};

    class CopyReg : public Instruction {
public:
    CopyReg(CPU* c, Memory* m, Registers* r) ;
    void execute(uint16_t instr) override ;};

    class Add : public Instruction {
public:
    Add(CPU* c, Memory* m, Registers* r) ;
    void execute(uint16_t instr) override;};

class AddFloat : public Instruction {
public:
    AddFloat(CPU* c, Memory* m, Registers* r) ;
    void execute(uint16_t instr) override ;};

    class OR : public Instruction {
public:
    OR(CPU* c, Memory* m, Registers* r) ;
    void execute(uint16_t instr) override;};

    class AND : public Instruction {
public:
    AND(CPU* c, Memory* m, Registers* r) ;
    void execute(uint16_t instr) override ;};

    class XOR : public Instruction {
public:
    XOR(CPU* c, Memory* m, Registers* r) ;
    void execute(uint16_t instr) override;};

    class ROR : public Instruction {  // Rotate Right
public:
    ROR(CPU* c, Memory* m, Registers* r) ;
    void execute(uint16_t instr) override;};

    class JEQ : public Instruction {  // Jump if Equal
public:
    JEQ(CPU* c, Memory* m, Registers* r) ;
    void execute(uint16_t instr) override;};
    class HALT : public Instruction {
public:
    HALT(CPU* c, Memory* m, Registers* r) ;
    void execute(uint16_t) override ; //stops execution
};

class JGT : public Instruction {  // Jump if Greater Than
public:
    JGT(CPU* c, Memory* m, Registers* r) ;
    void execute(uint16_t instr) override ;};
// Additional instruction classes can go here...

// Main CPU implementation
class CPU {
private:
    uint8_t pc;
    uint16_t ir;
    MainMemory* ram;
    Registers* regs;
    vector<Instruction*> ops;

public:
    CPU(MainMemory* mm, Registers* r);
    void execute(const string& hex_instr);
    void dump_state() const;
    ~CPU();


    uint8_t get_pc() const;
    void set_pc(uint8_t value);
};

// Utility function
bool valid_hex_instruction(const string& input);

#endif
