#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <map>
#include <algorithm>
#include "dictionary.h"

struct accumulator{
    int docpos;
    double weight;
    struct accumulator *next;
};

bool cmp(const pair<int, int> &a, const pair<int, int> &b)
{
    return a.second > b.second;
}

int v_flag;

int main(int argc, char* argv[]){
    //argv 로 받은 쿼리 파일열어서 query[]에 저장하기
    //query의 모든 문자 소문자로, stoppingword인지? 후 단어별로 토크나이징
    //단어 별 binary search해서 유사도 계산 -->  새로 발견되는 연관 문서마다 새 accumulator 만들기 
    //accumulator: 문서의 위치와 그 문서의 유사도
    
    if(argc < 2) {
        cout << "Usage: " << argv[0] << " query file name";
        return -1;
    }
    if (!strcmp(argv[1],"-v"))
		v_flag = 1;

    ifstream qin(argv[v_flag+1]);
    vector <string> query;
    char pquery[MAXLEN+1];
    int numdocs = NumDocs();
    int i;
    Dict_Term tm;
    Posting pst;

    cout << "[Query] " ;
    while(NextNonStopWord(qin, pquery)){  // 단어 하나씩 pquery에 저장
        cout << pquery << ' ';
    }
    cout << endl << "--------------------" << endl;
    qin.close();

    //ir.info에서 읽어온 numdocs 만큼 해쉬테이블 할당
    struct accumulator** acc_table = (struct accumulator**)malloc(sizeof(struct accumulator*)*numdocs);
    map<int, double> acc_map;
    map<int, double>::iterator it;

    vector<pair<int, double> > acc_sort;
    // map<int, double>::iterator it;
    for(i=0 ; i<numdocs ; i++){
		acc_table[i] = NULL;
	}

    ifstream rin(argv[v_flag+1]);
    ifstream din("ir.dictionary");
	
    SkipBlanks(rin);
    while(NextNonStopWord(rin, pquery)){  // 단어 하나씩 pquery에 저장
        Capital2Lower(pquery);
        if(BinarySearchDictionary(din, pquery , tm)){       //쿼리의 각 단어 별 ir.dic의 term 찾음
        //해당 단어의 포스팅별 문서 조사
            ifstream pin("ir.postings");
            if(!pin) { cerr<<"cannot open ir.postings \n"; return 1;}
            pin.seekg(tm.poststart*sizeof(Posting), ios::beg);

            for(i=0; i<tm.numposts; i++){
                pin.read((char*)&pst.docpos, sizeof(pst.docpos));
                pin.read((char*)&pst.freq, sizeof(pst.freq));

                int hash = pst.docpos % numdocs;
                struct accumulator* current = acc_table[hash];
                struct accumulator* previous = NULL;

                if(current == NULL){ //accumulator 할당하기
                    struct accumulator *newNode = (struct accumulator*)malloc(sizeof(struct accumulator));
                    newNode->docpos = pst.docpos;
                    newNode->weight = tm.idf;
                    newNode->next = NULL;
                    acc_table[hash] = newNode;

                    acc_map.insert(pair<int, double>(pst.docpos, tm.idf));
                }
                else{
                    while(current != NULL){
                        if(current->docpos == pst.docpos){
                            current->weight += tm.idf;
                            acc_map[pst.docpos] += tm.idf;
                            //accumulator 이미 할당 됨
                            break;
                        }
                        else {
                            previous = current;
                            current = current->next;
                        }
                    }
                    if(current==NULL){
                        //해당 인덱스 끝까지 돌았는데 찾는 accumulator 없음 -> accumulator 새로 할당하기
                        struct accumulator *newNode = (struct accumulator*)malloc(sizeof(struct accumulator));
                        newNode->docpos = pst.docpos;
                        newNode->weight = tm.idf;
                        newNode->next = NULL;
                        acc_map.insert(pair<int, double>(pst.docpos, tm.idf));
                        previous->next = newNode;
                    }
                }
                
            }
            //idf 별로 정렬하기 
            if(v_flag == 1){
                    cout << pquery << "(idf = " << tm.idf << "): " << tm.numposts << " Posting(s) from " << tm.poststart << "-th " <<endl;
            }
        }
    }
    if(acc_map.empty()){
        cout << "No Relevant Document Found!\n";
        return -1;
    }

    for( it = acc_map.begin(); it != acc_map.end(); ++it ) {
        acc_sort.push_back(make_pair(it->first,it->second));
    }
    sort(acc_sort.begin(), acc_sort.end(), cmp);

    for(i=0; i<acc_sort.size(); i++){
        ifstream dcin("ir.docnames");   //도 posting 개수만큼 읽어와야
        char docname[MAXLEN+1];
        if(!dcin) { cerr<<"cannot open ir.docnames \n"; return 1;}
        dcin.seekg(acc_sort[i].first, ios::beg);
        dcin >> docname;
        cout << '[' << i+1 << ']' << docname << endl;

        ifstream dcuin(docname);
        string in_line;
        int j=0;
        if(!dcuin) { cerr<<"cannot open ir.docnames \n"; return 1;}

        while(getline(dcuin,in_line)){
            if(j>3) break;
            cout<<in_line<<endl;
            j++;
        }
    }
    cout << endl;
    
    

}