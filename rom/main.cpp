/* take a machine code program in hex
 * and create a bin file for loading in the machine
 *
 *
 */

#include <iostream>
#include <string>
#include <iterator>
#include <cstdlib>

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage ./main [OUT FILE]\n");
    return 1;
  }

  std::string program;
  program = "0500BCDE02000000FE000503000102031FE401061FE407061FE011001FE3FF00"; // draw pixles redraw on each one
  //program = "0500BCDE020000000503000102031FE401061FE407061FE011001FE3FEFFFF00"; // draw pixles redraw at end

  std::string byte = "  ";
  FILE *pFile;
  int n;
  pFile = fopen(argv[1], "w");
  //for (int i = 0; i < std::size(program); i += 2)
  for (int i = 0; i < (program.length()); i += 2)
  {
    byte[0] = program[i];
    byte[1] = program[i + 1];
    n = std::stoi(byte, nullptr, 16);
    fputc(n, pFile);
    //printf("%c",n);
  }
  fclose(pFile);

  return 0;
}
