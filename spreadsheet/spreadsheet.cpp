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


    std::stack<std::string> spreadsheet::get_history()
    {
        return commands_received;
    }

    std::unordered_map<std::string, std::stack<std::string>> spreadsheet::get_cell_list()
    {
        return nonempty_cells;
    }

    void spreadsheet::add_to_history(std::string s)
    {
        commands_received.push(s);
    }

    std::string spreadsheet::get_cell_contents(std::string cell)
    {
        // return the last added contents
        if (nonempty_cells.find(cell) != nonempty_cells.end())
            return nonempty_cells.at(cell).top();
        return "";
    }

    bool spreadsheet::is_valid_name(std::string name)
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

    std::list<std::string> spreadsheet::set_contents_of_cell(std::string name, std::string contents)
    {

        name = formula::normalize(name);
        //
        //    if (content is null)
        //    throw new ArgumentNullException();

            name_check(name);

        std::list<std::string> dependency_list;

        /*if (formula::is_double(contents))
            dependency_list = set_cell_contents(name, std::stod(contents));*/
        if (contents[0] == '=')
        {
            dependency_list = spreadsheet::set_cell_contents(name, formula(contents.substr(1, contents.size() - 1)));
        } else
            dependency_list = spreadsheet::set_cell_contents(name, contents);

        //    foreach (string s in dependency_list)
        //{
        //    if (NonemptyCells.ContainsKey(s))
        //    NonemptyCells[s].UpdateValue(Lookup);
        //}

        changed = true;

        return dependency_list;
    }

    std::list<std::string> spreadsheet::set_cell_contents(std::string cell_name, std::string contents)
    {
        // If a cell exists, we replace its contents, otherwise we create it.
        if (nonempty_cells.find(cell_name) != nonempty_cells.end())
        {
            undo_stack.push({cell_name, get_cell_contents(cell_name)});

            nonempty_cells[cell_name].push(contents);
            dependencies.replace_dependees(cell_name, std::unordered_set<std::string>());
        } else
        {

            undo_stack.push({cell_name, get_cell_contents(cell_name)});

            // create a new cell
            std::stack<std::string> contents_history;
            contents_history.push((contents));
            nonempty_cells.insert({cell_name, contents_history});
        }

        if (get_cell_contents(cell_name) == "")
            nonempty_cells.erase(cell_name);

        return spreadsheet::get_cells_to_recalculate(cell_name);
    }

//A1 used to be 69
    std::list<std::string> spreadsheet::set_cell_contents(std::string name, formula expression)
    {

        //Storing the contents in case we need to revert_cell_contents back
        std::string previous_contents = get_cell_contents(name);
        std::unordered_set<std::string> previous_dependees = dependencies.get_dependees(name);

        //If a cell exists, we replace its contents, otherwise we create it.
        if (nonempty_cells.find(name) != nonempty_cells.end())
        {
            nonempty_cells[name].push("=" + expression.to_string());
        } else
        {
            std::stack<std::string> contents_history;
            contents_history.push("=" + expression.to_string());
            nonempty_cells.insert({name, contents_history});
        }

        dependencies.replace_dependees(name, expression.get_variables());

        //Try and catch to revert_cell_contents to the previous state before throwing
        try
        { return get_cells_to_recalculate(name); }

        catch (std::runtime_error)
        {
            if (previous_contents != "")
            {
                nonempty_cells[name].pop(); // TODO: check pushing onto the stackkk!!!!!!
                dependencies.replace_dependees(name, previous_dependees);
            } else
                nonempty_cells.erase(name);

            throw std::runtime_error("Circular dependencies found.");
        }
    }

    std::unordered_set<std::string> spreadsheet::get_direct_dependents(std::string name)
    {
        return dependencies.get_dependents(name);
    }


