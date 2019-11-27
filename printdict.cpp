#include <iostream>
#include <fstream>
#include <string>
#include "dictionary.h"

int main(){
    ifstream iin("ir.dictionary", ios::in | ios::binary);
    if(!iin) { cerr<<"cannot open ir.dictionary \n"; return 1;}
    ifstream pin("ir.postings", ios::in | ios::binary);
    if(!pin) { cerr<<"cannot open ir.postings \n"; return 1;}

    Dict_Term tm;
    Posting pst;

    int count=0;
    int post_count=0;
    int pstfreq;

    char word[MAXLEN+1];
    char docname[MAXLEN+1];

    while(iin.peek()!=EOF){        //ir.dictionary 다 읽을 때 까지
        iin.read((char*)&tm.wordpos, sizeof(tm.wordpos));
        iin.read((char*)&tm.poststart, sizeof(tm.poststart));
        iin.read((char*)&tm.numposts, sizeof(tm.numposts));
        iin.read((char*)&tm.totalfreq, sizeof(tm.totalfreq));
        iin.read((char*)&tm.idf, sizeof(tm.idf));

        ifstream win("ir.words");
        if(!win) { cerr<<"cannot open ir.words \n"; return 1;}
        win.seekg(tm.wordpos, ios::beg);
        win >> word;
        cout << word << " appeared ";
        win.close();
        cout << tm.totalfreq << " time(s) in " << tm.numposts << " document(s)  [idf = " << tm.idf << " ] " << endl;

        pin.seekg(tm.poststart*sizeof(Posting), ios::beg);
    
        for(post_count=0; post_count<tm.numposts; post_count++){
            pin.read((char*)&pst.docpos, sizeof(pst.docpos));
            pin.read((char*)&pst.freq, sizeof(pst.freq));

            ifstream din("ir.docnames");   //도 posting 개수만큼 읽어와야
            if(!din) { cerr<<"cannot open ir.docnames \n"; return 1;}
            din.seekg(pst.docpos, ios::beg);
            din >> docname;
            cout << docname << ' ' << pst.freq  << ' ';

            din.close();
        }
        cout<<endl;
    }

    int numdoc, numword;
    double idf;

    ifstream fin("ir.info");
    fin >> numdoc >> numword >> idf;
    cout << ' ' <<numdoc << ' ' << numword << ' ' << idf <<endl;

    fin.close();
    iin.close();
    pin.close();
}
