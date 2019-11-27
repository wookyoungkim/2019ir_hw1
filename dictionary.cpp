#include "dictionary.h"
#include <string>

bool IsAlpha(int c) {return (c>='a' && c<='z') || (c>='A' && c<='Z');}
bool IsDigit(int c) {return c>='0' && c<='9';}
bool IsBlack(int c) {return c==' ' || c=='\t' || c=='\n';}
bool IsOtherCode(int c){return c<-1 || c>127;}
bool IsLetter(int c){return IsAlpha(c) || IsOtherCode(c);}

void SkipBlanks(istream& in){       //  위의 IsBlank의 빈칸 정의에 따라서
    char c;
    while((c=in.peek())!=EOF&&IsBlack(c))
        in.get();
}
void Capital2Lower(char* word){     //word내의 대문자 모두 소문자로 바꾸기
    int i=0;
    char c;

    while (word[i]){
        c = word[i];
        if(IsAlpha(c)){
            if(c>='A'&&c<='Z')
                word[i] = c+32;
        }
        i++;
    }
}
void Filepos2Str(istream& in, int pos, char *str){
    if(!(in.seekg(pos, ios::beg)))      
    //seekg: 객체의 입력 위치 지정자 값을 설정-> iso::beg시작 위치로부터 일정 위치로 워프
        throw "Seeking Failed";
    in>>str;
}

bool NextWord(istream& in, char *word){     //[a-zA-Z]+를 찾아서 word에 저장하기
    char c;
    while(!IsLetter(c = in.peek())){
        if(c==EOF) return false;
        in.get();
    }
    int wordlen = 0;
    while(IsLetter(in.peek())){
        word[wordlen++] = in.get();
        if(wordlen == MAXLEN) break;
    }
    word[wordlen] = '\0';
    return true;
}
bool IsStopWord(char *word){
    //7-8개정도 정의해서 사용하기
    if(std::strcmp(word, "a") != 0 &&std::strcmp(word, "about") != 0 &&std::strcmp(word, "all") != 0 
    &&std::strcmp(word, "am") != 0 &&std::strcmp(word, "an") != 0 &&std::strcmp(word, "and") != 0 
    &&std::strcmp(word, "any") != 0 &&std::strcmp(word, "are") != 0 &&std::strcmp(word, "as") != 0 
    &&std::strcmp(word, "be") != 0 &&std::strcmp(word, "because") != 0 &&std::strcmp(word, "been") != 0 
    &&std::strcmp(word, "by") != 0 &&std::strcmp(word, "can") != 0 &&std::strcmp(word, "cannot") != 0 
    &&std::strcmp(word, "could") != 0 != 0 &&std::strcmp(word, "did") != 0 &&std::strcmp(word, "do") != 0 
    &&std::strcmp(word, "every") != 0 &&std::strcmp(word, "for") != 0 &&std::strcmp(word, "however") != 0 
    &&std::strcmp(word, "i") != 0 &&std::strcmp(word, "if") != 0 &&std::strcmp(word, "is") != 0 
    &&std::strcmp(word, "it") != 0 &&std::strcmp(word, "its") != 0 &&std::strcmp(word, "just") != 0 
    &&std::strcmp(word, "me") != 0 &&std::strcmp(word, "my") != 0 &&std::strcmp(word, "no") != 0 
    &&std::strcmp(word, "of") != 0  &&std::strcmp(word, "so") != 0 &&std::strcmp(word, "than") != 0 
    &&std::strcmp(word, "that") != 0 &&std::strcmp(word, "the") != 0 &&std::strcmp(word, "their") != 0 
    &&std::strcmp(word, "them") != 0 &&std::strcmp(word, "then") != 0 &&std::strcmp(word, "there") != 0 
    &&std::strcmp(word, "these") != 0 &&std::strcmp(word, "they") != 0 &&std::strcmp(word, "this") != 0 
    &&std::strcmp(word, "to") != 0 &&std::strcmp(word, "too") != 0  &&std::strcmp(word, "us") != 0 
    &&std::strcmp(word, "was") != 0 &&std::strcmp(word, "we") != 0 &&std::strcmp(word, "were") != 0 
    &&std::strcmp(word, "you") != 0 &&std::strcmp(word, "your") != 0 &&std::strcmp(word, "on") != 0 
    &&std::strcmp(word, "or") != 0 &&std::strcmp(word, "our") != 0 &&std::strcmp(word, "own") != 0 &&std::strcmp(word, "she") != 0)
    return false;
}


bool NextNonStopWord(istream& in, char *word){
    while(1){
        if(!NextWord(in, word))
            return false;
        if(!IsStopWord(word))
            return true;
    }
}

//consult에서 필요한 함수들 : ir.infos에 저장된 세개의 값중 하나 반환
int NumDocs(){
    ifstream iin("ir.info");
    int numdocs, numwords;
    float maxidf;
    if(!(iin>>numdocs>>numwords>>maxidf))
        throw "ir.info does not exist";
    iin.close();
    return numdocs;
}
int NumWords(){ //numdocs와 유사하게
    ifstream iin("ir.info");
    int numdocs, numwords;
    float maxidf;
    if(!(iin>>numdocs>>numwords>>maxidf))
        throw "ir.info does not exist";
    iin.close();
    return numwords;
}
double MaxIDF(){
    ifstream iin("ir.info");
    int numdocs, numwords;
    float maxidf;
    if(!(iin>>numdocs>>numwords>>maxidf))
        throw "ir.info does not exist";
    iin.close();
    return maxidf;
}

bool BinarySearchDictionary(ifstream& din, char* qword, Dict_Term& tm){ //disc file을 가지고 비교하기
    int length;
    int start = 0;
    char word[MAXLEN+1];
    
    din.seekg(0, ios::end);
    length=din.tellg();
    din.seekg(0, ios::beg);
    length = length / sizeof(Dict_Term);
    int end = length - 1;
    int middle;

    while(start <= end){
        middle = (start+end)/2;
        din.seekg(middle*sizeof(Dict_Term), ios::beg);
        din.read((char*)&tm.wordpos, sizeof(tm.wordpos));
        din.read((char*)&tm.poststart, sizeof(tm.poststart));
        din.read((char*)&tm.numposts, sizeof(tm.numposts));
        din.read((char*)&tm.totalfreq, sizeof(tm.totalfreq));
        din.read((char*)&tm.idf, sizeof(tm.idf));

        //ir.words에서 wordps에 있는 단어 찾기
        ifstream win("ir.words");
        if(!win) { cerr<<"cannot open ir.words \n"; return 1;}
        win.seekg(tm.wordpos, ios::beg);
        win >> word;
        win.close();

        if(strcmp(qword, word)==0) return true;
        if(strcmp(qword, word)>0) start = middle + 1;
        if(strcmp(qword, word)<0) end = middle - 1;
    }
    return false;
}
