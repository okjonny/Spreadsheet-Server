#include <iostream>
#include "dependency_graph.h"
#include <regex>

int main()
{
/*    ss::dependency_graph graph1;
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
    }*/

    std::regex p("^[A-Z]+[0-9]?[0-9]$");
    if(std::regex_match("A1", p))
        std::cout << "A1 matched" << std::endl;



    return 0;
}