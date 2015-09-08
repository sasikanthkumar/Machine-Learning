#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include <set>
using namespace std;

int main(){
    int n;  // number of records
    int m;  // number of attributes
    
    scanf("%d",&n);
    scanf("%d",&m);
    int store_i = 0;
    int exceptional_case = 0;
    vector<string> attr_names(m);
    int null_flag = 1;
    
    map<string,int> *attr_values = new map<string,int>[m];
    //attr_values[0].insert(pair<string,int>("sss",1));
    string temp_line;
    string temp;
    char ch;
    int line_length = 0;
    int start = 0;
    for (int i = 0; i<m; i++) {
        cin >> attr_names[i];
        scanf("%c",&ch);
        getline(cin,temp_line);
        line_length = temp_line.length();
        start = 0;
        //cout << endl;
        //cout << temp_line << endl;
        //cout << temp_line.substr(6,12) <<endl;
        int key = 1;
        for (int j = 0; j<line_length; j++) {
            
            if (temp_line[j] == ' ' || j == line_length-1) {
                if (temp_line[j] == ' ') {
                    //cout << j << start <<endl;
                    temp = temp_line.substr(start,j-start);
                    start = j+1;
                    //cout << temp <<endl;
                    attr_values[i].insert(pair<string,int>(temp,key));
                    key++;
                }else{
                    //cout << j << start<< endl;
                    temp = temp_line.substr(start,j-start+1);
                    //cout << temp << endl;
                    attr_values[i].insert(pair<string,int>(temp,key));
                    key++;
                }
            }
        }
    }
    
    
    
    /**
    map<string,int>::iterator it;
    for (int i = 0; i<attr_names.size(); i++) {
        for (it = attr_values[i].begin(); it != attr_values[i].end(); it++) {
            cout << it->first << " " << it->second <<endl;
        }
    }
    **/
    
    set<vector<int> > specific_boundary,generic_boundary;
    vector<int> curr_data;
    curr_data.clear();
    for (int i = 0; i<(m-1); i++) {
        curr_data.push_back(0);
    }
    specific_boundary.insert(curr_data);
    curr_data.clear();
    for (int i = 0; i<(m-1); i++) {
        curr_data.push_back(-2);
    }
    generic_boundary.insert(curr_data);
    curr_data.clear();
    
    
    
    for(int i = 0; i<n; i++) {
        string curr_trng_exmp;
        curr_data.clear();
        getline(cin,curr_trng_exmp);
        //cout << "i = "<< i  << endl;
        line_length = curr_trng_exmp.length();
        start = 0;
        int attr_index = 0;
        for (int j = 0; j<line_length; j++) {
            if (curr_trng_exmp[j] == ' '|| j == line_length-1) {
                if (curr_trng_exmp[j] == ' ') {
                    temp = curr_trng_exmp.substr(start,j-start);
                    start = j+1;
                    //cout << temp <<endl;
                    curr_data.push_back(attr_values[attr_index][temp]);
                    attr_index++;
                }else{
                    temp = curr_trng_exmp.substr(start,j-start+1);
                    curr_data.push_back(attr_values[attr_index][temp]);
                    attr_index++;
                }
            }
        }
       
        //printing curreent data
        /**
        for (int k = 0; k<curr_data.size(); k++) {
            cout << curr_data[k] << " ";
        }
        cout << endl;
        **/
        vector<int> first_step;
        first_step.clear();
        //cout << "curr data = " << curr_data.back() << endl;
        
        if(curr_data.back() == 1 && null_flag == 1){    //positive example
            //cout << "positive" <<endl;
            // removing from generic border any hypothesis inconsistent with the given training example
            set <vector<int> > temporary_g_border;
            temporary_g_border = generic_boundary;
            for (set<vector<int> >::iterator it = generic_boundary.begin(); it != generic_boundary.end() ; it++) {
                first_step = *it;
                int flag = 0;
                for (int j = 0; j<(m-1); j++) {
                    if (first_step[j] != curr_data[j] && first_step[j] != -2) {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 1) {
                    temporary_g_border.erase(first_step);
                }
                first_step.clear();
            }
            generic_boundary = temporary_g_border;
            if(generic_boundary.size() == 0){
                exceptional_case = 1;
                null_flag = 0;
                store_i = i;
                break;
            }
            
            set<vector<int> > consistent_in_s;
            
            for (set<vector<int> >::iterator it = specific_boundary.begin(); it != specific_boundary.end(); it++) {
                first_step = *(it);
                int flag = 1;
                for (int i = 0; i<(m-1); i++) {
                    if (first_step[i] == 0) {
                        flag = 0; // inconsistent
                        break;
                    }else if(first_step[i] != -2 && first_step[i] != curr_data[i]){
                        flag = 0;
                        break;
                    }
                }
                if (flag == 1) {
                    consistent_in_s.insert(first_step);
                }else{  // inconsistent
                    for (int i = 0; i<(m-1); i++) {
                        if (first_step[i] == 0) {
                            first_step[i] = curr_data[i];
                        }else if(first_step[i] != curr_data[i]){
                            first_step[i] = -2;
                        }
                    }
                    int flag_gchek = 1;
                    vector<int> g_check = *(generic_boundary.begin());
                    for (int p = 0; p<(m-1); p++) {
                        if ((first_step[p] != g_check[p])&&(first_step[p] != 0) &&(g_check[p] != -2)) {
                            flag_gchek = 0;
                            break;
                        }
                    }
                    if (flag_gchek == 1) {
                        consistent_in_s.insert(first_step);
                    }
                }
            }
            
            // remove from s that is more general
            set<vector<int> > more_general_in_s;
            vector<int> second_step;
            for (set<vector<int> >::iterator it1 = consistent_in_s.begin(); it1 != consistent_in_s.end(); it1++) {
                first_step = *(it1);
                for (set<vector<int> >::iterator it2 = consistent_in_s.begin(); it2 != consistent_in_s.end(); it2++) {
                    if (it1 == it2) {
                        continue;
                    }
                    second_step = *(it2);
                    int flag = 0;
                    for (int k = 0; k<(m-1); k++) {
                        if (first_step[k] == 0) {
                            flag = 1;
                            break;
                        }else if((first_step[k] != -2) && (second_step[k] == -2)){
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 1) {
                        more_general_in_s.insert(second_step);
                    }
                    
                }
                
            }
            
            specific_boundary.clear();
            for (set<vector<int> >::iterator it = consistent_in_s.begin(); it != consistent_in_s.end(); it++) {
                first_step = *(it);
                if (more_general_in_s.find(first_step) == more_general_in_s.end()) {
                    specific_boundary.insert(first_step);
                }
            }
            if (specific_boundary.size() == 0) {
                exceptional_case = 1;
                null_flag = 0;
                store_i  =i;
                break;
                
            }
            
        }else if(null_flag == 1){          // negative example
            //cout << "Negative example" <<endl;
            //cout << specific_boundary.size() <<endl;
            set <vector<int> > temporary_s_border;
            temporary_s_border = specific_boundary;
            //removing from specfic border any hypothesis inconsistent with given training example
            for (set<vector<int> >::iterator it = specific_boundary.begin(); it != specific_boundary.end() ; it++) {
                //cout << "in for loop " <<endl;
                first_step = *it;
                int flag = 0;
                for (int j = 0; j<(m-1); j++) {
                    if (first_step[j] == 0 || ((first_step[j] != curr_data[j])&&(first_step[j] != -2))) {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0) {
                    temporary_s_border.erase(first_step);
                }
                first_step.clear();
                //cout << "ou for lop" <<endl;
            }
            specific_boundary = temporary_s_border;
            if (specific_boundary.size() == 0) {
                exceptional_case = 1;
                null_flag = 0;
                store_i = i;
                break;
                
            }
            //cout << "hbsdfh" <<endl;
            //removing inconsistent hypothesis in g
            //if removed adding next consistent hypo in g
            set <vector<int> > consistent_in_g;
            consistent_in_g.clear();
            for (set<vector<int> >::iterator it = generic_boundary.begin(); it != generic_boundary.end(); it++) {
                
                first_step = *(it);
                int flag = 0;
                for (int i = 0; i<(m-1); i++) {
                    if (first_step[i] != -2 && first_step[i] != curr_data[i]) {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 1) {
                    //consistent
                    consistent_in_g.insert(first_step);
                }else{
                    //inconsistent
                    //removing and adding next specific and consistent with training ex
                    vector<int> temporary_hypothesis;
                    for (int i =0; i<(m-1); i++) {
                        if (first_step[i] == -2) {
                            temporary_hypothesis = first_step;
                            for (int j = 1; j<=attr_values[i].size(); j++) {
                                if (j == curr_data[i]) {
                                    continue;
                                }else{
                                    temporary_hypothesis[i] = j;
                                    int flag2 = 1;
                                    vector<int> first_specific = *(specific_boundary.begin());
                                    for (int k = 0; k<(m-1); k++) {
                                        if (temporary_hypothesis[k] != -2 && temporary_hypothesis[k] != first_specific[k] && first_specific[k] != 0) {
                                            flag2  =0;
                                            break;
                                            
                                        }
                                    }
                                    if (flag2 == 1) {
                                        consistent_in_g.insert(temporary_hypothesis);}
                                }
                            }
                        }
                    }
                }
            }
            
            generic_boundary.clear();
            set<vector<int> > more_specific_in_consistent_g;
            vector<int> second_step;
            for (set<vector<int> >::iterator it1 = consistent_in_g.begin(); it1 != consistent_in_g.end(); it1++) {
                first_step = *(it1);
                for (set<vector<int> >::iterator it2 = consistent_in_g.begin(); it2 != consistent_in_g.end(); it2++) {
                    if (it1 == it2) {
                        continue;
                    }
                    second_step = *(it2);
                    int flag3 = 1;
                    for (int k =0; k<(m-1); k++) {
                        if (first_step[k] != -2 && first_step[k]!=second_step[k]) {
                            flag3 = 0;
                            break;
                        }
                    }
                    if (flag3 == 1) {
                        more_specific_in_consistent_g.insert(second_step);
                    }
                }
            }
            
            for (set<vector<int> >::iterator it = consistent_in_g.begin(); it != consistent_in_g.end(); it++) {
                first_step = *(it);
                if (more_specific_in_consistent_g.find(first_step) == more_specific_in_consistent_g.end()) {
                    
                    generic_boundary.insert(first_step);
                }
            }
            if (generic_boundary.size() == 0) {
                exceptional_case = 1;
                null_flag = 0;
                store_i = i;
                break;
                
            }
            
            
            
        }
        
        if (specific_boundary == generic_boundary) {
            exceptional_case = 1;
            null_flag = 0;
            store_i = i;
            break;
        }
        
        
    }
    if (exceptional_case == 1) {
        string extra;
        for (int h = store_i; h<n-1; h++) {
            getline(cin,extra);
        }
        
        if (specific_boundary.size() == 0 || generic_boundary.size() == 0) {
            cout << "Specific and generic boundaries converged to NULL" << endl;
        }else if(specific_boundary == generic_boundary){
            cout << "Specific and generic boundaries are equal" << endl;
            vector<int> printing_eq;
            cout << endl;
            for (set<vector<int> >::iterator it = specific_boundary.begin(); it != specific_boundary.end() ; it++) {
                printing_eq = *(it);
                cout << "< ";
                for (int i = 0; i<(m-1); i++) {
                    if (printing_eq[i] == 0) {
                        cout << "#";
                    }else if(printing_eq[i] == -2){
                        cout << "?";
                    }else{
                        for (map<string,int>::iterator it1 = attr_values[i].begin(); it1 != attr_values[i].end(); it1++) {
                            if ((*it1).second == printing_eq[i]) {
                                cout << (*it1).first;
                            }
                        }
                    }
                    if (i != (m-2)) {
                        cout << " ,";
                    }
                }
                cout << " > ";
                
            }
            cout << endl;
            cout << endl;
        }
    }else{
        //we got version space printing left
        cout << endl;
        cout << "Printing specific boundary" << endl;
        cout << endl;
        //printing specific border
        vector<int> printing_s;
        for (set<vector<int> >::iterator it = specific_boundary.begin(); it != specific_boundary.end(); it++) {
            printing_s = *(it);
            cout << "< ";
            for (int i = 0; i<(m-1); i++) {
                //cout << printing_s[i] << " ";
                if (printing_s[i] == 0) {
                    cout << "#";
                }else if(printing_s[i] == -2){
                    cout << "?";
                }else{
                    for (map<string,int>::iterator it1 = attr_values[i].begin(); it1 != attr_values[i].end(); it1++) {
                        if ((*it1).second == printing_s[i]) {
                            cout << (*it1).first;
                        }
                    }
                }
                if (i != (m-2)) {
                    cout << " ,";
                }
            }
            cout << " > " ;
        }
        
        cout << endl;
        cout << endl;
        
        cout << "Printing Generic border" << endl;
        cout << endl;
        
        vector<int> printing_g;
        //cout << generic_boundary.size() << endl;
        printing_g = *(generic_boundary.begin());
        //cout << printing_g.size() << endl;
        
        for (set<vector<int> >::iterator it = generic_boundary.begin(); it != generic_boundary.end(); it++) {
            printing_g = *(it);
            cout << "< ";
            for (int i = 0; i<(m-1); i++) {
                if (printing_g[i] == 0) {
                    cout << "#";
                }else if(printing_g[i] == -2){
                    cout << "?";
                }else{
                    for (map<string,int>::iterator it1 = attr_values[i].begin(); it1 != attr_values[i].end(); it1++) {
                        if ((*it1).second == printing_g[i]) {
                            cout << (*it1).first;
                        }
                    }
                    
                }
                
                if (i != (m-2)) {
                    cout << " ,";
                }
            }
            cout << " > ";
        }
        cout << endl;
        cout << endl;
        
    
    }
    
    
    return 0;
}