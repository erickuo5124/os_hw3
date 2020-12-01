#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

FILE *fin, *fout;

void act_put(uint64_t, char*);
void act_get(uint64_t);
void act_scan(uint64_t, uint64_t);

int main(int argc, char** argv){
    char outfile_name[strlen(argv[1]) + 10];
    memset(outfile_name, '\0', strlen(outfile_name));
    strncat(outfile_name, argv[1], strlen(argv[1])-6);
    strcat(outfile_name, ".output");
    if(!(fin = fopen(argv[1], "r")) || !(fout = fopen(outfile_name, "w"))){
        printf("Open output file error!");
        exit(0);
    }

    char command[5];
    while(fscanf(fin, "%s", command) != EOF){
        printf("%s ", command);
        if(strcmp(command, "PUT") == 0){
            uint64_t key;
            char value[129];
            memset(value, '\0', sizeof(value));
            fscanf(fin, "%lu %s", &key, value);
            printf("%lu %s\n", key, value);
            act_put(key, value);
        } else if(strcmp(command, "GET") == 0){
            uint64_t key;
            fscanf(fin, "%lu", &key);
            printf("%lu\n", key);
            act_get(key);
        } else if(strcmp(command, "SCAN") == 0){
            uint64_t key1, key2;
            fscanf(fin, "%lu %lu", &key1, &key2);
            printf("%lu %lu\n", key1, key2);
            act_scan(key1, key2);
        }
    }

    fclose(fin);
    fclose(fout);
}

void act_put(uint64_t key, char value[129]){
    uint32_t arr_num = key >> 32;
    uint32_t index = key & 0xffffffff;

    char filepath[25] = "./stroage/";
    sprintf(filepath + 10, "%u", arr_num);
    strcat(filepath, ".tmp");

    FILE *stroage;
    if(!(stroage = fopen(filepath, "rb+"))){
        if(!(stroage = fopen(filepath, "wb"))){
            printf("Open tmp file error!\n");
            exit(0);
        }
    }
    fseek(stroage, index * 128, SEEK_SET);
    fwrite(value, sizeof(char), 128, stroage);
    fclose(stroage);
}

void act_get(uint64_t key){
    uint32_t arr_num = key >> 32;
    uint32_t index = key & 0xffffffff;
    
    char filepath[25] = "./stroage/";
    sprintf(filepath + 10, "%u", arr_num);
    strcat(filepath, ".tmp");

    FILE *stroage;
    if(!(stroage = fopen(filepath, "rb"))){
        fprintf(fout, "EMPTY\n");
    } else {
        char value[129];
        memset(value, '\0', sizeof(value));
        fseek(stroage, index * 128, SEEK_SET);
        fread(value, sizeof(char), 128, stroage);
        fprintf(fout, "%s\n", value[0] == '\0' ? "EMPTY" : value);
        fclose(stroage);
    }
}

void act_scan(uint64_t key1, uint64_t key2){
    for(;key1 <= key2; ++key1)
        act_get(key1);
}