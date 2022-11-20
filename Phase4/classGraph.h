#ifndef CLASS_GRAPH_H
#define CLASS_GRAPH_H

#include <map>
#include <set>
#include <vector>
#include <iostream>
#include "cool-tree.h"
using namespace std;
class GraphNode
{
public:
    GraphNode(const std::string& class_, GraphNode* parent_node)
        :   m_parent_node(parent_node),
            m_class_(class_)
    {
        
    };
    GraphNode* m_parent_node;
    std::string m_class_;
    std::map<std::string, GraphNode*> m_children;

    GraphNode*addChild(const std::string& class_name)
    {
        return m_children[class_name] = new GraphNode(class_name, this);
    }
    GraphNode* lookupChild(const std::string& class_name)
    {
        return m_children.find(class_name) != m_children.end() ?
                             m_children[class_name] :
                             NULL ;
    }

};



class InheritanceGraph
{
public:
    GraphNode* m_root;
    std::map<std::string, std::set<std::string> > m_childs;
    InheritanceGraph() {};
    
    InheritanceGraph(const std::string& root,
                     const std::map<std::string, std::set<std::string> >& childs,
	                 std::map<std::string, std::map<std::string, attr_class*> >& class_attrs,
                     std::map<std::string, std::map<std::string, method_class*> >& class_methods)
        : m_root(new GraphNode(root, NULL)),
          m_childs(childs)
    {
        buildGraph(m_root, class_attrs, class_methods);    
    }

    GraphNode* findClass(const std::string& target)
    {
        return findClass(m_root, target);
    }

    GraphNode* setRoot(const std::string& new_root)
    {
        return m_root = new GraphNode(new_root, NULL);
    }

    void printGraph()
    {
        printGraph(0, m_root);
    }

    std::string lub(const std::string& first, const std::string& second)
    {
        if(!first.size())
            return second;
        if(!second.size())
            return first;
        if(isParent(first, second))
            return first;
        if(isParent(second, first))
            return second;
        return lub(findClass(first)->m_parent_node->m_class_, second);
    }

    bool isParent(const std::string& parent, const std::string& child)
    {
        bool result = findClass(findClass(parent), child) != NULL;
        return result;
    }
private:
    void printGraph(int indent, GraphNode* node)
    {
        cout << std::string(indent, '\t') << node->m_class_ << "\n";
        for (std::map<std::string, GraphNode *>::iterator itter = node->m_children.begin();
                    itter != node->m_children.end();
                    itter++)
        {
            if (itter->second != NULL)
                printGraph(indent+1, itter->second);
        }
    }

    GraphNode* findClass(GraphNode* starting_node, const std::string& target)
    {
        if(starting_node == NULL) 
            return NULL;
        GraphNode* result = NULL;
        if (starting_node->m_class_ == target)
            return starting_node;
        else if (starting_node->m_children.find(target) != starting_node->m_children.end())
            return starting_node->m_children[target];
        else 
            for (std::map<std::string, GraphNode *>::iterator itter = starting_node->m_children.begin();
                    itter != starting_node->m_children.end();
                    itter++)
            {
                if ((result = findClass(itter->second, target)) != NULL)
                    return result;
            }
        return NULL;
    }

    void buildGraph(GraphNode* node,
                    std::map<std::string, std::map<std::string, attr_class*> >& class_attrs,
                    std::map<std::string, std::map<std::string, method_class*> >& class_methods)
    {
        std::set<std::string> children = m_childs[node->m_class_];
        for (std::set<std::string>::iterator set_itter = children.begin();
                    set_itter != children.end();
                    set_itter++)
        {
            GraphNode* child_node = node->addChild(*set_itter);
            std::map<std::string, attr_class*>& parent_attrs = class_attrs[node->m_class_];
            std::map<std::string, method_class*>& parent_methods = class_methods[node->m_class_];

            std::map<std::string, attr_class*>& child_attrs = class_attrs[child_node->m_class_];
            std::map<std::string, method_class*>& child_methods = class_methods[child_node->m_class_];

            for(std::map<std::string, attr_class*>::iterator itter = parent_attrs.begin();
                                                             itter != parent_attrs.end();
                                                             itter++)
            {
                if (child_attrs.find(itter->first) == child_attrs.end())
                {
                    child_attrs[itter->first] = itter->second;
                }
                
            }
            for(std::map<std::string, method_class*>::iterator itter = parent_methods.begin();
                                                             itter != parent_methods.end();
                                                             itter++)
            {
                if (child_methods.find(itter->first) == child_methods.end())
                {
                    child_methods[itter->first] = itter->second;
                }
                
            }
            buildGraph(child_node, class_attrs, class_methods);
        }
    }    

};




#endif
