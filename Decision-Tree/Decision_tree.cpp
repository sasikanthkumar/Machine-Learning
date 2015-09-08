#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <map>
using namespace std;



struct Current_Data
{
    vector<string> attribute_names;
    vector< vector<string> > examples;
    int num_rows;
    int num_columns;
};

class My_tree
{
public:
    int setLeafNode(bool boolval)
    {
        leaf=boolval;
        return 1;
    }
    int setRootNode(string str)
    {
        root=str;
        return 1;
    }
    int setTreeBranch(string str,My_tree tree)
    {
        branchs.insert(pair<string,My_tree>(str,tree));
        return 1;
    }
    string getRootNode()
    {
        return root;
    }
    bool getLeafNode()
    {
        return leaf;
    }
    map<string,My_tree> get_branches(){
        return branchs;
    }
    int show_tree(int num=0) const
    {
        string temp="      ";
        if(leaf)
        {
            for(int i=0;i!=2+num;++i)
                cout<<temp;
            cout<<"---> Leaf Node =  "<<root<<endl;
        }
        else
        {
            for(int i=0;i!=0+num;++i)
                cout<<temp;
            cout<<"---> Attribute Name = "<<root<<endl;
            for(map<string,My_tree>::const_iterator iter=branchs.begin();iter!=branchs.end();++iter)
            {
                for(int i=0;i!=1+num;++i)
                    cout<<temp;
                cout<<"---> Branch = "<<iter->first<<endl;
                (iter->second).show_tree(2+num);
            }
        }
        return 1;
    }
private:
    bool leaf;
    string root;
    map<string,My_tree> branchs;
};


vector<string> get_attribute_values(const string &att,const Current_Data &data)
{
    vector<string> val;
    string value;
    vector<string>::const_iterator iter_temp;
    for(vector< vector<string> >::const_iterator iter=data.examples.begin();iter!=data.examples.end();++iter)
    {
        iter_temp=find(data.attribute_names.begin(),data.attribute_names.end(),att);
        value=(*iter)[iter_temp-data.attribute_names.begin()];
        if(find(val.begin(),val.end(),value)==val.end())
            val.push_back(value);
    }
    return val;
}

Current_Data readDataFromFile(const string &fname)
{
    Current_Data my_data;
    ifstream data_file;
    data_file.open(fname.c_str(),ios::in);
    if(!data_file)
        cout << "file missing" <<endl;
    else
    {
        string line,word;
        getline(data_file,line);
        istringstream stream(line);
        while(stream>>word)
            my_data.attribute_names.push_back(word);
        my_data.attribute_names.pop_back();
        string s_temp = my_data.attribute_names[0];
        my_data.attribute_names.erase(my_data.attribute_names.begin());
        my_data.attribute_names.push_back(s_temp);
        
        my_data.num_columns = my_data.attribute_names.size();
        while(getline(data_file,line))
        {
            vector<string> v_str;
            istringstream stream(line);
            while(stream>>word)
                v_str.push_back(word);
            v_str.pop_back();
            string s_temp1 = v_str[0];
            v_str.erase(v_str.begin());
            v_str.push_back(s_temp1);
            
            my_data.examples.push_back(v_str);
        }
        my_data.num_rows = my_data.examples.size();
        data_file.close();
    }
    return my_data;
}


Current_Data read_training_data(const string &fname,int k,int data_size,int folds)
{
    Current_Data my_data;
    ifstream data_file;
    data_file.open(fname.c_str(),ios::in);
    if(!data_file)
        cout << "ERROR!" <<endl;
    else
    {
        string line,word;
        getline(data_file,line);
        istringstream stream(line);
        while(stream>>word)
            my_data.attribute_names.push_back(word);
        my_data.attribute_names.pop_back();
        string s_temp = my_data.attribute_names[0];
        my_data.attribute_names.erase(my_data.attribute_names.begin());
        my_data.attribute_names.push_back(s_temp);
        
        my_data.num_columns = my_data.attribute_names.size();
        int counter1 = 0;
        while(getline(data_file,line))
        {
            counter1++;
            if (((((k-1)*(data_size/folds))+1) <= counter1) &&(counter1 <= (k*(data_size/folds)))) {
                
                continue;
            }
            vector<string> v_str;
            istringstream stream(line);
            while(stream>>word)
                v_str.push_back(word);
            v_str.pop_back();
            string s_temp1 = v_str[0];
            v_str.erase(v_str.begin());
            v_str.push_back(s_temp1);
            
            my_data.examples.push_back(v_str);
        }
        my_data.num_rows = my_data.examples.size();
        data_file.close();
    }
    return my_data;
}


