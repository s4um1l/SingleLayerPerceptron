#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string.h>
#include <cmath>
#include <limits.h>
#include <set>
#include <stdlib.h>

int no_of_iterations=0;
double learning_rate;

using namespace std;
class ReadFromFile
{


public:
    vector< vector <int> > trainingList;// To hold the whole list of input data .Actually this is table on which we will operate

    vector <int> record;// A single file record

    int no_of_inputs;//No of Input Columns

    ReadFromFile()
    {
        //initialization of  values
        no_of_inputs=0;
    }
    void buildRecord(char * path)
    {
        string line;
        ifstream fp (path);
        if (!fp.is_open()) //
            cout<< "Error in the file path";
        else
        {
            getline(fp,line);

            istringstream iss(line);
            string token;
            int i=0,j=0;
            while(iss >> token)
            {
                no_of_inputs++;
            }
            //cout<<endl<<"No of attributes"<<no_of_inputs;
            while (!fp.eof())
            {

                getline(fp,line);
                if (line == ""||line=="\n") continue;
                istringstream iss(line);
                string token;

                while (iss>>token)
                {
                    int temp;
                    istringstream(token)>>temp;
                    record.push_back(temp);
                }
                trainingList.push_back(record);
                record.clear();

            }
        }


    }

    //function to display list of data stored
    void displayList()
    {
        vector<vector <int> >::iterator iter=trainingList.begin();
        vector <int> ::iterator iter1;
        cout << "size of the list" << trainingList.size();
        while (iter != trainingList.end())
        {
            //  cout<<"New Record"<<endl;
            for(iter1=(*iter).begin(); iter1!=(*iter).end(); iter1++)
            {
                cout <<*iter1;
            }
            cout<<endl;
            iter++;
        }


    }


};

class PerceptronLearning
{
    double* weights;
    ReadFromFile input;

public:
    void displayWeights(){
    for (int i=0;i<input.no_of_inputs;i++)
    cout<<weights[i]<<endl;
    }
    vector <int> output;

    PerceptronLearning(ReadFromFile train)
    {
        input= train;
        weights = new double[input.no_of_inputs];
        for (int i=0; i<input.no_of_inputs; i++)
            weights[i]=0;
        }

    void implementPLA();
    double calcSigmoid(double sum);
    int calcOutput(double o);
    void weightAdjuster(double o,double error,int row);
    double testPLA(ReadFromFile test);


};
double PerceptronLearning::testPLA(ReadFromFile test){
    int correct =0;
 for (int j=0; j<test.trainingList.size(); j++)
        {
            //for each example
            double sum=0;
            for (int k=0; k<test.no_of_inputs; k++)
            {
                //for each column
                double tempcalc;
                tempcalc=weights[k]*test.trainingList[j][k];
                sum+= tempcalc;
            }
            double o=calcSigmoid(sum); //  the o in lecture sigmoid output
            int out=calcOutput(o);
          //  cout << "Output for row "<<j<<"is  "<<out<<endl;
            output.push_back(calcOutput(o));
         //   cout<<test.trainingList[j][test.no_of_inputs]<<"this is whatI compareto"<<endl;
            if (test.trainingList[j][test.no_of_inputs]==out)
            correct++;

            //code for adjustment of weights
        }
       return (double)correct/(double)test.trainingList.size()*100;
    }



void PerceptronLearning::weightAdjuster(double o,double error,int row)
{
//code for weight adjustment
    for (int i=0; i<input.no_of_inputs; i++)
    {
        //for each attribute
        weights[i]=weights[i]+ (learning_rate*error*o*(1-o)*input.trainingList[row][i]);//decreasing errors

    }

}
int PerceptronLearning::calcOutput(double o)
{

    if (o>=0.5)
        return 1;
    else
        return 0;

}
double PerceptronLearning::calcSigmoid(double sum)
{
    // for calculation sigmoid basically g(in)
    sum= -1 *sum;
    double denom= 1+ exp(sum);
    return 1/denom;


}
void PerceptronLearning::implementPLA()
{
    int counter=0;

    for (int i=0; i<no_of_iterations; i++)
    {
        output.clear();
        //Keep on learning until convergence
        for (int j=0; j<input.trainingList.size(); j++)
        {
            //for each example
            double sum=0;
            for (int k=0; k<input.no_of_inputs; k++)
            {
                //for each column
                double tempcalc;
                tempcalc=weights[k]*input.trainingList[j][k];
                sum+= tempcalc;
            }
            double o=calcSigmoid(sum); //  the o in lecture sigmoid output
           // cout << "Output for row "<<j<<"Iteration "<<i<<"is  "<<calcOutput(o)<<endl;
            output.push_back(calcOutput(o));
            //code for adjustment of weights

            double  error= input.trainingList[j][input.no_of_inputs] - o;
            weightAdjuster(o,error,j);

        }
    }
}

int main(int argc, char* argv[])
{
    ReadFromFile train,test;
    train.buildRecord(argv[1]);
    test.buildRecord(argv[2]);

    sscanf(argv[3],"%lf",&learning_rate);
    sscanf(argv[4],"%d",&no_of_iterations);
    //cout<<learning_rate<<"Learning rate"<<endl;
    //cout<<no_of_iterations<<"No of iterations"<<endl;
    //cout<<train.trainingList.size()<<"Trainnig List size"<<endl;
    cout<<"<------------------------Single Layer Perceptron---------------------->"<<endl;
    // train.displayList();
    PerceptronLearning a(train);

    a.implementPLA();
    //a.displayWeights();
    cout<<endl<<"Accuracy on Training Set"<<a.testPLA (train)<<endl;
   cout<<endl<<"Accuracy on Test Set"<<a.testPLA(test)<<endl;
    return 0;
}
