#include <iostream>
#include "dependency_graph.h"


int main()
{
    ss::dependency_graph graph1;
    graph1.add_dependency("zyad", "ally");
    graph1.add_dependency("zyad", "zyad");
    graph1.add_dependency("zyad", "andrew");
    graph1.add_dependency("zyad", "mike");
    std::unordered_set<std::string> set1 = graph1.get_dependents("zyad");
    std::cout << "printing zyads fuckking set   " << std::endl;
    for (std::string s : set1)
    {
        std::cout << s << std::endl;
    }
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::unordered_set<std::string> new_set = {"jonny", "hibban", "tyler"};
    graph1.replace_dependents("zyad", new_set);


    std::unordered_set<std::string> set2 = graph1.get_dependents("zyad");
    std::cout << "printing zyads NEW NEW fuckking set   " << std::endl;
    for (std::string s : set2)
    {
        std::cout << s << std::endl;
    }
    return 0;
}