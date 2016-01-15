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
#include <random>

#include "global.h"
#include "statistics.h"
#include "myrand.h"
#include "ga.h"

#ifndef MAX_LINE
#define MAX_LINE     1024
#endif

GA::GA () : auxComp(this)
{
    seqAlph = 0;
    //ell = 0;
    posLen = 0;
    nInitial = 0;
    nCurrent = 0;
    fe = 0;

    nNextGeneration = 0;
    //maxGen = -1;
    maxFe = -1;

    population = NULL;
    offspring = NULL;
    selectionIndex = NULL;
}


GA::GA (int n_seqAlph, int n_posLen, int n_nInitial, int n_nElite, 
    int n_selectionPressure, double n_pc, double n_pm, int n_maxFe) : auxComp(this)
{
    init (n_seqAlph, n_posLen, n_nInitial, n_nElite, 
        n_selectionPressure, n_pc, n_pm, n_maxFe);
}


GA::~GA ()
{
    delete[]population;
    delete[]offspring;
    delete[]selectionIndex;
}


void
GA::init (int n_seqAlph, int n_posLen, int n_nInitial, int n_nElite, 
    int n_selectionPressure, double n_pc, double n_pm, int n_maxFe)
{
    int i;

    seqAlph = n_seqAlph;
    posLen = n_posLen;
    nInitial = n_nInitial;
    nElite = n_nElite;
    nCurrent = nInitial;
    selectionPressure = n_selectionPressure;
    pc = n_pc;
    pm = n_pm;
    //maxGen = n_maxGen;
    maxFe = n_maxFe;
    //repeat = n_repeat;
    generation = 0;

    population = new Chromosome[nInitial];
    offspring  = new Chromosome[nInitial];
    elite      = new Chromosome[nElite];
    selectionIndex = new int[nInitial];

    for (i = 0; i < nInitial; i++) {
        int seqLen = myRand.uniformInt(5,20);
        population[i].init (seqAlph, seqLen, posLen);
        //offspring[i].init (alph, ell);
    }
    for (i = 0; i < nElite; i++) {
        int seqLen = myRand.uniformInt(5,20);
        elite[i].init (seqAlph, seqLen, posLen);
    }

    initializePopulation ();
}


