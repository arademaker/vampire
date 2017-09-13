/*********************                                                        */
/** kind.h
 **
 ** Copyright 2010-2014  New York University and The University of Iowa,
 ** and as below.
 **
 ** This header file automatically generated by:
 **
 **     ../../../../../src/expr/mkkind ../../../../../src/expr/kind_template.h ../../../../../src/theory/builtin/kinds ../../../../../src/theory/booleans/kinds ../../../../../src/theory/uf/kinds ../../../../../src/theory/arith/kinds ../../../../../src/theory/bv/kinds ../../../../../src/theory/fp/kinds ../../../../../src/theory/arrays/kinds ../../../../../src/theory/datatypes/kinds ../../../../../src/theory/sep/kinds ../../../../../src/theory/sets/kinds ../../../../../src/theory/strings/kinds ../../../../../src/theory/quantifiers/kinds ../../../../../src/theory/idl/kinds
 **
 ** for the CVC4 project.
 **/

/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */

/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */

/* Edit the template file instead:                     */
/* ../../../../../src/expr/kind_template.h */

/*********************                                                        */
/*! \file kind_template.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Morgan Deters, Dejan Jovanovic, Paul Meng
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2017 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Template for the Node kind header
 **
 ** Template for the Node kind header.
 **/

#include "cvc4_public.h"

#ifndef __CVC4__KIND_H
#define __CVC4__KIND_H

#include <iosfwd>

#include "base/exception.h"

namespace CVC4 {
namespace kind {

enum CVC4_PUBLIC Kind_t {
  UNDEFINED_KIND = -1, /**< undefined */
  NULL_EXPR, /**< Null kind */

  /* from builtin */
  SORT_TAG, /**< sort tag (1) */
  SORT_TYPE, /**< specifies types of user-declared 'uninterpreted' sorts (2) */
  UNINTERPRETED_CONSTANT, /**< the kind of expressions representing uninterpreted constants; payload is an instance of the CVC4::UninterpretedConstant class (used in models) (3) */
  ABSTRACT_VALUE, /**< the kind of expressions representing abstract values (other than uninterpreted sort constants); payload is an instance of the CVC4::AbstractValue class (used in models) (4) */
  BUILTIN, /**< the kind of expressions representing built-in operators (5) */
  FUNCTION, /**< a defined function (6) */
  APPLY, /**< application of a defined function (7) */
  EQUAL, /**< equality (two parameters only, sorts must match) (8) */
  DISTINCT, /**< disequality (N-ary, sorts must match) (9) */
  VARIABLE, /**< a variable (not permitted in bindings) (10) */
  BOUND_VARIABLE, /**< a bound variable (permitted in bindings and the associated lambda and quantifier bodies only) (11) */
  SKOLEM, /**< a Skolem variable (internal only) (12) */
  SEXPR, /**< a symbolic expression (any arity) (13) */
  LAMBDA, /**< a lambda expression; first parameter is a BOUND_VAR_LIST, second is lambda body (14) */
  CHAIN, /**< chained operator (N-ary), turned into a conjunction of binary applications of the operator on adjoining parameters; first parameter is a CHAIN_OP representing a binary operator, rest are arguments to that operator (15) */
  CHAIN_OP, /**< the chained operator; payload is an instance of the CVC4::Chain class (16) */
  TYPE_CONSTANT, /**< a representation for basic types (17) */
  FUNCTION_TYPE, /**< a function type (18) */
  SEXPR_TYPE, /**< the type of a symbolic expression (19) */

  /* from booleans */
  CONST_BOOLEAN, /**< truth and falsity; payload is a (C++) bool (20) */
  NOT, /**< logical not (21) */
  AND, /**< logical and (N-ary) (22) */
  IMPLIES, /**< logical implication (exactly two parameters) (23) */
  OR, /**< logical or (N-ary) (24) */
  XOR, /**< exclusive or (exactly two parameters) (25) */
  ITE, /**< if-then-else, used for both Boolean and term ITE constructs; first parameter is (Boolean-sorted) condition, second is 'then', third is 'else' and these two parameters must have same base sort (26) */

