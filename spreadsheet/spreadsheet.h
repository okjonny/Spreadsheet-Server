//
// Created by hibban on 4/26/21.
//
#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <vector>
#include <mutex>
#include "socket_state.h"
#include "cell.h"
#include <unordered_map>
#include "server_controller.h"
#include "dependency_graph.h"
#include "formula.h"
#include <stack>
#include <list>
#include <unordered_set>

class server_controller;

namespace ss
{
    class spreadsheet {

        friend class server_controller;

        bool is_Undo;
        std::string name;
        std::vector<network_util::socket_state*> users_connected;
        std::stack<std::string> commands_received;

        /// Keeps track of edits made to this spreadsheet in a stack.
        std::stack<std::pair<std::string, std::string>> undo_stack;


    public:

        std::unordered_map<std::string, std::stack<std::string>> nonempty_cells;

        spreadsheet();

        ~spreadsheet();

        void add_user_to_spreadsheet(network_util::socket_state &state);

        /// returns a vector of users that are connected to this spreadsheet
        std::vector<network_util::socket_state*> get_users_connected();

        void undo();

        //std::unordered_map<std::string, std::vector<std::string>> get_cell_list();
        std::unordered_map<std::string, std::stack<std::string>> get_cell_list();

        void add_cell(std::string name, cell c);

        std::stack<std::string> get_history();

        void add_to_history(std::string);

        // SHOULD THESE BE PRIVATE HMMM

        /// Returns the most recent contents of this cell
        std::string get_cell_contents(std::string cell);

        /// Returns true if name starts with a letter or underscore, followed by any number
        /// of letters, numbers, or underscores. False otherwise.
        static bool is_valid_name(std::string name);

        std::vector<std::string> get_nonempty_cells();

        /// Checks to see if the contents are a formula or not, then sets the contents of a cell accordingly.
        std::list<std::string> set_contents_of_cell(std::string name, std::string contents);

        /// Sets the contents of this cell to a formula. Returns the list of this cell's direct and indirect dependents.
        /// If the input Formula created a CircularException, all changes to the spreadsheet (and dependencies) are reverted
        std::list<std::string> set_cell_contents(std::string name, formula contents);

        /// Sets the contents of this cell to input text (can be regular text or a double).
        /// Returns the list of the given cell's directs and indirect dependents.
        std::list<std::string> set_cell_contents(std::string cell_name, std::string contents);

        void revert_cell_contents(std::string cell_name);

        std::pair<std::string, std::string> get_undo_contents();

    private:

        bool changed;

        void name_check(std::string name);

        dependency_graph dependencies;

        /// Returns a set of the given cell's direct dependents.
        std::unordered_set<std::string> get_direct_dependents(std::string name);

        /// If any of the named cells are involved in a circular dependency, throws a runtime error.
        /// Otherwise, returns an enumeration of the names of all cells whose values must
        /// be recalculated, assuming that the contents of each cell named in names has changed.
        /// Names are enumerated in the order in which the calculations should be done.
        std::list<std::string> get_cells_to_recalculate(std::unordered_set<std::string> names);

        /// Convenience method for the other get_cells_to_recalculate taking in a set as param.
        /// Returns a vector of the name of the cells that must to be recalculated.
        std::list<std::string> get_cells_to_recalculate(std::string name);

        /// Helper method for get_cells_to_recalculate method
        // TODO: do we want a list for changed?

        void visit(std::string start, std::string name, std::unordered_set<std::string> visited,
                   std::list<std::string> changed);
    };
}

#endif //SPREADSHEET_H