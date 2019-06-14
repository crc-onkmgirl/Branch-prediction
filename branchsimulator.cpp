/*
Branch Prediction LAB

%% 2^(m+k) 2-bit saturating counters, k-bit BHR
%% m=line 1 in config.txt, k=line 2
%% trace.txt file each line one branch and its actual result
%% output file trace.txt.out each line one branch and its prediction
*/

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <math.h>
#include <bitset>
using namespace std;

int main(int argc, char** argv){
      unsigned long  pc;
      unsigned int   tagbit,m,k,index,miss,branchInst;
      bitset<1>      branch_result,prediction;
      bitset<32>     BHR;
      std::ofstream  traceout;

      traceout.open("trace.txt.out", std::ios_base::app);

//read m,k from config.txt
      string filename="config.txt";
      ifstream infile1(filename.c_str());
      string line;
      if(!infile1){
           cerr << "error opening file";
           exit(EXIT_FAILURE);
      }
      while (getline(infile1,line)){
          std::istringstream iss(line);
          if (!(iss>>m>>k)){
              break;
          }
      }
infile1.close();
infile1.clear();


//initialize the BHR
      for(int i=0;i<k;i++){
          BHR[i]=1;
      }


//initialize the PHT
      int row =  int(pow(2, m));
      int col =  int(pow(2, k));
      bitset<2>  PHT[row-1][col-1];
      for(int j=0;j<row;j++){
         for (int i=0;i<col;i++){
              PHT[j][i].set();                           //initialized counter value: ST
         }
      }


//open trace for pc.addr and branch_result
      string filename2="trace.txt";
      ifstream infile2(filename2.c_str());
      string line2;
      string result="";
        if(!infile2){
            cerr << "error opening file";
            exit(EXIT_FAILURE);
        }
        miss=branchInst=0;
        while (getline(infile2,line2)){
            std:stringstream word(line2);
            branchInst++;
            word>>std::hex>>pc>>result;                        //each line comes pc and then actual result
            branch_result=bitset<1>(result);
            tagbit = pc << 32 - m;
            tagbit >>= 32 - m;
            index= BHR.to_ulong();

//output prediction of PHT in trace.txt.out
            prediction[0]=(PHT[tagbit][index][1]);
            traceout << prediction << endl;

//verify the correctness of prediction
            if(prediction!=branch_result){
                miss++;
            }

//update PHT
            if(branch_result==1){
                if(PHT[tagbit][index]==bitset<2>(0)){
                    PHT[tagbit][index]=bitset<2>(1);
                }
                else
                {
                    PHT[tagbit][index]=bitset<2>(3);
                }
            }
            else
            {
                if(PHT[tagbit][index]==bitset<2>(3)){
                    PHT[tagbit][index]=bitset<2>(2);
                }
                else
                {
                    PHT[tagbit][index]=bitset<2>(0);
                }
            }

//update BHR
            for (int i=0;i<k-1;i++)
                BHR[i]=BHR[i+1];
            if (branch_result==1)
                BHR[k-1]=1;
            else
                BHR[k-1]=0;

        }
    infile2.close();
    infile2.clear();

    traceout.close();


// Display performance
    printf("\n// Performance of the Branch Predictor //\n\n");
    printf("Predictor parameter:  m = %d, k = %d\n", m, k);
    printf("Total Instructions:   %d \n", branchInst);
    printf("Misprediction rate:   %0.4f%%\n", (double)miss / branchInst * 100);

    return 0;
}

