#ifndef COOL_TREE_H
#define COOL_TREE_H
//////////////////////////////////////////////////////////
//
// file: cool-tree.h
//
// This file defines classes for each phylum and constructor
//
//////////////////////////////////////////////////////////


#include "tree.h"
#include "cool-tree.handcode.h"

enum ExprType {
      S_ASSIGN,
      S_STATIC_DISPATCH,
      S_DISPATCH,
      S_IF_THEN_ELSE,
      S_WHILE_LOOP,
      S_CASE,
      S_BLOCK,
      S_LET,
      S_PLUS,
      S_SUB,
      S_MUL,
      S_DIV,
      S_NEG,
      S_LT,
      S_EQ,
      S_LEQ,
      S_COMPLEMENT,
      S_INT_CONST,
      S_BOOL_CONST,
      S_STR_CONST,
      S_NEW,
      S_IS_VOID,
      S_NO_EXPR,
      S_OBJECT
   };


enum FeatureType{
   S_ATTR, 
   S_METHOD
};
class ClassTable;

// define the class for phylum
// define simple phylum - Program
typedef class Program_class *Program;

class Program_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Program(); }
   virtual Program copy_Program() = 0;

#ifdef Program_EXTRAS
   Program_EXTRAS
#endif
};


// define simple phylum - Class_
typedef class Class__class *Class_;

class Class__class : public tree_node {
public:
   tree_node *copy()		 { return copy_Class_(); }
   virtual Class_ copy_Class_() = 0;

#ifdef Class__EXTRAS
   Class__EXTRAS
#endif
};


// define simple phylum - Feature
typedef class Feature_class *Feature;

class Feature_class : public tree_node 
{
public:
   tree_node *copy()		 { return copy_Feature(); }
   virtual Feature copy_Feature() = 0;
#ifdef Feature_EXTRAS
   Feature_EXTRAS
#endif
   
   virtual FeatureType getType() = 0;
};


// define simple phylum - Formal
typedef class Formal_class *Formal;

class Formal_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Formal(); }
   virtual Formal copy_Formal() = 0;

#ifdef Formal_EXTRAS
   Formal_EXTRAS
#endif
};


// define simple phylum - Expression
typedef class Expression_class *Expression;

class Expression_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Expression(); }
   virtual Expression copy_Expression() = 0;

#ifdef Expression_EXTRAS
   Expression_EXTRAS
#endif
   
   virtual ExprType getExprType() = 0;
};

// define simple phylum - Case
typedef class Case_class *Case;

class Case_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Case(); }
   virtual Case copy_Case() = 0;

#ifdef Case_EXTRAS
   Case_EXTRAS
#endif
};


// define the class for phylum - LIST
// define list phlyum - Classes
typedef list_node<Class_> Classes_class;
typedef Classes_class *Classes;


// define list phlyum - Features
typedef list_node<Feature> Features_class;
typedef Features_class *Features;


// define list phlyum - Formals
typedef list_node<Formal> Formals_class;
typedef Formals_class *Formals;


// define list phlyum - Expressions
typedef list_node<Expression> Expressions_class;
typedef Expressions_class *Expressions;


// define list phlyum - Cases
typedef list_node<Case> Cases_class;
typedef Cases_class *Cases;


// define the class for constructors
// define constructor - program
class program_class : public Program_class {
protected:
   Classes classes;
public:
   program_class(Classes a1) {
      classes = a1;
   }
   Program copy_Program();
   void dump(ostream& stream, int n);

#ifdef Program_SHARED_EXTRAS
   Program_SHARED_EXTRAS
#endif
#ifdef program_EXTRAS
   program_EXTRAS
#endif
};


// define constructor - class_
class class__class : public Class__class {
public:
   Symbol name;
   Symbol parent;
   Features features;
   Symbol filename;
   class__class(Symbol a1, Symbol a2, Features a3, Symbol a4) {
      name = a1;
      parent = a2;
      features = a3;
      filename = a4;
   }
   Class_ copy_Class_();
   void dump(ostream& stream, int n);

#ifdef Class__SHARED_EXTRAS
   Class__SHARED_EXTRAS
#endif
#ifdef class__EXTRAS
   class__EXTRAS
#endif
};


// define constructor - method
class method_class : public Feature_class {
public:
   Symbol name;
   Formals formals;
   Symbol return_type;
   Expression expr;
   method_class(Symbol a1, Formals a2, Symbol a3, Expression a4) {
      name = a1;
      formals = a2;
      return_type = a3;
      expr = a4;
   }
   Feature copy_Feature();
   void dump(ostream& stream, int n);

#ifdef Feature_SHARED_EXTRAS
   Feature_SHARED_EXTRAS
#endif
#ifdef method_EXTRAS
   method_EXTRAS
#endif
   virtual FeatureType getType() 
      { return S_METHOD; } 
};


// define constructor - attr
class attr_class : public Feature_class {
public:
   Symbol name;
   Symbol type_decl;
   Expression init;
   attr_class(Symbol a1, Symbol a2, Expression a3) {
      name = a1;
      type_decl = a2;
      init = a3;
   }
   Feature copy_Feature();
   void dump(ostream& stream, int n);

#ifdef Feature_SHARED_EXTRAS
   Feature_SHARED_EXTRAS
#endif
#ifdef attr_EXTRAS
   attr_EXTRAS
#endif

