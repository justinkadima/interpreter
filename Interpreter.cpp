#include "Interpreter.h"

Token::Token(){
    this->precedence=0;
    this->rightAssociative=false;
    this->tip=Token::T_SPACE;
}


Interpreter::Interpreter(){
     this->pMap[Token::T_ADD]=10;
     this->pMap[Token::T_SUB]=10;
     this->pMap[Token::T_MULT]=20;
     this->pMap[Token::T_DIV]=20;
     this->pMap[Token::T_RP]=2;
     this->pMap[Token::T_LP]=1;
     this->pMap[Token::T_NUM]=0;
     
     this->pMap[Token::T_POW]=30;
}

void Interpreter::Tokenize(const string& exp){

    Token lastToken;

    Token::TokenType tip;

    for(size_t x=0;x<exp.size();x++){
        char c=exp.at(x);
        
        if(isspace(c)){
            tip=Token::T_SPACE;
            
        }

        
        if(isdigit(c)){

            tip=Token::T_NUM;

        }else{

            switch(c){
                 case '+':
                       tip=Token::T_ADD;
                    break;
                 case '-':
                       tip=Token::T_SUB;
                    break;
                 case '*':
                        tip=Token::T_MULT;
                    break;
                 case '/':
                        tip=Token::T_DIV;
                    break;
                 case '(':
                        tip=Token::T_LP;
                    break;
                 case ')':
                        tip=Token::T_RP;
                    break;

            
            }

        }

       
        if(lastToken.tip!=tip){
            
            //store the previous token
            
             if(lastToken.tip!=Token::T_SPACE){
                 
                    if(lastToken.tip==Token::T_MULT && lastToken.val.size()>1){
                        lastToken.tip=Token::T_POW;
                        lastToken.rightAssociative=true;
                    }
                    
                    lastToken.precedence=this->pMap[lastToken.tip];
                    this->tokens.push_back(lastToken);
                    
             }

             lastToken.val.clear();
        }

        lastToken.val.push_back(c);
        lastToken.tip=tip;

        

    }

    //store the last token
    if(lastToken.tip!=Token::T_SPACE){
       
        lastToken.precedence=this->pMap[lastToken.tip];             
        this->tokens.push_back(lastToken);
    }

    
}

//shunting yard 
void Interpreter::Parse(){
    
    stack<Token*> opstack;
  
  
    //insert number
            
    auto insertNumber=[&](Token& tok){
         this->postfixTokens.push_back(&tok);
    };
    
    
    //insert operator

    auto insertOperator=[&](Token& tok){
        if(opstack.empty()){
            opstack.push(&tok);
        }else{
           
            
            /*
            
            If the incoming symbol is an operator and has either higher precedence than the operator 
            on the top of the stack, or has the same precedence as the operator on the top of the stack
            and is right associative -- push it on the stack.

            If the incoming symbol is an operator and has either lower precedence than the operator on the top of the stack, 
            or has the same precedence as the operator on the top of the stack and is left associative --
            continue to pop the stack until this is not true. Then, push the incoming operator.
            
            */
            while(!opstack.empty()){
                
                
                if(tok.rightAssociative){
                    if(tok.precedence>=opstack.top()->precedence){
                        break;
                    }
                    
                }else{ //left associative
                    
                    if(tok.precedence>opstack.top()->precedence){
                        break;
                    }
                    
                }
                
                
                this->postfixTokens.push_back(opstack.top());
                opstack.pop();
                
                
            }
            
            opstack.push(&tok);
        }
    };
    
    
    //insert parenthese
    
    auto insertParenthese=[&](Token& tok){
        
        //If the incoming symbol is a left parenthesis, push it on the stack.

        if(tok.tip==Token::T_LP){
            
            opstack.push(&tok);
        
        }else{
            
            /*
            If the incoming symbol is a right parenthesis: discard the right parenthesis, pop and print the stack symbols until you see a left parenthesis.
            Pop the left parenthesis and discard it.
            */
            
            while(!opstack.empty()){
                if(opstack.top()->tip==Token::T_LP){
                    break;
                }
                
                this->postfixTokens.push_back(opstack.top());
                opstack.pop();
                
            }    
            
            if(opstack.empty()){
                throw invalid_argument("Unable to find a match for ) parenthese");
            }else{
                opstack.pop(); //remove (  parenthese
            }
            
            
        }
        
    };
  
  
    for(auto &tok: this->tokens){
       
            switch(tok.tip){
                
                case Token::T_SPACE:
                break;
                
                case Token::T_NUM:
                    insertNumber(tok);
                break;
                
                
                case Token::T_ADD:
                case Token::T_SUB:
                case Token::T_MULT:
                case Token::T_DIV:
                case Token::T_POW:
                    insertOperator(tok);
                break;
                
                case Token::T_LP:
                case Token::T_RP:
                    insertParenthese(tok);
                break;
                default:
                    break;
                
            }
        
    }
    
    
    //move the rest of operants from the stack to postfix result
    
    while(!opstack.empty()){
        if(opstack.top()->tip==Token::T_LP){
            throw invalid_argument("No match found for ( parenthese");
        }
        
        this->postfixTokens.push_back(opstack.top());
        opstack.pop();
    }
    
    
    
    
}

string Interpreter::Eval(){
    
    stack<int> evalStack;
    
    auto popStack=[&]()->int{
        
        int top=evalStack.top();
        evalStack.pop();
        return top;
    };
    
    
    auto evalNumber=[&](Token*& tok){
        evalStack.push(stoi(tok->val));
    };
    
    auto evalADD=[&](Token*& tok){
        
        int last=popStack();
        int prev=popStack();
    
        evalStack.push(prev+last);
    };
    
    auto evalSUB=[&](Token*& tok){
        int last=popStack();
        int prev=popStack();
        
        evalStack.push(prev-last);
    };
    
    auto evalMULT=[&](Token*& tok){
        int last=popStack();
        int prev=popStack();
        
        evalStack.push(prev*last);
    };
    
  
    auto evalDIV=[&](Token*& tok){
        int last=popStack();
        int prev=popStack();
        
        evalStack.push(prev/last);
    };
    
     auto evalPOW=[&](Token*& tok){
        int last=popStack();
        int prev=popStack();
        evalStack.push(pow(prev,last));
    };
    
    
     for(Token *tok: this->postfixTokens){
       
        switch(tok->tip){
                
            case Token::T_SPACE:
            case Token::T_LP:
            case Token::T_RP:
            break;
            
            case Token::T_NUM:
                evalNumber(tok);
            break;
            
            case Token::T_ADD:
                evalADD(tok);
            break;
            
            case Token::T_SUB:
                evalSUB(tok);
            break;
            
            case Token::T_MULT:
                evalMULT(tok);
            break;    
            
            case Token::T_DIV:
                evalDIV(tok);
            break;
            case Token::T_POW:
                evalPOW(tok);
            break;
            
        }
        
    }
    
    
    return to_string(evalStack.top());
}


    
