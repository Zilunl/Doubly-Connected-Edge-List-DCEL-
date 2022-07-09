#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "tower.h"

/* get the whole line in csv file */
void get_tower(char* line, char* rest, struct tower* new_tower){

    char* read = strtok_r(line, ",", &rest);
    int index = 0;

    if (read == NULL){
        exit(EXIT_FAILURE);
    }

    while (read != NULL){
        copy_tower(read, new_tower, index);
        read = strtok_r(NULL, ",", &rest);
        index++;
    }
        
}

/* copy the information of each line */
void copy_tower(char* read, struct tower *new_tower, int index){

    if (index == 0){
        new_tower->watchtower_ID = malloc(sizeof(char)*129);
        assert(new_tower->watchtower_ID);
        strcpy(new_tower->watchtower_ID, read);
    }
    if (index == 1){
        new_tower->postcode = malloc(sizeof(char)*129);
        assert(new_tower->postcode);
        strcpy(new_tower->postcode, read);
    }
    if (index == 2){
        new_tower->population=atoi(read);
    }
    if (index == 3){
        new_tower->contname_tower = malloc(sizeof(char)*129);
        assert(new_tower->contname_tower);
        strcpy(new_tower->contname_tower, read);
    }
    if (index == 4){
        char *end;
        new_tower->tower_x = strtod(read, &end);
    }
    if (index == 5){
        char *end;
        new_tower->tower_y = strtod(read, &end);
    }

}
