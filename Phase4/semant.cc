#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"
#include "MySymtab.h"
extern int semant_debug;
extern char *curr_filename;

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
static Symbol 
    arg,
    arg2,
    Bool,
    concat,
    cool_abort,
    copyy,
    Int,
    in_int,
    in_string,
    IO,
    length,
    Main,
    main_meth,
    No_class,
    No_type,
    Object,
    out_int,
    out_string,
    prim_slot,
    self,
    SELF_TYPE,
    Str,
    str_field,
    substr,
    type_name,
    val;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
    arg         = idtable.add_string("arg");
    arg2        = idtable.add_string("arg2");
    Bool        = idtable.add_string("Bool");
    concat      = idtable.add_string("concat");
    cool_abort  = idtable.add_string("abort");
    copyy       = idtable.add_string("copy");
    Int         = idtable.add_string("Int");
    in_int      = idtable.add_string("in_int");
    in_string   = idtable.add_string("in_string");
    IO          = idtable.add_string("IO");
    length      = idtable.add_string("length");
    Main        = idtable.add_string("Main");
    main_meth   = idtable.add_string("main");
    //   _no_class is a symbol that can't be the name of any 
    //   user-defined class.
    No_class    = idtable.add_string("_no_class");
    No_type     = idtable.add_string("_no_type");
    Object      = idtable.add_string("Object");
    out_int     = idtable.add_string("out_int");
    out_string  = idtable.add_string("out_string");
    prim_slot   = idtable.add_string("_prim_slot");
    self        = idtable.add_string("self");
    SELF_TYPE   = idtable.add_string("SELF_TYPE");
    Str         = idtable.add_string("String");
    str_field   = idtable.add_string("_str_field");
    substr      = idtable.add_string("substr");
    type_name   = idtable.add_string("type_name");
    val         = idtable.add_string("_val");
}


/**
 * @brief Given the class ClassTable we do all the semantic analyzing stuff here
 *      first few passes over the classes just stores the class names and methods and children
 * 
 * @param classes the input to the semantic analyzer which is the list of all the classes in the program
 */
ClassTable::ClassTable(Classes classes)
    : semant_errors(0) , error_stream(cerr), warning_stream(cout), m_classes(classes)
{
    this->install_basic_classes();
    for(int i = 0; i < classes->len(); i++)
    {
        class__class* current_class = (class__class*)classes->nth(i);
        std::string current_class_name = current_class->name->get_string();
        if(m_class_symtable.lookup(current_class_name) != NULL)
            semant_error(current_class) << "Class " << current_class_name << " is redefined" << std::endl;
            
        m_class_symtable.addid(current_class_name, current_class);
        for (size_t i = 0; current_class->features->more(i); i++)
        {
            Feature current_feature = current_class->features->nth(i);
            switch(current_feature->getType())
            {
                case S_METHOD:
                {
                    if(m_class_methods[current_class_name].find(((method_class*)(current_feature))->name->get_string()) == m_class_methods[current_class_name].end())
                        m_class_methods[current_class_name][((method_class*)current_feature)->name->get_string()] = (method_class*)current_feature;
                    else 
                        semant_error(current_class) << "Method " << ((method_class*)current_feature)->name 
                                                    << " of class " << current_class_name << " is redefined " << std::endl;
                    break;
                }
                case S_ATTR:
                {
                    if(m_class_attrs[current_class_name].find(((attr_class*)(current_feature))->name->get_string()) == m_class_attrs[current_class_name].end())
                        m_class_attrs[current_class_name][((attr_class*)current_feature)->name->get_string()] = (attr_class*)current_feature;
                    else 
                        semant_error(current_class) << "Attribute " << ((attr_class*)current_feature)->name 
                                                    << " of class " << current_class_name << " is redefined " << std::endl;
                    break;
                } 
            }
        }   
    }


}

/**
 * @brief this method is called before anything to build and store information about base and built-in classes
 * 
 */
