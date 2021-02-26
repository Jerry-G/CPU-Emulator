#include <fstream>  //file operations
#include <iostream> // I/O
#include <iterator> //std::size

class JM {
private:
  void flags(int R) {
    switch (R % 3) {
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

  void moveChar(uint16_t reg, uint16_t value) {
    uint16_t c = ((reg & 0xFF00) >> 8) * 4;
    uint16_t line = CHARS[c];
    uint16_t fourPixles = 0;
    uint8_t fg = (reg & 0x00F0) >> 4;
    uint8_t bg = (reg & 0x000F);

    for (int four = 0; four < 4; four++) {
      line = CHARS[c + four];

      fourPixles = 0;
      for (int i = 0; i < 4; i++) {
        if (((line >> 12) >> i) & 0x1) {
          fourPixles += fg << (i * 4);
        } else {
          fourPixles += bg << (i * 4);
        }
      }

      RAM[value] = fourPixles;
      fourPixles = 0;
      for (int i = 0; i < 4; i++) {
        if (((line >> 8) >> i) & 0x1) {
          fourPixles += fg << (i * 4);
        } else {
          fourPixles += bg << (i * 4);
        }
      }

      RAM[value + 1] = fourPixles;
      fourPixles = 0;
      for (int i = 0; i < 4; i++) {
        if (((line >> 4) >> i) & 0x1) {
          fourPixles += fg << (i * 4);
        } else {
          fourPixles += bg << (i * 4);
        }
      }

      RAM[value + 0x3C] = fourPixles;
      fourPixles = 0;
      for (int i = 0; i < 4; i++) {
        if (((line >> 0) >> i) & 0x1) {
          fourPixles += fg << (i * 4);
        } else {
          fourPixles += bg << (i * 4);
        }
      }

      RAM[value + 0x3D] = fourPixles;
      value += 0x3C * 2;
    }
  }

  const uint16_t CHARS[4 * 255] = {
      0x386C, 0xC6C6, 0xFEC6, 0xC600, // A  00
      0xFCC6, 0xC6FC, 0xC6C6, 0xFC00, // B  01
      0x3C66, 0xC0C0, 0xC066, 0x3C00, // C  02
      0xF8CC, 0xC6C6, 0xC6CC, 0xF800, // D  03
      0xFEC0, 0xC0FC, 0xC0C0, 0xFE00, // E  04
      0xFEC0, 0xC0FC, 0xC0C0, 0xC000, // F  05
      0x3E60, 0xC0CE, 0xC666, 0x3E00, // G  06
      0xC6C6, 0xC6FE, 0xC6C6, 0xC600, // H  07
      0x7E18, 0x1818, 0x1818, 0x7E00, // I  08
      0x0606, 0x0606, 0x0606, 0x7C00, // J  09
      0xC6CC, 0xD8F0, 0xF8DC, 0xCE00, // K  0A
      0x6060, 0x6060, 0x6060, 0x7E00, // L  0B
      0xC6EE, 0xFEFE, 0xD6C6, 0xC600, // M  0C
      0xC6E6, 0xF6FE, 0xDECE, 0xC600, // N  0D
      0x7CC6, 0xC6C6, 0xC6C6, 0x7C00, // O  0E
      0xFCC6, 0xC6C6, 0xFCC0, 0xC000, // P  0F
      0x7CC6, 0xC6C6, 0xDECC, 0x7A00, // Q  10
      0xFCC6, 0xC6CE, 0xF8DC, 0xCE00, // R  11
      0x78CC, 0xC07C, 0x06C6, 0x7C00, // S  12
      0x7E18, 0x1818, 0x1818, 0x1800, // T  13
      0xC6C6, 0xC6C6, 0xC6C6, 0x7C00, // U  14
      0xC6C6, 0xC6EE, 0x7C38, 0x1000, // V  15
      0xC6C6, 0xD6FE, 0xFEEE, 0xC600, // W  16
      0xC6EE, 0x7C38, 0x7CEE, 0xC600, // X  17
      0x6666, 0x663C, 0x1818, 0x1800, // Y  18
      0xFE0E, 0x1C38, 0x70E0, 0xFE00, // Z  19
      0x0030, 0x3000, 0x3030, 0x0000, // :  1A
      0x0030, 0x3000, 0x3030, 0x6000, // ;  1B
      0x0018, 0x3060, 0x3018, 0x0000, // <  1C
      0x0030, 0x180C, 0x1830, 0x0000, // >  1D
      0x3030, 0x3000, 0x0000, 0x0000, // '  1E
      0x6C6C, 0x6C00, 0x0000, 0x0000, // "  1F
      0x0000, 0x7C06, 0x7EC6, 0x7E00, // a  20
      0xC0C0, 0xFCC6, 0xC6C6, 0x7C00, // b  21
      0x0000, 0x7EC0, 0xC0C0, 0x7E00, // c  22
      0x0606, 0x7EC6, 0xC6C6, 0x7E00, // d  23
      0x0000, 0x7CC6, 0xFEC0, 0x7C00, // e  24
      0x0E18, 0x7E18, 0x1818, 0x1800, // f  25
      0x0000, 0x7EC6, 0xC67E, 0x067C, // g  26
      0xC0C0, 0xFCC6, 0xC6C6, 0xC600, // h  27
      0x1800, 0x3818, 0x1818, 0x7e00, // i  28
      0x0C00, 0x1C0C, 0x0C0C, 0x0C78, // j  29
      0xC0C0, 0xCEFC, 0xF8DC, 0xCE00, // k  2A
      0x3818, 0x1818, 0x1818, 0x7E00, // l  2B
      0x0000, 0xFCB6, 0xB6B6, 0xB600, // m  2C
      0x0000, 0xFCC6, 0xC6C6, 0xC600, // n  2D
      0x0000, 0x7CC6, 0xC6C6, 0x7C00, // o  2E
      0x0000, 0xFCC6, 0xC6FC, 0xC0C0, // p  2F
      0x0000, 0x7EC6, 0xC67E, 0x0606, // q  30
      0x0000, 0xCE70, 0x6060, 0x6000, // r  31
      0x0000, 0x7CC0, 0x7C06, 0xFC00, // s  32
      0x1818, 0x7E18, 0x1818, 0x1800, // t  33
      0x0000, 0xC6C6, 0xC6C6, 0x7E00, // u  34
      0x0000, 0x6666, 0x663C, 0x1800, // v  35
      0x0000, 0xB6B6, 0xB6B6, 0x7E00, // w  36
      0x0000, 0xC6FE, 0x38FE, 0xC600, // x  37
      0x0000, 0xC6C6, 0xC67E, 0x067C, // y  38
      0x0000, 0xFE1C, 0x3870, 0xFE00, // z  39
      0x3838, 0x3830, 0x3000, 0x3000, // !  3A
      0x7C82, 0xBAAA, 0xBE80, 0x7C00, // @  3B
      0x6CFE, 0x6C6C, 0x6CFE, 0x6C00, // #  3C
      0x107C, 0xD07C, 0x16FC, 0x1000, // $  3D
      0x62A4, 0xC810, 0x264A, 0x8C00, // %  3E
      0x1028, 0x4400, 0x0000, 0x0000, // ^  3F
      0x70D8, 0xD870, 0xDACC, 0x7E00, // &  40
      0x006C, 0x38FE, 0x386C, 0x0000, // *  41
      0x0C18, 0x3030, 0x3018, 0x0C00, // (  42
      0x6030, 0x1818, 0x1830, 0x6000, // )  43
      0x0018, 0x187E, 0x1818, 0x0000, // +  44
      0x0000, 0x007E, 0x0000, 0x0000, // -  45
      0x0000, 0x0000, 0x0000, 0x00FE, // _  46
      0x0000, 0x0000, 0x0030, 0x3060, // ,  47
      0x0000, 0x0000, 0x0030, 0x3000, // .  48
      0x0204, 0x0810, 0x2040, 0x8000, // /  49
      0x7CC6, 0xCED6, 0xE6C6, 0x7C00, // 0  4A
      0x1838, 0x1818, 0x1818, 0x7E00, // 1  4B
      0x7CC6, 0x0E3C, 0x78E0, 0xFE00, // 2  4C
      0x7E0C, 0x183C, 0x06C6, 0x7C00, // 3  4D
      0x1C3C, 0x6CCC, 0xFE0C, 0x0C00, // 4  4E
      0xFCC0, 0xFC06, 0x06C6, 0x7C00, // 5  4F
      0x3C60, 0xC0FC, 0xC6C6, 0x7C00, // 6  50
      0xFEC6, 0x0C18, 0x3030, 0x3000, // 7  51
      0x78C4, 0xE478, 0x9E86, 0x7C00, // 8  52
      0x7CC6, 0xC67E, 0x060C, 0x7800, // 9  53
      0x7CFE, 0xC60C, 0x3800, 0x3800, // ?  54
      0x0C18, 0x1830, 0x1818, 0x0C00, // {  55
      0x1818, 0x1818, 0x1818, 0x1800, // |  56
      0x6030, 0x3018, 0x3030, 0x6000, // }  57
      0x3C30, 0x3030, 0x3030, 0x3C00, // [  58
      0x8040, 0x2010, 0x0804, 0x0200, // backslash  59
      0x7818, 0x1818, 0x1818, 0x7800, // ]  5A
      0x0000, 0x7E00, 0x7E00, 0x0000, // =  5B
  };

public:
  // ram 0x0000 to 0x1FE0 is VRAM every ram address is 4 pixles
  // each pixle is a 4 bit nibble, that number is used as an index in the color
  // pallet. the screen is 240 x 136 geometry
  uint16_t RAM[0xFFFF];
  uint16_t IP;     // instuction pointer
  uint16_t REG[8]; // regesters AX is reg[0], BX is reg[1] ...
  bool ZF;         // zero flag
  bool HALT;       // halt flag
  bool DRAW;       // draw flag

  JM() {
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

  int load(char *ROM) {
    FILE *fp = std::fopen(ROM, "r");
    if (!fp) {
      std::perror("Rom opening failed");
      return -1;
    }
    int offset = 0;
    int c; // note: int, not char, required to handle EOF
    c = std::fgetc(fp);
    while (c != EOF) {
      RAM[0x1FE1 + offset] = (c << 8);
      c = std::fgetc(fp);
      RAM[0x1FE1 + offset] += c;
      c = std::fgetc(fp);
      offset++;
    }

    std::fclose(fp);

    return 0;
  }

  void printMem(uint16_t loc) { printf("%04X %04X  ", loc, RAM[loc]); }

  void printRegs() {
    printf("\n%s %04X  ", "A", REG[0]);
    printf("%s %04X  ", "B", REG[1]);
    printf("%s %04X  ", "C", REG[2]);
    printf("%s %04X\n", "D", REG[3]);
    printf("%s %04X  ", "E", REG[4]);
    printf("%s %04X  ", "F", REG[5]);
    printf("%s %04X  ", "I", REG[6]);
    printf("%s %04X\n", "H", REG[7]);
  }

  void run() {
    if (HALT) {
      printf("HALT\n");
      return;
    }
    uint8_t OPCODE = (RAM[IP] & 0xFF00) >> 8;

    // printf("RAM[%04X]: %04X\n", IP, RAM[IP]);
    // printf("RAM[%04X]: %04X\n\n", IP + 1, RAM[IP + 1]);
    // printf("OPCODE : %02X\n", OPCODE);

    uint8_t R = RAM[IP] & 0xFF;
    uint16_t VALUE = RAM[IP + 1];
    switch (OPCODE) {
    case 0x00: // NOP
      IP += 1;
      break;
    case 0x01: // MTR
      switch (R % 3) {
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
    case 0x02: // RTM
      switch (R % 3) {
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
    case 0x03: // ITR
      switch (R % 3) {
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

    case 0x04: // ADDM
      switch (R % 3) {
      case 0:
        REG[R / 3] = REG[R / 3] + RAM[VALUE];
        break;
      case 1:
        REG[R / 3] = (REG[R / 3] & 0xFF) |
                     ((REG[R / 3] & 0xFF00) + ((RAM[VALUE]) & 0xFF00));
        break;
      case 2:
        REG[R / 3] = (REG[R / 3] & 0xFF00) |
                     ((REG[R / 3] & 0xFF) + ((RAM[VALUE]) & 0xFF));
        break;
      }
      flags(R);
      IP += 2;
      break;
    case 0x05: // ADDI
      switch (R % 3) {
      case 0:
        REG[R / 3] = REG[R / 3] + VALUE;
        break;
      case 1:
        REG[R / 3] =
            (REG[R / 3] & 0xFF) | ((REG[R / 3] & 0xFF00) + ((VALUE)&0xFF00));
        break;
      case 2:
        REG[R / 3] =
            (REG[R / 3] & 0xFF00) | ((REG[R / 3] & 0xFF) + ((VALUE)&0xFF));
        break;
      }
      flags(R);
      IP += 2;
      break;
    case 0x06: // SUBM
      switch (R % 3) {
      case 0:
        REG[R / 3] = REG[R / 3] - RAM[VALUE];
        break;
      case 1:
        REG[R / 3] = (REG[R / 3] & 0xFF) |
                     ((REG[R / 3] & 0xFF00) - ((RAM[VALUE]) & 0xFF00));
        break;
      case 2:
        REG[R / 3] = (REG[R / 3] & 0xFF00) |
                     ((REG[R / 3] & 0xFF) - ((RAM[VALUE]) & 0xFF));
        break;
      }
      flags(R);
      IP += 2;
      break;
    case 0x07: // SUBI
      switch (R % 3) {
      case 0:
        REG[R / 3] = REG[R / 3] - VALUE;
        break;
      case 1:
        REG[R / 3] =
            (REG[R / 3] & 0xFF) | ((REG[R / 3] & 0xFF00) - ((VALUE)&0xFF00));
        break;
      case 2:
        REG[R / 3] =
            (REG[R / 3] & 0xFF00) | ((REG[R / 3] & 0xFF) - ((VALUE)&0xFF));
        break;
      }
      flags(R);
      IP += 2;
      break;
    case 0x08: // SUBIND
      switch (R % 3) {
      case 0:
        if ((REG[R / 3] - VALUE) == 0)
          ZF = 1;
        else
          ZF = 0;
        break;
      case 1:
        if ((((REG[R / 3] & 0xFF00) - ((VALUE)&0xFF00))) == 0)
          ZF = 1;
        else
          ZF = 0;
        break;
      case 2:
        if ((((REG[R / 3] & 0xFF) - ((VALUE)&0xFF))) == 0)
          ZF = 1;
        else
          ZF = 0;
        break;
      }
      IP += 2;
      break;
    case 0x09: // AND
      switch (R % 3) {
      case 0:
        REG[R / 3] = REG[R / 3] & RAM[VALUE];
        break;
      case 1:
        REG[R / 3] = (REG[R / 3] & 0xFF) |
                     ((REG[R / 3] & 0xFF00) & ((RAM[VALUE]) & 0xFF00));
        break;
      case 2:
        REG[R / 3] = (REG[R / 3] & 0xFF00) |
                     ((REG[R / 3] & 0xFF) & ((RAM[VALUE]) & 0xFF));
        break;
      }
      flags(R);
      IP += 2;
      break;
    case 0x0A: // OR
      switch (R % 3) {
      case 0:
        REG[R / 3] = REG[R / 3] | RAM[VALUE];
        break;
      case 1:
        REG[R / 3] = (REG[R / 3] & 0xFF) |
                     ((REG[R / 3] & 0xFF00) | ((RAM[VALUE]) & 0xFF00));
        break;
      case 2:
        REG[R / 3] = (REG[R / 3] & 0xFF00) |
                     ((REG[R / 3] & 0xFF) | ((RAM[VALUE]) & 0xFF));
        break;
      }
      flags(R);
      IP += 2;
      break;
    case 0x0B: // XOR
      switch (R % 3) {
      case 0:
        REG[R / 3] = REG[R / 3] ^ RAM[VALUE];
        break;
      case 1:
        REG[R / 3] = (REG[R / 3] & 0xFF) |
                     ((REG[R / 3] & 0xFF00) ^ ((RAM[VALUE]) & 0xFF00));
        break;
      case 2:
        REG[R / 3] = (REG[R / 3] & 0xFF00) |
                     ((REG[R / 3] & 0xFF) ^ ((RAM[VALUE]) & 0xFF));
        break;
      }
      flags(R);
      IP += 2;
      break;
    case 0x0C: // NOT
      switch (R % 3) {
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
    case 0x0D: // SHL
      switch (R % 3) {
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
    case 0x0E: // SHL
      switch (R % 3) {
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
    case 0x0F: // JMP
      IP = VALUE;
      break;
    case 0x10: // JZ
      if (ZF)
        IP = VALUE;
      else
        IP += 2;
      break;
    case 0x11: // JNZ
      if (!ZF)
        IP = VALUE;
      else
        IP += 2;
      break;
    case 0x20: // CHAR
      moveChar(REG[R], VALUE);
      IP += 2;
      break;
    case 0xFE: // DRAW
      DRAW = true;
      IP += 1;
      break;
    case 0xFF: // HALT
      HALT = true;
      break;
    default:
      std::cout << "Invalid Opcode of ";
      printf("%X\n", OPCODE);
      IP += 1;
      break;
    }

    if (IP > 0xFFFF) {
      printf("IP too big\n");
      HALT = 1;
    }
  }

  void nibbletrgb(uint8_t (&color)[3], u_int8_t nibble) {
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
    uint8_t red[16] = {0x00, 0x44, 0x88, 0xbb, 0x99, 0x66, 0x00, 0x00,
                       0x00, 0x00, 0x33, 0xff, 0xdd, 0xff, 0xff, 0xff};
    uint8_t grn[16] = {0x00, 0x44, 0x88, 0xbb, 0x66, 0x33, 0x66, 0xaa,
                       0x99, 0x00, 0x00, 0x00, 0x00, 0x66, 0xff, 0xff};
    uint8_t blu[16] = {0x00, 0x44, 0x88, 0xbb, 0x33, 0x00, 0x00, 0x00,
                       0xff, 0xcc, 0x99, 0x99, 0x00, 0x00, 0x00, 0xff};
    color[0] = red[nibble];
    color[1] = grn[nibble];
    color[2] = blu[nibble];
  }
};