  /* from uf */
  APPLY_UF, /**< application of an uninterpreted function; first parameter is the function, remaining ones are parameters to that function (27) */
  BOOLEAN_TERM_VARIABLE, /**< Boolean term variable (28) */
  CARDINALITY_CONSTRAINT, /**< cardinality constraint on sort S: first parameter is (any) term of sort S, second is a positive integer constant k that bounds the cardinality of S (29) */
  COMBINED_CARDINALITY_CONSTRAINT, /**< combined cardinality constraint; parameter is a positive integer constant k that bounds the sum of the cardinalities of all sorts in the signature (30) */
  PARTIAL_APPLY_UF, /**< partial uninterpreted function application (31) */
  CARDINALITY_VALUE, /**< cardinality value of sort S: first parameter is (any) term of sort S (32) */

  /* from arith */
  PLUS, /**< arithmetic addition (N-ary) (33) */
  MULT, /**< arithmetic multiplication (N-ary) (34) */
  NONLINEAR_MULT, /**< synonym for MULT (35) */
  MINUS, /**< arithmetic binary subtraction operator (36) */
  UMINUS, /**< arithmetic unary negation (37) */
  DIVISION, /**< real division, division by 0 undefined (user symbol) (38) */
  DIVISION_TOTAL, /**< real division with interpreted division by 0 (internal symbol) (39) */
  INTS_DIVISION, /**< integer division, division by 0 undefined (user symbol) (40) */
  INTS_DIVISION_TOTAL, /**< integer division with interpreted division by 0 (internal symbol) (41) */
  INTS_MODULUS, /**< integer modulus, division by 0 undefined (user symbol) (42) */
  INTS_MODULUS_TOTAL, /**< integer modulus with interpreted division by 0 (internal symbol) (43) */
  ABS, /**< absolute value (44) */
  DIVISIBLE, /**< divisibility-by-k predicate; first parameter is a DIVISIBLE_OP, second is integer term (45) */
  POW, /**< arithmetic power (46) */
  EXPONENTIAL, /**< exponential (47) */
  SINE, /**< sine (48) */
  COSINE, /**< consine (49) */
  TANGENT, /**< tangent (50) */
  DIVISIBLE_OP, /**< operator for the divisibility-by-k predicate; payload is an instance of the CVC4::Divisible class (51) */
  CONST_RATIONAL, /**< a multiple-precision rational constant; payload is an instance of the CVC4::Rational class (52) */
  LT, /**< less than, x < y (53) */
  LEQ, /**< less than or equal, x <= y (54) */
  GT, /**< greater than, x > y (55) */
  GEQ, /**< greater than or equal, x >= y (56) */
  IS_INTEGER, /**< term-is-integer predicate (parameter is a real-sorted term) (57) */
  TO_INTEGER, /**< convert term to integer by the floor function (parameter is a real-sorted term) (58) */
  TO_REAL, /**< cast term to real (parameter is an integer-sorted term; this is a no-op in CVC4, as integer is a subtype of real) (59) */
  PI, /**< pi (60) */