void 
ClassTable::install_basic_classes() {

    // The tree package uses these globals to annotate the classes built below.
   // curr_lineno  = 0;
    Symbol filename = stringtable.add_string("<basic class>");
    
    // The following demonstrates how to create dummy parse trees to
    // refer to basic Cool classes.  There's no need for method
    // bodies -- these are already built into the runtime system.
    
    // IMPORTANT: The results of the following expressions are
    // stored in local variables.  You will want to do something
    // with those variables at the end of this method to make this
    // code meaningful.

    // 
    // The Object class has no parent class. Its methods are
    //        abort() : Object    aborts the program
    //        type_name() : Str   returns a string representation of class name
    //        copy() : SELF_TYPE  returns a copy of the object
    //
    // There is no need for method bodies in the basic classes---these
    // are already built in to the runtime system.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class__class* Object_class =
	class_(Object, 
	       No_class,
	       append_Features(
			       append_Features(
					       single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
					       single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
			       single_Features(method(copyy, nil_Formals(), SELF_TYPE, no_expr()))),
	       filename);

    m_class_symtable.addid(Object_class->name->get_string(), Object_class);
    m_base_classes_symtable.addid(Object_class->name->get_string(), Object_class);
    // m_inheritance_graph.setRoot(Object_class);
    m_childs[Object->get_string()] = std::set<std::string>();
    for (size_t i = 0; Object_class->features->more(i); i++)
        if(Object_class->features->nth(i)->getType() == S_METHOD)
            m_class_methods[Object->get_string()][((method_class*)(Object_class->features->nth(i)))->name->get_string()] = (method_class*)Object_class->features->nth(i);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    // The IO class inherits from Object. Its methods are
    //        out_string(Str) : SELF_TYPE       writes a string to the output
    //        out_int(Int) : SELF_TYPE            "    an int    "  "     "
    //        in_string() : Str                 reads a string from the input
    //        in_int() : Int                      "   an int     "  "     "
    //
    class__class* IO_class = 
	class_(IO, 
	       Object,
	       append_Features(
			       append_Features(
					       append_Features(
							       single_Features(method(out_string, single_Formals(formal(arg, Str)),
										      SELF_TYPE, no_expr())),
							       single_Features(method(out_int, single_Formals(formal(arg, Int)),
										      SELF_TYPE, no_expr()))),
					       single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
			       single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
	       filename);  
    m_class_symtable.addid(IO_class->name->get_string(), IO_class);
    m_base_classes_symtable.addid(IO_class->name->get_string(), IO_class);
    // m_inheritance_graph.m_root->addChild(IO_class);
    m_childs[Object->get_string()].insert(IO->get_string());
    for (size_t i = 0; IO_class->features->more(i); i++)
        if(IO_class->features->nth(i)->getType() == S_METHOD)
            m_class_methods[IO->get_string()][((method_class*)(IO_class->features->nth(i)))->name->get_string()] = (method_class*)IO_class->features->nth(i);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //
    // The Int class has no methods and only a single attribute, the
    // "val" for the integer. 
    //
    class__class* Int_class =
	class_(Int, 
	       Object,
	       single_Features(attr(val, prim_slot, no_expr())),
	       filename);

    m_class_symtable.addid(Int_class->name->get_string(), Int_class);
    m_base_classes_symtable.addid(Int_class->name->get_string(), Int_class);
    // m_inheritance_graph.m_root->addChild(Int_class);
    m_childs[Object->get_string()].insert(Int->get_string());
    for (size_t i = 0; Int_class->features->more(i); i++)
        if(Int_class->features->nth(i)->getType() == S_METHOD)
            m_class_methods[Int->get_string()][((method_class*)(Int_class->features->nth(i)))->name->get_string()] = (method_class*)Int_class->features->nth(i);
        else 
            m_class_attrs[Int->get_string()][((attr_class*)(Int_class->features->nth(i)))->name->get_string()] = (attr_class*)Int_class->features->nth(i);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Bool also has only the "val" slot.
    //
    class__class* Bool_class =
	class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename);
    m_class_symtable.addid(Bool_class->name->get_string(), Bool_class);
    m_base_classes_symtable.addid(Bool_class->name->get_string(), Bool_class);
    // m_inheritance_graph.m_root->addChild(Bool_class);
    m_childs[Object->get_string()].insert(Bool->get_string());
    for (size_t i = 0; Bool_class->features->more(i); i++)
        if(Bool_class->features->nth(i)->getType() == S_METHOD)
            m_class_methods[Bool->get_string()][((method_class*)(Bool_class->features->nth(i)))->name->get_string()] = (method_class*)Bool_class->features->nth(i);
        else 
            m_class_attrs[Bool->get_string()][((attr_class*)(Bool_class->features->nth(i)))->name->get_string()] = (attr_class*)Bool_class->features->nth(i);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //
    // The class Str has a number of slots and operations:
    //       val                                  the length of the string
    //       str_field                            the string itself
    //       length() : Int                       returns length of the string
    //       concat(arg: Str) : Str               performs string concatenation
    //       substr(arg: Int, arg2: Int): Str     substring selection
    //       
    class__class* Str_class =
	class_(Str, 
	       Object,
	       append_Features(
			       append_Features(
					       append_Features(
							       append_Features(
									       single_Features(attr(val, Int, no_expr())),
									       single_Features(attr(str_field, prim_slot, no_expr()))),
							       single_Features(method(length, nil_Formals(), Int, no_expr()))),
					       single_Features(method(concat, 
								      single_Formals(formal(arg, Str)),
								      Str, 
								      no_expr()))),
			       single_Features(method(substr, 
						      append_Formals(single_Formals(formal(arg, Int)), 
								     single_Formals(formal(arg2, Int))),
						      Str, 
						      no_expr()))),
	       filename);
    m_class_symtable.addid(Str_class->name->get_string(), Str_class);
    m_base_classes_symtable.addid(Str_class->name->get_string(), Str_class);
    // m_inheritance_graph.m_root->addChild(Str_class);
    m_childs[Object->get_string()].insert(Str->get_string());
    for (size_t i = 0; Str_class->features->more(i); i++)
        if(Str_class->features->nth(i)->getType() == S_METHOD)
            m_class_methods[Str->get_string()][((method_class*)(Str_class->features->nth(i)))->name->get_string()] = (method_class*)Str_class->features->nth(i);
        else
            m_class_attrs[Str->get_string()][((attr_class*)(Str_class->features->nth(i)))->name->get_string()] = (attr_class*)Str_class->features->nth(i);

}

////////////////////////////////////////////////////////////////////
//
// semant_error is an overloaded function for reporting errors
// during semantic analysis.  There are three versions:
//
//    ostream& ClassTable::semant_error()                
//
//    ostream& ClassTable::semant_error(Class_ c)
//       print line number and filename for `c'
//
//    ostream& ClassTable::semant_error(Symbol filename, tree_node *t)  
//       print a line number and filename
//
///////////////////////////////////////////////////////////////////

ostream& 
ClassTable::semant_error(Class_ c)
{                                                             
    return semant_error(c->get_filename(),c);
}    

ostream& 
ClassTable::semant_error(Symbol filename, tree_node *t)
{
    error_stream << filename << ":" << t->get_line_number() << ": ";
    return semant_error();
}

ostream& 
ClassTable::semant_error()                  
{                                                 
    semant_errors++;                            
    return error_stream;
} 

ostream& 
ClassTable::semant_warning(Class_ c)
{                                                             
    return semant_warning(c->get_filename(),c);
}    

ostream& 
ClassTable::semant_warning(Symbol filename, tree_node *t)
{
    warning_stream << filename << ":" << t->get_line_number() << ": ";
    return semant_warning();
}

ostream& 
ClassTable::semant_warning()                  
{                                                 
    semant_warnings++;                            
    return warning_stream;
} 
/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// funcs added by me ////////////////////////////////////

/**
 * @brief this method check the inheritance relations between classes and handle error related to it
 * 
 */
void 
ClassTable::analyzeClassesRelations()
{
    for(int i = 0; m_classes->more(i); i++)
    {
        class__class* current_class = (class__class*)m_classes->nth(i);
        if (m_base_classes_symtable.lookup(current_class->name->get_string()) != NULL)
        {
            semant_error(current_class) << "Base class " 
                << current_class->name << " has redefined" << std::endl;
        }
        if(current_class->parent == NULL)
            current_class->parent = Object;
        if (m_class_symtable.lookup(current_class->parent->get_string()) == NULL)
        {
            semant_error(current_class) << "Class " << current_class->name
                << " has undefined parent" << std::endl;
            current_class->parent = Object; // TODO
        }
    }
    for(int i = 0; m_classes->more(i); i++)
    {
        class__class* current_class = (class__class*)m_classes->nth(i);
        class__class* parent_class = m_class_symtable.lookup(current_class->parent->get_string());
        while (*parent_class->name != *Object && *parent_class->name != *current_class->name)
        {
            parent_class = m_class_symtable.lookup(parent_class->parent->get_string());
        }
        if (*parent_class->name != *Object)
        {
            semant_error(current_class) << "Class " << current_class->name << " has cyclic inheritance relationship" << std::endl;
            current_class->parent = Object;
        }
        else
            m_class_symtable.addid(current_class->name->get_string(), current_class);
    }

    if(m_class_symtable.lookup("Main") == NULL)
        semant_error() << "No Main class Found" << std::endl;
    else if (m_class_methods["Main"].find("main") == m_class_methods["Main"].end())
        semant_error() << "No method main found in class Main" << std::endl;
}   
/**
 * @brief after a few passes we now can build the inheritance graph(using storing each classes children)
 * 
 */
void 
ClassTable::buildInheritanceGraph()
{
    for(int i = 0; m_classes->more(i); i++)
    {
        class__class* current_class = (class__class*)m_classes->nth(i);
        m_childs[current_class->parent->get_string()].insert(current_class->name->get_string());
    }
    m_inheritance_graph = InheritanceGraph(Object->get_string(), m_childs, m_class_attrs, m_class_methods);
}

/**
 * @brief This is actually the final and main pass to check everything else
 *      first we itterate over classes and check its methods and attributes
 * 
 */
void 
ClassTable::analyzeIndividualClasses()
{
    for (size_t i = 0; m_classes->more(i); i++)
    {
        class__class* current_class = (class__class*)m_classes->nth(i);
        Features current_features = current_class->features;
        const std::map<std::string, attr_class*>& attr_table = m_class_attrs[current_class->name->get_string()];
        const std::map<std::string, method_class*>& method_table = m_class_methods[current_class->name->get_string()];
        for (size_t j = 0; current_features->more(j); j++)
        {
            switch (current_features->nth(j)->getType())
            {
                case S_ATTR:
                    analyzeClassAttr(current_class,
                                     (attr_class*)current_features->nth(j),
                                     attr_table, 
                                     method_table);
                    break;
                case S_METHOD:
                    analyzeClassMethod(current_class, 
                                       (method_class*)current_features->nth(j), 
                                       attr_table, 
                                       method_table);
                    break;
            }
        }
    }
}

/**
 * @brief the method to check @param attr in the class  @param current_classes 
 *      which its methods and other attributes are stored in @param attr_table and @param method_table  
 */
void 
ClassTable::analyzeClassAttr(class__class* current_class,
                             attr_class* attr,
                             const std::map<std::string, attr_class*>& attr_table,
        				     const std::map<std::string, method_class*>& method_table)
{   
    if(m_class_symtable.lookup(attr->type_decl->get_string()) == NULL)
    {
        semant_error(current_class) << "Attribute " << attr->name->get_string() << " of class " 
                                    << current_class->name << " has undefined type" 
                                    << std::endl;
        attr->type_decl = Object;
    }
    ScopeTable scope_table = ScopeTable();
    scope_table.enterScope();
    for(std::map<std::string, attr_class*>::const_iterator itter = attr_table.begin();
                                                           itter != attr_table.end();
                                                           itter++)
    {   
        scope_table.addid(itter->first, itter->second->type_decl->get_string());
    }
    if(attr->init->getExprType() != S_NO_EXPR)
    {
        if (!m_inheritance_graph.isParent(attr->type_decl->get_string(), analyzeExpression(current_class, attr->init, scope_table)->get_string()))
        {
            semant_error(current_class) << "Attribute " << attr->name 
                                        << " of class " << current_class->name 
                                        << " doesn't match its initialization expression" 
                                        << std::endl;
        }
    }
}

/**
 * @brief similar to analyzeClassAttr this method checks each class method 
 *      basically it check its return type to be valid and then typecheck its body and match it with return type declaration 
 *      
 *      to check the body of function first we create a ScopeTable and add all attributes as a scope
 *      next we add all function parameters as a new scope
 *  
 *      and then we call analyzeExpression with this scope_table
 * @param current_class 
 * @param method 
 * @param attr_table 
 * @param method_table 
 */
void 
ClassTable::analyzeClassMethod(class__class* current_class,
                               method_class* method,
                               const std::map<std::string, attr_class*>& attr_table,
        				       const std::map<std::string, method_class*>& method_table)
{
    if(m_class_symtable.lookup(method->return_type->get_string()) == NULL &&
       *method->return_type != *SELF_TYPE)
    {
        semant_error(current_class) << "Method " << method->name << " of class " 
                                    << current_class->name << " has undefined return type" 
                                    << std::endl;

        method->return_type = Object;
    }
    ScopeTable scope_table = ScopeTable();
    scope_table.enterScope();
    for(std::map<std::string, attr_class*>::const_iterator itter = attr_table.begin();
                                                           itter != attr_table.end();
                                                           itter++)
    {
        if(scope_table.prob(itter->first).size())
            semant_error(current_class) << "Attribute " << itter->first << " is redefined" << endl;
        else
            scope_table.addid(itter->first, itter->second->type_decl->get_string());
    }
    scope_table.enterScope();
    for (size_t i = 0; i < (size_t)method->formals->len(); i++)
    {
        formal_class* current_formal = (formal_class*)method->formals->nth(i);
        if(scope_table.prob(current_formal->name->get_string()).size())
            semant_error(current_class) << "Parameter " << current_formal->name << " is redefined" << endl;
        else 
            scope_table.addid(current_formal->name->get_string(), current_formal->type_decl->get_string());
    }
    Symbol actual_return_type = analyzeExpression(current_class, method->expr, scope_table);
    if((*method->return_type != *SELF_TYPE &&
        !m_inheritance_graph.isParent(method->return_type->get_string(), method->expr->get_type()->get_string()))
            ||        
        (*method->return_type == *SELF_TYPE &&
        !m_inheritance_graph.isParent(current_class->name->get_string(), method->expr->get_type()->get_string())))
    {
        semant_error(current_class) << "Return type of method " << method->name 
                                    << "(" << method->return_type << ") " 
                                    << " of class " << current_class->name 
                                    << " doesn't match its body expression"
                                    << "(" << method->expr->get_type() << ") "
                                    << std::endl;
    }
}

/**
 * @brief The main recursive method that type checks 
 *        each expression in the scope of @param scope_table
 *        and in class @param current_class 
 *        first it gets the exprType of the expresssion
 *        and we have a huge switch statement to type check on each case
 * 
 * @param current_class     the class that the expression is in
 * @param expr              the expr
 * @param scope_table       the scope_table the expression is
 * @return Symbol           finally we ruturn the type of the expression
 */
Symbol 
ClassTable::analyzeExpression(class__class* current_class,
                              Expression_class* expr,
                              ScopeTable& scope_table)
{
    switch (expr->getExprType())
    {
        case S_ASSIGN:
        {
            assign_class* exprr = (assign_class*) expr;
            if(scope_table.lookup(exprr->name->get_string()) == std::string())
            {
                semant_error(current_class) << "Identifier " << exprr->name << " not defined" << std::endl;
                expr->set_type(Object);
                break;
            }
            Symbol provided_type = analyzeExpression(current_class, exprr->expr, scope_table);
            if(!m_inheritance_graph.isParent(scope_table.lookup(exprr->name->get_string()), provided_type->get_string()))
            {
                semant_error(current_class) << "Right hand side of the assignment must be a subtype of " 
                                            << scope_table.lookup(exprr->name->get_string())
                                            << " but " << provided_type << " provided" << endl;
            }
            expr->set_type(provided_type);
            break;
        }
        case S_STATIC_DISPATCH:
        {
            static_dispatch_class* exprr = (static_dispatch_class*)expr;
            Symbol e0_type = analyzeExpression(current_class, exprr->expr, scope_table); 
            method_class* declaration = m_class_methods[exprr->type_name->get_string()][exprr->name->get_string()];
            if (declaration == NULL)
            {
                semant_error(current_class) << "There is no method named " 
                                            << exprr->name << " for class " 
                                            << exprr->type_name << std::endl;
                expr->set_type(Object);
                break;
            }
            if(!m_inheritance_graph.isParent(exprr->type_name->get_string(), e0_type->get_string()))
            {
                semant_error(current_class) << "Expression to be dispatched on, must be a subtype of " << exprr->type_name << endl;
                expr->set_type(Object);
            }
            else if(declaration->formals->len() > exprr->actual->len())
            {
                semant_error(current_class) << "Too few arguments provided" << std::endl;
                expr->set_type(Object);
                break;
            }
            else if(declaration->formals->len() < exprr->actual->len())
            {
                semant_error(current_class) << "Too many arguments provided" << std::endl;
                expr->set_type(Object);
                break;
            }
            else
            {
                for (size_t i = 0; declaration->formals->more(i); i++)
                {
                    Symbol declared_type = ((formal_class*)declaration->formals->nth(i))->type_decl;
                    Symbol provided_type = analyzeExpression(current_class, exprr->actual->nth(i), scope_table);
                    if (!m_inheritance_graph.isParent(declared_type->get_string(),
                                                     provided_type->get_string()))
                    {
                        semant_error(current_class) << expr->get_line_number() << ": Expression provided for " 
                                                    << i+1 << "th argument " << "(" << provided_type << ")"
                                                    << " must be a sub type of " << declared_type 
                                                    << "(" << declared_type << ")" << std::endl;
                        expr->set_type(Object);
                        break;
                    }
                }
                if(*declaration->return_type == *SELF_TYPE)
                    expr->set_type(e0_type);
                else
                    expr->set_type(declaration->return_type);
                
            }
            break;
        }
        case S_DISPATCH:
        {
            dispatch_class* exprr = (dispatch_class*)expr;
            Symbol e0_type = analyzeExpression(current_class, exprr->expr, scope_table); 
            method_class* declaration = m_class_methods[e0_type->get_string()][exprr->name->get_string()];
            if (declaration == NULL)
            {
                semant_error(current_class) << "There is no method named " << exprr->name << " for class " << e0_type << std::endl;
                expr->set_type(Object);
                break;
            }
            else if(declaration->formals->len() > exprr->actual->len())
            {
                semant_error(current_class) << "Too few arguments provided" << std::endl;
                expr->set_type(Object);
                break;
            }
            else if(declaration->formals->len() < exprr->actual->len())
            {
                semant_error(current_class) << "Too many arguments provided" << std::endl;
                expr->set_type(Object);
                break;
            }
            else
            {
                for (size_t i = 0; declaration->formals->more(i); i++)
                {
                    Symbol declared_type = ((formal_class*)declaration->formals->nth(i))->type_decl;
                    Symbol provided_type = analyzeExpression(current_class, exprr->actual->nth(i), scope_table);
                    if (!m_inheritance_graph.isParent(declared_type->get_string(),
                                                     provided_type->get_string()))
                    {
                        semant_error(current_class) << expr->get_line_number() << ": Expression provided for " 
                                                    << i+1 << "th argument " << "(" << provided_type << ")"
                                                    << " must be a sub type of " << declared_type 
                                                    << "(" << declared_type << ")" << std::endl;
                        expr->set_type(Object);
                        break;
                    }
                }
                if(*declaration->return_type == *SELF_TYPE)
                    expr->set_type(e0_type);
                else
                    expr->set_type(declaration->return_type);
            }
            
            break;
        }
        case S_IF_THEN_ELSE:
        {
            cond_class* exprr = (cond_class*) expr;
            if(*analyzeExpression(current_class, exprr->pred, scope_table) != *Bool)
            {
                semant_error(current_class) << "If condition expression must have type " 
                                            << Bool << " but " << exprr->pred->get_type() 
                                            << " provided" << std::endl;
                                    
            }
            std::string lub_type = m_inheritance_graph.lub(
                                                            analyzeExpression(current_class,
                                                                              exprr->then_exp,
                                                                              scope_table)->get_string(),
                                                            analyzeExpression(current_class,
                                                                             exprr->else_exp,
                                                                             scope_table)->get_string()
                                                           );
            expr->set_type(m_class_symtable.lookup(lub_type)->name);
            break;
        }
        case S_WHILE_LOOP:
        {
            loop_class* exprr = (loop_class*) expr;
            if(*analyzeExpression(current_class, exprr->pred, scope_table) != *Bool)
            {
                semant_error(current_class) << "While loop predicate expression must have Bool type "
                                            << "provided " << exprr->pred->get_type() << std::endl;
            }
            analyzeExpression(current_class, exprr->body, scope_table);
            expr->set_type(Object);
            break;
        }
        case S_CASE:
        {
            typcase_class* exprr = (typcase_class*) expr;
            analyzeExpression(current_class, exprr->expr, scope_table);
            std::string lub_type;
            for (size_t i = 0; i < (size_t)exprr->cases->len(); i++)
            {
                branch_class* branch = (branch_class*)exprr->cases->nth(i);
                scope_table.enterScope();
                scope_table.addid(branch->name->get_string(), branch->type_decl->get_string());
                lub_type = m_inheritance_graph.lub(lub_type, analyzeExpression(current_class, branch->expr, scope_table)->get_string());
                scope_table.exitScope();
            }
            expr->set_type(m_class_symtable.lookup(lub_type)->name);
            break;
        }
        case S_BLOCK:
        {
            block_class* exprr = (block_class*) expr;
            for (int i = 0; i < exprr->body->len(); i++)
                analyzeExpression(current_class, exprr->body->nth(i), scope_table);

            expr->set_type(exprr->body->nth(exprr->body->len() - 1)->get_type());
            break;
        }
        case S_LET:
        {
            let_class* exprr = (let_class*) expr;
            Symbol init_type = analyzeExpression(current_class, exprr->init, scope_table);
            if(*init_type != *exprr->type_decl && *init_type != *No_type)
            {
                semant_error(current_class) << "Initialization expression type "
                                            << init_type 
                                            << " doesn't match the variable declared type "
                                            << exprr->type_decl << std::endl;
                
            }
            scope_table.enterScope();
            scope_table.addid(exprr->identifier->get_string(), exprr->type_decl->get_string());
            expr->set_type(analyzeExpression(current_class, exprr->body, scope_table));
            scope_table.exitScope();
            break;
        }
        case S_PLUS:
        {
            plus_class* exprr = (plus_class*)expr;
            if(*analyzeExpression(current_class, exprr->e1, scope_table) == *Int &&
               *analyzeExpression(current_class, exprr->e2, scope_table) == *Int)
            {
                expr->set_type(Int);
            }
            else
            {
                semant_error(current_class) << "Cannot add two expression exept for two Ints" << endl;
                expr->set_type(Object);
            }
            break;
        }
        case S_SUB:
        {
            sub_class* exprr = (sub_class*)expr;
            if(*analyzeExpression(current_class, exprr->e1, scope_table) == *Int &&
               *analyzeExpression(current_class, exprr->e2, scope_table) == *Int)
            {
                expr->set_type(Int);
            }
            else
            {
                semant_error(current_class) << "Cannot sub two expression exept for two Ints" << endl;
                expr->set_type(Object);
            }
            break;
        }
        case S_MUL:
        {
            mul_class* exprr = (mul_class*)expr;
            if(*analyzeExpression(current_class, exprr->e1, scope_table) == *Int &&
               *analyzeExpression(current_class, exprr->e2, scope_table) == *Int)
            {
                expr->set_type(Int);
            }
            else
            {
                semant_error(current_class) << "Cannot multiply two expression exept for two Ints" << endl;
                expr->set_type(Object);
            }
            break;
        }
        case S_DIV:
        {
            divide_class* exprr = (divide_class*)expr;
            if(*analyzeExpression(current_class, exprr->e1, scope_table) == *Int &&
               *analyzeExpression(current_class, exprr->e2, scope_table) == *Int)
            {
                expr->set_type(Int);
            }
            else
            {
                semant_error(current_class) << "Cannot divide two expression exept for two Ints" << endl;
                expr->set_type(Object);
            }
            break;
        }
        case S_NEG:
        {
            neg_class* exprr = (neg_class*)expr;
            if(*analyzeExpression(current_class, exprr->e1, scope_table) == *Int)
            {
                expr->set_type(Int);
            }
            else
            {
                semant_error(current_class) << "Cannot neg expression exept for two Ints" << endl;
                expr->set_type(Object);
            }
            break;
        }
        case S_LT:
        {
            lt_class* exprr = (lt_class*)expr;
            if(*analyzeExpression(current_class, exprr->e1, scope_table) == *Int &&
               *analyzeExpression(current_class, exprr->e2, scope_table) == *Int)
            {
                expr->set_type(Bool);
            }
            else
            {
                semant_error(current_class) << "Cannot lt two expression exept for two Ints" << endl;
                expr->set_type(Object);
            }
            break;
        }
        case S_EQ:
        {
            eq_class* exprr = (eq_class*)expr;
            if(*analyzeExpression(current_class, exprr->e1, scope_table) ==
               *analyzeExpression(current_class, exprr->e2, scope_table))
            {
                expr->set_type(Bool);
            }
            else
            {
                semant_warning(current_class) << "Equality test between different types" << endl;
                expr->set_type(Bool);
            }
            break;
        }
        case S_LEQ:
        {
            leq_class* exprr = (leq_class*)expr;
            if(*analyzeExpression(current_class, exprr->e1, scope_table) == *Int &&
               *analyzeExpression(current_class, exprr->e2, scope_table) == *Int)
            {
                expr->set_type(Bool);
            }
            else
            {
                semant_error(current_class) << "Cannot leq two expression exept for two Ints" << endl;
                expr->set_type(Object);
            }
            break;
        }
        case S_COMPLEMENT:
        {
            comp_class* exprr = (comp_class*)expr;
            Symbol to_comp_exp_type = analyzeExpression(current_class, exprr->e1, scope_table);
            if(*to_comp_exp_type == *Int || *to_comp_exp_type == *Bool)
                expr->set_type(to_comp_exp_type);
            else
            {
                semant_error(current_class) << "Cannot complement expression of any type exept Bool or Int";
                expr->set_type(Object);
            }
            break;
        }
        case S_INT_CONST:
        {
            expr->set_type(Int);
            break;
        }
        case S_BOOL_CONST:
        {
            expr->set_type(Bool);
            break;
        }
        case S_STR_CONST:
        {
            expr->set_type(Str);
            break;
        }
        case S_NEW:
        {
            new__class* exprr = (new__class*)expr;
            if(*exprr->type_name == *SELF_TYPE)
                expr->set_type(current_class->name);
            else 
                expr->set_type(exprr->type_name);
            break;
        }
        case S_IS_VOID:
        {
            expr->set_type(Bool);
            break;
        }
        case S_NO_EXPR:
        {
            expr->set_type(No_type);
            break;
        }
        case S_OBJECT:
        {
            object_class* exprr = (object_class*)expr;
            if(*exprr->name == *self)
                expr->set_type(current_class->name);
            else if(!scope_table.lookup(exprr->name->get_string()).size())
            {
                semant_error(current_class) << "Identifier " << exprr->name << " not defined" << std::endl;
                expr->set_type(Object);
            }
            else
            {
                expr->set_type(m_class_symtable.lookup(scope_table.lookup(exprr->name->get_string()))->name);
            }
            break;
        }
        default:
            semant_error() << "Got unknown expression type " << expr->get_type();
            break;
    }
    return expr->type;
}


/*   
    This is the entry point to the semantic checker.

    Your checker should do the following two things:

    1) Check that the program is semantically correct
    2) Decorate the abstract syntax tree with type information
    by setting the `type' field in each Expression node.
    (see `tree.h')

    You are free to first do 1), make sure you catch all semantic
    errors. Part 2) can be done in a second stage, when you want
    to build mycoolc.
 */
void program_class::semant()
{
    initialize_constants();
    /* ClassTable constructor may do some semantic analysis */
    ClassTable *classtable = new ClassTable(this->classes); // Main{}, C
    classtable->analyzeClassesRelations();
    classtable->buildInheritanceGraph();
    classtable->analyzeIndividualClasses();
    /* some semantic analysis code may go here */
    if (classtable->errors()) {
	    cerr << "Compilation halted due to static semantic errors." << endl;
	exit(1);
    }
}


