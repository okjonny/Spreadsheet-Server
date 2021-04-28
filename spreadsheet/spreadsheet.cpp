//
// Created by hibban on 4/25/21.
//

#include "spreadsheet.h"
#include <string>
#include <regex>
#include <unordered_map>
#include "formula.h"

namespace ss
{
//    struct cell {
//        std::string contents;
//    };

    spreadsheet::spreadsheet()
    {
    }

    void spreadsheet::add_user_to_spreadsheet(long s)
    {
        users_connected.push_back(s);
    }

    std::vector<long> spreadsheet::get_users_connected()
    {
        return users_connected;
    }


    std::vector<std::string> spreadsheet::get_commands_received()
    {
        return commands_received;
    }

    std::unordered_map<std::string, std::vector<std::string>> spreadsheet::get_cell_list()
    {
        return nonempty_cells;
    }

    void spreadsheet::add_command(std::string s)
    {
        commands_received.push_back(s);
    }

    std::string spreadsheet::get_cell_contents(std::string cell)
    {
        if (nonempty_cells.find(cell) != nonempty_cells.end())
            return nonempty_cells.at(cell)[nonempty_cells.at(cell).size() - 1];
        return "";
    }

    static bool is_valid_name(std::string name)
    {
        return std::regex_match(name, std::regex("^[a-zA-Z_][a-zA-Z0-9_]*$"));
    }

    std::vector<std::string> spreadsheet::get_nonempty_cells()
    {
        std::vector<std::string> cell_names;

        for (auto name : nonempty_cells)
            cell_names.push_back(name.first);

        return cell_names;
    }

     std::vector<std::string> spreadsheet::set_contents_of_cell(std::string name, std::string contents)
{
    //name = Normalize(name);
//
//    if (content is null)
//    throw new ArgumentNullException();

//    NameCheck(name);

   std::vector<std::string> dependency_list;

   /*if (formula::is_double(contents))
       dependency_list = set_cell_contents(name, std::stod(contents));*/
   if (contents[0] == '=') {
       dependency_list = set_cell_contents(name, formula(contents.substr(1, contents.size() - 1)));
   }
    else
        dependency_list = set_cell_contents(name, contents);

//    foreach (string s in dependency_list)
//{
//    if (NonemptyCells.ContainsKey(s))
//    NonemptyCells[s].UpdateValue(Lookup);
//}

changed = true;

return dependency_list;
}

    std::vector<std::string> spreadsheet::set_cell_contents(std::string name, std::string contents)
{
//If a cell exists, we replace its contents, otherwise we create it.
        if (nonempty_cells.find(name) != nonempty_cells.end())
{
nonempty_cells[name].push_back(contents); //////////////////
dependencies.replace_dependees(name, std::unordered_set<std::string>());
}
else
nonempty_cells.insert({name, contents});

if (get_cell_contents(name) == "")
    nonempty_cells.erase(name);

return new List<string>(GetCellsToRecalculate(name).ToList());
}


std::vector<std::string> set_cell_contents(std::string name, formula expression)
{
//Storing the contents in case we need to revert back
std::string previous_contents = get_cell_contents(name);
std::unordered_set<std::string> previous_dependees = dependencies.get_dependees(name);

//If a cell exists, we replace its contents, otherwise we create it.
if (nonempty_cells.ContainsKey(name))
{
nonempty_cells[name] = "=" + expression.to_string();
}
else
{
nonempty_cells.insert({name, "=" + expression.to_string()});
}


dependencies.replace_dependees(name, expression.get_variables());

//Try and catch to revert to the previous state before throwing
try { return get_cells_to_recalculate(name); }

catch (std::runtime_error)
{
if (previous_contents != "")
{
nonempty_cells[name] = previous_contents;
dependencies.replace_dependees(name, previous_dependees);
}

else
nonempty_cells.erase(name);

throw std::runtime_error("Circular dependencies found.");
}
}





//{}(network_util::socket_state s);
//string GetCellContents(string name)
//{
//    name = Normalize(name);
//    NameCheck(name);
//
//    if(NonemptyCells.TryGetValue(name, out Cell c))
//    return c.Contents;
//    return "";
//}
}