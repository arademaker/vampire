/**
 * @file ProofSimplifier.cpp
 * Implements class ProofSimplifier.
 */

#include "Kernel/Clause.hpp"
#include "Kernel/FormulaUnit.hpp"
#include "Kernel/Inference.hpp"

#include "Flattening.hpp"

#include "ProofSimplifier.hpp"

namespace Shell
{

ProofTransformer::ProofTransformer(UnitSpec refutation)
{
  CALL("ProofTransformer::ProofTransformer");

  _refutation = refutation;
}

void ProofTransformer::perform()
{
  CALL("ProofTransformer::perform");

  loadProof(_refutation, _origProof);

  preTransform();

  Stack<UnitSpec>::BottomFirstIterator prIt(_origProof);
  while(prIt.hasNext()) {
    UnitSpec u = prIt.next();
    UnitSpec tgt = transformUnit(u);
    registerTransformation(u, tgt);
    if(!tgt.isEmpty()) {
      derefInference(u, tgt);
      _newProof.push(tgt);
      //maybe we've simplified some unit into a refutation earlier...
      if(isRefutation(tgt)) {
	LOG("pt_units","proof transf:"<<endl<<
	      "  src: "<<u.toString()<<endl<<
	      "  tgt: "<<(tgt.isEmpty() ? "<empty>" : tgt.toString()));
	break;
      }
    }
    LOG("pt_units","proof transf:"<<endl<<
	"  src: "<<u.toString()<<endl<<
	"  tgt: "<<(tgt.isEmpty() ? "<empty>" : tgt.toString()));
  }
}

void ProofTransformer::registerTransformation(UnitSpec src, UnitSpec tgt)
{
  CALL("ProofTransformer::registerTransformation");
  ASS(!src.isEmpty());
  ALWAYS(_transformationMap.insert(src, tgt));
}

/**
 * Take the inference of src, transform the premises and assign it to tgt
 *
 * Parents of src must have been already processed and their transformation registered.
 */
void ProofTransformer::derefInference(UnitSpec src, UnitSpec tgt)
{
  CALL("derefInference");

  InferenceStore* inf = InferenceStore::instance();

  static Stack<UnitSpec> tgtPrems;
  tgtPrems.reset();

  Inference::Rule rule;
  UnitSpecIterator pit = inf->getParents(src, rule);
  while(pit.hasNext()) {
    UnitSpec srcPar = pit.next();
    UnitSpec tgtPar = _transformationMap.get(srcPar);
    if(!tgtPar.isEmpty()) {
      tgtPrems.push(tgtPar);
    }
  }

  unsigned premCnt = tgtPrems.size();
  InferenceStore::FullInference* finf = new(premCnt) InferenceStore::FullInference(premCnt);
  finf->rule = rule;
  for(unsigned i=0; i<premCnt; i++) {
    finf->premises[i] = tgtPrems[i];
  }
  inf->recordInference(tgt, finf);
}

bool ProofTransformer::isRefutation(UnitSpec u)
{
  CALL("ProofTransformer::isRefutation");
  ASS(!u.isEmpty());

  if(!u.prop()->isFalse()) { return false; }
  if(u.isClause()) { return u.cl()->isEmpty(); }
  FormulaUnit* fu = static_cast<FormulaUnit*>(u.unit());
  return fu->formula()->connective()==FALSE;
}

void ProofTransformer::loadProof(UnitSpec refutation, Stack<UnitSpec>& tgt)
{
  CALL("ProofTransformer::loadProof");

  static DHSet<UnitSpec> processed;
  processed.reset();

  static Stack<UnitSpec> stack;
  stack.reset();
  stack.push(refutation);

  while(stack.isNonEmpty()) {
    if(stack.top().isEmpty()) {
      stack.pop();
      ASS(!stack.top().isEmpty());
      UnitSpec proc = stack.pop();
      if(processed.insert(proc)) {
	tgt.push(proc);
      }
      continue;
    }
    UnitSpec current = stack.top();
    ASS(!current.isEmpty());
    if(processed.find(current)) {
      stack.pop();
      continue;
    }
    stack.push(UnitSpec(0));
    UnitSpecIterator pit = InferenceStore::instance()->getParents(current);
    stack.loadFromIterator(pit);
  }
}

///////////////////////
// ProofSimplifier
//

ProofSimplifier::ProofSimplifier(const Problem& prb, UnitSpec refutation, UnitList* defs)
 : ProofTransformer(refutation), _prb(prb), _defs(defs), _aig(_inl.aig()), _fsh(_inl.fsh()), _bddAig(_aig)
{
  _bddAig.loadBDDAssignmentFromProblem(prb);
}

AIGRef ProofSimplifier::getAIG(UnitSpec u)
{
  CALL("ProofSimplifier::getAIG");

  AIGRef bddA = _bddAig.b2a(u.prop());

  AIGRef formA;
  if(u.isClause()) {
    formA = _fsh.getAIG(u.cl());
  }
  else {
    FormulaUnit* fu = static_cast<FormulaUnit*>(u.unit());
    formA = _fsh.apply(fu->formula()).second;
  }

  AIGRef a = _aig.getDisj(bddA, formA);
  return a;
}

void ProofSimplifier::preTransform()
{
  CALL("ProofSimplifier::preTransform");

  Stack<UnitSpec>::BottomFirstIterator pit(_origProof);
  while(pit.hasNext()) {
    UnitSpec u = pit.next();
    AIGRef a = getAIG(u);
    _inl.addRelevant(a);
  }
  _inl.scan(_defs);
}

UnitSpec ProofSimplifier::transformUnit(UnitSpec u)
{
  CALL("ProofSimplifier::transformUnit");

  AIGRef a = getAIG(u);
  LOGV("pt_simpl_aig", a);
  AIGRef simplA = _inl.apply(a);
  LOGV("pt_simpl_aig", simplA);
  if(simplA.isTrue()) {
    return UnitSpec(0);
  }
  Formula* form = _fsh.aigToFormula(simplA);
  form = Flattening::flatten(form);
  FormulaUnit* res = new FormulaUnit(form, new Inference(Inference::TAUTOLOGY_INTRODUCTION), u.unit()->inputType());
  return UnitSpec(res);
}

}