  /* from bv */
  BITVECTOR_TYPE, /**< bit-vector type (61) */
  CONST_BITVECTOR, /**< a fixed-width bit-vector constant; payload is an instance of the CVC4::BitVector class (62) */
  BITVECTOR_CONCAT, /**< concatenation of two or more bit-vectors (63) */
  BITVECTOR_AND, /**< bitwise and of two or more bit-vectors (64) */
  BITVECTOR_OR, /**< bitwise or of two or more bit-vectors (65) */
  BITVECTOR_XOR, /**< bitwise xor of two or more bit-vectors (66) */
  BITVECTOR_NOT, /**< bitwise not of a bit-vector (67) */
  BITVECTOR_NAND, /**< bitwise nand of two bit-vectors (68) */
  BITVECTOR_NOR, /**< bitwise nor of two bit-vectors (69) */
  BITVECTOR_XNOR, /**< bitwise xnor of two bit-vectors (70) */
  BITVECTOR_COMP, /**< equality comparison of two bit-vectors (returns one bit) (71) */
  BITVECTOR_MULT, /**< multiplication of two or more bit-vectors (72) */
  BITVECTOR_PLUS, /**< addition of two or more bit-vectors (73) */
  BITVECTOR_SUB, /**< subtraction of two bit-vectors (74) */
  BITVECTOR_NEG, /**< unary negation of a bit-vector (75) */
  BITVECTOR_UDIV, /**< unsigned division of two bit-vectors, truncating towards 0 (undefined if divisor is 0) (76) */
  BITVECTOR_UREM, /**< unsigned remainder from truncating division of two bit-vectors (undefined if divisor is 0) (77) */
  BITVECTOR_SDIV, /**< 2's complement signed division (78) */
  BITVECTOR_SREM, /**< 2's complement signed remainder (sign follows dividend) (79) */
  BITVECTOR_SMOD, /**< 2's complement signed remainder (sign follows divisor) (80) */
  BITVECTOR_UDIV_TOTAL, /**< unsigned division of two bit-vectors, truncating towards 0 (defined to be the all-ones bit pattern, if divisor is 0) (81) */
  BITVECTOR_UREM_TOTAL, /**< unsigned remainder from truncating division of two bit-vectors (defined to be equal to the dividend, if divisor is 0) (82) */
  BITVECTOR_SHL, /**< bit-vector shift left (the two bit-vector parameters must have same width) (83) */
  BITVECTOR_LSHR, /**< bit-vector logical shift right (the two bit-vector parameters must have same width) (84) */
  BITVECTOR_ASHR, /**< bit-vector arithmetic shift right (the two bit-vector parameters must have same width) (85) */
  BITVECTOR_ULT, /**< bit-vector unsigned less than (the two bit-vector parameters must have same width) (86) */
  BITVECTOR_ULE, /**< bit-vector unsigned less than or equal (the two bit-vector parameters must have same width) (87) */
  BITVECTOR_UGT, /**< bit-vector unsigned greater than (the two bit-vector parameters must have same width) (88) */
  BITVECTOR_UGE, /**< bit-vector unsigned greater than or equal (the two bit-vector parameters must have same width) (89) */
  BITVECTOR_SLT, /**< bit-vector signed less than (the two bit-vector parameters must have same width) (90) */
  BITVECTOR_SLE, /**< bit-vector signed less than or equal (the two bit-vector parameters must have same width) (91) */
  BITVECTOR_SGT, /**< bit-vector signed greater than (the two bit-vector parameters must have same width) (92) */
  BITVECTOR_SGE, /**< bit-vector signed greater than or equal (the two bit-vector parameters must have same width) (93) */
  BITVECTOR_ULTBV, /**< bit-vector unsigned less than but returns bv of size 1 instead of boolean (94) */
  BITVECTOR_SLTBV, /**< bit-vector signed less than but returns bv of size 1 instead of boolean (95) */
  BITVECTOR_ITE, /**< same semantics as regular ITE, but condition is bv of size 1 instead of Boolean (96) */
  BITVECTOR_REDOR, /**< bit-vector redor (97) */
  BITVECTOR_REDAND, /**< bit-vector redand (98) */
  BITVECTOR_EAGER_ATOM, /**< formula to be treated as a bv atom via eager bit-blasting (internal-only symbol) (99) */
  BITVECTOR_ACKERMANIZE_UDIV, /**< term to be treated as a variable; used for eager bit-blasting Ackermann expansion of bvudiv (internal-only symbol) (100) */
  BITVECTOR_ACKERMANIZE_UREM, /**< term to be treated as a variable; used for eager bit-blasting Ackermann expansion of bvurem (internal-only symbol) (101) */
  BITVECTOR_BITOF_OP, /**< operator for the bit-vector boolean bit extract; payload is an instance of the CVC4::BitVectorBitOf class (102) */
  BITVECTOR_EXTRACT_OP, /**< operator for the bit-vector extract; payload is an instance of the CVC4::BitVectorExtract class (103) */
  BITVECTOR_REPEAT_OP, /**< operator for the bit-vector repeat; payload is an instance of the CVC4::BitVectorRepeat class (104) */
  BITVECTOR_ZERO_EXTEND_OP, /**< operator for the bit-vector zero-extend; payload is an instance of the CVC4::BitVectorZeroExtend class (105) */
  BITVECTOR_SIGN_EXTEND_OP, /**< operator for the bit-vector sign-extend; payload is an instance of the CVC4::BitVectorSignExtend class (106) */
  BITVECTOR_ROTATE_LEFT_OP, /**< operator for the bit-vector rotate left; payload is an instance of the CVC4::BitVectorRotateLeft class (107) */
  BITVECTOR_ROTATE_RIGHT_OP, /**< operator for the bit-vector rotate right; payload is an instance of the CVC4::BitVectorRotateRight class (108) */
  BITVECTOR_BITOF, /**< bit-vector boolean bit extract; first parameter is a BITVECTOR_BITOF_OP, second is a bit-vector term (109) */
  BITVECTOR_EXTRACT, /**< bit-vector extract; first parameter is a BITVECTOR_EXTRACT_OP, second is a bit-vector term (110) */
  BITVECTOR_REPEAT, /**< bit-vector repeat; first parameter is a BITVECTOR_REPEAT_OP, second is a bit-vector term (111) */
  BITVECTOR_ZERO_EXTEND, /**< bit-vector zero-extend; first parameter is a BITVECTOR_ZERO_EXTEND_OP, second is a bit-vector term (112) */
  BITVECTOR_SIGN_EXTEND, /**< bit-vector sign-extend; first parameter is a BITVECTOR_SIGN_EXTEND_OP, second is a bit-vector term (113) */
  BITVECTOR_ROTATE_LEFT, /**< bit-vector rotate left; first parameter is a BITVECTOR_ROTATE_LEFT_OP, second is a bit-vector term (114) */
  BITVECTOR_ROTATE_RIGHT, /**< bit-vector rotate right; first parameter is a BITVECTOR_ROTATE_RIGHT_OP, second is a bit-vector term (115) */
  INT_TO_BITVECTOR_OP, /**< operator for the integer conversion to bit-vector; payload is an instance of the CVC4::IntToBitVector class (116) */
  INT_TO_BITVECTOR, /**< integer conversion to bit-vector; first parameter is an INT_TO_BITVECTOR_OP, second is an integer term (117) */
  BITVECTOR_TO_NAT, /**< bit-vector conversion to (nonnegative) integer; parameter is a bit-vector (118) */

