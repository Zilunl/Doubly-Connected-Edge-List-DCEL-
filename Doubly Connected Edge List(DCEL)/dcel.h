struct halfEdge {
    int startVertex,        // index of the vertex at the start of the half-edge
        endVertex;          // index of the vertex at the end of the half-edge
    struct halfEdge *prev,  // pointer to the previous half-edge in the face
                    *next,  // pointer to the next half-edge in the face
                    *pair;  // pointer to the other half-edge (this edge runs in the opposite direction)
    int edgeIndex,               // index of its corresponding face
        faceIndex;               // index of the edge it forms a part of
};

typedef struct halfEdge HALF;

typedef struct vertex {
    double x,  // x-position
           y;  // y-position
}vertex_t;

HALF *create();
HALF *create_edge(int edge, int face, int start, int end, int pair);
HALF *find_edge(HALF *head, int edge_index);
vertex_t *set_vertex(double x, double y, struct vertex *new_vertex);

int find_face(HALF **all_faces, int start_edge, int end_edge, int face_num);
void insert(HALF *head, HALF *orig);
void insert_edge(HALF *head, HALF *new_edge, int start_index, int end_index, int edge);
void first_update(int edge, HALF *head, int index_edge, int start_index, int mid, int end_index, int face);
void end_update(int edge, HALF *head, int index_edge, int start_index, int mid, int end_index, int face);


