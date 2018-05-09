//
//  main.cpp
//  Lab 10
//
//  Created by Caleb Tenney on 4/11/18.
//  Copyright © 2018 Caleb Tenney. All rights reserved.
//

#include <iostream>
using namespace std;
#include <fstream>
#include <string>
#include <vector>
ifstream fin("input.txt");

string HexToBinary(string hex) {
    string binary;
    for (int i = 2; i < hex.length(); i++) { // i = 2 to skip the "0x" at beginning of hexadecimal value
        if (hex.at(i) == ' ')
            i+=2;
        else if (hex.at(i) == '0')
            binary.append("0000");
        else if (hex.at(i) == '1')
            binary.append("0001");
        else if (hex.at(i) == '2')
            binary.append("0010");
        else if (hex.at(i) == '3')
            binary.append("0011");
        else if (hex.at(i) == '4')
            binary.append("0100");
        else if (hex.at(i) == '5')
            binary.append("0101");
        else if (hex.at(i) == '6')
            binary.append("0110");
        else if (hex.at(i) == '7')
            binary.append("0111");
        else if (hex.at(i) == '8')
            binary.append("1000");
        else if (hex.at(i) == '9')
            binary.append("1001");
        else if (hex.at(i) == 'a')
            binary.append("1010");
        else if (hex.at(i) == 'b')
            binary.append("1011");
        else if (hex.at(i) == 'c')
            binary.append("1100");
        else if (hex.at(i) == 'd')
            binary.append("1101");
        else if (hex.at(i) == 'e')
            binary.append("1110");
        else if (hex.at(i) == 'f')
            binary.append("1111");
    }
    return binary;
}

string DisplayBinary(string s) {
    for (int i = 0; i < s.length(); i+=4)
        cout << s.at(i) << s.at(i+1) << s.at(i+2) << s.at(i+3) << " ";
    cout << endl;
    return s;
}

int GetOpcode(string instruction) {
    cout << "op:" << stoi(instruction.substr(0,6),nullptr,2) << endl;
    return stoi(instruction.substr(0,6),nullptr,2);
}

int GetRS(string instruction) {
    cout << "  rs:" << stoi(instruction.substr(6,5),nullptr,2) << endl;
    return stoi(instruction.substr(6,5),nullptr,2);
}

int GetRT(string instruction) {
    cout << "  rt:" << stoi(instruction.substr(11,5),nullptr,2) << endl;
    return stoi(instruction.substr(11,5),nullptr,2);
}

int GetRD(string instruction) {
    cout << "  rd:" << stoi(instruction.substr(16,5),nullptr,2) << endl;
    return stoi(instruction.substr(16,5),nullptr,2);
}

int GetSA(string instruction) {
    cout << "  sa:" << stoi(instruction.substr(21,5),nullptr,2) << endl;
    return stoi(instruction.substr(21,5),nullptr,2);
}

int GetFunction(string instruction) {
    cout << "  fn:" << stoi(instruction.substr(26,6),nullptr,2) << endl;
    return stoi(instruction.substr(26,6),nullptr,2);
}

int GetAddrImm(string instruction) {
    cout << "  ad:" << instruction.substr(16,16) << endl;
    return stoi(instruction.substr(16,16),nullptr,2);
}

int GetInstrIndex(string instruction) {
    cout << "  in:" << stoi(instruction.substr(6,26),nullptr,2) << endl;
    return stoi(instruction.substr(6,26),nullptr,2);
}

