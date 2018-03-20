#include <iostream>
#include "Interpreter.h"

using namespace std;


int main(int argc,char** argv){

    string exp;
    
    if(argc<2){
        cout<<"Enter mathematical expression:"<<endl;
        cin>>exp;
    }else{
        exp=argv[1];
    }
    
    
    Interpreter p;

    try{
        p.Tokenize(exp);
        
        p.Parse();
        
        cout<<exp<<"="<<p.Eval()<<endl;
        
    }catch(const exception& ex){
        cout<<ex.what()<<endl;
    }
    
    
    
    
    return 0;

}