Current_Data read_testing_data(const string &fname,int k,int data_size,int folds)
{
    Current_Data my_data;
    ifstream data_file;
    data_file.open(fname.c_str(),ios::in);
    if(!data_file)
        cout << "ERROR!" <<endl;
    else
    {
        string line,word;
        getline(data_file,line);
        istringstream stream(line);
        while(stream>>word)
            my_data.attribute_names.push_back(word);
        my_data.attribute_names.pop_back();
        string s_temp = my_data.attribute_names[0];
        my_data.attribute_names.erase(my_data.attribute_names.begin());
        my_data.attribute_names.push_back(s_temp);
        
        my_data.num_columns = my_data.attribute_names.size();
        int counter1 = 0;
        while(getline(data_file,line))
        {
            counter1++;
            if (((((k-1)*(data_size/folds))+1) <= counter1) &&(counter1 <= (k*(data_size/folds)))) {
                vector<string> v_str;
                istringstream stream(line);
                while(stream>>word)
                    v_str.push_back(word);
                v_str.pop_back();
                string s_temp1 = v_str[0];
                v_str.erase(v_str.begin());
                v_str.push_back(s_temp1);
                
                my_data.examples.push_back(v_str);
            }
        }
        my_data.num_rows = my_data.examples.size();
        data_file.close();
    }
    return my_data;
}


Current_Data get_items_for_attrVal(const string &att,const string &val,const Current_Data &data)
{
    Current_Data new_data;
    vector<string> item;
    vector<string>::const_iterator iter_temp;
    iter_temp=find(data.attribute_names.begin(),data.attribute_names.end(),att);
    if(iter_temp==data.attribute_names.end())
        cout<<"error"<<endl;
    else
    {
        int num=iter_temp-data.attribute_names.begin();
        for(vector< vector<string> >::const_iterator iter=data.examples.begin();iter!=data.examples.end();++iter)
            if((*iter)[num]==val)
                new_data.examples.push_back(*iter);
        new_data.attribute_names=data.attribute_names;
        new_data.num_columns=data.num_columns;
        new_data.num_rows=new_data.examples.size();
    }
    return new_data;
}
double attr_value_entropy(const Current_Data &data)
{
    vector<string> S;
    vector<string>::const_iterator iter_temp;
    vector<int> S_Count;
    int S_Sum=data.examples.size();
    for(vector< vector<string> >::const_iterator iter=data.examples.begin();iter!=data.examples.end();++iter)
    {
        iter_temp=find(S.begin(),S.end(),(*iter)[data.num_columns-1]);
        if(iter_temp==S.end())
        {
            S.push_back((*iter)[data.num_columns-1]);
            S_Count.push_back(1);
        }
        else
            ++S_Count[iter_temp-S.begin()];
    }
    vector<double> P(S_Count.size(),0.0);
    double I=0.0;
    for(int i=0;i!=P.size();++i)
    {
        P[i]=static_cast<double>(S_Count[i])/S_Sum;
        I=I-P[i]*log2(P[i]);
    }
    return I;
}
double attributeEntropy(const string &att,const Current_Data &data)
{
    vector<string> val=get_attribute_values(att,data);
    Current_Data new_data;
    double I=0.0,Val_E=0.0;
    for(vector<string>::const_iterator iter=val.begin();iter!=val.end();++iter)
    {
        new_data=get_items_for_attrVal(att,*iter,data);
        Val_E=attr_value_entropy(new_data);
        I=I+Val_E*new_data.num_rows/data.num_rows;
    }
    return I;
}