  /* from fp */
  CONST_FLOATINGPOINT, /**< a floating-point literal (119) */
  CONST_ROUNDINGMODE, /**< a floating-point rounding mode (120) */
  FLOATINGPOINT_TYPE, /**< floating-point type (121) */
  FLOATINGPOINT_FP, /**< construct a floating-point literal from bit vectors (122) */
  FLOATINGPOINT_EQ, /**< floating-point equality (123) */
  FLOATINGPOINT_ABS, /**< floating-point absolute value (124) */
  FLOATINGPOINT_NEG, /**< floating-point negation (125) */
  FLOATINGPOINT_PLUS, /**< floating-point addition (126) */
  FLOATINGPOINT_SUB, /**< floating-point sutraction (127) */
  FLOATINGPOINT_MULT, /**< floating-point multiply (128) */
  FLOATINGPOINT_DIV, /**< floating-point division (129) */
  FLOATINGPOINT_FMA, /**< floating-point fused multiply and add (130) */
  FLOATINGPOINT_SQRT, /**< floating-point square root (131) */
  FLOATINGPOINT_REM, /**< floating-point remainder (132) */
  FLOATINGPOINT_RTI, /**< floating-point round to integral (133) */
  FLOATINGPOINT_MIN, /**< floating-point minimum (134) */
  FLOATINGPOINT_MAX, /**< floating-point maximum (135) */
  FLOATINGPOINT_LEQ, /**< floating-point less than or equal (136) */
  FLOATINGPOINT_LT, /**< floating-point less than (137) */
  FLOATINGPOINT_GEQ, /**< floating-point greater than or equal (138) */
  FLOATINGPOINT_GT, /**< floating-point greater than (139) */
  FLOATINGPOINT_ISN, /**< floating-point is normal (140) */
  FLOATINGPOINT_ISSN, /**< floating-point is sub-normal (141) */
  FLOATINGPOINT_ISZ, /**< floating-point is zero (142) */
  FLOATINGPOINT_ISINF, /**< floating-point is infinite (143) */
  FLOATINGPOINT_ISNAN, /**< floating-point is NaN (144) */
  FLOATINGPOINT_ISNEG, /**< floating-point is negative (145) */
  FLOATINGPOINT_ISPOS, /**< floating-point is positive (146) */
  FLOATINGPOINT_TO_FP_IEEE_BITVECTOR_OP, /**< operator for to_fp from bit vector (147) */
  FLOATINGPOINT_TO_FP_IEEE_BITVECTOR, /**< convert an IEEE-754 bit vector to floating-point (148) */
  FLOATINGPOINT_TO_FP_FLOATINGPOINT_OP, /**< operator for to_fp from floating point (149) */
  FLOATINGPOINT_TO_FP_FLOATINGPOINT, /**< convert between floating-point sorts (150) */
  FLOATINGPOINT_TO_FP_REAL_OP, /**< operator for to_fp from real (151) */
  FLOATINGPOINT_TO_FP_REAL, /**< convert a real to floating-point (152) */
  FLOATINGPOINT_TO_FP_SIGNED_BITVECTOR_OP, /**< operator for to_fp from signed bit vector (153) */
  FLOATINGPOINT_TO_FP_SIGNED_BITVECTOR, /**< convert a signed bit vector to floating-point (154) */
  FLOATINGPOINT_TO_FP_UNSIGNED_BITVECTOR_OP, /**< operator for to_fp from unsigned bit vector (155) */
  FLOATINGPOINT_TO_FP_UNSIGNED_BITVECTOR, /**< convert an unsigned bit vector to floating-point (156) */
  FLOATINGPOINT_TO_FP_GENERIC_OP, /**< operator for a generic to_fp (157) */
  FLOATINGPOINT_TO_FP_GENERIC, /**< a generic conversion to floating-point, used in parsing only (158) */
  FLOATINGPOINT_TO_UBV_OP, /**< operator for to_ubv (159) */
  FLOATINGPOINT_TO_UBV, /**< convert a floating-point value to an unsigned bit vector (160) */
  FLOATINGPOINT_TO_SBV_OP, /**< operator for to_sbv (161) */
  FLOATINGPOINT_TO_SBV, /**< convert a floating-point value to a signed bit vector (162) */
  FLOATINGPOINT_TO_REAL, /**< floating-point to real (163) */

