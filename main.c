#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fin, *fout;

int main(int argc, char** argv){
    char outfile_name[strlen(argv[1]) + 5];
    memset(outfile_name, '\0', strlen(outfile_name));
    strncpy(outfile_name, argv[1], strlen(argv[1])-6);
    strcat(outfile_name, ".output");
    if(!(fin = fopen(argv[1], "r")) || !(fout = fopen(outfile_name, "w"))){
        printf("Open file error!");
        exit(0);
    }

    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fin)) != -1){
        printf("%s", line);
    }  

    fclose(fin);
    fclose(fout);
}