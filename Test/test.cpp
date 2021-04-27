//
// Created by hibban on 4/26/21.
//
#include <regex>
#include <iostream>
#include <string>
#include <cctype>

/*
std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}
*/

bool is_variable(char token)
{
std::string tokenism = "";
tokenism += token;
    std::regex variable(R"([a-zA-Z_](?: [a-zA-Z_]|\d)*)");
return std::regex_match(tokenism, variable);
}

bool is_double(char token)
{
    try
    {
        std::string tokenism(1, token);
        std::stod(tokenism);
    }
    catch(...)
    {
        return false;
    }
    return true;
}


int main()
{
    std::string formula = "((4.4   *(6  /5.3)))";
    std::vector<std::string> tokens;
    // Add the first character to a new token. We make the assumption
    // that the string is not empty.
    tokens.push_back(std::string(1, formula[0]));

    // Now iterate over the rest of the characters from the input string.
    for (int i = 1; i < formula.length(); i++)
    {
        char token = formula[i]; // Store the current character.
        char last_token = formula[i - 1]; // Store the last character.

        // We're checking if the last character is either a digit or the
        // dot, AND if the current character is either a digit or a dot.
        if (((is_double(token)) || (token == '.' && is_double(last_token)) || is_variable((token))) && (is_double(last_token) || last_token == '.' || is_variable((last_token))))
        {
            // If so, add the current character to the last token.
            int last_index = (tokens.size() - 1);
            tokens[last_index] = tokens[last_index] + token;
        }

        else if (token != ' ')
            tokens.push_back(std::string(1, token));

    }

    for(std::string &s : tokens)
        std::cout << s << std::endl;

























    // Patterns for individual tokens
    //std::string left_par = R"(\()";
    //std::string right_par = R"(\))";
    //std::string op = R"([\+\-*/])";
    //std::string var = R"([a-zA-Z_](?: [a-zA-Z_]|\d)*)";
    // std::string number = R"((?: \d+\.\d* | \d*\.\d+ | \d+ ) (?: [eE][\+-]?\d+)?)";
    //std::string space = R"(\s+)";

    // Overall pattern
/*    char temp_pattern [left_par.length() + right_par.length() + op.length() +
                                  var.length() + number.length() + space.length()];

    std::sprintf(temp_pattern,"(%s) | (%s) | (%s) | (%s) | (%s)",
                 left_par.c_str(), right_par.c_str(), op.c_str(), var.c_str(), number.c_str()*//*, space.c_str()*//*);


    std::string pattern = temp_pattern;

    std::cout << pattern << std::endl;*/
//---------------------------------------------------------
    /*       std::string formula = "1 + A6 + 3.2 -5";
           std::string beans = trim(formula);

           std::vector<std::string> elems;

           std::regex rgx(pattern);

           std::sregex_token_iterator iter(beans.begin(), beans.end(), rgx, -1);
           std::sregex_token_iterator end;

           std::remove(formula.begin(), formula.end(), ' ');


           while (iter != end)  {
               //std::cout << "S43:" << *iter << std::endl;
               elems.push_back(*iter);
               ++iter;
           }*/
//---------------------------------------------------------------

/*
        std::vector<std::string> split;

        std::regex patty(R"(^\s*$)");

        for(std::string &s : elems){
            if(!(std::regex_match(s, patty)))
                split.push_back(s);
        }

        for(std::string &s : split){
            std::cout << s << std::endl;
        }
*/


/*    // Enumerate matching tokens that don't consist solely of white space.
    foreach (std::string s in Regex.Split(formula, pattern, RegexOptions.IgnorePatternWhitespace))
    {
        if (!Regex.IsMatch(s, @"^\s*$", RegexOptions.Singleline))
        {
            yield return s;
        }
    }*/


}
