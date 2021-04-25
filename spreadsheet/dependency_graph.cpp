#include "dependency_graph.h"
#include <unordered_map>

namespace ss{
    dependency_graph::dependency_graph()
    {
        size = 0;
    }


    dependency_graph::~dependency_graph()
    = default;


    int dependency_graph::get_size() const
    {
        return size;
    }


    int dependency_graph::operator[](std::string s)
    {
        //If the key doesn't exist, then we consider it to have 0 dependees. Otherwise, we return the count of dependees.
        if (!contains(dependents, s))
            return 0;

        return dependents[s].size();
    }


    bool dependency_graph::has_dependents(std::string s)
    {
        return has_dependency(s, dependees);
    }


    bool dependency_graph::has_dependees(std::string s)
    {
        return has_dependency(s, dependents);
    }


    void dependency_graph::add_dependee(std::string s, std::string t)
    {
        if(contains(dependents, t))
            dependents[t].insert(s);
        else
        {
            std::unordered_set<std::string> deps = {s};
            dependents.insert({t, deps});
        }
        size++;
    }


    void dependency_graph::add_dependency(std::string s, std::string t)
    {
        if(!is_pair(s,t))
        {
            if(contains(dependees, s))
            {
                dependees[s].insert(t);
                add_dependee(s, t);
            }
            else
            {
                std::unordered_set<std::string> set = {t};
                dependees.insert({s, set});
                add_dependee(s, t);
            }
        }
    }


    std::unordered_set<std::string> dependency_graph::get_dependents(std::string s)
    {
        return get_dependency(dependees, s);
    }


    std::unordered_set<std::string> dependency_graph::get_dependees(std::string s)
    {
        return get_dependency(dependents, s);
    }


    void dependency_graph::remove_dependency(std::string s, std::string t)
    {
        if(is_pair(s,t))
        {
            dependees[s].erase(t);
            dependents[t].erase(s);
            size--;
        }
    }


    void dependency_graph::replace_dependees(std::string s, std::unordered_set<std::string> new_dependees)
    {
        for(auto &r : dependees)
            remove_dependency(r.first,s);

        for(std::string t : new_dependees)
            add_dependency(t,s);
    }


    void dependency_graph::replace_dependents(std::string s, std::unordered_set<std::string> new_dependents)
    {
        for(auto &r  : dependents)
            remove_dependency(s,r.first);

        for(std::string t : new_dependents)
            add_dependency(s,t);
    }



//PRIVATE HELPER FUNCTIONS

    bool dependency_graph::is_pair(std::string s, std::string t)
    {
        if(contains(dependees, s) && contains(dependents, t))
            return (dependees[s].find(t) != dependees[s].end()) && (dependents[t].find(s) != dependents[t].end());

        return false;
    }


    bool dependency_graph::contains(std::unordered_map<std::string, std::unordered_set<std::string>> map, std::string s)
    {
        return map.find(s) != map.end();
    }


    bool dependency_graph::has_dependency(std::string s,std::unordered_map<std::string, std::unordered_set<std::string>> dependency_list)
    {
        if(!contains(dependency_list, s))
            return false;

        return dependency_list[s].size() > 0;
    }


    std::unordered_set<std::string> dependency_graph::get_dependency(std::unordered_map<std::string, std::unordered_set<std::string>> dependency_list, std::string s)
    {
        if(!contains(dependency_list, s))
            return std::unordered_set<std::string>();

        return dependency_list[s];
    }
}