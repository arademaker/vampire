/**
 * @file Shell/Preprocess.cpp
 * Implements class Preprocess for preprocessing.
 * @since 05/01/2004 Manchester
 * @since 02/06/2007 Manchester, changed to new datastructures
 */

#include "Debug/Tracer.hpp"

#include "Kernel/Unit.hpp"
#include "Kernel/Clause.hpp"

#include "CNF.hpp"
#include "EPRInlining.hpp"
#include "EqResWithDeletion.hpp"
#include "EqualityPropagator.hpp"
#include "EqualityProxy.hpp"
#include "Flattening.hpp"
#include "FormulaIteExpander.hpp"
#include "FunctionDefinition.hpp"
#include "GeneralSplitting.hpp"
#include "InequalitySplitting.hpp"
#include "Naming.hpp"
#include "Normalisation.hpp"
#include "NNF.hpp"
#include "Options.hpp"
#include "PDInliner.hpp"
#include "PredicateDefinition.hpp"
#include "Preprocess.hpp"
#include "Property.hpp"
#include "Rectify.hpp"
#include "Skolem.hpp"
#include "SimplifyFalseTrue.hpp"
#include "SineUtils.hpp"
#include "Statistics.hpp"
#include "SpecialTermElimination.hpp"
#include "TheoryAxioms.hpp"

// #include "Lib/Sort.hpp"
// #include "ClausalDefinition.hpp"
// #include "Definition.hpp"
// #include "Environment.hpp"
// #include "EqualityProxy.hpp"
// #include "LiteralOrderingFinder.hpp"
// #include "Miniscope.hpp"
// #include "Ordering.hpp"
// #include "RowVariable.hpp"
// #include "TheoryFinder.hpp"
// #include "Problem.hpp"
// #include "PureLiteral.hpp"
// #include "Tautology.hpp"

using namespace Shell;

/**
 * Preprocess the problem.
 *
 * @since 16/07/2003 hotel Holiday Inn, Moscow, normalization added
 * @since 19/12/2003 Manchester, changed to move preprocessing to units.
 * @since 08/04/2004 Torrevieja pure literal deletion and
 *   clausal definition handling added
 * @since 12/04/2004 Torrevieja tautology and double literal deletion added
 * @since 22/05/2004 Manchester, equality proxy added
 */
