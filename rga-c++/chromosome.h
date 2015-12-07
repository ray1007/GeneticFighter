/***************************************************************************
 *   Copyright (C) 2004 by Tian-Li Yu                                      *
 *   tianliyu@cc.ee.ntu.edu.tw                                             *
 ***************************************************************************/

#ifndef _CHROMOSOME_H
#define _CHROMOSOME_H


class Chromosome
{
    public:
        Chromosome ();
        Chromosome (int n_ell);

        ~Chromosome ();

        Chromosome& operator= (const Chromosome & c);
        //bool operator< (const Chromosome & c);

        void init (int n_ell);

        double getVal (int index) const;
        void setVal (int index, double val);

        double getFitness ();
        void setFitness (double f);

        /** real evaluator */
        double evaluate ();

        double oneMax () const;

        bool isEvaluated () const;

        void printf () const;

        int getLength () const;

        double getMaxFitness () const;

    protected:
        double *gene;
        int length;
        double fitness;
        bool evaluated;
};
#endif
