#include <iostream> // I/O
#include <fstream>  //file operations
#include <iterator> //std::size

class JM
{
private:
  void flags(int R)
  {
    switch (R % 3)
    {
    case 0:
      if (REG[R / 3] == 0)
        ZF = 1;
      else
        ZF = 0;
      break;
    case 1:
      if ((REG[R / 3] & 0xFF00) == 0)
        ZF = 1;
      else
        ZF = 0;
      break;
    case 2:
      if ((REG[R / 3] & 0x00FF) == 0)
        ZF = 1;
      else
        ZF = 0;
      break;
    }
  }

public:
  // ram 0x0000 to 0x1FE0 is VRAM every ram address is 4 pixles
  // each pixle is a 4 bit nibble, that number is used as an index in the color
  // pallet. the screen is 240 x 136 geometry
  uint16_t RAM[0xFFFF];
  uint16_t IP;     //instuction pointer
  uint16_t REG[8]; //regesters AX is reg[0], BX is reg[1] ...
  bool ZF;         //zero flag
  bool HALT;       //halt flag
  bool DRAW;       //draw flag

  JM()
  {
    REG[0] = 0;
    REG[1] = 0;
    REG[2] = 0;
    REG[3] = 0;
    REG[4] = 0;
    REG[5] = 0;
    REG[6] = 0;
    REG[7] = 0;
    IP = 0x1FE1;
    ZF = 0;
    HALT = 0;
    DRAW = 0;
  }

  int load(char *ROM)
  {
    FILE *fp = std::fopen(ROM, "r");
    if (!fp)
    {
      std::perror("Rom opening failed");
      return -1;
    }
    int offset = 0;
    int c; // note: int, not char, required to handle EOF
    c = std::fgetc(fp);
    while (c != EOF && c != 10)
    {
      RAM[0x1FE1 + offset] = (c << 8);
      c = std::fgetc(fp);
      RAM[0x1FE1 + offset] += c;
      c = std::fgetc(fp);
      offset++;
    }

    std::fclose(fp);

    return 0;
  }

  void printMem(uint16_t loc)
  {
    printf("%04X %04X  ", loc, RAM[loc]);
  }

  void printRegs()
  {
    printf("\n%s %04X  ", "A", REG[0]);
    printf("%s %04X  ", "B", REG[1]);
    printf("%s %04X  ", "C", REG[2]);
    printf("%s %04X\n", "D", REG[3]);
    printf("%s %04X  ", "E", REG[4]);
    printf("%s %04X  ", "F", REG[5]);
    printf("%s %04X  ", "I", REG[6]);
    printf("%s %04X\n", "H", REG[7]);
  }

