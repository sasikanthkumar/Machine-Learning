#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

vector<int> pvj_values(10);

vector<int> pvj_values_for_k(10);
vector<int> training_labels_k;
vector<int> testing_labels_k;
vector<vector<vector<int> > > myMatrix_for_k;
vector<vector<vector<int> > > testing_data_for_k;
int likelihoodMatrix_for_k[10][28][28];

vector<int> training_labels;
vector<vector<vector<int> > > myMatrix;
vector<vector<vector<int> > > testing_data;
//vector<vector<vector<int> > > likelihoodMatrix;
int likelihoodMatrix[10][28][28];

double laplace_k = 5;
double laplace_v = 2;

void calculate_pvj_values(const string &fname){
    ifstream read(fname.c_str());
    int t;
    int numLines = 0;
    //cout << "in read" <<endl;
    while (read >> t ) {
        pvj_values[t]++;
        training_labels.push_back(t);
        numLines++;
    }
    
    /**
    cout << numLines << endl;
    cout << training_labels.size() << endl;
    
    for (int i = 0; i<10; i++) {
        cout << pvj_values[i] << endl;
        cout << training_labels[i] << endl;
    }
     **/
}


void calculate_pvj_values_for_k(const string &fname){
    ifstream read(fname.c_str());
    int t;
    int numLines = 0;
    //cout << "in read" <<endl;
    while (read >> t ) {
        if (numLines >= 4000) {
            testing_labels_k.push_back(t);
        }else{
            pvj_values_for_k[t]++;
            training_labels_k.push_back(t);
        }
        numLines++;
    }
}


void read_training_data_for_k(const string &fname){
    ifstream data_file;
    data_file.open(fname.c_str(),ios::in);
    if(!data_file){
        cout << "ERROR!" <<endl;
    }else{
        
        vector<vector<int> > temp1;
        vector<int> temp2;
        
        int numImages = 5000;
        
        
        for (int p = 0; p<5000; p++) {
            
            
            temp1.clear();
            string line;
            
            for (int i = 0; i<28; i++) {
                getline(data_file,line);
                //cout << line << endl;
                //cout << line.length() <<endl;
                temp2.clear();
                for (int h = 0; h<line.length(); h++) {
                    if (line[h] == ' ') {
                        temp2.push_back(0);
                        if (p<=3999) {
                            likelihoodMatrix_for_k[training_labels_k[p]][i][h]++;
                        }
                    }else{
                        temp2.push_back(1);
                    }
                }
                
                temp1.push_back(temp2);
            }
            if (p<=3999) {
                myMatrix_for_k.push_back(temp1);
                
            }else{
                testing_data_for_k.push_back(temp1);
            }
        }
    }
    data_file.close();
}


void read_training_data(const string &fname){
    ifstream data_file;
    data_file.open(fname.c_str(),ios::in);
    if(!data_file){
        cout << "ERROR!" <<endl;
    }else{
        
        vector<vector<int> > temp1;
        vector<int> temp2;
        
        int numImages = 5000;
        
        
        for (int p = 0; p<5000; p++) {
            
        
            temp1.clear();
            string line;
            for (int i = 0; i<28; i++) {
                
            
                getline(data_file,line);
                //cout << line << endl;
                //cout << line.length() <<endl;
                temp2.clear();
                for (int h = 0; h<line.length(); h++) {
                    if (line[h] == ' ') {
                        temp2.push_back(0);
                        likelihoodMatrix[training_labels[p]][i][h]++;
                    }else{
                        temp2.push_back(1);
                    }
                }
                
                temp1.push_back(temp2);
            }
            
            myMatrix.push_back(temp1);
        }
    }
    data_file.close();
}

/**
void compute_likelihood_matrix(){
    
    vector<vector<int> > temp1;
    vector<int> temp2;
    temp1.clear();
    temp2.clear();
    
    for (int c = 0; c<10; c++) {
        // calculate 0/class = 0 i.e i
        // calculate 1/ class = 0 i.e i
        // in every class 28*28 attrs
        
        int num_trng_exmples_frm_this_class = pvj_values[c];
        
        cout << c << endl;
        
        
        for (int i = 0; i<28; i++) {
            
            for (int j = 0; j<28; j++) {
                int num_times_value_f = 0;
                
                for (int k = 0; k<5000; k++) {
                    if (training_labels[k] == c) {
                        vector<vector<int> > temp_trng_exmple = myMatrix[k];
                        if (temp_trng_exmple[i][j] == 0) {
                            num_times_value_f++;
                        }
                    }
                }
                temp2.push_back(num_times_value_f);
            }
            temp1.push_back(temp2);
            temp2.clear();
        }
        likelihoodMatrix.push_back(temp1);
        temp1.clear();
    }
}
**/


void read_testing_data_for_k(const string &fname){
    ifstream data_file;
    data_file.open(fname.c_str(),ios::in);
    if(!data_file){
        cout << "ERROR!" <<endl;
    }else{
        
        vector<vector<int> > temp1;
        vector<int> temp2;
        
        int numImages = 5000;
        
        
        for (int p = 0; p<5000; p++) {
            temp1.clear();
            string line;
            for (int i = 0; i<28; i++) {
                getline(data_file,line);
                //cout << line << endl;
                //cout << line.length() <<endl;
                temp2.clear();
                for (int h = 0; h<line.length(); h++) {
                    if (line[h] == ' ') {
                        temp2.push_back(0);
                        //likelihoodMatrix[training_labels[p]][i][h]++;
                    }else{
                        temp2.push_back(1);
                    }
                }
                
                temp1.push_back(temp2);
            }
            if (p>3999) {
                testing_data_for_k.push_back(temp1);
            }
        }
    }
    data_file.close();



}

