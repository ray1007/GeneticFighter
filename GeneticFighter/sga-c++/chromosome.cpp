/***************************************************************************
 *   Copyright (C) 2004 by Tian-Li Yu                                      *
 *   tianliyu@cc.ee.ntu.edu.tw                                             *
 ***************************************************************************/

#include <stdio.h>
#include "global.h"
#include "chromosome.h"

Chromosome::Chromosome ()
{
    alph = 0;
    length = 0;
    gene = NULL;
    evaluated = false;
}


Chromosome::Chromosome (int n_alph, int n_length)
{
    gene = NULL;
    init (n_alph, n_length);
}


Chromosome::~Chromosome ()
{
    delete[]gene;
}


void Chromosome::init (int n_alph, int n_length)
{
    alph = n_alph;
    length = n_length;

    if (gene != NULL)
        delete[]gene;

    gene = new int[length];
    evaluated = false;
}

int Chromosome::getVal (int index) const
{
    if (index < 0 || index > length){
        ::printf("idx=%i len=%i\n", index, length);
        outputErrMsg ("Index overrange in Chromosome::operator[] (get)");
    }

    //return (gene[index])? 1:0;
    return gene[index];
}


void Chromosome::setVal (int index, int val)
{
    if (index < 0 || index > length)
        outputErrMsg ("Index overrange in Chromosome::operator[] (set)");

    gene[index] = val;
    evaluated = false;
}


double Chromosome::getFitness ()
{
    if (evaluated)
        return fitness;
    else
        return -1.0;
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

    if (length != c.length) {
        length = c.length;
        delete[]gene;
        gene = new int[length];
        //init (alph, length);
    }

    evaluated = c.evaluated;
    fitness = c.fitness;

    for (i = 0; i < length; i++)
        gene[i] = c.gene[i];

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
    for (i = 0; i < length; i++)
        ::printf ("%i", gene[i]);
}


int Chromosome::getLength () const
{
    return length;
}


double Chromosome::getMaxFitness () const
{
    // For OneMax
    return ((double)length-1e-6);
}