  /* from arrays */
  ARRAY_TYPE, /**< array type (164) */
  SELECT, /**< array select; first parameter is an array term, second is the selection index (165) */
  STORE, /**< array store; first parameter is an array term, second is the store index, third is the term to store at the index (166) */
  STORE_ALL, /**< array store-all; payload is an instance of the CVC4::ArrayStoreAll class (this is not supported by arrays decision procedure yet, but it is used for returned array models) (167) */
  ARR_TABLE_FUN, /**< array table function (internal-only symbol) (168) */
  ARRAY_LAMBDA, /**< array lambda (internal-only symbol) (169) */
  PARTIAL_SELECT_0, /**< partial array select, for internal use only (170) */
  PARTIAL_SELECT_1, /**< partial array select, for internal use only (171) */

  /* from datatypes */
  CONSTRUCTOR_TYPE, /**< constructor (172) */
  SELECTOR_TYPE, /**< selector (173) */
  TESTER_TYPE, /**< tester (174) */
  APPLY_CONSTRUCTOR, /**< constructor application; first parameter is the constructor, remaining parameters (if any) are parameters to the constructor (175) */
  APPLY_SELECTOR, /**< selector application; parameter is a datatype term (undefined if mis-applied) (176) */
  APPLY_SELECTOR_TOTAL, /**< selector application; parameter is a datatype term (defined rigidly if mis-applied) (177) */
  APPLY_TESTER, /**< tester application; first parameter is a tester, second is a datatype term (178) */
  DATATYPE_TYPE, /**< a datatype type index (179) */
  PARAMETRIC_DATATYPE, /**< parametric datatype (180) */
  APPLY_TYPE_ASCRIPTION, /**< type ascription, for datatype constructor applications; first parameter is an ASCRIPTION_TYPE, second is the datatype constructor application being ascribed (181) */
  ASCRIPTION_TYPE, /**< a type parameter for type ascription; payload is an instance of the CVC4::AscriptionType class (182) */
  TUPLE_UPDATE_OP, /**< operator for a tuple update; payload is an instance of the CVC4::TupleUpdate class (183) */
  TUPLE_UPDATE, /**< tuple update; first parameter is a TUPLE_UPDATE_OP (which references an index), second is the tuple, third is the element to store in the tuple at the given index (184) */
  RECORD_UPDATE_OP, /**< operator for a record update; payload is an instance CVC4::RecordUpdate class (185) */
  RECORD_UPDATE, /**< record update; first parameter is a RECORD_UPDATE_OP (which references a field), second is a record term to update, third is the element to store in the record in the given field (186) */
  DT_SIZE, /**< datatypes size (187) */
  DT_HEIGHT_BOUND, /**< datatypes height bound (188) */
  DT_SIZE_BOUND, /**< datatypes height bound (189) */
  DT_SYGUS_BOUND, /**< datatypes sygus bound (190) */
  DT_SYGUS_TERM_ORDER, /**< datatypes sygus term order (191) */
  DT_SYGUS_IS_CONST, /**< datatypes sygus is constant (192) */

