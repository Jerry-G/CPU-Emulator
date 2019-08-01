/* take a machine code program in hex
 * and create a bin file for loading in the machine
 *
 *
 */

#include <iostream>
#include <string>
#include <iterator>
#include <cstdlib>


int main(int argc, char* argv[]){
  if(argc != 2){
    printf("Usage ./main [OUT FILE]\n");
    return 1;
  }

  std::string program;
  program = "03005555070000010700000106001FEAFFFF0002";

  std::string byte = "  ";
  FILE * pFile;
  int n;
  pFile = fopen (argv[1],"w");
  for(int i = 0; i < std::size(program); i+=2){
    byte[0] = program[i];
    byte[1] = program[i+1];
    n = std::stoi(byte,nullptr,16);
    fputc(n,pFile);
    //printf("%c",n);
  }
  fclose (pFile);

  return 0;
}
