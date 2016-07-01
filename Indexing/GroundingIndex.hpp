/**
 * @file GroundingIndex.hpp
 * Defines class GroundingIndex.
 */

#ifndef __GroundingIndex__
#define __GroundingIndex__

#include "Forwards.hpp"

#include "Lib/ScopedPtr.hpp"

#include "SAT/SAT2FO.hpp"
#include "SAT/SATSolver.hpp"

#include "Index.hpp"

namespace Indexing {

using namespace SAT;
using namespace Shell;

class GroundingIndex : public Index {
public:
  CLASS_NAME(GroundingIndex);
  USE_ALLOCATOR(GroundingIndex);

  GroundingIndex(const Options& opt);

  SATSolverWithAssumptions& getSolver() { return *_solver; }
  GlobalSubsumptionGrounder& getGrounder() { return *_grounder; }

  SAT2FO& satNaming(){ return _sat2fo; }

protected:
  virtual void handleClause(Clause* c, bool adding);

private:
  SAT2FO _sat2fo;
  ScopedPtr<SATSolverWithAssumptions> _solver;
  ScopedPtr<GlobalSubsumptionGrounder> _grounder;
};

}

#endif // __GroundingIndex__