int main(int argc, const char * argv[]) {
    vector<int> registers(32);
    vector<string> instructions;
    vector<string> data;
    int pc; // Program Counter
    int hi = 0, lo = 0; // Special HI and LO Registers
    string line;
    
    // ASSUMPTIONS FOR BASIC FUNCTIONALITY DEMO
    registers.at(13) = 0x53FC;
    registers.at(14) = 0x6405;
    registers.at(16) = 0x10010000;
    
    
    //// FETCH ////
    
    
    // Read instructions from file and store each line as its own index in a vector
    getline(fin,line);
    while (line != "DATA SEGMENT") {
        instructions.push_back(line);
        getline(fin,line);
    }
    // Display all instructions in hex
    for (int i = 0; i < instructions.size(); i++)
        cout << instructions.at(i) << endl;
    
    // Convert all instructions to binary and display
    for (int i = 0; i < instructions.size(); i++) {
        instructions.at(i) = HexToBinary(instructions.at(i));
        instructions.at(i) = DisplayBinary(instructions.at(i));
    }
    
    // Read data segment from file and store each line as its own index in a vector
    while (getline(fin,line))
        data.push_back(line);
    
    // Convert all data to binary and display
    for (int i = 0; i < data.size(); i++) {
        data.at(i) = HexToBinary(data.at(i));
        data.at(i) = DisplayBinary(data.at(i));
    }
    
    
    //// DECODE & EXECUTE ////
    
    
    int opcode, rs, rt, rd, sa, funct, addrImm, instrIndex;
    for (pc = 0; pc < instructions.size(); pc++) {
        cout << endl << instructions.at(pc) << endl;
        opcode = GetOpcode(instructions.at(pc));
        
        // R-FORMAT
        if (opcode == 0) {
            funct = GetFunction(instructions.at(pc));
            rs = GetRS(instructions.at(pc));
            rt = GetRT(instructions.at(pc));
            rd = GetRD(instructions.at(pc));
            sa = GetSA(instructions.at(pc));
            
            if (funct == 0) {                                             // sll
                registers.at(rd) = registers.at(rt) << sa;
            }
            else if (funct == 2) {                                        // srl
                unsigned int u = registers.at(rt);
                registers.at(rd) = u >> sa;
            }
            else if (funct == 3) {                                        // sra
                registers.at(rd) = registers.at(rt) >> sa;
            }
            else if (funct == 4) {                                        // sllv
                registers.at(rd) = registers.at(rt) << registers.at(rs);
            }
            else if (funct == 6) {                                        // srlv
                unsigned int u = registers.at(rt);
                registers.at(rd) = u >> registers.at(rs);
            }
            else if (funct == 7) {                                        // srav
                registers.at(rd) = registers.at(rt) >> registers.at(rs);
            }
            else if (funct == 8) {                                        // jr
                
            }
            else if (funct == 9) {                                        // jalr
                
            }
            else if (funct == 10) {                                       // movz
                if (registers.at(rt) == 0)
                    registers.at(rd) = registers.at(rs);
            }
            else if (funct == 11) {                                       // movn
                if (registers.at(rt) != 0)
                    registers.at(rd) = registers.at(rs);
            }
            else if (funct == 12) {                                       // syscall
                if (registers.at(2) == 1) {       // print_int
                    
                }
                else if (registers.at(2) == 4) {  // print_string
                    
                }
                else if (registers.at(2) == 5) {  // read_int
                    
                }
                else if (registers.at(2) == 8) {  // read_string
                    
                }
                else if (registers.at(2) == 10) { // exit
                    
                }
            }
            else if (funct == 16) {                                       // mfhi
                registers.at(rd) = hi;
            }
            else if (funct == 17) {                                       // mthi
                hi = registers.at(rs);
            }
            else if (funct == 18) {                                       // mflo
                registers.at(rd) = lo;
            }
            else if (funct == 19) {                                       // mtlo
                lo = registers.at(rs);
            }
            else if (funct == 24) {                                       // mult
                long long p = registers.at(rs) * registers.at(rt);
                hi = (p >> 32);
                lo = p & ((1LL << 32) - 1);
            }
            else if (funct == 25) {                                       // multu
                unsigned int urs = registers.at(rs);
                unsigned int urt = registers.at(rt);
                unsigned long long p = urs * urt;
                hi = (p >> 32);
                lo = p & ((1LL << 32) - 1);
            }
            else if (funct == 26) {                                       // div
                long long q = registers.at(rs) / registers.at(rt);
                hi = (q >> 32);
                lo = q & ((1LL << 32) - 1);
            }
            else if (funct == 27) {                                       // divu
                unsigned int urs = registers.at(rs);
                unsigned int urt = registers.at(rt);
                unsigned long long q = urs / urt;
                hi = (q >> 32);
                lo = q & ((1LL << 32) - 1);
            }
            else if (funct == 32) {                                       // add
                registers.at(rd) = registers.at(rs) + registers.at(rt);
            }
            else if (funct == 33) {                                       // addu
                unsigned int urs = registers.at(rs);
                unsigned int urt = registers.at(rt);
                registers.at(rd) = urs + urt;
            }
            else if (funct == 34) {                                       // sub
                registers.at(rd) = registers.at(rs) - registers.at(rt);
            }
            else if (funct == 35) {                                       // subu
                unsigned int urs = registers.at(rs);
                unsigned int urt = registers.at(rt);
                registers.at(rd) = urs - urt;
            }
            else if (funct == 36) {                                       // and
                registers.at(rd) = registers.at(rs) & registers.at(rt);
            }
            else if (funct == 37) {                                       // or
                registers.at(rd) = registers.at(rs) | registers.at(rt);
            }
            else if (funct == 38) {                                       // xor
                registers.at(rd) = registers.at(rs) ^ registers.at(rt);
            }
            else if (funct == 39) {                                       // nor
                registers.at(rd) = ~(registers.at(rs) | registers.at(rt));
            }
            else if (funct == 42) {                                       // slt
                registers.at(rd) = registers.at(rs) < registers.at(rt);
            }
            else if (funct == 43) {                                       // sltu
                unsigned int urs = registers.at(rs);
                unsigned int urt = registers.at(rt);
                registers.at(rd) = urs < urt;
            }
        }
        
        // J-FORMAT
        else if (opcode == 2) {                                           // j
            instrIndex = GetInstrIndex(instructions.at(pc));
            pc = instrIndex - 1048586; // accounts for 9 QtSpim instructions
        }
        else if (opcode == 3) {                                           // jal
            instrIndex = GetInstrIndex(instructions.at(pc));
            pc = instrIndex - 1048586; // accounts for 9 QtSpim instructions
            registers.at(31) = instrIndex;
        }
        
        // I-FORMAT
        else {
            rs = GetRS(instructions.at(pc));
            rt = GetRT(instructions.at(pc));
            addrImm = GetAddrImm(instructions.at(pc));
            if (opcode == 4) {                                            // beq
                if (registers.at(rs) == registers.at(rt))
                    pc+= addrImm - 1;
            }
            else if (opcode == 5) {                                       // bne
                if (registers.at(rs) != registers.at(rt))
                    pc+= addrImm - 1;
            }
            else if (opcode == 6) {                                       // blez
                if (registers.at(rs) <= 0)
                    pc+= addrImm - 1;
            }
            else if (opcode == 7) {                                       // bgtz
                if (registers.at(rs) >= 0)
                    pc+= addrImm - 1;
            }
            else if (opcode == 8) {                                       // addi
                registers.at(rt) = registers.at(rs) + addrImm;
            }
            else if (opcode == 9) {                                       // addiu
                unsigned int urs = registers.at(rs);
                unsigned int uimm = addrImm;
                registers.at(rt) = urs + uimm;
            }
            else if (opcode == 10) {                                      // slti
                registers.at(rt) = registers.at(rs) < addrImm;
            }
            else if (opcode == 11) {                                      // sltiu
                unsigned int urs = registers.at(rs);
                unsigned int uimm = addrImm;
                registers.at(rt) = urs < uimm;
            }
            else if (opcode == 12) {                                      // andi
                registers.at(rt) = registers.at(rs) & addrImm;
            }
            else if (opcode == 13) {                                      // ori
                registers.at(rt) = registers.at(rs) | addrImm;
            }
            else if (opcode == 14) {                                      // xori
                registers.at(rt) = registers.at(rs) ^ addrImm;
            }
            else if (opcode == 15) {                                      // lui
                registers.at(rt) = addrImm << 16;
            }
            else if (opcode == 32) {                                      // lb
                
            }
            else if (opcode == 33) {                                      // lh
                
            }
            else if (opcode == 35) {                                      // lw
                
            }
            else if (opcode == 36) {                                      // lbu
                
            }
            else if (opcode == 37) {                                      // lhu
                
            }
            else if (opcode == 40) {                                      // sb
                
            }
            else if (opcode == 41) {                                      // sh
                
            }
            else if (opcode == 43) {                                      // sw
                
            }
        }
    }
    
    return 0;
}
