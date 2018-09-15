
/*
 * File CombSubstIterator.hpp.
 *
 * This file is part of the source code of the software program
 * Vampire. It is protected by applicable
 * copyright laws.
 *
 * This source code is distributed under the licence found here
 * https://vprover.github.io/license.html
 * and in the source directory
 *
 * In summary, you are allowed to use Vampire for non-commercial
 * purposes but not allowed to distribute, modify, copy, create derivatives,
 * or use in competitions. 
 * For other uses of Vampire please contact developers for a different
 * licence, which we will make an effort to provide. 
 */
/**
 * @file CombSubstIterator.hpp
 * Defines class CombSubstIterator.
 *
 */

#ifndef __CombSubstIterator__
#define __CombSubstIterator__

#include <utility>

#include "Forwards.hpp"
#include "Lib/DHMap.hpp"
#include "Lib/Set.hpp"
#include "Lib/Backtrackable.hpp"

#include "Indexing/Index.hpp"

#include "Signature.hpp"
#include "Term.hpp"
#include "RobSubstitution.hpp"
#include "HOSortHelper.hpp"

#if VDEBUG

#include <iostream>
#include "Lib/VString.hpp"

#endif

namespace Kernel
{

using namespace Indexing;
using namespace Lib;

//should split into separate file
class CombSubstitution
:public Backtrackable
{
  CLASS_NAME(CombSubstitution);
  USE_ALLOCATOR(CombSubstitution);

  public:

    CombSubstitution(TermList t1,int index1, TermList t2, int index2):_solved(false)
    {
      unsigned maxt1 = HOSortHelper::getMaxVar(t1);
      unsigned maxt2 = HOSortHelper::getMaxVar(t2);
      _nextFreshVar = max(maxt1, maxt2) + 1;
      HOSortHelper::HOTerm ht1 = HOSortHelper::deappify(t1);
      HOSortHelper::HOTerm ht2 = HOSortHelper::deappify(t2);
      UnificationPair up = UnificationPair(ht1, index1, ht2, index2);
      populateTransformations(up);
      _unificationPairs.push(up);
    }

    TermList apply(TermList t, int index) const;
    Literal* apply(Literal* lit, int index) const;

    enum AlgorithmStep{
       UNDEFINED,
       ADD_ARG,
       SPLIT,
       I_NARROW,
       K_NARROW,
       KX_NARROW,
       B_NARROW,
       BX_NARROW,
       C_NARROW,
       CX_NARROW,
       S_NARROW,
       SX_NARROW,
       I_REDUCE,
       K_REDUCE,
       B_REDUCE,
       C_REDUCE,
       S_REDUCE,
       DECOMP,
       ELIMINATE
    };
    
    /**
      * Used to record whether the algorithm step can be applied to 
      * the left of the pair, the right or applies to both (for example
      * ADD_ARG and DECOMP apply to both items in a unif pair).
      */
    enum ApplyTo{
       FIRST = 1,
       SECOND = 2,
       BOTH = 3
    };
          
  private:

    typedef pair<AlgorithmStep, ApplyTo>  Transform;
    typedef VirtualIterator<Transform> TransformIterator;
    typedef RobSubstitution::VarSpec VarSpec;
    typedef Signature::Symbol SS;
    typedef HOSortHelper HSH;
    typedef pair<HSH::HOTerm, int> HOTermSpec;
    typedef pair<HOTermSpec,HOTermSpec> TTPair;  

    struct UnificationPair
    {
      //CLASS_NAME(UnificationPair);
      //USE_ALLOCATOR(UnificationPair);

      UnificationPair(HSH::HOTerm ht1 ,int index1, HSH::HOTerm ht2, int index2)
      {
        HOTermSpec hts1 = make_pair(ht1,index1);
        HOTermSpec hts2 = make_pair(ht2,index2);
        unifPair = make_pair(hts1,hts2);
        lastStep = UNDEFINED;
        secondLastStep = UNDEFINED;
      }
      UnificationPair(HOTermSpec tl, HOTermSpec tr, AlgorithmStep ls, AlgorithmStep sls)
      : lastStep(ls), secondLastStep(sls)
      {
        unifPair = make_pair(tl,tr);
      }
      //stack that holds the potential transformations that can be carried out to
      //the left-hand (first) term of this unification pair
      Stack<Transform> transformsLeft;
      Stack<Transform> transformsRight;
      Stack<Transform> transformsBoth;
      
      AlgorithmStep lastStep;      
      AlgorithmStep secondLastStep;
      TTPair unifPair;
    };

   #if VDEBUG
    vstring unificationPairsToString(){
      vstring res;
      res =  "PRINTING THE UNIFICATION PAIRS \n";
      for(int i = _unificationPairs.size() -1; i >=0; i--){
         res += "<" + _unificationPairs[i].unifPair.first.first.toString() + " , " + 
                       _unificationPairs[i].unifPair.second.first.toString()  + ">\n";
      }
      return res;
    }
    
    vstring algorithmStepToString(AlgorithmStep as){
      switch(as){
       case SPLIT:
         return "SPLIT";
       case I_NARROW:
         return "I_NARROW";
       case K_NARROW:
         return "K_NARROW";
       case KX_NARROW:
         return "KX_NARROW";
       case B_NARROW:
         return "B_NARROW";
       case BX_NARROW:
         return "BX_NARROW";
       case C_NARROW:
         return "C_NARROW";
       case CX_NARROW:
         return "CX_NARROW";
       case S_NARROW:
         return "S_NARROW";
       case SX_NARROW:
         return "SX_NARROW";
       case I_REDUCE:
         return "I_REDUCE";
       case K_REDUCE:
         return "K_REDUCE";
       case B_REDUCE:
         return "B_REDUCE";
       case C_REDUCE:
         return "C_REDUCE";
       case S_REDUCE:
         return "S_REDUCE";
       case DECOMP:
         return "DECOMP";
       case ELIMINATE:  
         return "ELIMINATE";       
       default:
         return "UNKNOWN";
      }
    }
   #endif   
    
    TransformIterator availableTransforms();
    /*
     * Finds all relevant trandformations for top unif pair 
     * in _unifcationPairs of _unifSystem. Populates transformation
     * stacks.
     */
    void populateTransformations(UnificationPair&);   
    void populateSide(HSH::HOTerm&, ApplyTo, Stack<Transform>&,AlgorithmStep,AlgorithmStep);
    /** returns the particular narrow step relevant to the arg */
    AlgorithmStep reduceStep(HSH::HOTerm&);
    /** Carry out transformation represented bt t on top pair*/ 
    bool transform(Transform t);

    void transform(HSH::HOTerm&,AlgorithmStep,int);
    void iRdeuce(HSH::HOTerm&);
    void kReduce(HSH::HOTerm&);
    void bcsReduce(HSH::HOTerm&, AlgorithmStep);

    bool canPerformStep(AlgorithmStep, AlgorithmStep, AlgorithmStep);
    bool occurs(const VarSpec, const HOTermSpec);
    void eliminate(VarSpec, HSH::HOTerm);
    void eliminate(VarSpec, HSH::HOTerm, HOTermSpec&);
    void addToSolved(VarSpec, HSH::HOTerm);
    void pushNewPair(HSH::HOTerm&, int, HSH::HOTerm&, int, AlgorithmStep,
                     AlgorithmStep, bool leftChanged = true, bool rightChanged = true);
    void pushNewPair(HOTermSpec&, HOTermSpec&, AlgorithmStep, AlgorithmStep, 
                     bool leftChanged = true, bool rightChanged = true);
                     
    inline HSH::HOTerm newVar(unsigned sort){
      HSH::HOTerm ht = HSH::HOTerm(TermList(_nextFreshVar), sort);
      _nextFreshVar++;
      return ht;
    }

    //if subsitution represents solved system _solved set to true
    bool _solved;
    unsigned _nextFreshVar;

    Stack<UnificationPair> _unificationPairs;
    
    typedef DHMap<VarSpec,HSH::HOTerm,VarSpec::Hash1, VarSpec::Hash2> SolvedType;
    mutable SolvedType _solvedPairs;

    class BindingBacktrackObject
    : public BacktrackObject
    {
    public:
      BindingBacktrackObject(CombSubstitution* subst, VarSpec v)
      :_subst(subst), _var(v)
      {}

      void backtrack()
      {
        _subst->_solvedPairs.remove(_var);
      }

      CLASS_NAME(CombSubstitution::BindingBacktrackObject);
      USE_ALLOCATOR(BindingBacktrackObject);
    private:
      CombSubstitution* _subst;
      VarSpec _var;
    };

    class StackBacktrackObject
    : public BacktrackObject
    {
    public:
      StackBacktrackObject(CombSubstitution* subst, Stack<UnificationPair> st)
      :_subst(subst), _st(st)
      {}
      
      void backtrack()
      {
        // Not particularly elegant or efficient. 
        // Just resetting the whole stack
        // Should only be resetting elements that have changed.
        _subst->_unificationPairs = _st;
      }

      CLASS_NAME(CombSubstitution::StackBacktrackObject);
      USE_ALLOCATOR(StackBacktrackObject);
    private:
      CombSubstitution* _subst;
       Stack<UnificationPair> _st;
    };

    friend class CombSubstIterator;
};



class CombSubstIterator
: public IteratorCore<CombSubstitution*>
{
public:
  CLASS_NAME(CombSubstIterator);
  USE_ALLOCATOR(CombSubstIterator);
  
  CombSubstIterator(TermList t1,int index1, TermList t2, int index2)
  {
    _unifSystem = new CombSubstitution(t1, index1, t2, index2);
    transformIterators.push(_unifSystem->availableTransforms());
  }

  bool hasNext(){
    return hasNextUnifier();
  }
  /* 
   * Only valid if has been previous call to hasNext that
   * has returned true! Calling next multiple times for one
   * call of hasNext will result in the same unifier being return
   * multiple times.
   */
  CombSubstitution* next(){
    return _unifSystem;
  }
  
  
  /** struct containing first hash function of TTPair objects*/
  /*struct TTPairHash
  {
   static unsigned hash(TTPair& o)
   {
     return IdentityHash::hash(o.first.term.content())^o.first.index ^
       ((IdentityHash::hash(o.second.term.content())^o.second.index)<<1);
   }
  };
  */

private:

  typedef CombSubstitution::AlgorithmStep AlgorithmStep;
  typedef CombSubstitution::ApplyTo ApplyTo;
  typedef pair<AlgorithmStep,ApplyTo> Transform;
  typedef VirtualIterator<Transform> TransformIterator;

  /** Copy constructor is private and without a body, because we don't want any. */
  CombSubstIterator(const CombSubstIterator& obj);
  /** operator= is private and without a body, because we don't want any. */
  CombSubstIterator& operator=(const CombSubstIterator& obj);


  CombSubstitution* _unifSystem;
  Stack<TransformIterator> transformIterators;
  Stack<BacktrackData> bdStack;
  
  //These or similar functions required in CombSubsitution class
  //void bind(const VarSpec& v, const TermSpec& b);
  //void bindVar(const VarSpec& var, const VarSpec& to);

  bool hasNextUnifier();
  /** apply transformation t to the top unification pair in current system
   *  record any chanegs in bd so that transformation can be reversed
   */
  bool transform(Transform t, BacktrackData& bd);

#if VDEBUG

#endif
  
};

}
#endif /*__CombSubstIterator____*/