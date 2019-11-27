//step1의 출력을 입력으로 받아서 sorting하기 
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include "dictionary.h"

vector<string> TokenizeAndSort(const string& data, const char delimiter = ' ') {
	vector<string> result;
	string token;
	stringstream ss(data);

	while (getline(ss, token, delimiter)) {
		result.push_back(token);
	}

    sort(result.begin(), result.end());
    
    return result;
}

int main(){
    vector<pair<string, int> > str;
    string word;
    int docpos;
    int i=0;

    while(cin>>word>>docpos)
        str.push_back(pair<string, int>(word, docpos));
        
    sort(str.begin(), str.end());
    for (i = 0; i < str.size(); i++)
        cout << str[i].first  << ", " <<  str[i].second << endl;

    return 0;
}