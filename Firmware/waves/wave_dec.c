/* wave_dec.c - decimate the e950 wave_bank.h for display */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "wave_bank.h"

#define N_WAVES 32
#define N_SAMPS 256

int main(int argc, char **argv)
{
    int i, j, k;
    FILE *ofile;
    
    if(!(ofile = fopen("wave_bank_dec.h", "w")))
    {
        fprintf(stderr, "Can't open output file\n");
        exit(-1);
    }
    
    /* put on a header */
    fprintf(ofile, "const int8_t wave_bank_dec[] = {\n");

    /* iterate */
    for(i=0;i<N_WAVES;i++)
    {
        fprintf(ofile, "	/* wave %d */\n", i);
        
        for(j=0;j<N_SAMPS;j+=64)
        {
            fprintf(ofile, "\t");
            for(k=0;k<64;k+=8)
                fprintf(ofile, "0x%02X, ", wave_bank[i*N_SAMPS+j+k]&0xFF);
            fprintf(ofile, "\n");
        }
    }
    
    fprintf(ofile, "};\n");
    
    fclose(ofile);
    
    return 0;
}
