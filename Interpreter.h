#ifndef INTERPRETER_H
#define INTERPRETER_H

#include<iostream>
#include<vector>
#include<map>
#include<stack>
#include <stdexcept>
#include<string>
#include<cmath>

using namespace std;


class Token{
    public:
    
    enum TokenType{
        T_SPACE,
        T_NUM,
        T_ADD,
        T_SUB,
        T_MULT,
        T_DIV,
        T_LP,
        T_RP,
        T_POW
    };

    Token();
    
    string val;
    TokenType tip;
    unsigned int precedence;
    bool rightAssociative;

};


class Interpreter{
    public:
        Interpreter();
        void Tokenize(const string& exp);
        void Parse();
        string Eval();
        vector<Token> tokens;
        vector<Token*> postfixTokens;
        map<Token::TokenType,unsigned int> pMap;

};








#endif
