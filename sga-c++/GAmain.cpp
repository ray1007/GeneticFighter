/***************************************************************************
 *   Copyright (C) 2004 by Tian-Li Yu                                      *
 *   tianliyu@cc.ee.ntu.edu.tw                                             *
 *                                                                         *
 *   You can redistribute it and/or modify it as you like                  *
 ***************************************************************************/

#include <cmath>
#include <ctime>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <string>

#include "statistics.h"
#include "ga.h"
#include "chromosome.h"
#include "global.h"

using namespace std;

int main (int argc, char *argv[])
{
    // command line arguments
    if (argc != 9 && argc != 10) {
        printf ("GA n_alph nInitial nElite selectionPressure pc pm maxGen maxFe\n");
        return -1;
    }

    int alph = atoi (argv[1]);    // alphabet size                           
    int nInitial = atoi (argv[2]); // initial population size
    int nElite = atoi (argv[3]); // selection pressure
    int selectionPressure = atoi (argv[4]); // selection pressure
    double pc = atof (argv[5]);  // pc
    double pm = atof (argv[6]);  // pm
    int maxGen = atoi (argv[7]); // max generation
    int maxFe = atoi (argv[8]);  // max fe

    //
    //int repeat = atoi (argv[9]); // how many time to repeat
    
    /* internal parameter initialization.
    int alph = 8;    // alphabet size                           
    int nInitial = atoi (argv[2]); // initial population size
    int nElite = atoi (argv[3]); // selection pressure
    int selectionPressure = atoi (argv[4]); // selection pressure
    double pc = atof (argv[5]);  // pc
    double pm = atof (argv[6]);  // pm
    int maxGen = atoi (argv[7]); // max generation
    int maxFe = atoi (argv[8]);  // max fe
    //int repeat = atoi (argv[9]); // how many time to repeat
    */
    int i, j, k, seqLen;


    Statistics stGenS, stGenF;
    int usedGen;

    int failNum = 0;

    // variable length one-max problem.
    int pos[2] = {50, 9};
    int MaxFitness = 15;
    int posLen = 2;

    GA ga (alph, posLen, nInitial, nElite, selectionPressure, 
           pc, pm, maxFe);
    if(argc == 10)
        ga.loadfile(argv[9]);
    clock_t begin = clock();
    for (i = 0; i < maxGen; i++) {
        //ga.loadfile("test.spore");
        double* allFitness = new double[ga.getNChromosome()];
        
        for(j = 0; j < ga.getNChromosome(); j++){
            int* currentGene = ga.getChromosomeGene(j, seqLen);
            // unit test of variable length
            // with external evaluation
            allFitness[j] = -abs(MaxFitness - seqLen);
            for(k = 0; k < posLen; k++){
                allFitness[j] -= (double)abs(pos[k] - currentGene[k]) / 10;
                //printf("pos fit %f\n", abs(pos[k] - currentGene[k])/10);
            }
            for(k = posLen; k < posLen + seqLen; k++){
                if(k - posLen >= MaxFitness)
                    break;
                if(currentGene[k] == 1)
                    allFitness[j]++;
            }
            //printf("external fitness %i %f\n", j, allFitness[j]);
            

            // TODO
            // With chromosome length len 
            // and genes, update angel script.
            // Then run LF2, obtain fitness,
            // and save it to allFitness.
            //
            // allFitness[j] = 0;
        }
        
        // update fitness values of ga population.
        ga.setPopulationFitness(allFitness);
        delete[] allFitness;

        ga.oneRun(true);

        if(ga.shouldTerminate())
            break;
        
        /*
        if (repeat == 1)
            usedGen = ga.doIt (true);
        else
            usedGen = ga.doIt (false);
        
        //Chromosome ch(alph, ell);
        
        if (ga.stFitness.getMax() < MaxFitness) {
            printf ("-");
            failNum++;
            stGenF.record (usedGen);
        }
        else {
            printf ("+");
            stGenS.record (usedGen);
        }

        fflush (NULL);
        //
        */
    }
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("time elapsed:%f\n", elapsed_secs);
    string fname = "test"+to_string(ga.getGeneration())+".spore";
    ga.savefile(fname.c_str());


    printf ("\nAverage Gen of Success: %f\n", stGenS.getMean());
    printf ("Average Gen of Failure: %f\n", stGenF.getMean());
    printf ("Total number of Failure: %d\n", failNum);

    return EXIT_SUCCESS;
}
