//
// Created by hibban on 4/26/21.
//

#ifndef TEAMPOG_CS3505_FORMULA_H
#define TEAMPOG_CS3505_FORMULA_H

#include <string>
#include <locale>
#include <vector>
#include <unordered_set>

namespace ss
{
    class formula
    {
        std::vector<std::string> tokens;
        std::vector<std::string> variables;

    public:
        formula(std::string formula);
        std::unordered_set<std::string> get_variables();
        std::string to_string();
        bool equals(formula f);
        int get_hash_code();
        bool operator==(const formula& other);
        bool operator!=(const formula& other);

    private:
        static std::string normalize(std::string token);
        std::vector<std::string> get_tokens(std::string formula);

        static void syntax_check(std::vector<std::string> tokens);

        static bool is_legal(std::string token);
        static bool is_variable(std::string token);
        static bool is_operator(std::string token);
        static bool is_double(std::string token);
    };

}
#endif //TEAMPOG_CS3505_FORMULA_H
