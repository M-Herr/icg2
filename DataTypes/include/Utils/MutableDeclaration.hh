#ifndef MUTABLE_DECLARATION_HH
#define MUTABLE_DECLARATION_HH

#include <string>
#include <iostream>
#include <vector>

#include "Utils/LexicalAnalyzer.hh"


class MutableDeclaration {

    public:

    MutableDeclaration( std::string s );
    MutableDeclaration( std::string s, std::vector<int> dimensions);

    std::string getTypeSpecifier() const;
    std::string getVariableName() const;
    unsigned int getNumberOfDimensions() const;
    int getDimensionSize( unsigned int index) const;

    std::vector<int> getDims() const;

    std::string getDeclarator(std::string varName);
    std::string getAbstractDeclarator();

    int popDimension();
    void pushDimension(int dim);

    bool validateDimensions();

    private:
    
    bool is_valid_dimension (int dim);

    bool parseTypeSpecifier();
    bool parseDeclaration();
    bool parseDeclarator();
    bool parseDirectDeclarator();

    bool match(Token::e expected_token);

    Token::e nextToken;
    LexicalAnalyzer lexer;

    std::string typeSpec;
    std::string varName;
    std::vector<int> dims;
};

    // public:

    // DeclarationBuilder( std::string base_type, std::vector<int> dimensions);
    // DeclarationBuilder( std::string base_type);

    // std::string getDeclarator(std::string varName);
    // std::string getAbstractDeclarator();

    // int popDimension();
    // void pushDimension(int dim);

    // private:

    // bool validateDimensions();
    // static bool is_valid_dimension (int dim);

    // std::string typeSpec;
    // std::vector<int> dims;
#endif