  void run()
  {
    if (HALT)
    {
      printf("HALT\n");
      return;
    }
    uint8_t OPCODE = (RAM[IP] & 0xFF00) >> 8;

    //printf("RAM[IP]: %04x\n", RAM[IP]);
    //printf("OPCODE : %02x\n", OPCODE);

    uint8_t R = RAM[IP] & 0xFF;
    uint16_t VALUE = RAM[IP + 1];
    switch (OPCODE)
    {
    case 0x00: //NOP
      IP += 1;
      break;
    case 0x01: //MTR
      switch (R % 3)
      {
      case 0:
        REG[R / 3] = RAM[VALUE];
        break;
      case 1:
        REG[R / 3] = (REG[R / 3] & 0xFF) | (RAM[VALUE] & 0xFF00);
        break;
      case 2:
        REG[R / 3] = (REG[R / 3] & 0xFF00) | (RAM[VALUE] & 0xFF);
        break;
      }
      IP += 2;
      break;
    case 0x02: //RTM
      switch (R % 3)
      {
      case 0:
        RAM[VALUE] = REG[R / 3];
        break;
      case 1:
        RAM[VALUE] = (RAM[VALUE] & 0xFF) | (REG[R / 3] & 0xFF00);
        break;
      case 2:
        RAM[VALUE] = (RAM[VALUE] & 0xFF00) | (REG[R / 3] & 0xFF);
        break;
      }
      IP += 2;
      break;
    case 0x03: //ITR
      switch (R % 3)
      {
      case 0:
        REG[R / 3] = VALUE;
        break;
      case 1:
        REG[R / 3] = (REG[R / 3] & 0xFF) | ((VALUE & 0xFF) << 8);
        break;
      case 2:
        REG[R / 3] = (REG[R / 3] & 0xFF00) | (VALUE & 0xFF);
        break;
      }
      IP += 2;
      break;
    case 0x04: //ADDM
      switch (R % 3)
      {
      case 0:
        REG[R / 3] = REG[R / 3] + RAM[VALUE];
        break;
      case 1:
        REG[R / 3] = (REG[R / 3] & 0xFF) | ((REG[R / 3] & 0xFF00) + ((RAM[VALUE]) & 0xFF00));
        break;
      case 2:
        REG[R / 3] = (REG[R / 3] & 0xFF00) | ((REG[R / 3] & 0xFF) + ((RAM[VALUE]) & 0xFF));
        break;
      }
      flags(R);
      IP += 2;
      break;
    case 0x05: //ADDI
      switch (R % 3)
      {
      case 0:
        REG[R / 3] = REG[R / 3] + VALUE;
        break;
      case 1:
        REG[R / 3] = (REG[R / 3] & 0xFF) | ((REG[R / 3] & 0xFF00) + ((VALUE)&0xFF00));
        break;
      case 2:
        REG[R / 3] = (REG[R / 3] & 0xFF00) | ((REG[R / 3] & 0xFF) + ((VALUE)&0xFF));
        break;
      }
      flags(R);
      IP += 2;
      break;
    case 0x06: //SUBM
      switch (R % 3)
      {
      case 0:
        REG[R / 3] = REG[R / 3] - RAM[VALUE];
        break;
      case 1:
        REG[R / 3] = (REG[R / 3] & 0xFF) | ((REG[R / 3] & 0xFF00) - ((RAM[VALUE]) & 0xFF00));
        break;
      case 2:
        REG[R / 3] = (REG[R / 3] & 0xFF00) | ((REG[R / 3] & 0xFF) - ((RAM[VALUE]) & 0xFF));
        break;
      }
      flags(R);
      IP += 2;
      break;
    case 0x07: //SUBI
      switch (R % 3)
      {
      case 0:
        REG[R / 3] = REG[R / 3] - VALUE;
        break;
      case 1:
        REG[R / 3] = (REG[R / 3] & 0xFF) | ((REG[R / 3] & 0xFF00) - ((VALUE)&0xFF00));
        break;
      case 2:
        REG[R / 3] = (REG[R / 3] & 0xFF00) | ((REG[R / 3] & 0xFF) - ((VALUE)&0xFF));
        break;
      }
      flags(R);
      IP += 2;
      break;
    case 0x08: //SUBIND
      switch (R % 3)
      {
      case 0:
        if ((REG[R / 3] - VALUE) == 0)
          ZF = 1;
        else
          ZF = 0;
        break;
      case 1:
        if ((REG[R / 3] & 0xFF) | ((REG[R / 3] & 0xFF00) - ((VALUE)&0xFF00)) == 0)
          ZF = 1;
        else
          ZF = 0;
        break;
      case 2:
        if ((REG[R / 3] & 0xFF00) | ((REG[R / 3] & 0xFF) - ((VALUE)&0xFF)) == 0)
          ZF = 1;
        else
          ZF = 0;
        break;
      }
      IP += 2;
      break;
    case 0x09: //AND
      switch (R % 3)
      {
      case 0:
        REG[R / 3] = REG[R / 3] & RAM[VALUE];
        break;
      case 1:
        REG[R / 3] = (REG[R / 3] & 0xFF) | ((REG[R / 3] & 0xFF00) & ((RAM[VALUE]) & 0xFF00));
        break;
      case 2:
        REG[R / 3] = (REG[R / 3] & 0xFF00) | ((REG[R / 3] & 0xFF) & ((RAM[VALUE]) & 0xFF));
        break;
      }
      flags(R);
      IP += 2;
      break;
    case 0x0A: //OR
      switch (R % 3)
      {
      case 0:
        REG[R / 3] = REG[R / 3] | RAM[VALUE];
        break;
      case 1:
        REG[R / 3] = (REG[R / 3] & 0xFF) | ((REG[R / 3] & 0xFF00) | ((RAM[VALUE]) & 0xFF00));
        break;
      case 2:
        REG[R / 3] = (REG[R / 3] & 0xFF00) | ((REG[R / 3] & 0xFF) | ((RAM[VALUE]) & 0xFF));
        break;
      }
      flags(R);
      IP += 2;
      break;
    case 0x0B: //XOR
      switch (R % 3)
      {
      case 0:
        REG[R / 3] = REG[R / 3] ^ RAM[VALUE];
        break;
      case 1:
        REG[R / 3] = (REG[R / 3] & 0xFF) | ((REG[R / 3] & 0xFF00) ^ ((RAM[VALUE]) & 0xFF00));
        break;
      case 2:
        REG[R / 3] = (REG[R / 3] & 0xFF00) | ((REG[R / 3] & 0xFF) ^ ((RAM[VALUE]) & 0xFF));
        break;
      }
      flags(R);
      IP += 2;
      break;
    case 0x0C: //NOT
      switch (R % 3)
      {
      case 0:
        REG[R / 3] = !(RAM[VALUE]);
        break;
      case 1:
        REG[R / 3] = (REG[R / 3] & 0xFF) | (!((RAM[VALUE]) & 0xFF00));
        break;
      case 2:
        REG[R / 3] = (REG[R / 3] & 0xFF00) | (!((RAM[VALUE]) & 0xFF));
        break;
      }
      flags(R);
      IP += 2;
      break;
    case 0x0D: //SHL
      switch (R % 3)
      {
      case 0:
        REG[R / 3] = REG[R / 3] << VALUE;
        break;
      case 1:
        REG[R / 3] = (REG[R / 3] & 0xFF) | ((REG[R / 3] & 0xFF00) << VALUE);
        break;
      case 2:
        REG[R / 3] = (REG[R / 3] & 0xFF00) | ((REG[R / 3] & 0xFF) << VALUE);
        break;
      }
      flags(R);
      IP += 2;
      break;
    case 0x0E: //SHL
      switch (R % 3)
      {
      case 0:
        REG[R / 3] = REG[R / 3] >> VALUE;
        break;
      case 1:
        REG[R / 3] = (REG[R / 3] & 0xFF) | ((REG[R / 3] & 0xFF00) >> VALUE);
        break;
      case 2:
        REG[R / 3] = (REG[R / 3] & 0xFF00) | ((REG[R / 3] & 0xFF) >> VALUE);
        break;
      }
      flags(R);
      IP += 2;
      break;
    case 0x0F: //JMP
      IP = VALUE;
      break;
    case 0x10: //JZ
      if (ZF)
        IP = VALUE;
      else
        IP += 2;
      break;
    case 0x11: //JNZ
      if (!ZF)
        IP = VALUE;
      else
        IP += 2;
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
      printf("%X\n", OPCODE);
      break;
    }

    if (IP > 0xFFFF)
    {
      printf("IP too big\n");
      HALT = 1;
    }
  }

