#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <iostream>
#include <fstream>
#include <cstring>
#define MAXLEN 256
using namespace std;

//extern int totaldocs;

template <class T>
void ChangeSizeID(T *&a, const int oldSize, const int newSize){     //  배열 크기 변경 프로그램
    if(newSize < 0) throw "뭐야이거~";
    T* tmp = new T[newSize];
    int number = min(oldSize,newSize);
    copy(a, a+number, tmp);
    delete []a;
    a = tmp;
}

struct Posting{     //어느 문서에 몇번??
    Posting(int dp=-1, int fr=0): docpos(dp), freq(fr){}    //docpos: position, freq: 몇번? -->가변 길이 일정하게 하기 위해서 ir.postings 따로 만들기
    int docpos;
    int freq;
};

struct Dict_Term{       //사전 내의 엔트리
    int wordpos;
    int poststart;
    int numposts;
    int totalfreq;
    double idf;
};

bool IsAlpha(int c);
bool IsDIgit(int c);
bool IsBlack(int c);
bool IsOtherCode(int c);        //한글인지?
bool IsLetter(int c);

void SkipBlanks(istream& in);
void Capital2Lower(char *word);
void Filepos2Str(istream& in, char *wstr);

bool NextWord(istream& in, char *word);
bool IsStopWord(char *word);
bool NextNonStopWord(istream& in, char *word);

bool BinarySearchDictionary(ifstream& din, char* word, Dict_Term& tm);

int NumDocs();
int NumWords();
double MaxIDF();

#endif