void GA::initializePopulation ()
{
    int i, j;

    for (i = 0; i < nInitial; i++){
        for (j = 0; j < population[i].getSeqLength(); j++)
            population[i].setSeqVal (j, myRand.uniformInt(1, seqAlph) );
        for (j = 0; j < population[i].getPosLength(); j++)
            population[i].setPosVal (j, myRand.uniformInt(-100,100) );
    }
    for (i = 0; i < nElite; i++){
        for (j = 0; j < elite[i].getSeqLength(); j++)
            elite[i].setSeqVal (j, myRand.uniformInt(1, seqAlph) );
        for (j = 0; j < elite[i].getPosLength(); j++)
            elite[i].setPosVal (j, myRand.uniformInt(-100,100) );
    }
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
    //fscanf( fp, "%s %i", token, &repeat);
    fscanf( fp, "%s %i", token, &seqAlph);
    fscanf( fp, "%s %i", token, &posLen);
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
    offspring  = new Chromosome[nCurrent];
    elite      = new Chromosome[nElite];
    selectionIndex = new int[nCurrent];

    int i, j;
    int seqLen;
    double fitness;
    int val;
    for(i=0; i<nElite; ++i){
        fscanf( fp, "%i", &seqLen);
        elite[i].init (seqAlph, seqLen, posLen);
        for(j=0; j<posLen; ++j){
            fscanf( fp, "%i", &val);
            elite[i].setPosVal(j, val);
        }
        for(j=0; j<seqLen; ++j){
            fscanf( fp, "%i", &val);
            elite[i].setSeqVal(j, val);
        }
        fscanf( fp, "%f", &fitness);
        elite[i].setFitness(fitness);
    }
    for(i=0; i<nCurrent; ++i){
        fscanf( fp, "%i", &seqLen);
        population[i].init (seqAlph, seqLen, posLen);
        for(j=0; j<posLen; ++j){
            fscanf( fp, "%i", &val);
            population[i].setPosVal(j, val);
        }
        for(j=0; j<seqLen; ++j){
            fscanf( fp, "%i", &val);
            population[i].setSeqVal(j, val);
        }
        fscanf( fp, "%f", &fitness);
        population[i].setFitness(fitness);
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
    //fprintf( fp, "repeat: %i\n", repeat);
    fprintf( fp, "seqAlph: %i\n", seqAlph);
    fprintf( fp, "posLen: %i\n", posLen);
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
    for(i=0; i<nElite; ++i){
        fprintf(fp, "%i ", elite[i].getSeqLength());
        for(j=0; j<elite[i].getPosLength(); ++j)
            fprintf( fp, "%i ", elite[i].getPosVal(j));
        for(j=0; j<elite[i].getSeqLength(); ++j)
            fprintf( fp, "%i ", elite[i].getSeqVal(j));
        fprintf(fp, "%f ", elite[i].getFitness());
        fprintf(fp, "\n");
    }
    for(i=0; i<nCurrent; ++i){
        fprintf(fp, "%i ", population[i].getSeqLength());
        for(j=0; j<population[i].getPosLength(); ++j)
            fprintf( fp, "%i ", population[i].getPosVal(j));
        for(j=0; j<population[i].getSeqLength(); ++j)
            fprintf( fp, "%i ", population[i].getSeqVal(j));
        fprintf(fp, "%f ", population[i].getFitness());
        fprintf(fp, "\n");
    }

    fclose(fp);
}

//std::vector<double> GA::getPopulationGene(int idx)
int* GA::getChromosomeGene(int idx, int& seqLength)
{
    int i;
    if(idx < nInitial){ // population
        int seqLen = population[idx].getSeqLength();
        int* output = new int[posLen + seqLen];
        for(i=0; i<posLen; ++i)
            output[i] = population[idx].getPosVal(i);
        for(i=posLen; i<posLen + seqLen; ++i)
            output[i] = population[idx].getSeqVal(i - posLen);
        seqLength = seqLen;
        return output;
    } else { // elites
        int seqLen = elite[idx - nInitial].getSeqLength();
        int* output = new int[posLen + seqLen];
        for(i=0; i<posLen; ++i)
            output[i] = elite[idx - nInitial].getPosVal(i);
        for(i=posLen; i<posLen+seqLen; ++i)
            output[i] = elite[idx - nInitial].getSeqVal(i - posLen);
        seqLength = seqLen;
        return output;
    }
}

void GA::setPopulationFitness(double* values)
{
    int i;
    for(i=0; i<nInitial+nElite; ++i){
        if(i < nInitial)
            population[i].setFitness( values[i] );
        else{
            elite[i - nInitial].setFitness( values[i] );
            //printf("elite %f\n", values[i]);
        }
    }
    
        
    fe += nInitial + nElite;
}

// For now, assuming fixed population size
int GA::getNChromosome ()
{
    return nCurrent+nElite;
}

int GA::getNextPopulation ()
{
    //return nCurrent;
    return nInitial;
}

void GA::pickElite()
{
    //if(nElite == 0)
    //    return;

    int i, j, k;
    std::vector<int> sortedIdx;
    for(i=0; i<getNChromosome(); ++i)
        sortedIdx.push_back(i);
    
    std::sort(sortedIdx.begin(), sortedIdx.end(), auxComp);
    
    /*for(i=0; i<nElite + nInitial; ++i)
        if(sortedIdx[i] < nInitial)
            printf("sIdx %i, fit:%f\n", sortedIdx[i], population[sortedIdx[i]].getFitness());
        else
            printf("sIdx %i, fit:%f\n", sortedIdx[i], elite[sortedIdx[i]-nInitial].getFitness());
    */

    Chromosome* oldPop    = new Chromosome[nInitial];
    Chromosome* oldElites = new Chromosome[nElite];
    for(i=0; i<nInitial; ++i)
        oldPop[i] = population[i];
    for(i=0; i<nElite; ++i)
        oldElites[i] = elite[i];

    // take first nElite chromosome as new elite.
    for(i=0; i<nElite; ++i){
        if(sortedIdx[i] < nInitial)
            elite[i] = oldPop[ sortedIdx[i] ];
        else
            elite[i] = oldElites[ sortedIdx[i]-nInitial ];
        //printf("elite idx %i, from %i, fit:%f\n", i, sortedIdx[i], elite[i].getFitness());
    }

    for(i=nElite; i<nElite+nInitial; ++i){
        if(sortedIdx[i] < nInitial)
            population[i - nElite] = oldPop[ sortedIdx[i] ];
        else
            population[i - nElite] = oldElites[ sortedIdx[i] - nInitial ];
        //printf("pop idx %i, from %i, fit:%f\n", i- nElite, sortedIdx[i], population[i-nElite].getFitness());
    }
    
    /*
    // if elite not in population, add only one instance.
    for(i=0; i<nElite; ++i){
        // if find identical one then skip.
        bool found = false;
        for(j = 0; j <nInitial; j++){
            if(population[j].getSeqLength() == elite[i].getSeqLength()){
                bool identical = true;
                for(k=0; k<population[j].getSeqLength(); k++){
                    if( population[j].getSeqVal(k) != elite[i].getSeqVal(k) ){
                        identical = false;
                        break; // break k-loop
                    }
                }
                if(identical){
                    found = true;
                    break; // break j-loop
                }
            }
        }

        if(!found)
            population[nInitial-i-1] = elite[i];
        //population[nInitial-i-1] = elite[i];
    }
    */

    delete[] oldElites;
    delete[] oldPop;
    
}

void GA::selection ()
{
    tournamentSelection ();
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

        //for (i = 0; i < nNextGeneration; i++)
            //offspring[i].init (alph, ell);
    }

    //int randArray[selectionPressure * nNextGeneration];
    int* randArray = new int[selectionPressure * nNextGeneration];

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
    delete[] randArray;
}