void Preprocess::preprocess (UnitList*& units)
{
  CALL("Preprocess::preprocess");

  // before any preprocessing, row variables must be expanded
//   if (RowVariable::occurredInInput &&
//       _options.rowVariableMaxLength() >= 0) {
//     RowVariable rv(_options.rowVariableMaxLength());
//     UnitChain::DelIterator iterator (_problem.giveUnits());
//     while (iterator.hasNext()) {
//       Unit u (iterator.next());
//       if (u.unitType() == FORMULA) {
// 	UnitList us;
// 	if (rv.expandRowVariables(u,us)) {
// 	  ASS(us.isNonEmpty());
// 	  if (us.head() != u) { // there was a row variable expansion
// 	    VL::Iterator<Unit> newUnits(us);
// 	    while (newUnits.hasNext()) {
//   	      iterator.insert(newUnits.next());
// 	    }
// 	    iterator.del();
// 	  }
// 	}
// 	else { // non-expandable row variable found
// 	  iterator.del();
// 	}
//       }
//     }
//   }

  SpecialTermElimination().apply(units);

  // reorder units
  if (_options.normalize()) {
    env.statistics->phase=Statistics::NORMALIZATION;
    Normalisation norm;
    units = norm.normalise(units);
  }

  if(env.options->sineSelection()!=Options::SS_OFF) {
    env.statistics->phase=Statistics::SINE_SELECTION;
    SineSelector().perform(units);
  }

  if(env.options->theoryAxioms()) {
    env.statistics->phase=Statistics::INCLUDING_THEORY_AXIOMS;
    TheoryAxioms().apply(units, &_property);
  }

  {
    env.statistics->phase=Statistics::PREPROCESS_1;
    UnitList::DelIterator us(units);
    while (us.hasNext()) {
      Unit* u = us.next();
      Unit* v = preprocess1(u);
      if (v != u) {
	us.replace(v);
      }
    }
  }

  // discover known theories
//   TheoryFinder theoryFinder(_problem,0);
//   theoryFinder.search();

  {
    FormulaIteExpander().apply(units);
  }

  if (_options.equalityPropagation()) {
    //Another equality propagation is between preprocess2 and preprocess3.
    //the ENNF form there allows us to propagate more equalities.
    //Here we're trying to propagate equalities as well because that might
    //reveal some more formulas to be definitions.
    env.statistics->phase=Statistics::EQUALITY_PROPAGATION;
    EqualityPropagator().apply(units);
  }

  if (_options.eprPreservingSkolemization()) {
    env.statistics->phase=Statistics::EPR_PRESERVING_SKOLEMIZATION;
    EPRSkolem().apply(units);
  }

  if (_options.eprRestoringInlining()) {
    env.statistics->phase=Statistics::PREDICATE_DEFINITION_INLINING;
    EPRInlining().apply(units);
  }

  if (_options.predicateDefinitionInlining()!=Options::INL_OFF) {
    env.statistics->phase=Statistics::PREDICATE_DEFINITION_INLINING;
    PDInliner pdInliner(_options.predicateDefinitionInlining()==Options::INL_AXIOMS_ONLY);
    pdInliner.apply(units);
  }

  if (_options.unusedPredicateDefinitionRemoval()) {
    env.statistics->phase=Statistics::UNUSED_PREDICATE_DEFINITION_REMOVAL;
    PredicateDefinition pdRemover;
    pdRemover.removeUnusedDefinitionsAndPurePredicates(units);
  }

  if (_property.hasFormulas()) {
    env.statistics->phase=Statistics::PREPROCESS_2;
    UnitList::DelIterator us(units);
    while (us.hasNext()) {
      Unit* u = us.next();
      if(u->isClause()) {
	continue;
      }
      Unit* v = preprocess2(static_cast<FormulaUnit*>(u));
      if (v != u) {
	us.replace(v);
      }
    }
  }

  if (_options.equalityPropagation()) {
    env.statistics->phase=Statistics::EQUALITY_PROPAGATION;
    EqualityPropagator().apply(units);
  }

  if (_property.hasFormulas() && _options.naming()) {
    env.statistics->phase=Statistics::NAMING;
    UnitList::DelIterator us(units);
    Naming naming(_options.naming(), _options.eprPreservingNaming());
    while (us.hasNext()) {
      Unit* u = us.next();
      if (u->isClause()) {
	continue;
      }
      UnitList* defs;
      FormulaUnit* fu = static_cast<FormulaUnit*>(u);
      FormulaUnit* v = naming.apply(fu,defs);
      if (v != fu) {
	ASS(defs);
 	us.insert(defs);
	us.replace(v);
      }
    }
  }

  {
    env.statistics->phase=Statistics::PREPROCESS_3;
    UnitList::DelIterator us(units);
    while (us.hasNext()) {
      Unit* u = us.next();
      Unit* v = preprocess3(u);
      if (v != u) {
	us.replace(v);
      }
    }
  }

  //we redo the naming if the last naming was restricted by preserving EPR
  if (_property.hasFormulas() && _options.naming() && _options.eprPreservingNaming()) {
    env.statistics->phase=Statistics::NAMING;
    UnitList::DelIterator us(units);
    Naming naming(min(_options.naming()*2+1,32767), true);
    while (us.hasNext()) {
      Unit* u = us.next();
      if (u->isClause()) {
	continue;
      }
      UnitList* defs;
      FormulaUnit* fu = static_cast<FormulaUnit*>(u);
      FormulaUnit* v = naming.apply(fu,defs);
      if (v != fu) {
	ASS(defs);
	while(defs) {
	  Unit* d=preprocess3(UnitList::pop(defs));
	  us.insert(new UnitList(d,0));
	}
	us.replace(v);
      }
    }
  }

//   // find ordering on literals
//   Ordering* ordering = environment.ordering(0);
//   if (ordering) {
//     ordering->fill(Signature::sig);
//   }
//   else {
//     LiteralOrderingFinder lof(_problem,_options,Signature::sig);
//     lof.findOrdering();
//   }

  if (_property.hasFormulas()) {
    env.statistics->phase=Statistics::CLAUSIFICATION;
    UnitList::DelIterator us(units);
    CNF cnf;
    Stack<Clause*> clauses(32);
    while (us.hasNext()) {
      Unit* u = us.next();
      if (! u->isClause()) {
	cnf.clausify(u,clauses);
	while (! clauses.isEmpty()) {
	  Unit* u = clauses.pop();
	  us.insert(u);
	}
	us.del();
      }
    }
  }

  if(_property.hasProp(Property::PR_HAS_FUNCTION_DEFINITIONS)) {
    env.statistics->phase=Statistics::FUNCTION_DEFINITION_ELIMINATION;
    if(_options.functionDefinitionElimination() == Options::FDE_ALL) {
      FunctionDefinition fd;
      fd.removeAllDefinitions(units);
    }
    else if(_options.functionDefinitionElimination() == Options::FDE_UNUSED) {
      FunctionDefinition::removeUnusedDefinitions(units);
    }
  }


  if (_options.inequalitySplitting() != 0) {
    env.statistics->phase=Statistics::INEQUALITY_SPLITTING;
    InequalitySplitting is;
    is.perform(units);
  }

//   // remove tautologies, duplicate literals, and literals t != t
//   UnitChain::DelIterator units (_problem.giveUnits());
//   while (units.hasNext()) {
//     Unit newUnit;
//     switch (Tautology::isTautology(units.next(),newUnit)) {
//     case -1:
//       units.replace(newUnit);
//       break;

//     case 0:
//       break;

//     case 1:
//       units.del();
//       break;
//     }
//   }

   if (_options.equalityResolutionWithDeletion()!=Options::RA_OFF &&
	   _property.hasProp(Property::PR_HAS_INEQUALITY_RESOLVABLE_WITH_DELETION) ) {
     env.statistics->phase=Statistics::EQUALITY_RESOLUTION_WITH_DELETION;
     EqResWithDeletion resolver;
     resolver.apply(units);
   }

   if (_options.equalityProxy()!=Options::EP_OFF &&
	   (_property.hasProp(Property::PR_HAS_X_EQUALS_Y) ||
	    _options.equalityProxy()!=Options::EP_ON) ) {
     env.statistics->phase=Statistics::EQUALITY_PROXY;
     EqualityProxy proxy;
     proxy.apply(units);
   }

   if (_options.generalSplitting()!=Options::RA_OFF) {
     env.statistics->phase=Statistics::GENERAL_SPLITTING;
     GeneralSplitting gs;
     gs.apply(units);
   }

} // Preprocess::preprocess ()


