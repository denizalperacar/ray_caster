#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdint>
#include <vector>
#include <cmath>
#include <sstream>
#define intvec vector<int>
#define intvecvec vector<vector<int>>
#define floatvec vector<float>
#define vvec vector<vector<float>>
#define fvec vector<vector<vector<float>>>
#ifndef QSORT_h
#define QSORT_h
using namespace std;
intvecvec getpos(int po,floatvec a){
    //gets position for po and returns new vector and pos
    intvec temp;
    intvec temp1;
    for(int i=0;i<a.size();i++){
        if(a[i]<a[po]){
            temp.push_back(i);
        }
        else if(a[i]>a[po]){
            temp1.push_back(i);
        }
    }
    int aa=temp.size();
    temp.push_back(po);
    temp.insert(temp.end(),temp1.begin(),temp1.end());
    intvecvec j;
    j.push_back({});
    j.push_back({});
    j[0] = temp;
    j[1].push_back(aa);
    return j;
}
intvec sortvec(floatvec a){
    intvec pivot;
    intvec pivot1;
    for(int i=0;i<a.size();i++){
        pivot1.push_back(i);
    }
    intvec aa;
    bool x=0;
    int y=0;
    int hi=a.size();
    int lo=0;
    int p=0;
    int pvtpos=0;
    floatvec tempvec;
    floatvec tempvec1;
    /*floatvec tempvec2;*/
    intvec pvtemp;
    intvecvec j;
    tempvec1.insert(tempvec1.end(),a.begin(),a.end());
    int t=0;
    while(x==0){
        //cout<<"pass:"<<t<<endl;
        t++;
        //getpos for hi lo and p//
        tempvec.clear();
        pvtemp.clear();
        for(int i=lo;i<hi;i++){
            tempvec.push_back(tempvec1[i]);
            /*pvtemp.push_back(pivot1[i]);*/
        }
        j.clear();
        j=getpos(p,tempvec);
        /*tempvec2.clear();*/
        pvtemp.clear();
        tempvec.clear();
        for (int i=0;i<j[0].size();i++){
            tempvec.push_back(tempvec1[lo+j[0][i]]);
            pvtemp.push_back(pivot1[lo+j[0][i]]);
        }
        //cout<<endl;
        //update pivot//
        //cout<<j[1][0]<<"J"<<endl;
        pivot.insert(pivot.begin()+pvtpos,(j[1][0])+lo);
            for(int i=0 ; i<pivot.size() ; i++){
        	//cout<<pivot[i]<<"t--";
        }
        //cout<<endl;
        //update tempvec1//
        tempvec1.erase(tempvec1.begin()+lo,tempvec1.begin()+hi);
        pivot1.erase(pivot1.begin()+lo,pivot1.begin()+hi);
        tempvec1.insert(tempvec1.begin()+lo,tempvec.begin(),tempvec.end());
        pivot1.insert(pivot1.begin()+lo,pvtemp.begin(),pvtemp.end());
        for (int i=0;i<tempvec1.size();i++){
            //cout<<tempvec1[i]<<",";
        }
        //cout<<endl;
        for (int i=0;i<pivot.size();i++){
            //cout<<pivot[i]<<" p ";
        }
        //cout<<endl;
        
        //set hi  lo and p pvtpos//
        while(pvtpos==pivot[pvtpos] && pvtpos < pivot.size()){
            pvtpos=pvtpos+1;
            //cout<<pvtpos<<"pvtpos"<<endl;
        }
        lo=pvtpos;
        if(pvtpos==pivot.size()){
            hi=a.size();
        }
        else{
            hi=pivot[pvtpos];
        }
        //cout<<lo<<"-"<<hi<<endl;
        for(int i=0 ; i<pivot1.size() ; i++){
        	//cout<<pivot1[i]<<"--";
        }
        //cout<<endl;
        //check if finished//
        if(pivot.size()>=a.size()){
            x=1;
        }
    }
    aa.insert(aa.end(),pivot1.begin(),pivot1.end());
    return aa;
}
#endif
