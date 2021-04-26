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
#include <locale>

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
        /// described in the class comment.  If the expression is syntactically invalid,
        /// throws a FormulaFormatException with an explanatory Message.
        ///
        /// The associated normalizer is the identity function, and the associated validator
        /// maps every string to true.
        /// </summary>
    public Formula(String formula) :
        this(formula, s => s, s => true)
        {
        }

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
            variables = std::list<std::string>();

            //Go through the tokens and ensure that every double is properly truncated
            //and that every variable is legal. Throw exceptions for illegal variables.
            for (int i = 0; i < tokens.size(); i++)
            {
                if (tokens[i].is_double())
                {
                    tokens[i] = double.Parse(tokens[i]).to_string();
                }
                else if (tokens[i].is_variable())
                {
                    tokens[i] = normalize(tokens[i]);
                    variables.insert(tokens[i]);

                    if (!tokens[i].is_variable())
                        throw new FormulaFormatException(tokens[i] + " is an illegal variable. Change it to be legal.");
                    if (!std::regex_match(tokens[i], p))
                        throw new FormulaFormatException(tokens[i] + " is an invalid variable. Change it to be valid.");
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
    public IEnumerable<String> get_variables()
        {
            return new HashSet<string>(variables);
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
    std::list<std::string> formula::get_tokens(std::string formula)
        {
            // Patterns for individual tokens
            String lpPattern = @"\(";
            String rpPattern = @"\)";
            String opPattern = @"[\+\-*/]";
            String varPattern = @"[a-zA-Z_](?: [a-zA-Z_]|\d)*";
            String doublePattern = @"(?: \d+\.\d* | \d*\.\d+ | \d+ ) (?: [eE][\+-]?\d+)?";
            String spacePattern = @"\s+";

            // Overall pattern
            String pattern = String.Format("({0}) | ({1}) | ({2}) | ({3}) | ({4}) | ({5})",
                                           lpPattern, rpPattern, opPattern, varPattern, doublePattern, spacePattern);

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
    private static void syntax_check(IEnumerable<String> tokens)
        {
            //variables to keep track of opening and closed paranthesis
            int opening_par_count = 0;
            int closing_par_count = 0;

            //Variable to store the previous string in the iteration for comparison to the current one
            string lastString = "";

            //An empty formula is not a formula and an exception must be thrown.
            if (!(tokens.Count() > 0))
                throw new FormulaFormatException("The formula provided contains no tokens. Provide at least one token.");

            if (!(tokens.First().is_double() || tokens.First().is_variable()|| tokens.First().equals("(")))
                throw new FormulaFormatException("The first token is not a valid number, variable, or opening paranthesis. Change it to be as such.");


            if (!(tokens.Last().is_double() || tokens.Last().is_variable()|| tokens.Last().equals(")")))
                throw new FormulaFormatException("The last token is not a valid number, variable, or closing paranthesis. Change it to be as such.");

            //Iterate through every token to ensure it is valid. These tests, unlike the ones above, are sensitive in that any token at any point could trigger these exceptions.
            foreach (string t in tokens)
            {
                if (!t.is_legal())
                    throw new FormulaFormatException(t + " is an invalid token. Use a valid token instead.");

                if (closing_par_count > opening_par_count)
                    throw new FormulaFormatException("The number of opening paranthesis exceeds the number of closing paranthesis. Balance the paranthesis in the formula.");

                if (lastString.equals("(") || lastString.is_operator())
                {
                    if (!(t.is_double() || t.is_variable() || t.equals("(")))
                        throw new FormulaFormatException(t + " is not a valid number, variable, or opening paranthesis. Change it to be as such.");
                }

                if (lastString.is_double() || lastString.is_variable() || lastString.equals(")"))
                {
                    if (!(t.is_operator() || t.equals(")")))
                        throw new FormulaFormatException(t + " is not a valid operator or closing paranthesis. Change it to be as such.");
                }

                if (t.equals("("))
                    opening_par_count++;

                if (t.equals(")"))
                    closing_par_count++;

                lastString = t;
            }


            if (opening_par_count != closing_par_count)
                throw new FormulaFormatException("The number of opening paranthesis and closing parenthesis are different. Balance the paranthesis in the formula.");
        }



    std::string formula::normalize(std::string token)
    {
        std::locale loc;
        std::string normalized = token;

        for(char c : normalized)
            std::toupper(c, loc);

        return normalized;
    }


    /// <summary>
    /// Used to report syntactic errors in the argument to the Formula constructor.
    /// </summary>
    public class FormulaFormatException : Exception
    {
        /// <summary>
        /// Constructs a FormulaFormatException containing the explanatory message.
        /// </summary>
    public FormulaFormatException(String message)
                : base(message)
        {
        }
    }


    /// <summary>
    /// A class full of extensions to the string class that will be useful in ensuring that Formula Objects contain correct tokens.
    /// </summary>
    internal static class StringExtensions
    {
        /// <summary>
        /// Checks if a string is a legal operator, variable, number, or paranthesis.
        /// </summary>
        /// <param name="token">The token in question.</param>
        /// <returns></returns>
    public static bool is_legal(this string token)
        {
            return (token.equals("(")  ||
                    token.equals(")")  ||
                    token.is_operator() ||
                    token.is_variable() ||
                    Double.TryParse(token, out _));
        }

        /// <summary>
        /// Checks if a string is a legal variable.
        /// </summary>
        /// <param name="token">The token in question.</param>
        /// <returns></returns>
    public static bool is_variable(this string token)
        {
            return Regex.IsMatch(token, @"[a-zA-Z_](?: [a-zA-Z_]|\d)*");
        }

        /// <summary>
        /// Checks if a string is a legal operator.
        /// </summary>
        /// <param name="token">The token in question.</param>
        /// <returns></returns>
    public static bool is_operator(this string token)
        {
            return (token.equals("*") ||
                    token.equals("/") ||
                    token.equals("+") ||
                    token.equals("-"));
        }

        /// <summary>
        /// Checks if a string is a legal number.
        /// </summary>
        /// <param name="token">The token in question.</param>
        /// <returns></returns>
    public static bool is_double(this string token)
        {
            return Double.TryParse(token, out _);
        }
    }
}
}