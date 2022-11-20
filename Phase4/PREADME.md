# `MySymbolTable`

### The class that wraps a `std::map` and provides search and add functionality just to be compatible with 
course SymbolTable

# `ScopeTable`

### The scope table that has been standardizationed after the course Symbol Table

### The skeletol is exactly as mentioned in the course

### We used `std::vector` for stack structure (and also to be able to itterate over it)

### And also std::map for each individual scope 

# `GraphNode`
    simple class to store each node in the inheritance graph

-   ### `m_parent_node` a pointer to its parent noe
-   ### `m_class_` the class name
-   ### `m_children` a simple map to store the children nodes

# `InheritanceGraph`
-   ### `m_root` the root of the graph
-   ### `m_childs` a referene to a map that map each class to a `std::vector` of its children
-   ### `findClass` simply finds a class in graph using DFS and return its `GraphNode`
-   ### `printGraph` just for debugging
-   ### `lub` finds the Least Upper Bound of two given class names
-   ### `isParent` checks to see if the first param is the father of second param
-   ### `buildGraph` the main method in this class that builds the graph recursively using `m_childs` and adds the parent features to its children in case they don't have one.


# `classTable`
### The main analyzer class.

## Attrs:  
-  ### `m_childs`: 
    maps each each class name (`std::string`) to its children which its self is a map that maps childrens name to their `class__class*`  
- ### `m_class_symtable`
    maps each class name(`std::string`) to their `class__class*`
- ### `m_base_classes_symtab`
    similar to `m_class_symtable` 
    but for base classes
- ### `m_class_methods` 
    simple maps each class name to its methods which itself is a `std::map` that maps each method name to its `method_class*`
- ### `m_class_attr` 
    similar to `m_class_methods`

- ### `analyzeClassesRelations`
    so basically this method checks some relationships like there's no cyclic inheritance or all the prents mentioned are defined ...
- ### `buildInheritanceGraph` 
    builds the inheritance graph
- ### `analyzeIndividualClasses` 
    This method basically itterate over each class features and checks them(if method or attribute)
- ### `analyzeClassAttr` 
    This method checks a class Attribute as told in the comments
- ### `analyzeClassMethod` 
    Similar to `analyzeClassAttr`
- ### `analyzeExpression`
    This method type check the given `Expression expr` in the class `class__class* current_class` with the scope of `ScopeTable& scope_table`

# To Test!
    just use the simple bash code in directory `badTests` and run it which automatically runs 22 tests.