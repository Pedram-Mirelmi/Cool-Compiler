#include <iostream>
#include "classGraph.h"

int main()
{
    std::map<std::string, std::set<std::string> > m_childs;
    m_childs["Object"].insert("shit");

    m_childs["shit"].insert("shit1");

    m_childs["shit"].insert("shit2");

    m_childs["shit"].insert("shit2");
    m_childs["Object"].insert("Int");
    m_childs["Object"].insert("String");
    m_childs["Object"].insert("Bool");
    m_childs["Object"].insert("Io");


    m_childs["Object"].insert("Main");

    m_childs["String"].insert("arrrr");
    m_childs["arrrr"].insert("arrrr1");

    
    
    
    
    InheritanceGraph graph("Object", m_childs);
    graph.printGraph();
    cout << graph.lub("shit", "shit2");

}