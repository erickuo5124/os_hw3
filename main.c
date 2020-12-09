#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

FILE *fin, *fout;

char *get_filename(char *);
void act_put(uint64_t, char*);
void act_get(uint64_t);
void act_scan(uint64_t, uint64_t);

struct stat st = {0};

int main(int argc, char** argv){
    char *outfile_name;
    outfile_name = get_filename(argv[1]);
    if(!(fin = fopen(argv[1], "r")) || !(fout = fopen(outfile_name, "w"))){
        printf("Open output file error!");
        exit(0);
    }
    if(stat("./stroage", &st) == -1){
    	mkdir("./stroage", 0700);
    }

    char command[5];
    while(fscanf(fin, "%s", command) != EOF){
        // printf("%s ", command);
        if(strcmp(command, "PUT") == 0){
            uint64_t key;
            char value[129];
            memset(value, '\0', sizeof(value));
            fscanf(fin, "%lu %s", &key, value);
            // printf("%lu %s\n", key, value);
            act_put(key, value);
        } else if(strcmp(command, "GET") == 0){
            uint64_t key;
            fscanf(fin, "%lu", &key);
            // printf("%lu\n", key);
            act_get(key);
        } else if(strcmp(command, "SCAN") == 0){
            uint64_t key1, key2;
            fscanf(fin, "%lu %lu", &key1, &key2);
            // printf("%lu %lu\n", key1, key2);
            act_scan(key1, key2);
        }
    }

    fclose(fin);
    fclose(fout);
}

char *get_filename(char *filepath){
    char *filename = malloc(sizeof(char) * 100);
    printf("%s\n", filepath);
    memset(filename, '\0', strlen(filename));
    int i = strlen(filepath) - 1;
    for(; i >= 0; --i){
        if(filepath[i] == '/'){
            ++i;
            strncat(filename, filepath + i, strlen(filepath) - i - 6);
            break;
        }
    }
    if(i < 0) strncat(filename, filepath, strlen(filepath) - 6);
    strcat(filename, ".output");
    return filename;
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