  /* from sep */
  SEP_NIL, /**< separation nil (193) */
  SEP_EMP, /**< separation empty heap (194) */
  SEP_PTO, /**< points to relation (195) */
  SEP_STAR, /**< separation star (196) */
  SEP_WAND, /**< separation magic wand (197) */
  SEP_LABEL, /**< separation label (internal use only) (198) */

  /* from sets */
  EMPTYSET, /**< the empty set constant; payload is an instance of the CVC4::EmptySet class (199) */
  SET_TYPE, /**< set type, takes as parameter the type of the elements (200) */
  UNION, /**< set union (201) */
  INTERSECTION, /**< set intersection (202) */
  SETMINUS, /**< set subtraction (203) */
  SUBSET, /**< subset predicate; first parameter a subset of second (204) */
  MEMBER, /**< set membership predicate; first parameter a member of second (205) */
  SINGLETON, /**< the set of the single element given as a parameter (206) */
  INSERT, /**< set obtained by inserting elements (first N-1 parameters) into a set (the last parameter) (207) */
  CARD, /**< set cardinality operator (208) */
  COMPLEMENT, /**< set COMPLEMENT (with respect to finite universe) (209) */
  UNIVERSE_SET, /**< (finite) universe set, all set variables must be interpreted as subsets of it. (210) */
  JOIN, /**< set join (211) */
  PRODUCT, /**< set cartesian product (212) */
  TRANSPOSE, /**< set transpose (213) */
  TCLOSURE, /**< set transitive closure (214) */
  JOIN_IMAGE, /**< set join image (215) */
  IDEN, /**< set identity (216) */

  /* from strings */
  STRING_CONCAT, /**< string concat (N-ary) (217) */
  STRING_IN_REGEXP, /**< membership (218) */
  STRING_LENGTH, /**< string length (219) */
  STRING_SUBSTR, /**< string substr (220) */
  STRING_CHARAT, /**< string charat (221) */
  STRING_STRCTN, /**< string contains (222) */
  STRING_STRIDOF, /**< string indexof (223) */
  STRING_STRREPL, /**< string replace (224) */
  STRING_PREFIX, /**< string prefixof (225) */
  STRING_SUFFIX, /**< string suffixof (226) */
  STRING_ITOS, /**< integer to string (227) */
  STRING_STOI, /**< string to integer (total function) (228) */
  CONST_STRING, /**< a string of characters (229) */
  CONST_REGEXP, /**< a regular expression (230) */
  STRING_TO_REGEXP, /**< convert string to regexp (231) */
  REGEXP_CONCAT, /**< regexp concat (232) */
  REGEXP_UNION, /**< regexp union (233) */
  REGEXP_INTER, /**< regexp intersection (234) */
  REGEXP_STAR, /**< regexp * (235) */
  REGEXP_PLUS, /**< regexp + (236) */
  REGEXP_OPT, /**< regexp ? (237) */
  REGEXP_RANGE, /**< regexp range (238) */
  REGEXP_LOOP, /**< regexp loop (239) */
  REGEXP_EMPTY, /**< regexp empty (240) */
  REGEXP_SIGMA, /**< regexp all characters (241) */
  REGEXP_RV, /**< regexp rv (internal use only) (242) */

