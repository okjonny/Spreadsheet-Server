#include <nlohmann/json.hpp>
/**
 * Structs for all the json messages we send to clients
 */
namespace ss
{
    /**
     * select_cell json struct
     */
    struct select_cell {
        std::string requestType;
        std::string cellName;

        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(select_cell, requestType, cellName)
    };

    /**
     * undo_cell json struct
     */
    struct undo_cell {
        std::string requestType;

        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(undo_cell, requestType)
    };

    /**
     * redo_cell json struct
     */
    struct redo_cell {
        std::string requestType;
        std::string cellName;

        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(redo_cell, requestType, cellName)
    };

    /**
    * redo_cell json struct
    */
    struct selected_cell {
        std::string messageType;
        std::string cellName;
        int selector;
        std::string selectorName;

        selected_cell(std::string cell, int _selector, std::string name)
        {
            messageType = "cellSelected";
            cellName = cell;
            selector = _selector;
            selectorName = name;
        }

        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(selected_cell, messageType, cellName, selector, selectorName)
    };

    /**
     * cell_updated json struct
     */
    struct cell_updated {
        std::string messageType;
        std::string cellName;
        std::string contents;

        cell_updated(std::string name, std::string cell_contents)
        {
            messageType = "cellUpdated";
            cellName = name;
            contents = cell_contents;
        }

        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(cell_updated, messageType, cellName, contents)
    };

    /**
     * invalid_request json struct
     */
    struct invalid_request {
        std::string messageType;
        std::string cellName;
        std::string message;

        invalid_request(std::string cell, std::string error_message)
        {
            messageType = "requestError";
            cellName = cell;
            message = error_message;
        }

        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(invalid_request, messageType, cellName, message)
    };

    /**
     * server_shutdown json struct
     */
    struct server_shutdown {
        std::string messageType;
        std::string message;

        server_shutdown(std::string error_message)
        {
            messageType = "serverError";
            message = error_message;
        }

        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(server_shutdown, messageType, message)
    };

    /**
     * disconnected json struct
     */
    struct disconnected {
        std::string messageType;
        int user;

        disconnected(int _user)
        {
            messageType = "disconnected";
            user = _user;
        }

        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(disconnected, messageType, user)
    };
}