void read_testing_data(const string &fname){
    ifstream data_file;
    data_file.open(fname.c_str(),ios::in);
    if(!data_file){
        cout << "ERROR!" <<endl;
    }else{
        
        vector<vector<int> > temp1;
        vector<int> temp2;
        
        int numImages = 1000;
        
        
        for (int p = 0; p<1000; p++) {
            temp1.clear();
            string line;
            for (int i = 0; i<28; i++) {
                getline(data_file,line);
                //cout << line << endl;
                //cout << line.length() <<endl;
                temp2.clear();
                for (int h = 0; h<line.length(); h++) {
                    if (line[h] == ' ') {
                        temp2.push_back(0);
                        //likelihoodMatrix[training_labels[p]][i][h]++;
                    }else{
                        temp2.push_back(1);
                    }
                }
                
                temp1.push_back(temp2);
            }
            
            testing_data.push_back(temp1);
        }
    }
    data_file.close();

}

double predict_and_calc_accuracy(int k){
    double accu_sum = 0.0;
    for (int tt = 0; tt<=999; tt++) {
        double max = -1 * INFINITY;
        int max_index = 0;
        
        vector<vector<int> > myTestImage = testing_data_for_k[tt];
        for (int c = 0; c<10; c++) {
            double sum = 0;
            double p_val = 0;
            for (int i = 0; i<28; i++) {
                for (int j = 0; j<28; j++) {
                    
                    if (myTestImage[i][j] == 0) {
                        // cal log(fij = 0/class = c)
                        double denominator = k*laplace_v + pvj_values_for_k[c];
                        
                        double numerator = k + likelihoodMatrix_for_k[c][i][j];
                        
                        p_val = numerator/denominator;
                        
                        p_val = log(p_val);
                        
                    }else{
                        //myTestImage[i][j] == 1
                        double denominator = k*laplace_v + pvj_values_for_k[c];
                        
                        double numerator = k + pvj_values_for_k[c] - likelihoodMatrix_for_k[c][i][j];
                        
                        p_val = numerator/denominator;
                        
                        p_val = log(p_val);
                        
                    }
                    //cout << "p_val = " << p_val << " " << "sum = " << sum << endl;
                    sum = sum + p_val;
                }
            }
            
            if (sum > max) {
                max = sum;
                max_index = c;
            }
        }
        //cout << "max_index = " << max_index << endl;
        //cout << "testing label = " << testing_labels_k[tt] << endl;
        
        if (max_index == testing_labels_k[tt]) {
            accu_sum = accu_sum+1;
        }
        
    }
    
    return accu_sum;
}

int main(){
    
    calculate_pvj_values("traininglabels");
    
    read_training_data("trainingimages");
    
    // compute_likelihood_matrix();
    
    read_testing_data("testimages");
    
    // first we do for 1 test image
    
    
    // now find the k which maximizes the accuracy so we should divide the training data in to two parts one for training and one for testing
    
    // 4000 for training 1000 for testing
    
    calculate_pvj_values_for_k("traininglabels");
    
    //cout << pvj_values_for_k.size() << endl;
    
    read_training_data_for_k("trainingimages");
    
    //read_testing_data_for_k("trainingimages");
    
    //cout << testing_data_for_k.size() << endl;
    //cout << myMatrix_for_k.size() << endl;
    
    // now we should predict the accuracy for each k value from 1 to 50
    double max_accu = 0;
    int max_accu_k = 0;
    
    /**
    for (int k = 1;k<=50 ; k++) {
        double accuracy = predict_and_calc_accuracy(k);
        cout << accuracy << endl;
        if (accuracy > max_accu) {
            max_accu = accuracy;
            max_accu_k = k;
        }
    }
     **/
    //cout << "max accu k = "<< max_accu_k << endl;
    
    laplace_k = 4;
    
    //cout << testing_data.size() << endl;
    
    ofstream myfile;
    myfile.open ("testlabels_1.txt");

    for (int tt = 0; tt<1000; tt++) {
        
        
        double max = -1 * INFINITY;
        int max_index = 0;
        
        vector<vector<int> > myTestImage = testing_data[tt];
        
        
        
        for (int c = 0; c<10; c++) {
            double sum = 0;
            double p_val = 0;
            for (int i = 0; i<28; i++) {
                for (int j = 0; j<28; j++) {
                    
                    if (myTestImage[i][j] == 0) {
                        // cal log(fij = 0/class = c)
                        double denominator = laplace_k*laplace_v + pvj_values[c];
                        
                        double numerator = laplace_k + likelihoodMatrix[c][i][j];
                        
                        p_val = numerator/denominator;
                        
                        p_val = log(p_val);
                    
                    }else{
                        //myTestImage[i][j] == 1
                        double denominator = laplace_k*laplace_v + pvj_values[c];
                        
                        double numerator = laplace_k + pvj_values[c] - likelihoodMatrix[c][i][j];
                        
                        p_val = numerator/denominator;
                        
                        p_val = log(p_val);
                    
                    }
                    //cout << "p_val = " << p_val << " " << "sum = " << sum << endl;
                    sum = sum + p_val;
                }
            }
            
            if (sum > max) {
                max = sum;
                max_index = c;
            }
        }
        //cout << max_index << endl;
        myfile << tt+1 << " " << max_index << endl;
        
    }
    myfile.close();
    return 0;
}