#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "hashDuplo.h"

int main() {
    FILE *file;
    tmunicipio temp;
    tmunicipio* municipio;
    thash h;
    int nbuckets = 5571;
    int option;
    char ibge[9];
    int remove;

    hash_constroi(&h,nbuckets,get_key);

    // Abrir o arquivo CSV para leitura
    file = fopen("municipios.csv", "r");
    if (file == NULL) {
        printf("\nErro ao abrir o arquivo municipios.\n\n");
        exit(1);
    } else {
        while(feof(file) == 0){
            fscanf(file, " %[^,], %[^,],%lf,%lf,%d,%d, %[^,],%d, %[^\n]", 
                temp.cod_ibge, temp.nome, &temp.coord[0], &temp.coord[1], 
                &temp.capital, &temp.codigo_uf, temp.siafi_id, &temp.ddd,
                temp.fuso_horario);
            hash_insere(&h, aloca_muni(temp.cod_ibge, temp.nome, temp.coord[0], temp.coord[1], 
                temp.capital, temp.codigo_uf, temp.siafi_id, temp.ddd,
                temp.fuso_horario));
        }
    }

    // Fechar o arquivo
    fclose(file);

    do {
        printf("0 - Sair\n1 - Procurar codigo IBGE\n2 - Remover municipio\n");
        scanf("%d", &option);

        if(option == 1){
            printf("Digite o codigo IBGE: ");
            scanf("%s", ibge);

            municipio = hash_busca(h,ibge);

            if(municipio == NULL)
                printf("municipio nao encontrado\n");
            else {
                printf("municipio encontrado\n%s %s %lf %lf %d %d %s %d %s\n\n", temp.cod_ibge, temp.nome, temp.coord[0], temp.coord[1], temp.capital, temp.codigo_uf, temp.siafi_id, temp.ddd,
                temp.fuso_horario);
            }
        }

        else if(option == 2){
            printf("Digite o codigo IBGE: ");
            scanf("%s", ibge);

            remove = hash_remove(&h,ibge);

            if(remove == 0)
                printf("\nFalha ao remover.\n");
            else
                printf("\nRemovido com sucesso.\n");
        }

    } while(option != 0);

    hash_apaga(&h);

    return 0;
}
