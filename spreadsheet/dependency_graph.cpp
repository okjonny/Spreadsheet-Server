#include "dependency_graph.h"
#include <unordered_map>

namespace ss{
    //Constructor
    dependency_graph::dependency_graph()
    {
        size = 0;
    }

    //Deconstructor
    dependency_graph::~dependency_graph()
    = default;

    //Returns the number of dependencies within the graph
    int dependency_graph::get_size() const
    {
        return size;
    }

    //[] operator override
    //Returns the number of dependees assosiated with the passed in key
    int dependency_graph::operator[](std::string s)
    {
        //If the key doesn't exist, then we consider it to have 0 dependees. Otherwise, we return the count of dependees.
        if (!contains(dependents, s))
            return 0;

        return dependents[s].size();
    }

    //Reports if dependents are assosiated with passed in key
    bool dependency_graph::has_dependents(std::string s)
    {
        return has_dependency(s, dependees);
    }

    //Reports if dependees are assosiated with passed in key
    bool dependency_graph::has_dependees(std::string s)
    {
        return has_dependency(s, dependents);
    }

    //Adds a dependee to the dependents dictionary to help AddDependency enforce
    //the two-way nodal connection between a dependee and dependent.
    void dependency_graph::add_dependee(std::string s, std::string t)
    {
        //If the dependent "t" is already existent, link it to dependee "s"
        if (contains(dependents, t))
            dependents[t].insert(s);
            //Otherwise, label it as a dependee.
        else
        {
            std::unordered_set<std::string> deps = {s};
            dependents.insert({t, deps});
        }
        size++;
    }

    //Assists the "Get" methodsin that all their logic is stored in this helper method in a generic and reusable way.
    //Allows for the retrieval of either dependees or dependents of a given key, depending on the method calling it.
    void dependency_graph::add_dependency(std::string s, std::string t)
    {
        //If the provided list does not contain the key, it means there are no dependees/dependents and we must return an empty set.
        if (!is_pair(s, t))
        {
            if (contains(dependees, s))
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

    //Enumerates dependents(s)
    std::unordered_set<std::string> dependency_graph::get_dependents(std::string s)
    {
        return get_dependency(dependees, s);
    }

//Enumerates dependees(s)
    std::unordered_set<std::string> dependency_graph::get_dependees(std::string s)
    {
        return get_dependency(dependents, s);
    }

// Removes the ordered pair (s,t), if it exists
    void dependency_graph::remove_dependency(std::string s, std::string t)
    {
        //If pair exits, remove it
        if (is_pair(s, t))
        {
            dependees[s].erase(t);
            dependents[t].erase(s);
            size--;
        }
    }

    //Removes all existing ordered pairs of the form (r,s). Then, for each t
    //in new_dependees, adds the ordered pair (t,s).
    void dependency_graph::replace_dependees(std::string s, std::unordered_set<std::string> new_dependees)
    {
        //iterate and remove all dependencies
        for (auto &r : dependees)
            remove_dependency(r.first, s);
        //iterate and add new dependencies
        for (std::string t : new_dependees)
            add_dependency(t, s);
    }

    //Removes all existing ordered pairs of the form (s,r). Then, for each
    //t in new_dependents, adds the ordered pair (s,t).
    void dependency_graph::replace_dependents(std::string s, std::unordered_set<std::string> new_dependents)
    {
        for (auto &r  : dependents)
            remove_dependency(s, r.first);

        for (std::string t : new_dependents)
            add_dependency(s, t);
    }



    //PRIVATE HELPER FUNCTIONS

    //Checks if the given depdendency already exists
    bool dependency_graph::is_pair(std::string s, std::string t)
    {
        //if contained within both lists -> true
        if(contains(dependees, s) && contains(dependents, t))
            return (dependees[s].find(t) != dependees[s].end()) && (dependents[t].find(s) != dependents[t].end());
        //else its false
        return false;
    }


    // Checks if the given key has a value
    bool dependency_graph::contains(std::unordered_map<std::string, std::unordered_set<std::string>> map, std::string s)
    {
        return map.find(s) != map.end();
    }

    //Assists the "Has" methods in that all their logic is stored in this helper method in a generic and reusable way.
    //If the corresponding dictionary (either Dependees or Dependents) does not contain the key in question, it returns false.
    //Otherwise, it returns true if its corresponding set of dependees/dependents contains values, and false if it is empty.
    bool dependency_graph::has_dependency(std::string s,
                                          std::unordered_map<std::string, std::unordered_set<std::string>> dependency_list)
    {
        if (!contains(dependency_list, s))
            return false;

        return dependency_list[s].size() > 0;
    }

    //Assists the "Get" methodsin that all their logic is stored in this helper method in a generic and reusable way.
    //Allows for the retrieval of either dependees or dependents of a given key, depending on the method calling it.
    std::unordered_set<std::string>
    dependency_graph::get_dependency(std::unordered_map<std::string, std::unordered_set<std::string>> dependency_list,
                                     std::string s)
    {
        //If the provided list does not contain the key, it means there are no dependees/dependents and we must return an empty set
        if (!contains(dependency_list, s))
            return std::unordered_set<std::string>();

        return dependency_list[s];
    }
}