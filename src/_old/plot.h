#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdint>
#include <vector>
#include <cmath>
#define coco vector<vector<vector<uint8_t>>>
#ifndef plot_h
#define plot_h
using namespace std;

/* converts decimal value to bytenumber bytes */

vector<uint8_t> tobyte(int bytenumber,int value){
    vector<uint8_t> a;
    int d=0;
    do{
        uint8_t b=value%256;
        a.push_back(b);
        value= value/256;
        //cout<<"?"<<b;
        d++;
    }
    while(value != 0);
    //cout<<d<<endl;
    for(int i =d;bytenumber-i>0;i++){
        a.push_back(0);
    }
    return a;
}

/*plots vector coco on bmp file b*/

int plotbmp(coco *a,char* b){
    int imgsize = (((*a).size())*((*a)[0].size())*3);
    int filesize = imgsize+54;
    vector<uint8_t> fs = tobyte(4,filesize);
    vector<uint8_t> h = tobyte(4,(*a).size());
    vector<uint8_t> w = tobyte(4,((*a)[0].size()));
    vector<uint8_t> imsize = tobyte(4,imgsize);
    uint8_t fileheader[14]={66,77,fs[0],fs[1],fs[2],fs[3],0,0,0,0,54,0,0,0};
    uint8_t DIBheader[40]={40,0,0,0,w[0],w[1],w[2],w[3],h[0],h[1],h[2],h[3],1,0,24,0,0,0,0,0,imsize[0],imsize[1],imsize[2],imsize[3],0,0,0,0,0,0,0,0,0,0,0,0};
    fstream c;
    c.open(b,fstream::binary|fstream::in|fstream::out|fstream::trunc);
    for (int i=0;i<14;i++){
        c<<fileheader[i];
    }
        for (int i=0;i<40;i++){
        c<<DIBheader[i];
    }

    for(int x=0; x<(*a).size(); x++){
        for(int y=0; y<(*a)[x].size(); y++){
                int xx=((*a).size()-x)-1;
                int yy=((*a)[x].size()-y)-1;
                c << (*a)[x][y][0]<< (*a)[x][y][1]<< (*a)[x][y][2];
        }
    }
    c.close();
    return 0;
}
#endif
