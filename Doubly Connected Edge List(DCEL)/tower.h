struct tower{
    char* watchtower_ID;
    char* postcode;
    int population;
    char* contname_tower;
    double tower_x;
    double tower_y;
};

typedef struct tower tower_t;

void get_tower(char* line, char* rest, struct tower *new_tower);
void copy_tower(char* read, struct tower *new_tower, int index);


