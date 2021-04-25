#ifndef DEPENDENCY_GRAPH_H
#define DEPENDENCY_GRAPH_H

//includes here
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <bits/stdc++.h>
#include <functional>

using namespace std;
namespace ss
{
    class dependency_graph
    {
        int size;

        std::unordered_map<std::string, std::unordered_set<std::string>> dependents;
        std::unordered_map<std::string, std::unordered_set<std::string>> dependees;

    private:
        bool is_pair(std::string s, std::string t);
        bool contains(std::unordered_map<std::string, std::unordered_set<std::string>> map, std::string s);
        bool has_dependency(std::string s,std::unordered_map<std::string, std::unordered_set<std::string>> dependency_list);
        std::unordered_set<std::string> get_dependency(std::unordered_map<std::string, std::unordered_set<std::string>> dependency_list, std::string s);
    public :
        //Constructor/Destructor
        dependency_graph();
        ~dependency_graph();

        int get_size() const;
        int operator[](std::string s);

        bool has_dependents(std::string s);
        bool has_dependees(std::string s);

        void add_dependency(std::string s, std::string t);
        void add_dependee(std::string s, std::string t);

        std::unordered_set<std::string> get_dependents(std::string s);
        std::unordered_set<std::string> get_dependees(std::string s);
        void remove_dependency(std::string s, std::string t);

        //Replace
        void replace_dependees(std::string s, std::unordered_set<std::string> new_dependees);
        void replace_dependents(std::string s, std::unordered_set<std::string> new_dependents);
    };
}
#endif