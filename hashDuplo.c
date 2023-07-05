#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "hashDuplo.h"
#define SEED 0x12345678
#define SEED2 0x98765432

char* get_key(void* reg) {
    return ((tmunicipio*)reg)->cod_ibge;
}

void *aloca_muni(char *cod_ibge, char *nome, double coord0, double coord1, int capital, int codigo_uf, char *siafi_id, int ddd, char *fuso_horario){
	tmunicipio *municipios;
	municipios = (tmunicipio* ) malloc(sizeof(tmunicipio));

	strcpy(municipios->cod_ibge, cod_ibge);
	strcpy(municipios->nome ,nome);
	municipios->coord[0] = coord0;
	municipios->coord[1] = coord1; 
	municipios->capital = capital;
	municipios->codigo_uf = codigo_uf;
	strcpy(municipios->siafi_id, siafi_id);
	municipios->ddd = ddd;
	strcpy(municipios->fuso_horario, fuso_horario);

	if(municipios->codigo_uf == 11){
		strcpy(municipios->uf,"RO");
		strcpy(municipios->regiao,"Norte");
	}
	else if(municipios->codigo_uf == 12){
		strcpy(municipios->uf,"AC");
		strcpy(municipios->regiao,"Norte");
	}
	else if(municipios->codigo_uf == 13){
		strcpy(municipios->uf,"AM");
		strcpy(municipios->regiao,"Norte");
	}
	else if(municipios->codigo_uf == 14){
		strcpy(municipios->uf,"RR");
		strcpy(municipios->regiao,"Norte");
	}
	else if(municipios->codigo_uf == 15){
		strcpy(municipios->uf,"PA");
		strcpy(municipios->regiao,"Norte");
	}
	else if(municipios->codigo_uf == 16){
		strcpy(municipios->uf,"AP");
		strcpy(municipios->regiao,"Norte");
	}
	else if(municipios->codigo_uf == 17){
		strcpy(municipios->uf,"TO");
		strcpy(municipios->regiao,"Norte");
	}
	else if(municipios->codigo_uf == 21){
		strcpy(municipios->uf,"MA");
		strcpy(municipios->regiao,"Nordeste");
	}
	else if(municipios->codigo_uf == 22){
		strcpy(municipios->uf,"PI");
		strcpy(municipios->regiao,"Nordeste");
	}
	else if(municipios->codigo_uf == 23){
		strcpy(municipios->uf,"CE");
		strcpy(municipios->regiao,"Nordeste");
	}
	else if(municipios->codigo_uf == 24){
		strcpy(municipios->uf,"RN");
		strcpy(municipios->regiao,"Nordeste");
	}
	else if(municipios->codigo_uf == 25){
		strcpy(municipios->uf,"PB");
		strcpy(municipios->regiao,"Nordeste");
	}
	else if(municipios->codigo_uf == 26){
		strcpy(municipios->uf,"PE");
		strcpy(municipios->regiao,"Nordeste");
	}
	else if(municipios->codigo_uf == 27){
		strcpy(municipios->uf,"AL");
		strcpy(municipios->regiao,"Nordeste");
	}
	else if(municipios->codigo_uf == 28){
		strcpy(municipios->uf,"SE");
		strcpy(municipios->regiao,"Nordeste");
	}
	else if(municipios->codigo_uf == 29){
		strcpy(municipios->uf,"BA");
		strcpy(municipios->regiao,"Nordeste");
	}
	else if(municipios->codigo_uf == 31){
		strcpy(municipios->uf,"MG");
		strcpy(municipios->regiao,"Sudeste");
	}
	else if(municipios->codigo_uf == 32){
		strcpy(municipios->uf,"ES");
		strcpy(municipios->regiao,"Sudeste");
	}
	else if(municipios->codigo_uf == 33){
		strcpy(municipios->uf,"RJ");
		strcpy(municipios->regiao,"Sudeste");
	}
	else if(municipios->codigo_uf == 35){
		strcpy(municipios->uf,"SP");
		strcpy(municipios->regiao,"Sudeste");
	}
	else if(municipios->codigo_uf == 41){
		strcpy(municipios->uf,"PR");
		strcpy(municipios->regiao,"Sul");
	}
	else if(municipios->codigo_uf == 42){
		strcpy(municipios->uf,"SC");
		strcpy(municipios->regiao,"Sul");
	}
	else if(municipios->codigo_uf == 43){
		strcpy(municipios->uf,"RS");
		strcpy(municipios->regiao,"Sul");
	}
	else if(municipios->codigo_uf == 50){
		strcpy(municipios->uf,"MS");
		strcpy(municipios->regiao,"Centro-Oeste");
	}
	else if(municipios->codigo_uf == 51){
		strcpy(municipios->uf,"MT");
		strcpy(municipios->regiao,"Centro-Oeste");
	}
	else if(municipios->codigo_uf == 52){
		strcpy(municipios->uf,"GO");
		strcpy(municipios->regiao,"Centro-Oeste");
	}
	else if(municipios->codigo_uf == 53){
		strcpy(municipios->uf,"DF");
		strcpy(municipios->regiao,"Centro-Oeste");
	}

	return municipios;
}

