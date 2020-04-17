#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fields.h"
#include <math.h>

//Istenen degiskenler
int initial_range, jump_range, num_jumps, initial_power = 0;
double power_reduction;

int Toplam_Iyilesme;
int **iyilestirme;

struct node **temp_path;
int **anlik_iyilsetirme;

struct node *test1;
struct node *test2;

//Hastalar icin bir node olusturduk
//Liste biciminde oldugu icin sonraki ve gidilmis dugum bilgileri gerekiyor
struct node
{
    int X, Y;
    int Mevcut_PP, Max_PP;
    char *Ad;

    struct node *birSonraki;
    int gidilmisDugum;

    int adj_size; 
    struct node **adj_list;

    int iyilestirme;
};


void tedaviEt(struct node *node, int hop, int num_jumpsL, int total_healing, struct node *from, int counter_array)
{

    int ihtiyacOlanCan;
    int reset_init_power = initial_power;

    if (node->gidilmisDugum == 1 || hop > num_jumpsL)
    {
        node->gidilmisDugum = 0;
        return;
    }
    int count;

    temp_path[counter_array] = node;

    if (counter_array == num_jumpsL - 1)
    {
        int loop_C;
        for (loop_C = 0; loop_C < num_jumpsL; loop_C++)
        {
            ihtiyacOlanCan = temp_path[loop_C]->Max_PP - temp_path[loop_C]->Mevcut_PP;
            if (ihtiyacOlanCan < initial_power)
                temp_path[loop_C]->iyilestirme = ihtiyacOlanCan;
            else
                temp_path[loop_C]->iyilestirme = initial_power;

            //islem sonrasi olusan decimal kismi rint ile yuvarladik    
            initial_power = rint(initial_power * (1 - power_reduction));
            
            total_healing = total_healing + temp_path[loop_C]->iyilestirme;

            anlik_iyilsetirme[loop_C] = &(temp_path[loop_C]->iyilestirme);

            //Istenilen bicimde isim ve iyilestirilen miktar yaziliyor
            printf("\n\n%s   %d\n", temp_path[loop_C]->Ad, *(anlik_iyilsetirme[loop_C]));
        }

        initial_power = reset_init_power;

        int x;
        if (total_healing > Toplam_Iyilesme)
        {

            Toplam_Iyilesme = total_healing;
            for (x = 0; x < num_jumpsL; x++)
            {
                iyilestirme[x] = anlik_iyilsetirme[x];
            }
        }
    }

    count = 0; 
    while (count < node->adj_size)
    {
        node->gidilmisDugum = 1;
        tedaviEt(node->adj_list[count], hop + 1, num_jumpsL, total_healing, node->birSonraki, counter_array + 1);
        count++;
    }
}

int main(int argc, char **argv)
{
    IS is;
    struct node *first;
    struct node *current;
    struct node *temp;

    first = (struct node *)malloc(sizeof(struct node));
    current = (struct node *)malloc(sizeof(struct node));

    int X, Y, Mevcut_PP, Max_PP;
    char *Ad = malloc(sizeof(char) * 100);
    
    //Command Line Okunuyor ve hekim degerleri okunuyor
    initial_range = atoi(argv[1]);
    jump_range = atoi(argv[2]);
    num_jumps = atoi(argv[3]);
    initial_power = atoi(argv[4]);
    power_reduction = strtod(argv[5], NULL);

    int node_counter = 0;
    is = new_inputstruct(NULL);

    //Libfdr ile command line'dan alinan dosya adresinden okuma gerceklesiyor
    while (get_line(is) >= 0)
    {
        //Okunan Satirlarda ki degerler gerekli alanlara ataniyor
        temp = (struct node *)malloc(sizeof(struct node));
        temp->X = atoi(is->fields[0]);
        temp->Y = atoi(is->fields[1]);
        temp->Mevcut_PP = atoi(is->fields[2]);
        temp->Max_PP = atoi(is->fields[3]);
        temp->Ad = malloc(sizeof(char) * 100);
        strcpy(temp->Ad, is->fields[4]);

        //Eger ilk calistirma ise first e atama oluyor degilse bir sonraki kismina node ataniyor
        if (node_counter <= 0)
        {
            first = temp;
            first->birSonraki = NULL;
            current = first;
        }
        else
        {
            current->birSonraki = temp;
            current = temp;
            current->birSonraki = NULL;
        }

        node_counter++;
    }

    current = first;

    struct node *outer_current = (struct node *)malloc(sizeof(struct node));
    struct node *inner_current = (struct node *)malloc(sizeof(struct node));

    outer_current = first;

    while (outer_current)
    {
        int adj_counter = 0;
        inner_current = first;

        while (inner_current)
        {
            if (outer_current != inner_current)
            {
                //Uzaklik olcumu yapiliyor ki iyilesme sirasi ona gore duzenleniyor
                if (sqrt(((outer_current->X - inner_current->X) * (outer_current->X - inner_current->X)) + ((outer_current->Y - inner_current->Y) * (outer_current->Y - inner_current->Y))) <= jump_range)
                {
                    adj_counter++;
                }
            }

            inner_current = inner_current->birSonraki;
        }

        outer_current->adj_size = adj_counter;
        outer_current = outer_current->birSonraki;
    }

    current = first;

    outer_current = first;

    while (outer_current)
    {
        outer_current->adj_list = (struct node **)malloc(sizeof(struct node *) * outer_current->adj_size);
        int index_counter = 0;
        inner_current = first;

        while (inner_current)
        {

            if (outer_current != inner_current)
            {
                if (sqrt(((outer_current->X - inner_current->X) * (outer_current->X - inner_current->X)) + ((outer_current->Y -
                                                                                                             inner_current->Y) *
                                                                                                            (outer_current->Y - inner_current->Y))) <= jump_range)
                {
                    outer_current->adj_list[index_counter++] = inner_current;
                }
            }

            inner_current = inner_current->birSonraki;
        }
        outer_current = outer_current->birSonraki;
    }

    int listC = 0;
    current = first;

    temp_path = (struct node **)malloc(sizeof(struct node *) * num_jumps);
    anlik_iyilsetirme = (int **)malloc(sizeof(int *) * num_jumps);

    iyilestirme = (int **)malloc(sizeof(int *) * num_jumps);

    test1 = (struct node *)malloc(sizeof(struct node));
    test2 = (struct node *)malloc(sizeof(struct node));
    
    Toplam_Iyilesme = 0;

    int counter;
    current = first;

    for (counter = 0; counter < node_counter; counter++)
    {
        if (sqrt(((first->X - current->X) * (first->X - current->X)) +
                 ((first->Y - current->Y) * (first->Y - current->Y))) <= initial_range)
        {
            tedaviEt(current, 1, num_jumps, 0, NULL, 0);
            current = current->birSonraki;
        }
    }

    
    printf("Toplam_İyileşme: %d", Toplam_Iyilesme);
    printf("\n\nProgram Sonu!\n\n");
    return 0;
}