  /* from quantifiers */
  FORALL, /**< universally quantified formula; first parameter is an BOUND_VAR_LIST, second is quantifier body, and an optional third parameter is an INST_PATTERN_LIST (243) */
  EXISTS, /**< existentially quantified formula; first parameter is an BOUND_VAR_LIST, second is quantifier body, and an optional third parameter is an INST_PATTERN_LIST (244) */
  INST_CONSTANT, /**< instantiation constant (245) */
  BOUND_VAR_LIST, /**< a list of bound variables (used to bind variables under a quantifier) (246) */
  INST_PATTERN, /**< instantiation pattern (247) */
  INST_NO_PATTERN, /**< instantiation no-pattern (248) */
  INST_ATTRIBUTE, /**< instantiation attribute (249) */
  INST_PATTERN_LIST, /**< a list of instantiation patterns (250) */
  INST_CLOSURE, /**< predicate for specifying term in instantiation closure. (251) */
  REWRITE_RULE, /**< general rewrite rule (for rewrite-rules theory) (252) */
  RR_REWRITE, /**< actual rewrite rule (for rewrite-rules theory) (253) */
  RR_REDUCTION, /**< actual reduction rule (for rewrite-rules theory) (254) */
  RR_DEDUCTION, /**< actual deduction rule (for rewrite-rules theory) (255) */

  /* from idl */

  LAST_KIND /**< marks the upper-bound of this enumeration */

};/* enum Kind_t */

}/* CVC4::kind namespace */

// import Kind into the "CVC4" namespace but keep the individual kind
// constants under kind::
typedef ::CVC4::kind::Kind_t Kind;

namespace kind {

std::ostream& operator<<(std::ostream&, CVC4::Kind) CVC4_PUBLIC;

#line 48 "../../../../../src/expr/kind_template.h"

/** Returns true if the given kind is associative. This is used by ExprManager to
 * decide whether it's safe to modify big expressions by changing the grouping of
 * the arguments. */
/* TODO: This could be generated. */
bool isAssociative(::CVC4::Kind k) CVC4_PUBLIC;
std::string kindToString(::CVC4::Kind k) CVC4_PUBLIC;

struct KindHashFunction {
  inline size_t operator()(::CVC4::Kind k) const {
    return k;
  }
};/* struct KindHashFunction */

}/* CVC4::kind namespace */

/**
 * The enumeration for the built-in atomic types.
 */
enum TypeConstant {
  BUILTIN_OPERATOR_TYPE, /**< the type for built-in operators */
  BOOLEAN_TYPE, /**< Boolean type */
  REAL_TYPE, /**< real type */
  INTEGER_TYPE, /**< integer type */
  ROUNDINGMODE_TYPE, /**< floating-point rounding mode */
  STRING_TYPE, /**< String type */
  REGEXP_TYPE, /**< RegExp type */
  BOUND_VAR_LIST_TYPE, /**< the type of bound variable lists */
  INST_PATTERN_TYPE, /**< instantiation pattern type */
  INST_PATTERN_LIST_TYPE, /**< the type of instantiation pattern lists */
  RRHB_TYPE, /**< head and body of the rule type (for rewrite-rules theory) */

#line 70 "../../../../../src/expr/kind_template.h"
  LAST_TYPE
};/* enum TypeConstant */

/**
 * We hash the constants with their values.
 */
struct TypeConstantHashFunction {
  inline size_t operator()(TypeConstant tc) const {
    return tc;
  }
};/* struct TypeConstantHashFunction */

std::ostream& operator<<(std::ostream& out, TypeConstant typeConstant);

namespace theory {

enum TheoryId {
  THEORY_BUILTIN,
  THEORY_BOOL,
  THEORY_UF,
  THEORY_ARITH,
  THEORY_BV,
  THEORY_FP,
  THEORY_ARRAY,
  THEORY_DATATYPES,
  THEORY_SEP,
  THEORY_SETS,
  THEORY_STRINGS,
  THEORY_QUANTIFIERS,

#line 89 "../../../../../src/expr/kind_template.h"
  THEORY_LAST
};/* enum TheoryId */

const TheoryId THEORY_FIRST = static_cast<TheoryId>(0);
const TheoryId THEORY_SAT_SOLVER = THEORY_LAST;

inline TheoryId& operator ++ (TheoryId& id) {
  return id = static_cast<TheoryId>(((int)id) + 1);
}

std::ostream& operator<<(std::ostream& out, TheoryId theoryId);
TheoryId kindToTheoryId(::CVC4::Kind k) CVC4_PUBLIC;
TheoryId typeConstantToTheoryId(::CVC4::TypeConstant typeConstant);

}/* CVC4::theory namespace */
}/* CVC4 namespace */

#endif /* __CVC4__KIND_H */
