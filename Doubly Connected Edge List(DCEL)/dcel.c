#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "dcel.h"

/* create an empty cycle linkedlist*/
HALF *create(){

    HALF *head = (HALF*)malloc(sizeof(HALF));
    assert(head!=NULL);
    head->edgeIndex = -1;
    head->faceIndex = -1;
    head->startVertex = -1;
    head->endVertex = -1;

    head->prev = head;
    head->next = head;
    head -> pair = NULL;

    return head;
}

/* create new edges which is in the split edges */
HALF *create_edge(int edge, int face, int start, int end, int pair){

    HALF *each_edge = (HALF*)malloc(sizeof(HALF));
    assert(each_edge);
    each_edge -> edgeIndex = edge;
    each_edge -> faceIndex = face;
    each_edge -> startVertex = start;
    each_edge -> endVertex = end;

    if (pair != 0){
        HALF *twins = (HALF*)malloc(sizeof(HALF));
        assert(twins);
        twins -> edgeIndex = edge;
        twins -> faceIndex = face+1;
        twins -> startVertex = end;
        twins -> endVertex = start;
        twins -> pair = each_edge;
        each_edge -> pair = twins;
    }

    return each_edge;
}

/* use edge_index to find the edge in the cycle linkedlist*/
HALF *find_edge(HALF *head, int edge_index){

    HALF *p = head;
    while (p->edgeIndex != edge_index){
       p = p->next;
    }
    return p;

}

vertex_t *set_vertex(double x, double y, struct vertex *new_vertex){

    new_vertex -> x = x;
    new_vertex -> y = y;
    return new_vertex;

}

/* find with face will be split */
int find_face(HALF **all_faces, int start_edge, int end_edge, int face_num){

    for (int i=0; i<face_num; i++){
        int num = 0;
        HALF *p = all_faces[i];
        if ((p->edgeIndex == start_edge) || (p->edgeIndex == end_edge)){
            num++;
        }
        p = p->next;
        while (p != all_faces[i]){
            if ((p->edgeIndex == start_edge) || (p->edgeIndex == end_edge)){
                num++;
            }
            p = p->next;
        }
        if (num == 2){
            return i;
        }        
    }
    return -1;
}

void insert(HALF *head, HALF *orig){

    if (head != NULL){
        HALF *p = head;
        while (p->next != head){
            p = p->next;
        }
        orig->prev = p;
        head->prev = orig;
        p->next = orig;
        orig->next = head;
    }
    else{
        printf("Error!\n");
    }
}

/* this function is use to insert the new create edge of two mid points,
   and set it and its pair to correct point */
void insert_edge(HALF *head, HALF *new_edge, int start_index, int end_index, int edge){
    
    HALF *start1 = find_edge(head, start_index);
    HALF *end1 = find_edge(head, end_index);

    HALF *start2 = find_edge(head, edge+2);
    HALF *end2 = find_edge(head, edge+3);

    new_edge->prev=start1;
    new_edge->next=end1;  
    (new_edge->pair)->next = start2;
    (new_edge->pair)->prev = end2;  

    start1->next = new_edge;
    end1->prev = new_edge;
    start2->prev = new_edge->pair;
    end2->next = new_edge->pair;

}

/* connect with new and old edges of first split and set them to correct position */
void first_update(int edge, HALF *head, int index_edge, int start_index, int mid, int end_index, int face){

    HALF *p = find_edge(head, index_edge);
    p->endVertex = mid;

    HALF *end = create_edge(edge, face, mid, end_index, 0);
    end->prev = p;
    end->next = p->next;
    p->next->prev = end;
    p->next = end;

}

/* connect with new and old edges of end split and set them to correct position */
void end_update(int edge, HALF *head, int index_edge, int start_index, int mid, int end_index, int face){
   
    HALF *p = find_edge(head, index_edge);   
    p->startVertex = mid;  

    HALF *start = create_edge(edge, face, start_index, mid, 0);
    
    start->prev = p->prev;
    start->next = p;
    p->prev->next = start;
    p->prev = start;

}