/**
 * Preprocess the unit using options from opt. Preprocessing may
 * involve inferences and replacement of this unit by a newly inferred one.
 * Preprocessing formula units consists of the following steps:
 * <ol>
 *   <li>Rectify the formula and memorise the answer atom, if necessary.</li>
 *   <li>Flatten the formula.</li>
 * </ol>
 *
 * Preprocessing clause does not change it.
 */
Unit* Preprocess::preprocess1 (Unit* unit)
{
  CALL("Preprocess::preprocess1");

  if (unit->isClause()) {
    return unit;
  }

  // formula unit
  FormulaUnit* fu = static_cast<FormulaUnit*>(unit);
  // Rectify the formula and memorise the answer atom, if necessary
  fu = Rectify::rectify(fu);
  // Simplify the formula if it contains true or false
  fu = SimplifyFalseTrue::simplify(fu);
  fu = Flattening::flatten(fu);
  return fu;
} // Preprocess::preprocess1


/**
 * Preprocess the unit using options from opt. Preprocessing may
 * involve inferences and replacement of this unit by a newly inferred one.
 * Preprocessing formula units consists of the following steps:
 * <ol>
 *   <li>Transform the formula to ENNF.</li>
 *   <li>Flatten it.</li>
 * </ol>
 * @since 14/07/2005 flight Tel-Aviv-Barcelona changed to stop before naming
 */
FormulaUnit* Preprocess::preprocess2 (FormulaUnit* unit)
{
  CALL("Preprocess::preprocess2");
  ASS (! unit->isClause());

  unit = NNF::ennf(unit);
  return Flattening::flatten(unit);
} // Peprocess::preprocess2


/**
 * Preprocess the unit using options from opt. Preprocessing may
 * involve inferences and replacement of this unit by a newly inferred one.
 * Preprocessing formula units consists of the following steps:
 * <ol>
 *   <li>Transform the formula to NNF.</li>
 *   <li>Flatten it.</li>
 *   <li>(Optional) miniscope the formula.</li>
 * </ol>
 * @since 14/07/2005 flight Tel-Aviv-Barcelona
 */
Unit* Preprocess::preprocess3 (Unit* unit)
{
  CALL("Preprocess::preprocess3");

  if (unit->isClause()) {
    return unit;
  }
  FormulaUnit* fu = static_cast<FormulaUnit*>(unit);
  // Transform the formula to NNF
  fu = NNF::nnf(fu);
  // flatten it
  fu = Flattening::flatten(fu);
  // (Optional) miniscope the formula
//     if (_options.miniscope()) {
//       Miniscope::miniscope(fu);
//     }
//   return unit;
  return Skolem::skolemise(fu);
} // Peprocess::preprocess3


