/**
 * @file ShortConflictMetaDP.hpp
 * Defines class ShortConflictMetaDP.
 */

#ifndef __ShortConflictMetaDP__
#define __ShortConflictMetaDP__

#include "Forwards.hpp"

#include "Lib/ScopedPtr.hpp"
#include "Lib/Stack.hpp"

#include "SAT/SAT2FO.hpp"
#include "SAT/SATSolver.hpp"

#include "DecisionProcedure.hpp"


namespace DP {

using namespace Lib;
using namespace Kernel;
using namespace SAT;

class ShortConflictMetaDP : public DecisionProcedure {
public:
  /**
   * Create object using @c inner decision procedure. Object takes ownership of the @c inner object.
   */
  ShortConflictMetaDP(DecisionProcedure* inner, SAT2FO& sat2fo, SATSolver& solver)
  : _inner(inner), _sat2fo(sat2fo), _solver(solver) {}

  virtual void addLiterals(LiteralIterator lits) {
    CALL("ShortConflictMetaDP::addLiterals");
    _inner->addLiterals(lits);
  }

  virtual void reset() {
    CALL("ShortConflictMetaDP::reset");
    _inner->reset();
    _unsatCores.reset();
  }

  virtual Status getStatus(bool getMultipleCores);

  /**
   * Return number of unsatisfiable cores that can be retrieved.
   * 0 is returned if the status is SATISFIABLE or UNKNOWN. If UNSATISFIABLE,
   * number greater than zero is returned.
   *
   * Can be called only after getStatus before any next call to addLiterals.
   */
  virtual unsigned getUnsatCoreCount() { return _unsatCores.size(); }
  virtual void getUnsatCore(LiteralStack& res, unsigned coreIndex);


private:
  unsigned getCoreSize(const LiteralStack& core);


  Stack<LiteralStack> _unsatCores;

  ScopedPtr<DecisionProcedure> _inner;
  SAT2FO& _sat2fo;
  SATSolver& _solver;
};

}

#endif // __ShortConflictMetaDP__