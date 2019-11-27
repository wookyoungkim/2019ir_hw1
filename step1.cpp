#include <iostream>
#include <fstream>
#include "dictionary.h"

using namespace std;

void PrintAllWordsWithDocPos(istream& in, int docpos){
    char word[MAXLEN+1];
    int count=0;

    while(NextNonStopWord(in, word)){
        Capital2Lower(word);
        cout<<word<<" ";
        cout.width(12);
        cout<<docpos<<endl;
    }
}

bool GetDocnamenDocpos(ifstream& fin, char* docname, int& docpos){
    SkipBlanks(fin);
    if(fin.peek()==EOF) return false;
    docpos=fin.tellg();
    if(!(fin>>docname)) throw "말도안돼~";
    return true;
}

int main(){
    ifstream fin("ir.docnames");
    ofstream iout("ir.info");
    if(!fin) { cerr<<"뭐야\n"; return 1;}

    int docpos;
    int docnums=0;
    char docname[MAXLEN+1];
    try{
        while(GetDocnamenDocpos(fin, docname, docpos)){
            ifstream in(docname);
            if(!in) {cerr << docname << " in ir.docnames does not exist. \n"; continue;}
            PrintAllWordsWithDocPos(in, docpos);
            docnums++;
            in.close();
        }
    } catch(char const *msg){
        cout << msg << endl;
    }
    iout << docnums << ' ';

    fin.close();
}