My_tree FormDecisionTree(const Current_Data &data)
{
    My_tree tree,tree_temp;
    tree.setLeafNode(false);
    tree.setRootNode("ROOT");
    tree_temp.setLeafNode(false);
    tree_temp.setRootNode("ROOT");
    double entropy=attr_value_entropy(data);
    double gain,Att_E,temp=0.0;
    for(vector<string>::const_iterator iter=data.attribute_names.begin();iter!=data.attribute_names.end()-1;++iter)
    {
        Att_E=attributeEntropy(*iter,data);
        gain=entropy-Att_E;
        if(gain>temp)
        {
            temp=gain;
            tree.setRootNode(*iter);
        }
    }
    if(tree.getRootNode()!="ROOT")
    {
        vector<string> branchs=get_attribute_values(tree.getRootNode(),data);
        for(vector<string>::const_iterator iter=branchs.begin();iter!=branchs.end();++iter)
        {
            Current_Data new_data=get_items_for_attrVal(tree.getRootNode(),*iter,data);
            tree_temp=FormDecisionTree(new_data);
            tree.setTreeBranch(*iter,tree_temp);
        }
    }
    else
    {
        tree.setLeafNode(true);
        tree.setRootNode((data.examples[0])[data.num_columns-1]);
    }
    return tree;
}


int main(){
    
    Current_Data data=readDataFromFile("Monks1.txt");
    int data_size = data.examples.size();
    int folds = 11;
    //cout << data_size <<endl;
    double sum_accuracy = 0.0;
    for (int k = 1; k<=folds; k++) {
        Current_Data training_data = read_training_data("Monks1.txt",k,data_size,folds);
        Current_Data testing_data = read_testing_data("Monks1.txt",k,data_size,folds);
        My_tree tree=FormDecisionTree(training_data);
        tree.show_tree();
        //cout << "training data = " << training_data.examples.size() <<endl;
        //cout << "test data = " << testing_data.examples.size() <<endl;
        
        int counter = 0;
        for (int h = 0; h<testing_data.examples.size(); h++) {
            My_tree temp_tree = tree;
            int hhh_flag = 0;
            //cout << h <<endl;
            while (temp_tree.getLeafNode() == false) {
                int p = 0;
                //cout << temp_tree.getRootNode() << endl;
                for (vector<string>::iterator it = (data.attribute_names).begin(); it != (data.attribute_names).end(); it++) {
                    p++;
                    if (*(it) == temp_tree.getRootNode()) {
                        break;
                    }
                }
                int index = p-1;
                //cout<<"jbdhjb"<<endl;
                map<string,My_tree> temp_map = temp_tree.get_branches();
                //cout << index <<endl;
                //cout << "hhh" <<endl;
                //cout << temp_tree.getRootNode() << "  bb" <<endl;
                if (temp_tree.getRootNode() == "") {
                    //cout << "fff" <<endl;
                    hhh_flag = 1;
                    break;
                }
              
                temp_tree =  temp_map[(testing_data.examples)[h][index]];
                //cout<<temp_tree.getLeafNode()<<endl;
                //cout << temp_tree.getRootNode() <<endl;
            }
            if (hhh_flag == 1) {
                //donothing
            }else if (temp_tree.getRootNode() == (((testing_data.examples)[h])).back()){
                counter++;
            }
            //cout << h <<endl;
            
        }
        //cout << counter <<endl;
        double accuracy = ((double)counter*(double)folds)/((double)data_size);
        cout << "Accuracy in fold " << k << " = " << accuracy <<endl;
        sum_accuracy += accuracy;
    }
    
    double avg_accuracy = sum_accuracy/(double)folds;
    cout << "Average accuracy = " << avg_accuracy << endl;
    cout << "Number of folds = " << folds << endl;
    return 1;
}