//    std::vector<std::string> spreadsheet::get_cells_to_recalculate(std::unordered_set<std::string> names)
//    {
//        std::vector<std::string> result;
//        return result;
//    }
//
//    std::vector<std::string> spreadsheet::get_cells_to_recalculate(std::string name)
//    {
//        std::vector<std::string> result;
//        return result;
//    }
//
//    // TODO: do we want a list for changed?
//    void spreadsheet::visit(std::string start, std::string name, std::unordered_set<std::string> visited,
//                            std::list<std::string> changed)
//    {
//
//    }



    /// <summary>
    /// A convenience method for invoking the other version of GetCellsToRecalculate
    /// with a singleton set of names.  See the other version for details.
    /// </summary>
    std::list<std::string> spreadsheet::get_cells_to_recalculate(std::string name)
    {
        return get_cells_to_recalculate(std::unordered_set<std::string>{name});
    }


    /// <summary>
    /// Requires that names be non-null.  Also requires that if names contains s,
    /// then s must be a valid non-null cell name.
    ///
    /// If any of the named cells are involved in a circular dependency,
    /// throws a CircularException.
    ///
    /// Otherwise, returns an enumeration of the names of all cells whose values must
    /// be recalculated, assuming that the contents of each cell named in names has changed.
    /// The names are enumerated in the order in which the calculations should be done.
    ///
    /// For example, suppose that
    /// A1 contains 5
    /// B1 contains 7
    /// C1 contains the formula A1 + B1
    /// D1 contains the formula A1 * C1
    /// E1 contains 15
    ///
    /// If A1 and B1 have changed, then A1, B1, and C1, and D1 must be recalculated,
    /// and they must be recalculated in either the order A1,B1,C1,D1 or B1,A1,C1,D1.
    /// The method will produce one of those enumerations.
    ///
    /// PLEASE NOTE THAT THIS METHOD DEPENDS ON THE ABSTRACT METHOD GetDirectDependents.
    /// IT WON'T WORK UNTIL GetDirectDependents IS IMPLEMENTED CORRECTLY.
    /// </summary>
    std::list<std::string> spreadsheet::get_cells_to_recalculate(std::unordered_set<std::string> names)
    {
        std::list<std::string> changed;
        std::unordered_set<std::string> visited;
        for (std::string name : names)
        {
            if (visited.find(name) == visited.end())
            {
                visit(name, name, visited, changed);
            }
        }
        return changed;
    }


    /// <summary>
    /// A helper for the GetCellsToRecalculate method.
    /// </summary>
    void spreadsheet::visit(std::string start, std::string name, std::unordered_set<std::string> visited,
                            std::list<std::string> changed)
    {
        visited.insert(name);

        // iterate through the current name's list of direct dependents
        for (std::string n : get_direct_dependents(name))
        {
            // during recursion, the start point should not be listed as a dependent
            if (n == (start))
            {
                   throw std::runtime_error("Circular dependencies found.");
            }
                // if this name's dependents have not been searched yet, enter another frame of recursion
            else if (visited.find(name) == visited.end())
            {
                visit(start, n, visited, changed);
            }
        }

        // once every cell's (direct and indirect dependents of name) dependents have been searched,
        // add them to the changed list as the method frames begin returning.
        changed.push_front(name);
    }

    void spreadsheet::name_check(std::string name)
    {
        if (!(std::regex_match(name, std::regex("^[a-zA-Z]+[0-9]+$"))) || !(regex_match(name, std::regex("^[A-Z]+[0-9]?[0-9]$"))))
            throw std::runtime_error(name + " is an invalid cell name.");
    }

    void spreadsheet::revert_cell_contents(std::string name)
    {
        std::unordered_set<std::string> previous_dependees = dependencies.get_dependees(name);
        std::string previous_contents = get_cell_contents(name);
        nonempty_cells[name].pop();

        try
        { get_cells_to_recalculate(name); }
        catch(std::runtime_error){
            if (previous_contents != "")
            {
                nonempty_cells[name].push(previous_contents); // TODO: check pushing onto the stackkk!!!!!!
                dependencies.replace_dependees(name, previous_dependees);
            } else
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