   virtual FeatureType getType() 
      { return S_ATTR; }
};


// define constructor - formal
class formal_class : public Formal_class {
public:
   Symbol name;
   Symbol type_decl;
   formal_class(Symbol a1, Symbol a2) {
      name = a1;
      type_decl = a2;
   }
   Formal copy_Formal();
   void dump(ostream& stream, int n);

#ifdef Formal_SHARED_EXTRAS
   Formal_SHARED_EXTRAS
#endif
#ifdef formal_EXTRAS
   formal_EXTRAS
#endif
};


// define constructor - branch
class branch_class : public Case_class {
public:
   Symbol name;
   Symbol type_decl;
   Expression expr;
   branch_class(Symbol a1, Symbol a2, Expression a3) {
      name = a1;
      type_decl = a2;
      expr = a3;
   }
   Case copy_Case();
   void dump(ostream& stream, int n);

#ifdef Case_SHARED_EXTRAS
   Case_SHARED_EXTRAS
#endif
#ifdef branch_EXTRAS
   branch_EXTRAS
#endif
};


// define constructor - assign
class assign_class : public Expression_class {
public:
   Symbol name;
   Expression expr;
   assign_class(Symbol a1, Expression a2) {
      name = a1;
      expr = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef assign_EXTRAS
   assign_EXTRAS
#endif
   ExprType getExprType()
      { return S_ASSIGN; }
};
// X
// A
// a@X.f(1 ,2 3);


// define constructor - static_dispatch
class static_dispatch_class : public Expression_class {
public:
   Expression expr;
   Symbol type_name;
   Symbol name;
   Expressions actual;
   static_dispatch_class(Expression a1, Symbol a2, Symbol a3, Expressions a4) {
      expr = a1;
      type_name = a2;
      name = a3;
      actual = a4;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef static_dispatch_EXTRAS
   static_dispatch_EXTRAS
#endif
   ExprType getExprType()
      { return S_STATIC_DISPATCH; }
};


// define constructor - dispatch
class dispatch_class : public Expression_class {
public:
   Expression expr;
   Symbol name;
   Expressions actual;
   dispatch_class(Expression a1, Symbol a2, Expressions a3) {
      expr = a1;
      name = a2;
      actual = a3;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef dispatch_EXTRAS
   dispatch_EXTRAS
#endif
   ExprType getExprType()
      { return S_DISPATCH; }
};


// define constructor - cond
class cond_class : public Expression_class {
public:
   Expression pred;
   Expression then_exp;
   Expression else_exp;
   cond_class(Expression a1, Expression a2, Expression a3) {
      pred = a1;
      then_exp = a2;
      else_exp = a3;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef cond_EXTRAS
   cond_EXTRAS
#endif
   ExprType getExprType()
      { return S_IF_THEN_ELSE; }
};


// define constructor - loop
class loop_class : public Expression_class {
public:
   Expression pred;
   Expression body;
   loop_class(Expression a1, Expression a2) {
      pred = a1;
      body = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef loop_EXTRAS
   loop_EXTRAS
#endif
   ExprType getExprType()
      { return S_WHILE_LOOP; }
};


// define constructor - typcase
class typcase_class : public Expression_class {
public:
   Expression expr;
   Cases cases;
   typcase_class(Expression a1, Cases a2) {
      expr = a1;
      cases = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef typcase_EXTRAS
   typcase_EXTRAS
#endif
   ExprType getExprType()
      { return S_CASE; }
};


// define constructor - block
class block_class : public Expression_class {
public:
   Expressions body;
   block_class(Expressions a1) {
      body = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef block_EXTRAS
   block_EXTRAS
#endif
   ExprType getExprType()
      { return S_BLOCK; }
};


// define constructor - let
class let_class : public Expression_class {
public:
   Symbol identifier;
   Symbol type_decl;
   Expression init;
   Expression body;
   let_class(Symbol a1, Symbol a2, Expression a3, Expression a4) {
      identifier = a1;
      type_decl = a2;
      init = a3;
      body = a4;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef let_EXTRAS
   let_EXTRAS
#endif
   ExprType getExprType()
      { return S_LET; }
};


// define constructor - plus
class plus_class : public Expression_class {
public:
   Expression e1;
   Expression e2;
   plus_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef plus_EXTRAS
   plus_EXTRAS
#endif
   ExprType getExprType()
      { return S_PLUS; }
};


// define constructor - sub
class sub_class : public Expression_class {
public:
   Expression e1;
   Expression e2;
   sub_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef sub_EXTRAS
   sub_EXTRAS
#endif
   ExprType getExprType()
      { return S_SUB; }
};


// define constructor - mul
class mul_class : public Expression_class {
public:
   Expression e1;
   Expression e2;
   mul_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef mul_EXTRAS
   mul_EXTRAS
#endif
   ExprType getExprType()
      { return S_MUL; }
};


// define constructor - divide
class divide_class : public Expression_class {
public:
   Expression e1;
   Expression e2;
   divide_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef divide_EXTRAS
   divide_EXTRAS
#endif
   ExprType getExprType()
      { return S_DIV; }
};


// define constructor - neg
class neg_class : public Expression_class {
public:
   Expression e1;
   neg_class(Expression a1) {
      e1 = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef neg_EXTRAS
   neg_EXTRAS
#endif
   ExprType getExprType()
      { return S_NEG; }
};


// define constructor - lt
class lt_class : public Expression_class {
public:
   Expression e1;
   Expression e2;
   lt_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef lt_EXTRAS
   lt_EXTRAS
#endif
   ExprType getExprType()
      { return S_LT; }
};


// define constructor - eq
class eq_class : public Expression_class {
public:
   Expression e1;
   Expression e2;
   eq_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef eq_EXTRAS
   eq_EXTRAS
#endif
   ExprType getExprType()
      { return S_EQ; }
};


// define constructor - leq
class leq_class : public Expression_class {
public:
   Expression e1;
   Expression e2;
   leq_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef leq_EXTRAS
   leq_EXTRAS
#endif
   ExprType getExprType()
      { return S_LEQ; }
};


// define constructor - comp
class comp_class : public Expression_class {
public:
   Expression e1;
   comp_class(Expression a1) {
      e1 = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef comp_EXTRAS
   comp_EXTRAS
#endif
   ExprType getExprType()
      { return S_COMPLEMENT; }
};


// define constructor - int_const
class int_const_class : public Expression_class {
public:
   Symbol token;
   int_const_class(Symbol a1) {
      token = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef int_const_EXTRAS
   int_const_EXTRAS
#endif
   ExprType getExprType()
      { return S_INT_CONST; }
};


// define constructor - bool_const
class bool_const_class : public Expression_class {
public:
   Boolean val;
   bool_const_class(Boolean a1) {
      val = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef bool_const_EXTRAS
   bool_const_EXTRAS
#endif
   ExprType getExprType()
      { return S_BOOL_CONST; }
};


// define constructor - string_const
class string_const_class : public Expression_class {
public:
   Symbol token;
   string_const_class(Symbol a1) {
      token = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef string_const_EXTRAS
   string_const_EXTRAS
#endif
   ExprType getExprType()
      { return S_STR_CONST; }
};


// define constructor - new_
class new__class : public Expression_class {
public:
   Symbol type_name;
   new__class(Symbol a1) {
      type_name = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef new__EXTRAS
   new__EXTRAS
#endif
   ExprType getExprType()
      { return S_NEW; }
};


// define constructor - isvoid
class isvoid_class : public Expression_class {
public:
   Expression e1;
   isvoid_class(Expression a1) {
      e1 = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef isvoid_EXTRAS
   isvoid_EXTRAS
#endif
   ExprType getExprType()
      { return S_IS_VOID; }
};


// define constructor - no_expr
class no_expr_class : public Expression_class {
public:
   no_expr_class() {
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef no_expr_EXTRAS
   no_expr_EXTRAS
#endif
   ExprType getExprType()
      { return S_NO_EXPR; }
};


// define constructor - object
class object_class : public Expression_class {
public:
   Symbol name;
   object_class(Symbol a1) {
      name = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef object_EXTRAS
   object_EXTRAS
#endif
   ExprType getExprType()
      { return S_OBJECT; }
};


// define the prototypes of the interface
Classes nil_Classes();
Classes single_Classes(Class_);
Classes append_Classes(Classes, Classes);
Features nil_Features();
Features single_Features(Feature);
Features append_Features(Features, Features);
Formals nil_Formals();
Formals single_Formals(Formal);
Formals append_Formals(Formals, Formals);
Expressions nil_Expressions();
Expressions single_Expressions(Expression);
Expressions append_Expressions(Expressions, Expressions);
Cases nil_Cases();
Cases single_Cases(Case);
Cases append_Cases(Cases, Cases);
Program program(Classes);
class__class* class_(Symbol, Symbol, Features, Symbol);
Feature method(Symbol, Formals, Symbol, Expression);
Feature attr(Symbol, Symbol, Expression);
Formal formal(Symbol, Symbol);
Case branch(Symbol, Symbol, Expression);
Expression assign(Symbol, Expression);
Expression static_dispatch(Expression, Symbol, Symbol, Expressions);
Expression dispatch(Expression, Symbol, Expressions);
Expression cond(Expression, Expression, Expression);
Expression loop(Expression, Expression);
Expression typcase(Expression, Cases);
Expression block(Expressions);
Expression let(Symbol, Symbol, Expression, Expression);
Expression plus(Expression, Expression);
Expression sub(Expression, Expression);
Expression mul(Expression, Expression);
Expression divide(Expression, Expression);
Expression neg(Expression);
Expression lt(Expression, Expression);
Expression eq(Expression, Expression);
Expression leq(Expression, Expression);
Expression comp(Expression);
Expression int_const(Symbol);
Expression bool_const(Boolean);
Expression string_const(Symbol);
Expression new_(Symbol);
Expression isvoid(Expression);
Expression no_expr();
Expression object(Symbol);


#endif
