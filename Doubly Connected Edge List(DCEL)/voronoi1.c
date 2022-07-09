#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "tower.h"
#include "dcel.h"

#define MID 2 //mid point of each edge
#define EXIT_FAILURE 1

/* It looks good of some output when I used in terminal, 
but I do not know why I can not Mark:(, please use terminal to check */

int main(int argc, char *argv[]) {

    /* open and read the first csv file */
    char *csv_file = argv[1];
    FILE* input = fopen(csv_file, "r");
    if (input == NULL){
        printf("Can't read the first file\n");
        exit(EXIT_FAILURE);
    }

    char* line = NULL;
    char* rest = NULL;
    size_t lineBufferLength = 0;
    /* get the title line of the csv file */
    getline(&line, &lineBufferLength, input);
    int text_size;
    text_size = getline(&line, &lineBufferLength, input);

    int towernum=0;
    tower_t **towers;
    towers = (tower_t**)malloc(sizeof(tower_t)*(towernum+1));
    assert(towers);
   
    /* get and store every element in each line */  
    while(text_size > 0){
        
        struct tower* new_tower = malloc(sizeof(struct tower));
        if (new_tower == NULL){
            printf("Can't read the file\n");
            exit(EXIT_FAILURE);
        }
        get_tower(line, rest, new_tower);
        towers[towernum] = new_tower;
        towernum++;
        text_size = getline(&line, &lineBufferLength, input);
        towers = realloc(towers, sizeof(tower_t)*(towernum+1));
        assert(towers);

    }
    fclose(input);    
    
    /* open and read the second initial polygon file */
    char *origin_poly = argv[2];
    FILE *fp = fopen(origin_poly, "r");
    if (fp == NULL){
        printf("Can't read the second file\n");
        exit(EXIT_FAILURE);
    }

    HALF *origHead = create();
    int vertex_count = 0;
    vertex_t **vertex_array = (vertex_t**)malloc(sizeof(vertex_t)*(vertex_count+1));
    assert(vertex_array);

    HALF *original;
    double initial_x, initial_y;
    while (fscanf(fp, "%lf %lf", &initial_x, &initial_y) ==2){
   
        struct vertex *vertex = (vertex_t*)malloc(sizeof(struct vertex));
        assert(vertex);

        /* set new vertex */
        vertex_array[vertex_count] = set_vertex(initial_x, initial_y, vertex);     
        vertex_array = realloc(vertex_array, sizeof(vertex_t)*(vertex_count+1));
        assert(vertex_array);

        /* create new edge */
        original = create_edge(vertex_count, 0, vertex_count, vertex_count+1, 0);

        vertex_count++;
        insert(origHead, original);
        
    }

    fclose(fp);

    int edge = original->edgeIndex;

    /* delete the empty one */
    HALF *last_edge = find_edge(original, edge);
    last_edge -> endVertex = 0;
    HALF *first_start = find_edge(original, 0); // find edge0
    HALF *first_final = find_edge(original, edge); // find the last edge of initial polygon

    first_start->prev = first_final;
    first_final->next = first_start;

    int face=1;

    HALF **all_faces;
    all_faces = (HALF**)malloc(sizeof(HALF)*(face+1));
    assert(all_faces);
    all_faces[0] = original;
    all_faces = realloc(all_faces, sizeof(vertex_t)*(face+2));
    assert(all_faces);
    
    /* read data from stdin and split faces */
    int first_split, end_split;
    while (scanf("%d %d\n", &first_split, &end_split) == 2){
        int old_face = find_face(all_faces, first_split, end_split, face);

        /* find tow split edges */
        HALF *first_edge, *end_edge;
        first_edge = find_edge(original, first_split);
        end_edge = find_edge(original, end_split);
        
        /* find start and end vertexes of two split edges */
        int first_startindex, fisrt_endindex, end_startindex, end_endindex;
        first_startindex = first_edge->startVertex;
        fisrt_endindex = first_edge->endVertex;
        end_startindex = end_edge->startVertex;
        end_endindex = end_edge->endVertex;

        /* set two new vertexes in the mid of split edges */
        double first_startx, first_starty;
        first_startx = (vertex_array[first_startindex]->x + vertex_array[fisrt_endindex]->x)/MID;
        first_starty = (vertex_array[first_startindex]->y + vertex_array[fisrt_endindex]->y)/MID;

        double end_startx, end_starty;
        end_startx = (vertex_array[end_startindex]->x + vertex_array[end_endindex]->x)/MID;
        end_starty = (vertex_array[end_startindex]->y + vertex_array[end_endindex]->y)/MID;

        struct vertex *vertex = (vertex_t*)malloc(sizeof(struct vertex));
        assert(vertex);

        /* set new vertexes */
        int new_start, new_end;
        new_start = vertex_count;
        vertex_array[new_start] = set_vertex(first_startx, first_starty, vertex);
        vertex_count++;

        vertex = (vertex_t*)malloc(sizeof(struct vertex));
        assert(vertex);

        new_end = vertex_count;
        vertex_array[new_end] = set_vertex(end_startx, end_starty, vertex);
        vertex_count++;

        vertex_array = realloc(vertex_array, sizeof(vertex_t)*(vertex_count+2));

        /* update the information of mid edges */
        first_update(edge+2, original, first_split, first_startindex, new_start, fisrt_endindex, 0);      
        end_update(edge+3, original, end_split, end_startindex, new_end, end_endindex, 0);

        /* create new edge created by the mid */
        HALF* new_edge = create_edge(edge+1, face, new_start, new_end, 1);
        insert_edge(original, new_edge, first_split, end_split, edge);
        
        HALF* temp = find_edge(original, edge+1)->next;

        /* update the face information of each face and store them */
        while(temp->edgeIndex != new_edge->edgeIndex){      
            temp->faceIndex = old_face;
            temp = temp->next;
        }
        
        temp->faceIndex = old_face;
        all_faces[old_face] = temp;
        
        temp = find_edge(original, edge+1)->pair->next;
        while(temp->edgeIndex != new_edge->pair->edgeIndex){
            temp->faceIndex = face;
            temp = temp->next;
        }
        
        temp->faceIndex = face;
        all_faces[face] = temp;

        all_faces = realloc(all_faces, sizeof(vertex_t)*(face+2));

        edge=edge+3;
        face++;

    }

    /* judge if the tower int the face and write the infomation */
    char *output_file = argv[3];
    FILE *fp_write = fopen(output_file , "w");
    if (fp_write == NULL){
        printf("Can't read the file\n");
        exit(EXIT_FAILURE);
    }

    int face_popu[face]; // an array which store population of each face
    int totallen = 0;
    for (int i=0; i<face; i++){
        
        fprintf(fp_write, "%d\n", i);
        int start_index,end_index;
        totallen++;
        
        int population=0;
        /* find if any town in the face */
        for (int k = 0; k<towernum; k++){
            int yes = 0;
            double result1 = 0, result2 = 0, result3 = 0;

            /* check if the tower in which side of the edge */
            HALF *p=all_faces[i];
            start_index = p->startVertex;
            end_index = p->endVertex;
            result1 = (towers[k]->tower_y - vertex_array[start_index]->y) * 
                (vertex_array[end_index]->x - vertex_array[start_index]->x);
            result2 = (towers[k]->tower_x - vertex_array[start_index]->x) * 
                (vertex_array[end_index]->y - vertex_array[start_index]->y);
            result3 = result1 - result2;

            /* if the tower in the face */
            if(result3 >= 0){
                yes = 1;
            }
            p = p->next;

            /* same cheack with above just check if it is in the right of all edges */
            while (p!=all_faces[i]){
                start_index = p->startVertex;
                end_index = p->endVertex;
                result1 = (towers[k]->tower_y - vertex_array[start_index]->y) * 
                    (vertex_array[end_index]->x - vertex_array[start_index]->x);
                result2 = (towers[k]->tower_x - vertex_array[start_index]->x) * 
                    (vertex_array[end_index]->y - vertex_array[start_index]->y);
                result3 = result1 - result2;

                if(result3 >= 0){
                    yes = 1;
                }
                p = p->next;
            }

            /* wirte the information if the tower in the face */
            if (yes == 0){
                fprintf(fp_write, "Watchtower ID: %s", towers[k]->watchtower_ID);
                fprintf(fp_write, ", Postcode:  %s", towers[k]->postcode);
                fprintf(fp_write, ", Watchtower ID: %d", towers[k]->population);
                fprintf(fp_write, ", Watchtower ID: %s", towers[k]->contname_tower);
                fprintf(fp_write, ", Watchtower ID: %.7lf", towers[k]->tower_x);
                fprintf(fp_write, ", Watchtower ID: %.8lf\n", towers[k]->tower_y);
                totallen++;
                population = population+towers[k]->population;
            }

        }
        face_popu[i] = population;
    }

    /* write the population of each face to the output file*/
    for (int i=0; i<face; i++){
        fprintf(fp_write, "Face %d population served: %d\n", i, face_popu[i]);
        totallen++;
    }
    fclose(fp_write);

    FILE *fp_read = fopen(origin_poly, "r");
    if (fp_read == NULL){
        printf("Can't read the file\n");
        exit(EXIT_FAILURE);
    }

    /* read all things in the output file */
    char buf[totallen-1];
    int length;
    while(fgets(buf, totallen-1, fp_read)!=NULL){
        length = strlen(buf);
        buf[length-1] = '\0';
    }
    fclose(fp_read);

    /* free memory */
    for (int i=0; i<towernum; i++){
        free(towers[i]);
    }

    for (int i=0; i<vertex_count; i++){
        free(vertex_array[i]);
    }

    for (int i=0; i<face; i++){
        free(all_faces[i]);
    }

    free(origHead);


}
