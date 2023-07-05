#include<stdint.h>
#define MIN 100

typedef struct {
    char cod_ibge[9];
    char nome[MIN];
    double coord[2];
    int capital;
    int codigo_uf;
    char siafi_id[5];
    int ddd;
    char fuso_horario[MIN];
    char regiao[15];
    char uf[3];
} tmunicipio;

char *get_key(void* reg);

void *aloca_muni(char *cod_ibge, char *nome, double coord0, double coord1, int capital, int codigo_uf, char *siafi_id, int ddd, char *fuso_horario);

typedef struct {
    uintptr_t * table;
    int size;
    int max;
    uintptr_t deleted;
    char * (*get_key)(void *);
}thash;

uint32_t hashf(const char* str, uint32_t h);

int hash_insere(thash *h, void *bucket);

int hash_constroi(thash *h,int nbuckets, char * (*get_key)(void *) );

void *hash_busca(thash h, const char* key);

int hash_remove(thash *h, const char* key);

void hash_apaga(thash *h);
