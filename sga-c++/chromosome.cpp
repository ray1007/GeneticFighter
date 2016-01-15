/***************************************************************************
 *   Copyright (C) 2004 by Tian-Li Yu                                      *
 *   tianliyu@cc.ee.ntu.edu.tw                                             *
 ***************************************************************************/

#include <stdio.h>
#include "global.h"
#include "chromosome.h"

Chromosome::Chromosome ()
{
    seqAlph = 0;
    seqLength = 0;
    posLength = 0;
    seqGene = NULL;
    posGene = NULL;
    evaluated = false;
}


Chromosome::Chromosome (int n_seqAlph, int n_seqLen, int n_posLen)
{
    seqGene = NULL;
    posGene = NULL;
    init (n_seqAlph, n_seqLen, n_posLen);
}


Chromosome::~Chromosome ()
{
    delete[]seqGene;
    delete[]posGene;
}


void Chromosome::init (int n_seqAlph, int n_seqLen, int n_posLen)
{
    seqAlph = n_seqAlph;
    //length = n_length;
    seqLength = n_seqLen;
    posLength = n_posLen;

    if (seqGene != NULL)
        delete[]seqGene;
    if (posGene != NULL)
        delete[]posGene;

    seqGene = new int[seqLength];
    posGene = new int[posLength];
    evaluated = false;
}

int Chromosome::getSeqVal (int index) const
{
    if (index < 0 || index > seqLength){
        ::printf("idx=%i len=%i\n", index, seqLength);
        outputErrMsg ("Index overrange in Chromosome::operator[] (get)");
    }

    //return (gene[index])? 1:0;
    return seqGene[index];
}


void Chromosome::setSeqVal (int index, int val)
{
    if (index < 0 || index > seqLength)
        outputErrMsg ("Index overrange in Chromosome::operator[] (set)");

    seqGene[index] = val;
    evaluated = false;
}

int Chromosome::getPosVal (int index) const
{
    if (index < 0 || index > posLength){
        ::printf("idx=%i len=%i\n", index, posLength);
        outputErrMsg ("Index overrange in Chromosome::operator[] (get)");
    }

    //return (gene[index])? 1:0;
    return posGene[index];
}


void Chromosome::setPosVal (int index, int val)
{
    if (index < 0 || index > posLength)
        outputErrMsg ("Index overrange in Chromosome::operator[] (set)");

    posGene[index] = val;
    evaluated = false;
}


double Chromosome::getFitness ()
{
    if (evaluated)
        return fitness;
    else
        return -0.0;
        //return (fitness = evaluate ());
}


bool Chromosome::isEvaluated () const
{
    return evaluated;
}

void Chromosome::setFitness (double f)
{
    fitness = f;
    evaluated = true;
}

/*
double Chromosome::evaluate ()
{
    evaluated = true;
    return oneMax ();
}
*/

/*
// OneMax
double Chromosome::oneMax () const
{
    int i;
    double result = 0;

    for (i = 0; i < length; i++)
        result += gene[i];

    return result;
}*/


Chromosome & Chromosome::operator= (const Chromosome & c)
{
    int i;

    if (seqLength != c.seqLength) {
        seqLength = c.seqLength;
        delete[]seqGene;
        seqGene = new int[seqLength];
        //init (alph, length);
    }
    if (posLength != c.posLength) {
        posLength = c.posLength;
        delete[]posGene;
        posGene = new int[posLength];
        //init (alph, length);
    }


    evaluated = c.evaluated;
    fitness = c.fitness;

    for (i = 0; i < seqLength; i++)
        seqGene[i] = c.seqGene[i];
    for (i = 0; i < posLength; i++)
        posGene[i] = c.posGene[i];

    return *this;
}

/*
bool Chromosome::operator< (const Chromosome & c)
{
    return (fitness < c.fitness);
}
*/

void Chromosome::printf () const
{
    int i;
    for (i = 0; i < posLength; i++)
        ::printf ("%i,", posGene[i]);
    for (i = 0; i < seqLength; i++)
        ::printf ("%i,", seqGene[i]);
}


int Chromosome::getSeqLength () const
{
    return seqLength;
}

int Chromosome::getPosLength () const
{
    return posLength;
}

/*
double Chromosome::getMaxFitness () const
{
    // For OneMax
    return ((double)length-1e-6);
}
*/