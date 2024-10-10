#include <string>
#include <vector>

// TOKEN LISTS
std::vector<char> delimiters = {
    ',',
    '(',
    ')',
    ';',
    ':',
    '{',
    '}',
};

std::vector<char> operators = {
    '+',
    '-',
    '*',
    '/',
    '=',
    '%',
    '&',
    '|',
    '!',
    '<',
    '>'
};

std::vector<std::string> keywords = {
    "auto",
    "bool",
    "break",
    "case",
    "char",
    "class",
    "const",
    "continue",
    "do",
    "double",
    "else if",
    "else",
    "false",
    "float",
    "for",
    "if",
    "inline",
    "int",
    "namespace",
    "private",
    "protected",
    "public",
    "return",
    "switch",
    "this",
    "true",
    "using",
    "void",
    "while",
};

std::vector<std::string> identifiers = {
    "cin",
    "cout",
    "endl",
    "main",
    "std"
};

// HIDDEN SPACES LIST
std::vector<char> getRidOfHiddenSpaces = {
    '\n',
    '\f',
    '\r'
};

// STORED UNIQUE TOKENS
std::vector<char> outputDelimiters;
std::vector<char> outputOperators;
std::vector<std::string> outputTwoOperators;
std::vector<std::string> outputLiterals;
std::vector<std::string> outputKeywords;
std::vector<std::string> outputIdentifiers;
std::vector<std::string> outputremainingIdentifiers;

// STORED TOKEN COUNTS
int delimiterCount = 0;
int operatorCount = 0;
int literalCount = 0;
int keywordCount = 0;
int identifierCount = 0;
int totalTokenCount = 0;

// STORED CLEANED FILE CONTENTS
std::string output;
