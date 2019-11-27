 //ir.words, ir.dictionary, ir.postings, ir.info file저장
 //ir.words: collection 내 단어들이 줄 당 5개씩
 //ir.dictionary: struct Dict_Term의 entry들을 저장 --> binary
 //ir.postings: struct Postings의 원소들이 저장 --> binary
 //ir.infos: 총 문서 수(numdocs), 총 단어 수(numwords), 최대 IDF(maxIDF)의 정보 저장


#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include "dictionary.h"

using namespace std;

int TenToTwo(int num){
    int result=0;
    int i;
    for(i=0; num>0; i*=10){
        int binary = num%2;
        result+=binary+i;
        num/=2;
    }
    return result;
}

int main(){
    char temp[MAXLEN+1];
    int numdocs=0;

    ifstream din("ir.docnames");
    while(!din.eof()){ 
        din >> temp;
        ifstream in(temp);
            if(!in) continue;
        numdocs++;
    }
    numdocs-=1;
    din.close();


    // ifstream in("ir.info");
    // float numdocs;
    // if(!in)
    //     throw "ir.info does not exist";
    // SkipBlanks(in);
    // in >> numdocs;
    // cout << numdocs << endl;
    // in.close();
   
    ofstream wout("ir.words");
    ofstream dout("ir.dictionary", ios::out | ios::binary);
    ofstream pout("ir.postings", ios::out | ios::binary);
    ofstream iout("ir.info");

    ifstream fin("ir.docnames");
    if(!fin) { cerr<<"뭐야\n"; return 1;}
    int docpos; char docname[MAXLEN+1];


    Dict_Term tm={0, 0, 0, 0, 0.0};
    Posting pst;
    char word[MAXLEN+1]; // MAXLEN는 충분히 큰 수로 define
    int flag = -1; int i; int count=0;
    int numwords=0; float maxIDF=0.0;    //ir.infos의 변수들
    int numposts=0; int wordfreq=0; //ir.dictionary의 변수
    int docpost=-1; int freqindoc=0; // ir.postings의 변수

    while (cin >> word) {
        if(IsLetter(word[0])){   //word읽어온 경우
            if(flag != -1 )
                dout.write( reinterpret_cast< const char * > ( &tm ),sizeof( Dict_Term ) );

            flag = 0;
            numwords++;
            tm.wordpos = wout.tellp();  //file의 현재 위치
            if(count==5){
                wout<<'\n'; count=0;
            }
            count++;
            wout << word<< ' ';

            cin >> tm.numposts >> tm.totalfreq;

            tm.poststart = pout.tellp() / sizeof(Posting);  //첫번째 posting에 대해 ir.postings에서의 위치
            tm.idf=((log((float)numdocs/tm.numposts))/log(2));
            if(tm.idf>maxIDF) maxIDF=tm.idf;

            cin >> pst.docpos >> pst.freq;
            pout.write( reinterpret_cast< const char * > ( &pst ),sizeof( Posting ) ); 
        }
        else{       //posting이 두개 이상인경우 -> word에 numposts값
            pst.docpos = atoi(word);
            cin >> pst.freq;
            pout.write( reinterpret_cast< const char * > ( &pst ),sizeof( Posting ) );
        }
    }
    dout.write( reinterpret_cast< const char * > ( &tm ),sizeof( Dict_Term ) );

    iout << numdocs << ' ' << numwords <<' '<< maxIDF;

    iout.close();
    wout.close();
    dout.close();
    pout.close();
}

