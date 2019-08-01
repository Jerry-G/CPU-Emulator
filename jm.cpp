#include <iostream>  // I/O
#include <fstream>   //file operations
#include <iterator>  //std::size

class JM{
  private:
    void flags(int R){
      switch(R%3){
        case 0: if(REG[R/3] == 0) ZF = 1; 
                else ZF = 0;
                break;
        case 1: if((REG[R/3]&0xFF00) == 0) ZF =1;
                else ZF = 0;
                break;
        case 2: if((REG[R/3]&0x00FF) == 0) ZF =1;
                else ZF = 0;
                break;
      }
    }
  public:
  uint16_t RAM[0xFFFF + 1]; //CGA grafx, 8bit color
  uint16_t IP;              //instuction pointer
  uint16_t REG[8];          //regesters AX is reg[0], BX is reg[1] ...
  bool     ZF;              //zero flag
  bool     HALT;            //halt flag
  bool     DRAW;            //draw flag

  JM(){
    REG[0] = 0; REG[1] = 0; REG[2] = 0; REG[3] = 0; 
    REG[4] = 0; REG[5] = 0; REG[6] = 0; REG[7] = 0;
    IP = 0x1FE1;
    ZF = 0;
    HALT = 0;
    DRAW = 0;
  }

  int load(char* ROM){
    FILE* fp = std::fopen(ROM, "r");
    if(!fp) {
      std::perror("Rom opening failed");
      return -1;
    }
    int offset = 0;
    int c; // note: int, not char, required to handle EOF
    c = std::fgetc(fp);
    while (c != EOF && c != 10) {
      RAM[0x1FE1 + offset] = (c << 8);
      c = std::fgetc(fp);
      RAM[0x1FE1 + offset] += c;
      c = std::fgetc(fp);
      offset++;
    }

    std::fclose(fp);

    return 0;
  }

  void printMem(uint16_t loc){
    printf("%04X %04X  ",loc,RAM[loc]);
  }

  void printRegs(){
    printf("\n%s %04X  ","A",REG[0]); printf("%s %04X  ","B",REG[1]);
    printf("%s %04X  ","C",REG[2]); printf("%s %04X\n","D",REG[3]);
    printf("%s %04X  ","E",REG[4]); printf("%s %04X  ","F",REG[5]);
    printf("%s %04X  ","I",REG[6]); printf("%s %04X\n","H",REG[7]);
  }