void GA::crossover ()
{
    int i;
    int randIdx;

    if ((nNextGeneration & 0x1) == 0) { 
    	// nNextGeneration is even
        for (i = 0; i < nNextGeneration; i += 2){
            /*randIdx = myRand.uniformInt(0, nNextGeneration-1);
            SPX_onePointXO( population[selectionIndex[i]],
                            population[selectionIndex[i+1]], 
                            population[selectionIndex[randIdx]],
                            offspring[i], 
                            offspring[i+1]);*/
            pairwiseXO (population[selectionIndex[i]],
                        population[selectionIndex[i + 1]],
                        offspring[i],
                        offspring[i + 1]);
        }
    }
    else {
        for (i = 0; i < nNextGeneration - 1; i += 2) {
            /*randIdx = myRand.uniformInt(0, nNextGeneration-1);
            SPX_onePointXO( population[selectionIndex[i]],
                            population[selectionIndex[i+1]], 
                            population[selectionIndex[randIdx]],
                            offspring[i], 
                            offspring[i+1]);*/
            
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
	    //BLX_onePointXO (p1, p2, c1, c2);
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
    
    // seqence part : one point XO
    int crossSite1 = myRand.uniformInt(1, p1.getSeqLength()-1);
    int crossSite2 = myRand.uniformInt(1, p2.getSeqLength()-1);
    int newSeqLen1 = crossSite1 + p2.getSeqLength()-crossSite2;
    int newSeqLen2 = crossSite2 + p1.getSeqLength()-crossSite1;
    c1.init(seqAlph, newSeqLen1, posLen);
    c2.init(seqAlph, newSeqLen2, posLen);
 
    for (i = 0; i < p1.getSeqLength(); i++) {
        if(i < crossSite1)
            c1.setSeqVal (i, p1.getSeqVal(i));
        else
            c2.setSeqVal (crossSite2-crossSite1+i, p1.getSeqVal(i));
    }
    for (i = 0; i < p2.getSeqLength(); i++) {
        if(i < crossSite2)
            c2.setSeqVal (i, p2.getSeqVal(i));
        else
            c1.setSeqVal (crossSite1-crossSite2+i, p2.getSeqVal(i));
    }

    for (i = 0; i < posLen; i++){
        c1.setPosVal(i, p1.getPosVal(i));
        c2.setPosVal(i, p2.getPosVal(i));
    }
}

/*
void GA::SPX_onePointXO (const Chromosome & p1, const Chromosome & p2, 
    const Chromosome& p3, Chromosome & c1, Chromosome & c2)
{
    int i;
    
    // seqence part : one point XO
    int crossSite1 = myRand.uniformInt(1, p1.getSeqLength()-1);
    int crossSite2 = myRand.uniformInt(1, p2.getSeqLength()-1);
    int newSeqLen1 = crossSite1 + p2.getSeqLength()-crossSite2;
    int newSeqLen2 = crossSite2 + p1.getSeqLength()-crossSite1;
    c1.init(seqAlph, newSeqLen1, posLen);
    c2.init(seqAlph, newSeqLen2, posLen);
 
    for (i = 0; i < p1.getSeqLength(); i++) {
        if(i < crossSite1)
            c1.setSeqVal (i, p1.getSeqVal(i));
        else
            c2.setSeqVal (crossSite2-crossSite1+i, p1.getSeqVal(i));
    }
    for (i = 0; i < p2.getSeqLength(); i++) {
        if(i < crossSite2)
            c2.setSeqVal (i, p2.getSeqVal(i));
        else
            c1.setSeqVal (crossSite1-crossSite2+i, p2.getSeqVal(i));
    }

    // position part : SPX 
    double a, b, c;
    a = myRand.uniform(0, 1);
    b = myRand.uniform(0, 1);
    if( a+b > 1)
        b = 1-a;
    c = myRand.uniform(0, 1);
    if (a+b+c > 1)
        c = 1-a-b;
    for(i = 0; i < posLen; i++){
        double val = 0;
        val += a * p1.getPosVal(i);
        val += b * p2.getPosVal(i);
        val += c * p3.getPosVal(i);
        c1.setPosVal(i, (int)val * pow(posLen+2, 0) );
    }

    a = myRand.uniform(0, 1);
    b = myRand.uniform(0, 1);
    if( a+b > 1)
        b = 1-a;
    c = myRand.uniform(0, 1);
    if (a+b+c > 1)
        c = 1-a-b;
    for(i = 0; i < posLen; i++){
        double val = 0;
        val += a * p1.getPosVal(i);
        val += b * p2.getPosVal(i);
        val += c * p3.getPosVal(i);
        c2.setPosVal(i, (int)val * pow(posLen+2, 0) );
    }
}*/

void GA::BLX_onePointXO (const Chromosome & p1, const Chromosome & p2, 
    Chromosome & c1, Chromosome & c2)
{
    int i;
    
    // seqence part : one point XO
    int crossSite1 = myRand.uniformInt(1, p1.getSeqLength()-1);
    int crossSite2 = myRand.uniformInt(1, p2.getSeqLength()-1);
    int newSeqLen1 = crossSite1 + p2.getSeqLength()-crossSite2;
    int newSeqLen2 = crossSite2 + p1.getSeqLength()-crossSite1;
    c1.init(seqAlph, newSeqLen1, posLen);
    c2.init(seqAlph, newSeqLen2, posLen);
 
    for (i = 0; i < p1.getSeqLength(); i++) {
        if(i < crossSite1)
            c1.setSeqVal (i, p1.getSeqVal(i));
        else
            c2.setSeqVal (crossSite2-crossSite1+i, p1.getSeqVal(i));
    }
    for (i = 0; i < p2.getSeqLength(); i++) {
        if(i < crossSite2)
            c2.setSeqVal (i, p2.getSeqVal(i));
        else
            c1.setSeqVal (crossSite1-crossSite2+i, p2.getSeqVal(i));
    }

    // position part : BLX mod 
    double alpha = 0.2;
    for (i = 0; i < posLen; i++){
        int x = p1.getPosVal(i);
        int y = p2.getPosVal(i);
        if( x > y ){
            int temp = x;
            x = y;
            y = temp;
        }

        // BLX mod
        double I = y - x;
        if(myRand.flip(0.5)){ // take small one
            int v1 = (int)myRand.uniform(x - alpha*I, x + alpha*I);
            int v2 = (int)myRand.uniform(y - alpha*I, y + alpha*I);
            c1.setPosVal(i, v1);
            c2.setPosVal(i, v2);
        } else {
            int v1 = (int)myRand.uniform(y - alpha*I, y + alpha*I);
            int v2 = (int)myRand.uniform(x - alpha*I, x + alpha*I);
            c1.setPosVal(i, v1);
            c2.setPosVal(i, v2);
        }

        // BLX
        /*int v1 = (int)myRand.uniform(x - alpha*I, y + alpha*I);
        int v2 = (int)myRand.uniform(x - alpha*I, y + alpha*I);
        //c1.setPosVal(i, (v1 >=0 )?v1 :0);
        //c2.setPosVal(i, (v2 >= 0)?v2 :0);
        c1.setPosVal(i, v1);
        c2.setPosVal(i, v2);*/
    }
}

/*
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
*/

void GA::mutation ()
{
    simpleMutation ();
    //mutationClock ();
}


void GA::simpleMutation ()
{
    int i, j;
   
    for (i = 0; i < nNextGeneration; i++){
        if(myRand.flip(pm)){
            for(j = 0; j < offspring[i].getPosLength(); j++){
                int val = offspring[i].getPosVal(j);
                val = myRand.uniformInt(val-10, val+10);
                offspring[i].setPosVal(j, val);
            }
        }    
    }
}


/*
void GA::mutationClock ()
{
    if (pm <= 1e-6) return; // can't deal with too small pm

    int pointer = (int) (log(1-myRand.uniform()) / log(1-pm) + 1);

    while (pointer < nNextGeneration * ell) {
	    int q = pointer / ell;
	    int r = pointer % ell;

        int val = offspring[q].getPosVal(r);
        double sigma = 10;
        double sample = exp( -power(x-val, 2) / ( 2*power(sigma, 2) ) ) 
                        / sigma / power(2*3.14 ,0.5);

	    // Compute next mutation clock
	    pointer += (int) (log(1-myRand.uniform()) / log(1-pm) + 1);
    };
}
*/

void GA::showStatistics ()
{

    printf ("Gen:%d  Fitness:(Max/Mean/Min):%f/%f/%f ", generation, 
        stFitness.getMax (), stFitness.getMean (), stFitness.getMin ());
    int len;
    if(bestIndex < nInitial)
        len = population[bestIndex].getSeqLength() ;
    else
        len = elite[bestIndex - nInitial].getSeqLength();
    printf("Chromsome Seq Length:%d\n", len);
    printf ("best chromosome:");
    if(bestIndex < nInitial)
        population[bestIndex].printf ();
    else
        elite[bestIndex - nInitial].printf ();
    printf ("\n");
}


void GA::replacePopulation ()
{
    int i, j, k;
    if ((nNextGeneration) != nCurrent) {
        delete[]population;
        population = new Chromosome[nNextGeneration];
    }
    /*
    for (i = 0; i < nNextGeneration; i++){
        population[i] = offspring[i];
    */
    int RTRwindowSize = 50;
    int* randArray = new int[nNextGeneration];
    myRand.uniformArray (randArray, nNextGeneration, 0, nNextGeneration - 1);

    for (i = 0; i < nNextGeneration; i++){
        int minHamDist = INT_MAX;
        int nearestIdx = 0;
        //printf("%i ", i);
        int oSeqLen = offspring[i].getSeqLength();
        // open a window
        // gen a rand array.
        for(j = 0; j < RTRwindowSize; j++){
            int popIdx = randArray[j];
            int pSeqLen = population[popIdx].getSeqLength();
            int seqLen = (pSeqLen > oSeqLen)? oSeqLen: pSeqLen;
            int hamDist = 0;
            for(k = 0; k < seqLen; k++)
                if(offspring[i].getSeqVal(k) != population[popIdx].getSeqVal(k))
                    hamDist++;
            if(minHamDist > hamDist){
                minHamDist = hamDist;
                nearestIdx = popIdx;
            }
        }
        //if(nearestIdx == 0) 
        //    nearestIdx = i;
        if(offspring[i].getFitness() > population[nearestIdx].getFitness())
            population[nearestIdx] = offspring[i];
    }
    
    nCurrent = nNextGeneration;
}


void GA::oneRun (bool output)
{
    int i;
    
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
    for (i = 0; i < nElite; i++) {
        double fitness = elite[i].getFitness ();
        if (fitness > max) {
            max = fitness;
            bestIndex = i + nCurrent;
        }
        stFitness.record (fitness);
    }

    if (output)
        showStatistics ();

    pickElite();
    selection ();
    crossover ();
    mutation ();
    replacePopulation ();
    
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
    /*if (maxGen != -1) {
        if (generation >= maxGen)
            termination = true;
    }*/

    // Found a satisfactory solution (from extern)

    // The population loses diversity
    if (stFitness.getMax()-1e-6 < stFitness.getMean())
	termination = true;

    return termination;
}
