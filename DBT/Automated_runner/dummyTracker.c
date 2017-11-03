#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
  if(argc == 5){
    FILE* outfile = fopen(argv[1],"w");
    if(outfile == NULL){
      printf("NO such file\n");
    }
    fprintf(outfile, "%s %s %s\n", argv[2], argv[3], argv[4]);
    fclose(outfile);
  }
  return 0;
}
