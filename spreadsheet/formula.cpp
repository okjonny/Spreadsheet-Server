// Skeleton written by Joe Zachary for CS 3500, September 2013
// Read the entire skeleton carefully and completely before you
// do anything else!

// Version 1.1 (9/22/13 11:45 a.m.)

// Change log:
//  (Version 1.1) Repaired mistake in get_tokens
//  (Version 1.1) Changed specification of second constructor to
//                clarify description of how validation works

// (Daniel Kopta)
// Version 1.2 (9/10/17)

// Change log:
//  (Version 1.2) Changed the definition of equality with regards
//                to numeric tokens

//Author: (Hibban Butt)
//Version 2.0 (9.18.20)
#include "formula.h"
#include <list>
#include <string>
#include <regex>
#include <locale>
#include <stdexcept>
#include <unordered_set>

namespace ss
{
    /// <summary>
    /// Represents formulas written in standard infix notation using standard precedence
    /// rules.  The allowed symbols are non-negative numbers written using double-precision
    /// floating-point syntax (without unary preceeding '-' or '+');
    /// variables that consist of a letter or underscore followed by
    /// zero or more letters, underscores, or digits; parentheses; and the four operator
    /// symbols +, -, *, and /.
    ///
    /// Spaces are significant only insofar that they delimit tokens.  For example, "xy" is
    /// a single variable, "x y" consists of two variables "x" and y; "x23" is a single variable;
    /// and "x 23" consists of a variable "x" and a number "23".
    ///
    /// Associated with every formula are two delegates:  a normalizer and a validator.  The
    /// normalizer is used to convert variables into a canonical form, and the validator is used
    /// to add extra restrictions on the validity of a variable (beyond the standard requirement
    /// that it consist of a letter or underscore followed by zero or more letters, underscores,
    /// or digits.)  Their use is described in detail in the constructor and method comments.
    /// </summary>
        //Class-wide variables that store the tokens and variables belonging to the Formula object.



        /// <summary>
        /// Creates a Formula from a string that consists of an infix expression written as
        /// described in the class comment.  If the expression is syntactically incorrect,
        /// throws a FormulaFormatException with an explanatory Message.
        ///
        /// The associated normalizer and validator are the second and third parameters,
        /// respectively.
        ///
        /// If the formula contains a variable v such that normalize(v) is not a legal variable,
        /// throws a FormulaFormatException with an explanatory message.
        ///
        /// If the formula contains a variable v such that is_valid
        //(normalize(v)) is false,
        /// throws a FormulaFormatException with an explanatory message.
        ///
        /// Suppose that N is a method that converts all the letters in a string to upper case, and
        /// that V is a method that returns true only if a string consists of one letter followed
        /// by one digit.  Then:
        ///
        /// new Formula("x2+y3", N, V) should succeed
        /// new Formula("x+y3", N, V) should throw an exception, since V(N("x")) is false
        /// new Formula("2x+y3", N, V) should throw an exception, since "2x+y3" is syntactically incorrect.
        /// </summary>
        formula::formula(std::string formula)
        {
            std::regex valid("^[A-Z]+[0-9]?[0-9]$");
            tokens = get_tokens(formula);
            variables = std::vector<std::string>();

            //Go through the tokens and ensure that every double is properly truncated
            //and that every variable is legal. Throw exceptions for illegal variables.
            for (int i = 0; i < tokens.size(); i++)
            {
                if (is_double(tokens[i]))
                {
                    tokens[i] = std::to_string(std::stod(tokens[i]));
                    //double.Parse(tokens[i]).to_string();
                }
                else if (is_variable(tokens[i]))
                {
                    tokens[i] = normalize(tokens[i]);
                    variables.push_back(tokens[i]);

                    if (!is_variable(tokens[i]))
                        throw std::exception();
                    //throw new FormulaFormatException(tokens[i] + " is an illegal variable. Change it to be legal.");
                    if (!std::regex_match(tokens[i], valid))
                        throw std::exception();
                        //throw new FormulaFormatException(tokens[i] + " is an invalid variable. Change it to be valid.");
                }
            }

            syntax_check(tokens);
        }

