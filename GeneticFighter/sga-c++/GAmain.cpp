/***************************************************************************
 *   Copyright (C) 2004 by Tian-Li Yu                                      *
 *   tianliyu@cc.ee.ntu.edu.tw                                             *
 *                                                                         *
 *   You can redistribute it and/or modify it as you like                  *
 ***************************************************************************/

#include <cmath>
#include <cstdio>
#include <iostream>
#include <cstdlib>

#include "statistics.h"
#include "ga.h"
#include "chromosome.h"
#include "global.h"

using namespace std;

int main (int argc, char *argv[])
{
    // command line arguments
    if (argc != 9) {
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
    int i, j, k, len;


    Statistics stGenS, stGenF;
    int usedGen;

    int failNum = 0;

    // variable length one-max problem.
    int MaxFitness = 15;

    GA ga (alph, nInitial, nElite, selectionPressure, 
           pc, pm, maxFe);

    for (i = 0; i < maxGen; i++) {
        //ga.loadfile("test.spore");
        double* allFitness = new double[ga.getNChromosome()];
        
        for(j = 0; j < ga.getNChromosome(); j++){
            int* currentGene = ga.getChromosomeGene(j, len);
            // unit test of variable length
            // with external evaluation
            allFitness[j] = -abs(MaxFitness - len);
            for(k = 0; k < len; k++){
                if(k >= MaxFitness)
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
        //ga.savefile("test2.spore");
        */
        
    }


    printf ("\nAverage Gen of Success: %f\n", stGenS.getMean());
    printf ("Average Gen of Failure: %f\n", stGenF.getMean());
    printf ("Total number of Failure: %d\n", failNum);

    return EXIT_SUCCESS;
}
