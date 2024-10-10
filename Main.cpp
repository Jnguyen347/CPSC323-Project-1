#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "Main.h"

int main()
{
    // INPUT FILE NAME HERE
    std::fstream inputFile("inputTestFileA.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error opening this file!\n";
        return 1;
    }


    // CLEANING INPUT FILE
    std::string line;

    while (!(inputFile >> std::ws).eof()) {
        getline(inputFile, line);
        std::istringstream fileLine(line);

        char comment;
        fileLine >> std::skipws >> comment;

        if (line.find_first_not_of("\f\n\r\t\v") == line.npos) {
            continue;
        }
        else if ((comment != '/') && (comment != '#') && (comment != '*')) {
            int commentPos = static_cast<int>(line.find("//"));

            if (commentPos >= 0) {
                output += line.substr(0, commentPos) + '\n';
            }
            else {
                output += line + '\n';
            }
        }
    }
    inputFile.close();


    // PRINT: "CLEANED" FILE CONTENTS
    std::cout << "\nFile Contents:\n==============\n"
        << output << std::endl;


    // GATHERING TOKENS SECTION
    int outputLength = static_cast<int>(output.length());


    // GATHERING: delimiters -> outputDelimiters
    for (int i = 0; i < outputLength; ++i) {

        // Skips literals
        if (output[i] == '"') {
            i++;
            while (output[i] != '"') {
                i++;
            }
        }

        else if (std::find(delimiters.begin(), delimiters.end(), output[i]) != delimiters.end()) {
                if (std::find(outputDelimiters.begin(),
                    outputDelimiters.end(),
                    output[i]) == outputDelimiters.end()) {
                        outputDelimiters.push_back(output[i]);
                }
                
                ++delimiterCount;
        }
    }


    // GATHERING: operators -> outputOperators
    for (int i = 0; i < outputLength; ++i) {

        // Skips literals
        if (output[i] == '"') {
            i++;
            while (output[i] != '"') {
                i++;
            }
        }

        // Double Operators
        else if ((std::find(operators.begin(), operators.end(), output[i]) != operators.end()) &&
            (std::find(operators.begin(), operators.end(), output[i + 1]) != operators.end())) {
                std::string twoOperators;
                twoOperators += output[i];
                twoOperators += output[i + 1];

                if (std::find(outputTwoOperators.begin(),
                    outputTwoOperators.end(),
                    twoOperators) == outputTwoOperators.end()) {
                        outputTwoOperators.push_back(twoOperators);
                }

                ++operatorCount;
                ++i;
             }

        // Single Operators
        else if (std::find(operators.begin(), operators.end(), output[i]) != operators.end()) {
            if (std::find(outputOperators.begin(),
                outputOperators.end(),
                output[i]) == outputOperators.end()) {
                    outputOperators.push_back(output[i]);
            }

            ++operatorCount;
        }
    }


    // GATHERING: literals -> outputLiterals
    for (int i = 0; i < outputLength; ++i) {
        if (isdigit(output[i]) && !isalpha(output[i - 1])) {
            std::string digit;
            digit += output[i];
            int pos = i;

            while (isdigit(output[pos + 1])) {
                digit += output[i + 1];
                ++pos;
            }

            if (std::find(outputLiterals.begin(), outputLiterals.end(), digit) == outputLiterals.end()) {
                outputLiterals.push_back(digit);
            }

            ++literalCount;
            i = pos;
        }
        else if (output[i] == '"') {
            int pos = i;
            std::string literal;

            literal += output[pos]; // To include the opening " quote.
            ++pos;
            while (output[pos] != '"') {
                literal += output[pos];
                ++pos;
            }
            literal += output[pos]; // To include the closing " quote.
            ++pos;

            outputLiterals.push_back(literal);

            ++literalCount;
            i = pos;
        }
    }


    // GATHERING: keywords -> outputKeywords
    for (std::string elemKeyword : keywords) {      
        size_t keywordPos = output.find(elemKeyword);

        while (keywordPos != std::string::npos) {
            if (std::find(outputKeywords.begin(),
                outputKeywords.end(),
                elemKeyword) == outputKeywords.end()) {
                    outputKeywords.push_back(elemKeyword);
            }

            ++keywordCount;
            keywordPos = output.find(elemKeyword, keywordPos + 1);
        }
    }


    // GATHERING: identifiers -> outputIdentifiers
    for (std::string elemIdentifier : identifiers) {
        size_t identifierPos = output.find(elemIdentifier);

        while (identifierPos != std::string::npos) {
            if (std::find(outputIdentifiers.begin(),
                outputIdentifiers.end(),
                elemIdentifier) == outputIdentifiers.end()) {
                    outputIdentifiers.push_back(elemIdentifier);
            }

            ++identifierCount;
            identifierPos = output.find(elemIdentifier, identifierPos + 1);
        }
    }


    // ERASING SECTION
    for (int i = 0; i < outputLength; ++i) {
        // ERASE: Literals (Quotes)
        if (output[i] == '"') {
            int startPos = i;   // To include the starting " quote
            int endPos = i + 1;

            while (output[endPos] != '"') {
                ++endPos;
            }
            ++endPos;           // To include the closing " quote

            int literalLength = endPos - startPos;
            output.erase(startPos, literalLength);
            output.insert(output.begin() + startPos, ' ');

            outputLength = static_cast<int>(output.length());
        }
        // ERASE: Literals (Non-quotes)
        else if (isdigit(output[i]) && !isalpha(output[i - 1])) {
            output.erase(output.begin() + i);
            output.insert(output.begin() + i, ' ');
        }
        // ERASE: Hidden Spaces
        else if (std::find(getRidOfHiddenSpaces.begin(), getRidOfHiddenSpaces.end(), output[i])
            != getRidOfHiddenSpaces.end()) {
                output.erase(output.begin() + i);
                output.insert(output.begin() + i, ' ');
             }
        // ERASE: Delimiters
        else if (std::find(delimiters.begin(), delimiters.end(), output[i]) != delimiters.end()) {
                output.erase(output.begin() + i);
                output.insert(output.begin() + i, ' ');
        }
        // ERASE: Operators
        else if (std::find(operators.begin(), operators.end(), output[i]) != operators.end()) {
                output.erase(output.begin() + i);
                output.insert(output.begin() + i, ' ');
        };
    }

    // ERASE: Keywords
    for (std::string elemKeyword : keywords) {
        int outputKeywordPos = static_cast<int>(output.find(elemKeyword));

        while (outputKeywordPos != std::string::npos) {
            int elemLength = static_cast<int>(elemKeyword.length());

            output.erase(outputKeywordPos, elemLength);
            outputKeywordPos = static_cast<int>(output.find(elemKeyword));
        }
    }

    // ERASE: Identifiers
    for (std::string elemIdentifier : identifiers) {
        int outputIdentifierPos = static_cast<int>(output.find(elemIdentifier));

        while (outputIdentifierPos != std::string::npos) {
            int elemLength = static_cast<int>(elemIdentifier.length());

            output.erase(outputIdentifierPos, elemLength);
            outputIdentifierPos = static_cast<int>(output.find(elemIdentifier));
        }
    }


    // Updates outputLength after ERASING
    outputLength = static_cast<int>(output.length());


    // GATHERING: Remaining identifiers -> outputremainingIdentifiers
    for (int i = 0; i < outputLength; ++i) {
        std::string remainingVariable;

        while (!isblank(output[i])) {
            remainingVariable += output[i];
            ++i;
        }
        if (remainingVariable != "" && std::find(outputremainingIdentifiers.begin(),
            outputremainingIdentifiers.end(), remainingVariable)
            == outputremainingIdentifiers.end()) {
                outputremainingIdentifiers.push_back(remainingVariable);
                ++identifierCount;
        }
        else if (remainingVariable != "") {
            ++identifierCount;
        }
    }


    // PRINT: TOKEN TABLE
    std::cout << "\t\tTOKEN TABLE\n\n"
        << "Category\tCount\t\tTokens"
        << "\n========\t=====\t\t======\n";

    std::cout << "Keywords\t" << keywordCount << "\t\t";
    for (const std::string &elem : outputKeywords) {
        std::cout << elem << ' ';
    };

    std::cout << "\nIdentifiers\t" << identifierCount << "\t\t";
    for (const std::string &elem : outputIdentifiers) {
        std::cout << elem << ' ';
    };
    for (const std::string &elem : outputremainingIdentifiers) {
        std::cout << elem << ' ';
    };

    std::cout << "\nOperators\t" << operatorCount << "\t\t";
    for (const auto &elem : outputOperators) {
        std::cout << elem << ' ';
    };
    for (const std::string &elem : outputTwoOperators) {
        std::cout << elem << ' ';
    };

    std::cout << "\nDelimiters\t" << delimiterCount << "\t\t";
    for (const auto &elem : outputDelimiters) {
        std::cout << elem << ' ';
    };

    std::cout << "\nLiterals\t" << literalCount << "\t\t";
    for (const std::string &elem : outputLiterals) {
        std::cout << elem << ' ';
    };


    // COUNTING TOKENS
    totalTokenCount += delimiterCount
        + operatorCount
        + literalCount
        + keywordCount
        + identifierCount;


    // PRINT: TOTAL TOKEN COUNT
    std::cout << "\n\nToken Count (Total): "
        << totalTokenCount << "\n" << std::endl;


    return 0;
}
