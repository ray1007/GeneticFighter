/***************************************************************************
 *   Copyright (C) 2004 by Tian-Li Yu                                      *
 *   tianliyu@cc.ee.ntu.edu.tw                                             *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <fstream>
#include <algorithm>
#include <vector>

#include "global.h"
#include "statistics.h"
#include "myrand.h"
#include "ga.h"

#ifndef MAX_LINE
#define MAX_LINE     1024
#endif

GA::GA () : auxComp(this)
{

    ell = 0;
    nInitial = 0;
    nCurrent = 0;
    fe = 0;

    nNextGeneration = 0;
    maxGen = -1;
    maxFe = -1;

    population = NULL;
    offspring = NULL;
    selectionIndex = NULL;

}


GA::GA (int n_ell, int n_nInitial, int n_nElite, int n_selectionPressure,
        double n_pc, double n_pm, int n_maxGen, int n_maxFe, int n_repeat) : auxComp(this)
{
    init (n_ell, n_nInitial, n_nElite, n_selectionPressure,
          n_pc, n_pm, n_maxGen, n_maxFe, n_repeat);
}


GA::~GA ()
{

    delete[]population;
    delete[]offspring;
    delete[]selectionIndex;
}


void
GA::init (int n_ell, int n_nInitial, int n_nElite, int n_selectionPressure,
          double n_pc, double n_pm, int n_maxGen, int n_maxFe, int n_repeat)
{
    int i;

    ell = n_ell;
    nInitial = n_nInitial;
    nCurrent = nInitial;
    nElite = n_nElite;
    selectionPressure = n_selectionPressure;
    pc = n_pc;
    pm = n_pm;
    maxGen = n_maxGen;
    maxFe = n_maxFe;
    repeat = n_repeat;

    population = new Chromosome[nInitial];
    offspring = new Chromosome[nInitial];
    selectionIndex = new int[nInitial];

    for (i = 0; i < nInitial; i++) {
        population[i].init (ell);
        offspring[i].init (ell);
    }

    initializePopulation ();
}


void GA::initializePopulation ()
{
    int i, j;

    for (i = 0; i < nInitial; i++)
        for (j = 0; j < ell; j++)
            population[i].setVal (j, myRand.uniform() );
}

void GA::loadfile(const char* filename)
{
    // if population is not NULL.
    if(population){ 
        delete[]population;
        delete[]offspring;
        delete[]selectionIndex; 
    }

    FILE *fp = fopen( filename, "r");
    if( fp == NULL ){
        perror(filename);
        exit(1);
    }

    char token[MAX_LINE] = "";
    
    fscanf( fp, "%s %i", token, &generation); 
    fscanf( fp, "%s %i", token, &repeat);
    fscanf( fp, "%s %i", token, &ell);
    fscanf( fp, "%s %i", token, &nInitial);
    fscanf( fp, "%s %i", token, &nCurrent); 
    fscanf( fp, "%s %i", token, &nElite); 
    fscanf( fp, "%s %i", token, &selectionPressure);
    fscanf( fp, "%s %lf", token, &pc);
    fscanf( fp, "%s %lf", token, &pm);
    fscanf( fp, "%s %i", token, &fe);
    //fscanf( fp, "%s %i", token, &maxGen);
    //fscanf( fp, "%s %i", token, &maxFe);

    population = new Chromosome[nCurrent];
    offspring = new Chromosome[nCurrent];
    selectionIndex = new int[nCurrent];

    int i, j;
    double val;
    for(i=0; i<nCurrent; ++i){
        population[i].init (ell);
        offspring[i].init (ell);
        for(j=0; j<ell; ++j){
            fscanf( fp, "%lf", &val);
            population[i].setVal(j, val);
        }
    }
    
    fclose(fp);
}

void GA::savefile(const char* filename)
{
    FILE *fp = fopen( filename, "w");
    if( fp == NULL ){
        perror(filename);
        exit(1);
    }

    fprintf( fp, "generation: %i\n", generation);
    fprintf( fp, "repeat: %i\n", repeat);
    fprintf( fp, "ell: %i\n", ell);
    fprintf( fp, "nInitial: %i\n", nInitial);
    fprintf( fp, "nCurrent: %i\n", nCurrent);
    fprintf( fp, "nElite: %i\n", nElite);
    fprintf( fp, "selectionPressure: %i\n", selectionPressure);
    fprintf( fp, "pc: %f\n", pc);
    fprintf( fp, "pm: %f\n", pm);
    fprintf( fp, "fe: %i\n", fe);
    //fprintf( fp, "maxGen: %i\n", maxGen);
    //fprintf( fp, "maxFe: %i\n", maxFe);

    int i, j;
    for(i=0; i<nCurrent; ++i){
        for(j=0; j<ell; ++j)
            fprintf( fp, "%f ", population[i].getVal(j));
        fprintf(fp, "\n");
    }

    fclose(fp);
}

//std::vector<double> GA::getPopulationGene(int idx)
double* GA::getPopulationGene(int idx)
{
    int i;
    //std::vector<double> output;
    double* output = new double[ell];
    for(i=0; i<ell; ++i)
        output[i] = population[idx].getVal(i);
    return output;
}

void GA::setPopulationFitness(double* values)
{
    int i;
    for(i=0; i<nCurrent; ++i)
        population[i].setFitness( values[i] );
    fe += nCurrent;
}

// For now, assuming fixed population size
int GA::getNextPopulation ()
{
    //return nCurrent;
    return nInitial;
}

void GA::pickElite()
{
    int i;
    std::vector<int> sortedIdx;
    for(i=0; i<nCurrent; ++i)
        sortedIdx.push_back(i);
    
    std::sort(sortedIdx.begin(), sortedIdx.end(), auxComp);

    for(i=0; i<nElite; ++i)
        elite[i] = population[ sortedIdx[i] ];
}

void GA::selection ()
{
    //rwSelection ();
    tournamentSelection ();
}

// Roulette wheel selection
// This is a O(n^2) implementation
// You can achieve O(nlogn) by using binary search
void GA::rwSelection ()
{
    int i, j;

    // Adjusting population size 
    nNextGeneration = getNextPopulation ();

    if (nNextGeneration != nCurrent) {
        delete[]selectionIndex;
        delete[]offspring;
        selectionIndex = new int[nNextGeneration];
        offspring = new Chromosome[nNextGeneration];

        for (i = 0; i < nNextGeneration; i++)
            offspring[i].init (ell);
    }

    double totalFitness = 0.0;
    for (i = 0; i < nCurrent; i++) 
	    totalFitness += population[i].getFitness();

    for (i = 0; i < nNextGeneration; i++) {
    	double pointer = totalFitness * myRand.uniform();
    	int index = -1;
    	double partialSum = 0.0;
	    for (j = 0; j < nCurrent; j++) {
	        partialSum += population[j].getFitness();
            if (partialSum >= pointer) {
                index = j;
                break;
            }
	    }
	    if (index == -1) index = nCurrent - 1;

	    selectionIndex[i] = index;
    }

}

// tournamentSelection without replacement
void GA::tournamentSelection ()
{
    int i, j;

    // Adjusting population size 
    nNextGeneration = getNextPopulation ();

    if (nNextGeneration != nCurrent) {
        delete[]selectionIndex;
        delete[]offspring;
        selectionIndex = new int[nNextGeneration];
        offspring = new Chromosome[nNextGeneration];

        for (i = 0; i < nNextGeneration; i++)
            offspring[i].init (ell);
    }

    int randArray[selectionPressure * nNextGeneration];

    int q = (selectionPressure * nNextGeneration) / nCurrent;
    int r = (selectionPressure * nNextGeneration) % nCurrent;

    for (i = 0; i < q; i++)
        myRand.uniformArray (randArray + (i * nCurrent), nCurrent, 0, nCurrent - 1);

    myRand.uniformArray (randArray + (q * nCurrent), r, 0, nCurrent - 1);

    for (i = 0; i < nNextGeneration; i++) {

        int winner = 0;
        double winnerFitness = -DBL_MAX;

        for (j = 0; j < selectionPressure; j++) {
            int challenger = randArray[selectionPressure * i + j];
            double challengerFitness = population[challenger].getFitness ();

            if (challengerFitness > winnerFitness) {
                winner = challenger;
                winnerFitness = challengerFitness;
            }

        }
        selectionIndex[i] = winner;
    }
}


void GA::crossover ()
{
    int i;

    if ((nNextGeneration & 0x1) == 0) { 
    	// nNextGeneration is even
    	
        for (i = 0; i < nNextGeneration; i += 2)
            pairwiseXO (population[selectionIndex[i]], 
                        population[selectionIndex[i + 1]],
                        offspring[i], 
                        offspring[i + 1]);
    }
    else {
        for (i = 0; i < nNextGeneration - 1; i += 2) {
            pairwiseXO (population[selectionIndex[i]],
                        population[selectionIndex[i + 1]],
                        offspring[i],
                        offspring[i + 1]);
        }
        offspring[nNextGeneration - 1] =
            population[selectionIndex[nNextGeneration - 1]];
    }

}


void GA::pairwiseXO (const Chromosome & p1, const Chromosome & p2, Chromosome & c1, Chromosome & c2)
{
    if (myRand.uniform () < pc) {
	    onePointXO (p1, p2, c1, c2);
//      uniformXO (p1, p2, c1, c2, 0.5);
    }
    else {
        c1 = p1;
        c2 = p2;
    }
}

void GA::onePointXO (const Chromosome & p1, const Chromosome & p2, Chromosome & c1, Chromosome & c2)
{
    int i;
    int crossSite = myRand.uniformInt(1, ell-1);

    for (i = 0; i < crossSite; i++) {
        c1.setVal (i, p1.getVal(i));
        c2.setVal (i, p2.getVal(i));
    }

    for (i = crossSite; i < ell; i++) {
        c1.setVal (i, p2.getVal(i));
        c2.setVal (i, p1.getVal(i));
    }
}

void GA::uniformXO (const Chromosome & p1, const Chromosome & p2, Chromosome & c1, Chromosome & c2, double prob)
{
    int i;

    for (i = 0; i < ell; i++) {
        if (myRand.flip (prob)) {
            c1.setVal (i, p1.getVal(i));
            c2.setVal (i, p2.getVal(i));
        }
        else {
            c1.setVal (i, p2.getVal(i));
            c2.setVal (i, p1.getVal(i));
        }
    }
}

void GA::mutation ()
{
    //simpleMutation ();
    mutationClock ();
}


void GA::simpleMutation ()
{
    int i, j;

    for (i = 0; i < nNextGeneration; i++)
        for (j = 0; j< ell; j++)
            if (myRand.flip(pm)) {
                int val = offspring[i].getVal(j);
                offspring[i].setVal(j, 1-val);
            }
}

void GA::mutationClock ()
{
    if (pm <= 1e-6) return; // can't deal with too small pm

    int pointer = (int) (log(1-myRand.uniform()) / log(1-pm) + 1);

    while (pointer < nNextGeneration * ell) {
	    int q = pointer / ell;
	    int r = pointer % ell;

        int val = offspring[q].getVal(r);
        offspring[q].setVal(r, 1-val);

	    // Compute next mutation clock
	    pointer += (int) (log(1-myRand.uniform()) / log(1-pm) + 1);
    };
}


void GA::showStatistics ()
{

    printf ("Gen:%d  Fitness:(Max/Mean/Min):%f/%f/%f Chromsome Length:%d\n",
        generation, stFitness.getMax (), stFitness.getMean (),
        stFitness.getMin (), population[0].getLength ());
    printf ("best chromosome:");
    population[bestIndex].printf ();
    printf ("\n");
}


void GA::replacePopulation ()
{
    int i;

    if ((nNextGeneration + nElite) != nCurrent) {
        delete[]population;
        population = new Chromosome[nNextGeneration];
    }

    for (i = 0; i < nNextGeneration; i++)
        population[i] = offspring[i];
    for (i = 0; i < nElite; i++)
        population[i+nNextGeneration] = elite[i];

    nCurrent = nNextGeneration + nElite;
}


void GA::oneRun (bool output)
{
    int i;

    //setPopulationFitness();
    //pickElite();
    selection ();
    crossover ();
    mutation ();
    replacePopulation ();

    double max = -DBL_MAX;
    stFitness.reset ();
    for (i = 0; i < nCurrent; i++) {
        double fitness = population[i].getFitness ();
        if (fitness > max) {
            max = fitness;
            bestIndex = i;
        }
        stFitness.record (fitness);
    }

    if (output)
        showStatistics ();

    generation++;
}


int GA::doIt (bool output)
{
    //generation = 0;

    while (!shouldTerminate ()) {
        oneRun (output);
    }
    return generation;
}


bool GA::shouldTerminate ()
{
    bool termination = false;

    // Reach maximal # of function evaluations
    if (maxFe != -1) {
        if (fe > maxFe)
            termination = true;
    }

    // Reach maximal # of generations
    if (maxGen != -1) {
        if (generation >= maxGen)
            termination = true;
    }

    // Found a satisfactory solution
    if (stFitness.getMax() >= population[0].getMaxFitness())
        termination = true;

    // The population loses diversity
    if (stFitness.getMax()-1e-6 < stFitness.getMean())
	termination = true;

    return termination;

}
