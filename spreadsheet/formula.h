//
// Created by hibban on 4/26/21.
//

#ifndef TEAMPOG_CS3505_FORMULA_H
#define TEAMPOG_CS3505_FORMULA_H

#include <list>
#include <string>
#include <locale>

namespace ss
{
    class formula
    {
        std::list<std::string> tokens;
        std::list<std::string> variables;

    public:
        formula(std::string formula);

    private:
        std::string normalize(std::string token);
        static std::list<std::string> get_tokens(std::string formula);
    };

}
#endif //TEAMPOG_CS3505_FORMULA_H