  void nibbletrgb(uint8_t (&color)[3], u_int8_t nibble)
  {
    /*
      0  black   #00 00 00
      1  drkgry  #44 44 44
      2  grey    #88 88 88
      3  ltgrey  #bb bb bb
      4  ltbrwn  #99 66 33
      5  brown   #66 33 00
      6  green   #00 66 00
      7  lime    #00 aa 00
      8  cyan    #00 99 ff
      9  blue    #00 00 cc
      a  violet  #33 00 99
      b  magenta #ff 00 99
      c  red     #dd 00 00
      d  orange  #ff 66 00
      e  yellow  #ff ff 00
      f  white   #ff ff ff
  */
    nibble = nibble & 0xF;
    uint8_t red[16] = {0x00, 0x44, 0x88, 0xbb, 0x99, 0x66, 0x00, 0x00, 0x00, 0x00, 0x33, 0xff, 0xdd, 0xff, 0xff, 0xff};
    uint8_t grn[16] = {0x00, 0x44, 0x88, 0xbb, 0x66, 0x33, 0x66, 0xaa, 0x99, 0x00, 0x00, 0x00, 0x00, 0x66, 0xff, 0xff};
    uint8_t blu[16] = {0x00, 0x44, 0x88, 0xbb, 0x33, 0x00, 0x00, 0x00, 0xff, 0xcc, 0x99, 0x99, 0x00, 0x00, 0x00, 0xff};
    color[0] = red[nibble];
    color[1] = grn[nibble];
    color[2] = blu[nibble];
  }
};
