#include <string.h>
#include <stdio.h>
#include "ex1.h"

/* Returns the number of times LETTER appears in STR.
There are two different ways to iterate through a string.
1st way hint: strlen() may be useful
2nd way hint: all strings end in a null terminator */
int num_occurrences(char *str, char letter) {
    int count = 0;
    int len = strlen(str);
    while ((str[count] != letter)) { // or (*(str + count) != letter)
        // if not find, count equals len, then, return 0
        if (count == len) {
            return 0;
        }
        count++;
    }
    // we find the str
    // we need "order", so we should let count + 1
    return count + 1;
}

/* Populates DNA_SEQ with the number of times each nucleotide appears.
Each sequence will end with a NULL terminator and will have up to 20 nucleotides.
All letters will be upper case. */
void compute_nucleotide_occurrences(DNA_sequence *dna_seq) {
    int i = 0;
    // initialize all counts in dna_seq
    dna_seq->A_count = 0;
    dna_seq->T_count = 0;
    dna_seq->C_count = 0;
    dna_seq->G_count = 0;
    // counting all counts in dna_seq
    while (dna_seq->sequence[i]) { // or dna_seq->sequence[i] != '\0'
        switch(dna_seq->sequence[i]) {
            case 'A': 
                dna_seq->A_count++;
                break;
            case 'T': 
                dna_seq->T_count++;
                break;
            case 'C': 
                dna_seq->C_count++;
                break;
            case 'G': 
                dna_seq->G_count++;
                break;
        }
        i++;
    }
    return;
}
