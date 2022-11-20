#ifndef MY_SYMTAB_H
#define MY_SYMTAB_H
#include "symtab.h"
#include <map>
#include <vector>

/**
 * @brief The class that wraps a std::map and provides search and add functionality just to be compatible with 
 *        course SymbolTable
 * 
 * @tparam Key      the key of the map
 * @tparam Info     the value that each key is corresponds to
 */
template<class Key, class Info>
class MySymboTable
{
public:
    std::map<Key, Info> m_table;
    Info lookup(const Key& key)
    {
        if(m_table.find(key) != m_table.end())
            return m_table[key];
        return Info();
    }
    void addid(const Key& key, const Info& info)
    {
        m_table[key] = info;
    }
};

/**
 * @brief The scope table that has been standardizationed after the course Symbol Table
 *        The skeletol is exactly as mentioned in the course
 *        We used std::vector for stack structure (and also to be able to itterate over it)
 *        And also std::map for each individual scope 
 */
class ScopeTable
{
    std::vector<std::map<std::string, std::string> > m_table;
public:
    void enterScope()
    {
        m_table.push_back(std::map<std::string, std::string>());
    }
    void exitScope()
    {
        m_table.pop_back();
    }
    std::string lookup(const std::string& symbol)
    {
        if(m_table.size())
            for(int i = m_table.size() - 1; i >= 0; --i)
            {   
                if (m_table[i].find(symbol) != m_table[i].end())
                    return m_table[i][symbol];
            }
        
        return std::string();
    }
    std::string prob(const std::string& symbol)
    {
        if(m_table.size() && m_table.back().find(symbol) != m_table.back().end())
            return m_table.back()[symbol];
        return std::string();
    }
    void addid(const std::string& key, const std::string& value)
    {
        m_table.back()[key] = value;
    }
};


#endif