        /// <summary>
        /// Enumerates the normalized versions of all of the variables that occur in this
        /// formula.  No normalization may appear more than once in the enumeration, even
        /// if it appears more than once in this Formula.
        ///
        /// For example, if N is a method that converts all the letters in a string to upper case:
        ///
        /// new Formula("x+y*z", N, s => true).get_variables() should enumerate "X", "Y", and "Z"
        /// new Formula("x+X*z", N, s => true).get_variables() should enumerate "X" and "Z".
        /// new Formula("x+X*z").get_variables() should enumerate "x", "X", and "z".
        /// </summary>
        std::unordered_set<std::string> formula::get_variables()
        {
            return std::unordered_set<std::string>(variables.begin(), variables.end());
        }

        /// <summary>
        /// Returns a string containing no spaces which, if passed to the Formula
        /// constructor, will produce a Formula f such that this.equals(f).  All of the
        /// variables in the string should be normalized.
        ///
        /// For example, if N is a method that converts all the letters in a string to upper case:
        ///
        /// new Formula("x + y", N, s => true).to_string() should return "X+Y"
        /// new Formula("x + Y").to_string() should return "x+Y"
        /// </summary>
        public override string to_string()
        {
            //Combine all tokens into one string without any spaces
            return string.Join(" ", tokens);
        }

        /// <summary>
        /// If obj is null or obj is not a Formula, returns false.  Otherwise, reports
        /// whether or not this Formula and obj are equal.
        ///
        /// Two Formulae are considered equal if they consist of the same tokens in the
        /// same order.  To determine token equality, all tokens are compared as strings
        /// except for numeric tokens and variable tokens.
        /// Numeric tokens are considered equal if they are equal after being "normalized"
        /// by C#'s standard conversion from string to double, then back to string. This
        /// eliminates any inconsistencies due to limited floating point precision.
        /// Variable tokens are considered equal if their normalized forms are equal, as
        /// defined by the provided normalizer.
        ///
        /// For example, if N is a method that converts all the letters in a string to upper case:
        ///
        /// new Formula("x1+y2", N, s => true).equals(new Formula("X1  +  Y2")) is true
        /// new Formula("x1+y2").equals(new Formula("X1+Y2")) is false
        /// new Formula("x1+y2").equals(new Formula("y2+x1")) is false
        /// new Formula("2.0 + x7").equals(new Formula("2.000 + x7")) is true
        /// </summary>
    public override bool equals(object obj)
        {
            if(Referenceequals(obj, null) || !(obj is Formula))
            return false;
            return this.to_string().equals(obj.to_string());
        }

        /// <summary>
        /// Reports whether f1 == f2, using the notion of equality from the equals method.
        /// Note that if both f1 and f2 are null, this method should return true.  If one is
        /// null and one is not, this method should return false.
        /// </summary>
    public static bool operator ==(Formula f1, Formula f2)
        {
            if (Referenceequals(f1, null))
                return Referenceequals(f2, null);
            return f1.equals(f2);
        }

        /// <summary>
        /// Reports whether f1 != f2, using the notion of equality from the equals method.
        /// Note that if both f1 and f2 are null, this method should return false.  If one is
        /// null and one is not, this method should return true.
        /// </summary>
    public static bool operator !=(Formula f1, Formula f2)
        {
            if (Referenceequals(f1, null))
                return !Referenceequals(f2, null);
            return !f1.equals(f2);
        }

        /// <summary>
        /// Returns a hash code for this Formula.  If f1.equals(f2), then it must be the
        /// case that f1.GetHashCode() == f2.GetHashCode().  Ideally, the probability that two
        /// randomly-generated unequal Formulae have the same hash code should be extremely small.
        /// </summary>
    public override int GetHashCode()
        {
            return this.to_string().GetHashCode();
        }

        /// <summary>
        /// Given an expression, enumerates the tokens that compose it.  tokens are left paren;
        /// right paren; one of the four operator symbols; a string consisting of a letter or underscore
        /// followed by zero or more letters, digits, or underscores; a double literal; and anything that doesn't
        /// match one of those patterns.  There are no empty tokens, and no token contains white space.
        /// </summary>
        std::vector<std::string> formula::get_tokens(std::string formula)
        {
            // Patterns for individual tokens
            std::string left_par = R"(\()";
            std::string right_par = R"(\))";
            std::string op = R"([\+\-*/])";
            std::string var = R"([a-zA-Z_](?: [a-zA-Z_]|\d)*)";
            std::string number = R"((?: \d+\.\d* | \d*\.\d+ | \d+ ) (?: [eE][\+-]?\d+)?)";
            std::string space = R"(\s+)";

            // Overall pattern
            char* temp_pattern = new char[left_par.length() + right_par.length() + op.length() +
                                            var.length() + number.length() + space.length()];

            std::sprintf(temp_pattern,"(%s) | (%s) | (%s) | (%s) | (%s) | (%s)",
                         left_par.c_str(), right_par.c_str(), op.c_str(), var.c_str(), number.c_str(), space.c_str());

            std::string pattern = temp_pattern;

            // Enumerate matching tokens that don't consist solely of white space.
            foreach (String s in Regex.Split(formula, pattern, RegexOptions.IgnorePatternWhitespace))
            {
                if (!Regex.IsMatch(s, @"^\s*$", RegexOptions.Singleline))
                {
                    yield return s;
                }
            }

        }