  void run(){
    if(HALT){ printf("HALT\n"); return; }
    uint8_t  OPCODE = (RAM[IP] & 0xFF00) >> 8; 

    //printf("RAM[IP]: %04x\n",RAM[IP]);
    //printf("OPCODE : %02x\n",OPCODE);

    uint8_t  R = RAM[IP] & 0xFF;
    uint16_t VALUE  = RAM[IP+1];
    switch(OPCODE){
      case 0x00: //NOP
        IP += 1; break; 
      case 0x01: //MTR
        switch(R%3){
          case 0: REG[R/3] = RAM[VALUE]; break;
          case 1: REG[R/3] = (REG[R/3]&0xFF)|(RAM[VALUE]&0xFF00); break;
          case 2: REG[R/3] = (REG[R/3]&0xFF00)|(RAM[VALUE]&0xFF); break;
        }
        IP += 2;
        break;
      case 0x02: //RTM
        switch(R%3){
          case 0: RAM[VALUE] = REG[R/3]; break;
          case 1: RAM[VALUE] = (RAM[VALUE]&0xFF)|(REG[R/3]&0xFF00); break;
          case 2: RAM[VALUE] = (RAM[VALUE]&0xFF00)|(REG[R/3]&0xFF); break;
        }
        IP += 2;
        break;
      case 0x03: //ITR
        switch(R%3){
          case 0: REG[R/3] = VALUE; break;
          case 1: REG[R/3] = (REG[R/3]&0xFF)|((VALUE&0xFF)<<8); break;
          case 2: REG[R/3] = (REG[R/3]&0xFF00)|(VALUE&0xFF); break;
        }
        IP += 2;
        break;
      case 0x04: //ADDM
        switch(R%3){
          case 0: REG[R/3] = REG[R/3] + RAM[VALUE]; break;
          case 1: REG[R/3] = (REG[R/3]&0xFF)|((REG[R/3]&0xFF00)+((RAM[VALUE])&0xFF00)); break;
          case 2: REG[R/3] = (REG[R/3]&0xFF00)|((REG[R/3]&0xFF)+((RAM[VALUE])&0xFF)); break;
        }
        flags(R);
        IP += 2;
        break;
      case 0x05: //ADDI
        switch(R%3){
          case 0: REG[R/3] = REG[R/3] + VALUE; break;
          case 1: REG[R/3] = (REG[R/3]&0xFF)|((REG[R/3]&0xFF00)+((VALUE)&0xFF00)); break;
          case 2: REG[R/3] = (REG[R/3]&0xFF00)|((REG[R/3]&0xFF)+((VALUE)&0xFF)); break;
        }
        flags(R);
        IP += 2;
        break;
      case 0x06: //SUBM
        switch(R%3){
          case 0: REG[R/3] = REG[R/3] - RAM[VALUE]; break;
          case 1: REG[R/3] = (REG[R/3]&0xFF)|((REG[R/3]&0xFF00)-((RAM[VALUE])&0xFF00)); break;
          case 2: REG[R/3] = (REG[R/3]&0xFF00)|((REG[R/3]&0xFF)-((RAM[VALUE])&0xFF)); break;
        }
        flags(R);
        IP += 2;
        break;
      case 0x07: //SUBI
        switch(R%3){
          case 0: REG[R/3] = REG[R/3] - VALUE; break;
          case 1: REG[R/3] = (REG[R/3]&0xFF)|((REG[R/3]&0xFF00)-((VALUE)&0xFF00)); break;
          case 2: REG[R/3] = (REG[R/3]&0xFF00)|((REG[R/3]&0xFF)-((VALUE)&0xFF)); break;
        }
        flags(R);
        IP += 2;
        break;
      case 0x08: //SUBIND
        switch(R%3){
          case 0: if((REG[R/3] - VALUE)==0) ZF = 1;
                  else ZF = 0; 
                  break;
          case 1: if((REG[R/3]&0xFF)|((REG[R/3]&0xFF00)-((VALUE)&0xFF00)) == 0) ZF = 1;
                  else ZF = 0; 
                  break;
          case 2: if((REG[R/3]&0xFF00)|((REG[R/3]&0xFF)-((VALUE)&0xFF)) == 0) ZF = 1; 
                  else ZF = 0;
                  break;
        }
        IP += 2;
        break;
      case 0x09: //AND
        switch(R%3){
          case 0: REG[R/3] = REG[R/3] & RAM[VALUE]; break;
          case 1: REG[R/3] = (REG[R/3]&0xFF)|((REG[R/3]&0xFF00)&((RAM[VALUE])&0xFF00)); break;
          case 2: REG[R/3] = (REG[R/3]&0xFF00)|((REG[R/3]&0xFF)&((RAM[VALUE])&0xFF)); break;
        }
        flags(R);
        IP += 2;
        break;
      case 0x0A: //OR
        switch(R%3){
          case 0: REG[R/3] = REG[R/3] | RAM[VALUE]; break;
          case 1: REG[R/3] = (REG[R/3]&0xFF)|((REG[R/3]&0xFF00)|((RAM[VALUE])&0xFF00)); break;
          case 2: REG[R/3] = (REG[R/3]&0xFF00)|((REG[R/3]&0xFF)|((RAM[VALUE])&0xFF)); break;
        }
        flags(R);
        IP += 2;
        break;
      case 0x0B: //XOR
        switch(R%3){
          case 0: REG[R/3] = REG[R/3] ^ RAM[VALUE]; break;
          case 1: REG[R/3] = (REG[R/3]&0xFF)|((REG[R/3]&0xFF00)^((RAM[VALUE])&0xFF00)); break;
          case 2: REG[R/3] = (REG[R/3]&0xFF00)|((REG[R/3]&0xFF)^((RAM[VALUE])&0xFF)); break;
        }
        flags(R);
        IP += 2;
        break;
      case 0x0C: //NOT
        switch(R%3){
          case 0: REG[R/3] = !(RAM[VALUE]); break;
          case 1: REG[R/3] = (REG[R/3]&0xFF)|(!((RAM[VALUE])&0xFF00)); break;
          case 2: REG[R/3] = (REG[R/3]&0xFF00)|(!((RAM[VALUE])&0xFF)); break;
        }
        flags(R);
        IP += 2;
        break;
      case 0x0D: //SHL
        switch(R%3){
          case 0: REG[R/3] = REG[R/3] << VALUE; break;
          case 1: REG[R/3] = (REG[R/3]&0xFF)|((REG[R/3]&0xFF00) << VALUE); break;
          case 2: REG[R/3] = (REG[R/3]&0xFF00)|((REG[R/3]&0xFF) << VALUE); break;
        }
        flags(R);
        IP += 2;
        break;
      case 0x0E: //SHL
        switch(R%3){
          case 0: REG[R/3] = REG[R/3] >> VALUE; break;
          case 1: REG[R/3] = (REG[R/3]&0xFF)|((REG[R/3]&0xFF00) >> VALUE); break;
          case 2: REG[R/3] = (REG[R/3]&0xFF00)|((REG[R/3]&0xFF) >> VALUE); break;
        }
        flags(R);
        IP += 2;
        break;
      case 0x0F: //JMP
        IP = VALUE;
        break;
      case 0x10: //JZ
        if(ZF) IP = VALUE;
        break;
      case 0x11: //JNZ
        if(!ZF) IP = VALUE;
        break;
      case 0xFE: //DRAW
        DRAW = true;
        IP += 1;
        break;
      case 0xFF: //HALT
        HALT = true;
        break;
      default:
        std::cout << "Invalid Opcode of ";
        printf("%X\n",OPCODE);
        break;
    }


    if(IP > 0xFFFF){
      printf("IP too big\n");
      HALT = 1;
    }
  }
  
  /* implement this in a working manner
  void rgbitrgb(uint8_t (&color)[3], bool r, bool g, bool b, bool i){
    color[0] = 192;
    color[1] = 192;
    color[2] = 192;
    if(!r && !g && !b && i)  return;

    color[0] = (r)?255:0;
    color[1] = (g)?255:0;
    color[2] = (b)?255:0;

    if(i){
      color[0] /= 2;
      color[1] /= 2;
      color[2] /= 2;
    }
  }
  */




};
