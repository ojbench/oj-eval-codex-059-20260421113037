#include <bits/stdc++.h>
#include src.hpp
using namespace std;

string get_data () {
    string ret =  , line;
    // consume leftover newline if any
    if (cin.peek()=='\n') cin.get();
    while (getline(cin , line)) {
        if (line.length() == 0) {
            continue;
        }
        if (line == #####) {
            break;
        }
        ret += line + n;
    }
    return ret;
}

int main () {
    char judger_name[100];
    if(!(cin >> judger_name)) return 0;
    size_t time_limit , mem_limit;
    cin >> time_limit >> mem_limit;
    BaseJudger *judger = nullptr;
    string tmp;
    if (strcmp(judger_name , OIJudger) == 0) {
        tmp = get_data();
        judger = new OIJudger(time_limit , mem_limit , tmp.c_str());
    } else if (strcmp(judger_name , ICPCJudger) == 0) {
        tmp = get_data();
        judger = new ICPCJudger(time_limit , mem_limit , tmp.c_str());
    } else if (strcmp(judger_name ,SpacialJudger) == 0) {
        size_t full_score_time , full_score_memory;
        cin >> full_score_time >> full_score_memory;
        tmp = get_data();
        judger = new SpacialJudger(time_limit , mem_limit , full_score_time ,
                                   full_score_memory , tmp.c_str());
    } else {
        cout << Unknown