        /// <summary>
        /// Checks to see if basic infix and variable syntax as outlined by the API is being followed. Throws exceptions for specific scenarios.
        /// </summary>
        /// <param name="tokens">The enumeration of tokens to iterate through and ensure that there are no invalid tokens.</param>
        void formula::syntax_check(std::vector<std::string> tokens)
        {
            //variables to keep track of opening and closed paranthesis
            int opening_par_count = 0;
            int closing_par_count = 0;

            //Variable to store the previous string in the iteration for comparison to the current one
            std::string last_string;

            //An empty formula is not a formula and an exception must be thrown.
            if (tokens.empty()) //!(tokens.size() > 0)
                throw std::runtime_error("The formula provided contains no tokens. Provide at least one token.");

            if (!(is_double(tokens.front()) || is_variable(tokens.front())|| tokens.front() == "("))
                throw std::runtime_error("The first token is not a valid number, variable, or opening paranthesis. Change it to be as such.");


            if (!(is_double(tokens.back()) || is_variable(tokens.back())|| tokens.back() == ")"))
                throw std::runtime_error("The last token is not a valid number, variable, or closing paranthesis. Change it to be as such.");

            //Iterate through every token to ensure it is valid. These tests, unlike the ones above, are sensitive in that any token at any point could trigger these exceptions.
            for (const std::string& t : tokens)
            {
                if (!is_legal(t))
                    throw std::runtime_error(t + " is an invalid token. Use a valid token instead.");

                if (closing_par_count > opening_par_count)
                    throw std::runtime_error("The number of opening paranthesis exceeds the number of closing paranthesis. Balance the paranthesis in the formula.");

                if (last_string == "(" || is_operator(last_string))
                {
                    if (!(is_double(t) || is_variable(t) || t == "("))
                        throw std::runtime_error(t + " is not a valid number, variable, or opening paranthesis. Change it to be as such.");
                }
                if (is_double(last_string) || is_variable(last_string) || last_string == ")")
                {
                    if (!(is_operator(t) || t == ")"))
                        throw std::runtime_error(t + " is not a valid operator or closing paranthesis. Change it to be as such.");
                }


                if (t == "(")
                    opening_par_count++;

                if (t == ")")
                    closing_par_count++;

                last_string = t;
            }


            if (opening_par_count != closing_par_count)
                throw std::runtime_error("The number of opening paranthesis and closing parenthesis are different. Balance the paranthesis in the formula.");
        }



        std::string formula::normalize(std::string token)
        {
            std::locale loc;
            std::string normalized = token;

            for(char c : normalized)
                std::toupper(c, loc);

            return normalized;
        }


        bool formula::is_legal(std::string token)
        {
            return (token == "("  ||
                    token == ")"  ||
                    is_operator(token) ||
                    is_variable(token) ||
                    is_double(token));
        }

        /// <summary>
        /// Checks if a string is a legal variable.
        /// </summary>
        /// <param name="token">The token in question.</param>
        /// <returns></returns>
        bool formula::is_variable(std::string token)
        {
            std::regex variable(R"([a-zA-Z_](?: [a-zA-Z_]|\d)*)");
            return std::regex_match(token, variable);
        }

        /// <summary>
        /// Checks if a string is a legal operator.
        /// </summary>
        /// <param name="token">The token in question.</param>
        /// <returns></returns>
        bool formula::is_operator(std::string token)
        {
            return (token == "*" ||
                    token == "/" ||
                    token == "+" ||
                    token == "-");
        }

        /// <summary>
        /// Checks if a string is a legal number.
        /// </summary>
        /// <param name="token">The token in question.</param>
        /// <returns></returns>
        bool formula::is_double(std::string token)
        {
            try
            {
                std::stod(token);
            }
            catch(...)
            {
                return false;
            }
            return true;
        }

    }
