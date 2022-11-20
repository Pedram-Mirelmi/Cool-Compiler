#ifndef SEMANT_H_
#define SEMANT_H_

#include <assert.h>
#include <iostream>  
#include <set>
#include "cool-tree.h"
#include "stringtab.h"
#include "symtab.h"
#include "list.h"
#include "classGraph.h"
#include "MySymtab.h"


#define TRUE 1
#define FALSE 0

class ClassTable;
typedef ClassTable *ClassTableP;

// This is a structure that may be used to contain the semantic
// information such as the inheritance graph.  You may use it or not as
// you like: it is only here to provide a container for the supplied
// methods.


class ClassTable {
private:
    int semant_errors, semant_warnings;
    void install_basic_classes();
    ostream& error_stream;
	ostream& warning_stream;
    Classes m_classes;
	
	std::map<std::string, std::set<std::string> > m_childs;
    InheritanceGraph m_inheritance_graph;

    MySymboTable<std::string, class__class*> m_class_symtable;
    MySymboTable<std::string, class__class*> m_base_classes_symtable;


	std::map<std::string, std::map<std::string, method_class*> > m_class_methods;
	std::map<std::string, std::map<std::string, attr_class*> > m_class_attrs;
	
public:
    ClassTable(Classes); 
    int errors() { return semant_errors; }
    ostream& semant_error();
    ostream& semant_error(Class_ c);
    ostream& semant_error(Symbol filename, tree_node *t);
	ostream& semant_warning();
    ostream& semant_warning(Class_ c);
    ostream& semant_warning(Symbol filename, tree_node *t);
	// funcs added by me:

	void analyzeClassesRelations();
	void buildInheritanceGraph();
	void analyzeIndividualClasses();
	void analyzeClassAttr(class__class* current_class,
	 					  attr_class* attr, 
						  const std::map<std::string, attr_class*>& attr_table,
        				  const std::map<std::string, method_class*>& method_table);

	void analyzeClassMethod(class__class* current_class, 
	  					    method_class* attr, 
							const std::map<std::string, attr_class*>& attr_table,
        				  	const std::map<std::string, method_class*>& method_table);

	Symbol analyzeExpression(class__class* current_class,
							 Expression expr,
							 ScopeTable& scope_table);


protected:
	
};


#endif

