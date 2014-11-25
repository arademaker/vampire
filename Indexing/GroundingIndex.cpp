/**
 * @file GroundingIndex.cpp
 * Implements class GroundingIndex.
 */

#include "GroundingIndex.hpp"

#include "Lib/SharedSet.hpp"

#include "Kernel/Grounder.hpp"
#include "Kernel/Inference.hpp"

#include "Shell/Options.hpp"

#include "SAT/TWLSolver.hpp"
#include "SAT/LingelingInterfacing.hpp"
#include "SAT/MinisatInterfacing.hpp"
#include "SAT/BufferedSolver.hpp"

#include "Saturation/SaturationAlgorithm.hpp"

namespace Indexing
{

GroundingIndex::GroundingIndex(Grounder* gnd, const Options& opt)
 : _grounder(gnd)
{
  CALL("GroundingIndex::GroundingIndex");

  switch(opt.satSolver()){
    case Options::SatSolver::VAMPIRE:
    	_solver = new TWLSolver(opt,true);
    	break;
    case Options::SatSolver::BUFFERED_VAMPIRE:
    	_solver = new BufferedSolver(new TWLSolver(opt,true));
    	break;
    case Options::SatSolver::BUFFERED_LINGELING:
    	_solver = new BufferedSolver(new LingelingInterfacing(opt, true));
    	break;
    case Options::SatSolver::LINGELING:
      _solver = new LingelingInterfacing(opt,true);
      break;
    case Options::SatSolver::BUFFERED_MINISAT:
    	_solver = new BufferedSolver(new MinisatInterfacing(opt, true));
    	break;
    case Options::SatSolver::MINISAT:
      _solver = new MinisatInterfacing(opt,true);
      break;
    default:
      // static_cast necessary due to use of class enums
      ASSERTION_VIOLATION_REP(static_cast<unsigned>(opt.satSolver()));
  }

}

void GroundingIndex::handleClause(Clause* c, bool adding)
{
  CALL("GroundingIndex::handleClause");

  //We are adding clauses into the index when performing the subsumption check
}

}