uint32_t hashf(const char* str, uint32_t h){
    /* One-byte-at-a-time Murmur hash 
    Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp */
    for (; *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

int hash_insere(thash *h, void *bucket) {
    uint32_t hash = hashf(h->get_key(bucket), SEED);
    uint32_t hash2 = hashf(h->get_key(bucket), SEED2);
    int pos = hash % (h->max);
    int step = 1;

    if (h->max == (h->size + 1)) {
        free(bucket);
        return EXIT_FAILURE;
    } else {
        while (h->table[pos] != 0) {
            if (h->table[pos] == h->deleted)
                break;

            pos = (pos + step++ * (hash2 % h->max)) % h->max;

            if(step >= h->max){
                free(bucket);
                return EXIT_FAILURE;    
            }
        }
        h->table[pos] = (uintptr_t) bucket;
        h->size +=1;
    }
    return EXIT_SUCCESS;
}


int hash_constroi(thash * h,int nbuckets, char * (*get_key)(void *) ){
    h->table = calloc(sizeof(uintptr_t),nbuckets +1);
    if (h->table == NULL){
        return EXIT_FAILURE;
    }
    h->max = nbuckets +1;
    h->size = 0;
    h->deleted = (uintptr_t) & (h->size);
    h->get_key = get_key;
    return EXIT_SUCCESS;
}


void* hash_busca(thash h, const char* key) {
    uint32_t hash = hashf(key, SEED);
    uint32_t hash2 = hashf(key, SEED2);
    int pos = hash % (h.max);
    int step = 0;
    while(h.table[pos] != 0){
        if (strcmp (h.get_key((void*)h.table[pos]),key) == 0)
            return (void *)h.table[pos];
        else
            pos = (pos + step++ * (hash2 % h.max)) % h.max;
    }
    return NULL;
}

int hash_remove(thash* h, const char* key) {
    uint32_t hash = hashf(key, SEED);
    uint32_t hash2 = hashf(key, SEED2);
    int pos = hash % (h->max);
    int step = 0;
    while(h->table[pos]!=0){
        if (strcmp (h->get_key((void*)h->table[pos]),key) ==0){
            free((void *) h->table[pos]);
            h->table[pos] = h->deleted;
            h->size -=1;
            return 1;
        }else{
            pos = (pos + step++ * (hash2 % h->max)) % h->max;
        }

    }
    return 0;
}

void hash_apaga(thash* h) {
    int pos;
    for(pos =0;pos< h->max;pos++){
        if (h->table[pos] != 0){
            if (h->table[pos]!=h->deleted){
                free((void *)h->table[pos]);
            }
        }
    }
    free(h->table);
}
