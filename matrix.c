#include "matrix.h"
#include "msg.h"

#include <stdlib.h>
#include <stdio.h>

#define MATRIX__DEBUG_MSG 1
#define MATRIX__ERROR_MSG 1


struct float_matrix {
    float **data;
    int size_x;
    int size_y;
};

float_matrix *float_matrix__new(int x, int y) {
    float_matrix *m = (float_matrix *)malloc(sizeof(*m));
    if (m==NULL) {
        perror("malloc");
        return NULL;
    }
    m->size_x = x;
    m->size_y = y;
    m->data = (float **)malloc(sizeof(*(m->data)) * x);
    if (m->data == NULL) {
        perror("malloc");
        free(m);
        return NULL;
    }
    int i, j;
    for (i=0; i<x; i++) {
        m->data[i] = (float*) malloc(sizeof(**(m->data)) * y);
        if (m->data[i] == NULL) {
            perror("malloc");
            for (i--; i>=0; i--) {
                free(m->data[i]);
            }
            free(m->data);
            free(m);
            return NULL;
        }
        for (j=0; j<y; j++) {
            m->data[i][j] = 0;
        }
    }
    return m;
}

void float_matrix__free(float_matrix *m) {
    int i;
    for (i=0; i < m->size_x; i++) {
        free(m->data[i]);
    }
    free(m->data);
    free(m);
}

void float_matrix__init(float_matrix *m, float value) {
    int i, j;
    for (i=0; i < m->size_x; i++) {
        for (j=0; j < m->size_y; j++) {
            m->data[i][j] = value;
        }
    }
}

float float_matrix__get(float_matrix *m, int x, int y) {
    if (x>=0 && x < m->size_x && y>=0 && y < m->size_y) {
        return m->data[x][y];
    }
    return -1;
}

int float_matrix__set(float_matrix *m, int x, int y, float value) {
    if (x>=0 && x < m->size_x && y>=0 && y < m->size_y) {
        m->data[x][y] = value;
        return 0;
    }
    return -1;
}

int float_matrix__size_x(float_matrix *m) {
    return m->size_x;
}
int float_matrix__size_y(float_matrix *m) {
    return m->size_y;
}

int float_matrix__copy(float_matrix *d, float_matrix *s) {
    if (s->size_x != d->size_x || s->size_y != d->size_y){
        ERROR_MSG(MATRIX, "sizes mismatching during copy.")
        return -1;
    }

    int i, j;
    for (i=0; i < s->size_x; i++) {
        for (j=0; j < s->size_y; j++) {
            d->data[i][j] = s->data[i][j];
        }
    }

    return 0;
}

void float_matrix__debug_print(float_matrix *m) {
    int i, j;
    for (i=0; i<float_matrix__size_x(m); i++) {
        for (j=0; j<float_matrix__size_y(m); j++) {
            printf("%.2f ", float_matrix__get(m, i, j));
        }
        printf("\n");
    }
}

int float_matrix__save(float_matrix *m, char *file_path) {
    FILE *file;
    file = fopen(file_path,"wb");
    if (file == NULL){
        ERROR_MSG(MATRIX, "opening file %s for writing.", file_path)
        return 1;
    }

    fwrite(&(m->size_x), sizeof(int), 1, file);
    fwrite(&(m->size_y), sizeof(int), 1, file);

    int i;
    for (i=0; i < m->size_x; i++) {
        fwrite((m->data[i]), sizeof(float), m->size_y, file);
    }
    fclose(file);

    DEBUG_MSG(MATRIX, "file %s written successfuly.", file_path);

    return 0;
}


float_matrix *float_matrix__open(char *file_path) {
    FILE *file;
    file = fopen(file_path,"rb");
    if (NULL == file){
        ERROR_MSG(MATRIX, "opening file %s for reading.", file_path)
        return NULL;
    }

    int size_x, size_y;
    fread(&size_x, sizeof(int), 1, file);
    fread(&size_y, sizeof(int), 1, file);

    float_matrix *m = float_matrix__new(size_x, size_y);
    if (NULL == m){
        ERROR_MSG(MATRIX, "new matrix error during float_matrix__open.")
        fclose(file);
        return NULL;
    }

    int i;
    for (i=0; i < m->size_x; i++) {
        fread(m->data[i], sizeof(float), m->size_y, file);
    }

    fclose(file);

    DEBUG_MSG(MATRIX, "file %s read successfuly.", file_path)

    return m;
}


