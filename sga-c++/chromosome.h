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
        Chromosome (int n_seqAlph, int n_seqLen, int n_posLen);

        ~Chromosome ();

        Chromosome& operator= (const Chromosome & c);
        //bool operator< (const Chromosome & c);

        void init (int n_seqAlph, int n_seqLen, int n_posLen);

        int  getSeqVal (int index) const;
        void setSeqVal (int index, int val);   
        
        int  getPosVal (int index) const;
        void setPosVal (int index, int val);

        double  getFitness ();
        void    setFitness (double f);

        /** real evaluator */
        //double evaluate ();

        //double oneMax () const;

        bool isEvaluated () const;

        void printf () const;

        int getSeqLength () const;
        int getPosLength () const;

        //double getMaxFitness () const;

    protected:
        int *seqGene;
        int *posGene;
        int seqAlph;
        int seqLength;
        int posLength;
        double fitness;
        bool evaluated;
};
#endif
