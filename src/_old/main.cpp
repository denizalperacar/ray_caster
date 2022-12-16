#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdint>
#include <vector>
#include <cmath>
#include <sstream>
#include "plot.h"
#include "QSORT.h"
#include <chrono>
#include <execution>
#include <algorithm>
#define intvec vector<int>
#define intvecvec vector<vector<int>>
#define floatvec vector<float>
#define vvec vector<vector<float>>
#define fvec vector<vector<vector<float>>>
#define coco vector<vector<vector<uint8_t>>>
#define PIv 3.14159265
using namespace std;

/*class ray with origin and destenation given 
has vector,length,equation*/

class ray{
    public:
    floatvec Org;
    floatvec Des;
    floatvec XP;
    floatvec YP;
    floatvec vec(){
        floatvec v;
        for (int i=0;i<3;i++){
        v.push_back(Des[i]-Org[i]);
        }
        return v;
    }
    float length(){
        float a=sqrt(pow(vec()[0],2)+pow(vec()[1],2)+pow(vec()[2],2));
        return a;
    }
};
class P{
	public:
	float radius;
	int res;
	int c2wr;
	int c2hr;
	int c1hr;
	float c1h;
	float c2h;
	float c2w;
	float c2o;
	float fo;
};

/*class object has vertex,face,atn*/


class object{
    public:
    floatvec OAtn;
    intvec o;
    vvec OVP;
    fvec OF;
    fvec plane;
    vector<vector<int>> ref;
    vector<vector<int>> temp;
    vvec temp1;
};

/*converts obj file to object*/

object objtovec(char *FN){
    fstream a;
    a.open(FN,fstream::in);
    string buff;
    object c;
    getline(a,buff);
    getline(a,buff);
    getline(a,buff);
    int ONu=-1;
    int OVPNu = -1;
    int OFNu = -1;
    stringstream buff1(buff);
    while(a){
        buff1.str("");
        buff1.clear();
        buff1<<buff;
        getline(buff1,buff,' ');
        if(buff=="o"){
            getline(buff1,buff,'_');
            getline(buff1,buff);
            c.OAtn.push_back(stof(buff));
            c.OF.push_back({});
            ONu++;
            c.o.push_back(ONu);
            OFNu = -1;
            /*cout<<"o"<<buff<<" ";*/
        }
        else if(buff=="v"){
            OVPNu++;
            c.OVP.push_back({});
            getline(buff1,buff,' ');
            /*cout<<OVPNu<<"vw"<<buff<<" ";*/
            c.OVP[OVPNu].push_back(stof(buff));
            getline(buff1,buff,' ');
            /*cout<<buff<<" ";*/
            c.OVP[OVPNu].push_back(stof(buff));
            getline(buff1,buff);
            /*cout<<buff<<" "<<endl;*/
            c.OVP[OVPNu].push_back(stof(buff));
        }
        else if(buff=="f"){
            OFNu++;
            c.OF[ONu].push_back({});
            getline(buff1,buff,' ');
            /*cout<<OFNu<<"fw"<<buff<<" ";*/
            c.OF[ONu][OFNu].push_back(stoi(buff)-1);
            getline(buff1,buff,' ');
            /*cout<<buff<<" ";*/
            c.OF[ONu][OFNu].push_back(stoi(buff)-1);
            getline(buff1,buff);
            /*cout<<buff<<" "<<endl;*/
            c.OF[ONu][OFNu].push_back(stoi(buff)-1);
        }
        getline(a,buff);
    }
    a.close();
    return c;
}

/*faces of an object to plane equation xX+yY+zZ+d=0*/

fvec FtoP(object *cc){
	object c = (*cc);
    fvec fp;
    float x;
    float y;
    float z;
    float d;
    floatvec va;
    floatvec vb;
    floatvec vc;
    floatvec ba;
    floatvec ca;
    //float test;
    for(int i=0; i<c.OF.size(); i++){
            fp.push_back({});
            for (int j=0; j<c.OF[i].size(); j++){
                fp[i].push_back({});
                va.clear();
                vb.clear();
                vc.clear();
                ba.clear();
                ca.clear();
                for(int k=0;k<3;k++){
                    va.push_back(c.OVP[(c.OF[i][j][0])][k]);
                    vb.push_back(c.OVP[(c.OF[i][j][1])][k]);
                    vc.push_back(c.OVP[(c.OF[i][j][2])][k]);
                    ba.push_back(vb[k]-va[k]);
                    ca.push_back(vc[k]-va[k]);
                }
                x=(ba[1]*ca[2]) - (ba[2]*ca[1]);
                y=(ba[2]*ca[0]) - (ba[0]*ca[2]);
                y=(ba[2]*ca[0]) - (ba[0]*ca[2]);
                z=(ba[0]*ca[1]) - (ba[1]*ca[0]);
                d=(-1)*((va[0]*x)+(va[1]*y)+(va[2]*z));
                fp[i][j].push_back(x);
                fp[i][j].push_back(y);
                fp[i][j].push_back(z);
                fp[i][j].push_back(d);
                //test = d;
                //test= test +(c.OVP[(c.OF[i][j][2])][0]*x);
                //test= test +(c.OVP[(c.OF[i][j][2])][1]*y);
                //test= test +(c.OVP[(c.OF[i][j][2])][2]*z);
                //cout<<test<<endl;
            }

    }
    //cout<<"-----------------"<<endl;
    return fp;
}

/*given a ray and a face of an object gives intesection*/

floatvec PLIntrsctn(ray &ray,floatvec &face){
    floatvec aa;
    float px = face[0];
    float py = face[1];
    float pz = face[2];
    float pd = face[3];
    float vx = ray.Des[0]-ray.Org[0];
    float vy = ray.Des[1]-ray.Org[1];
    float vz = ray.Des[2]-ray.Org[2];
    float t;
    float temp1=(px*ray.Org[0])+(py*ray.Org[1])+(pz*ray.Org[2]);
    temp1 = temp1+pd;
    temp1 = (-1)*temp1;
    float temp2=(px*vx)+(py*vy)+(pz*vz);
    t=temp1/temp2;
    float x=(ray.Org[0])+(t*vx);
    float y=(ray.Org[1])+(t*vy);
    float z=(ray.Org[2])+(t*vz);
    //cout<<"test "<<(px*x)+(py*y)+(pz*z)+(pd)<<endl;
    aa={x,y,z};
    return aa;
}

/*if a point is insde a face*/

bool isinside(floatvec &point,vvec &face){
    floatvec p1=face[0];
    floatvec p2=face[1];
    floatvec p3=face[2];
    floatvec vi;
    floatvec temp11;
    //2D//
    float temp1;
    float temp2;
    float L21=0;
    float L31=0;
    float Li1=0;
    float Lt=0;
    float crsp=0;
    float crsp1=0;
    float crsp2=0;
    float crsp3=0;
    float w1=0;
    float w2=0;
    floatvec vw2;
    floatvec OP1;
    floatvec OP2;
    floatvec OP0;
    bool state=false;
    for (int i=0;i<3;i++){
        L21 = L21+pow(p2[i]-p1[i],2);
        L31 = L31+pow(p3[i]-p1[i],2);
        crsp=crsp+((p2[i]-p1[i])*(p3[i]-p1[i]));
        vi.push_back(point[i]-p1[i]);
        Li1 = Li1+pow(vi[i],2);
        crsp1=crsp1+((vi[i])*(p2[i]-p1[i]));
    }
    Li1=sqrt(Li1);
    L21=sqrt(L21);
    L31=sqrt(L31);
    temp1= crsp/L21;
    for (int i=0;i<3;i++){
    	temp11.push_back((p1[i]+((p2[i]-p1[i])*temp1/L21))-p3[i]);
    	crsp2=crsp2+((p3[i]-p1[i])*(temp11[i]));
    	crsp3=crsp3+((point[i]-p1[i])*(temp11[i]));
    	Lt = Lt+pow(temp11[i],2);
    }
    temp2= crsp2/Lt;
    //enew coords//
    OP1={L21,0};
    OP2={temp1,temp2};
    temp1= crsp1/L21;
    temp2= crsp3/Lt;
    OP0={temp1,temp2};
    temp1=OP0[1]*(OP2[0]/OP2[1]);
    vw2={temp1,OP0[1]};
    w2=L31*(OP0[1]/OP2[1]);
    w2=w2/L31;
    w1=OP0[0]-vw2[0];
    w1=w1/L21;
        //cout<<"w "<<w1+w2<<" "<<w1<<" "<<w2<<endl;
        //cout<<endl;
        //cout<<p1[0]<<"  "<<p1[1]<<"  "<<p1[2]<<endl;
        //cout<<p2[0]<<"  "<<p2[1]<<"  "<<p2[2]<<endl;
        //cout<<p3[0]<<"  "<<p3[1]<<"  "<<p3[2]<<endl;
        //cout<<point[0]<<"  "<<point[1]<<"  "<<point[2]<<endl;
    if(w1+w2-1<0.0001 && w1>-0.0001 && w2>-0.0001){
        state = true;
    }
    return state;
}

/*intersects ray and object and gives distance and atunation for each intersection*/

vvec getattun(object *object,ray *ray){
    floatvec a;
    floatvec b;
    vvec c;
    int temp=0;
    vvec face(3);
    floatvec inscn;
    float test;
    float test1;
    //cout<<(*object).ref.size()<<endl;
    for (int i = 0; i<(*object).ref.size(); i++){
                for (int k=0; k<3; k++){
                    face[k]=(*object).OVP[(*object).OF[(*object).ref[i][0]][(*object).ref[i][1]][k]];
                }
                inscn = PLIntrsctn((*ray),(*object).plane[(*object).ref[i][0]][(*object).ref[i][1]]);
                if (isinside(inscn,face) == true){
                	//test=(*object).plane[(*object).ref[i][0]][(*object).ref[i][1]][k]][3];
                	//test1=(*object).plane[(*object).ref[i][0]][(*object).ref[i][1]][k]][3];
                    //cout<<"+"<<endl;
                    float distance=0;
                    for (int k=0; k<3; k++){
                    	//test=test+((*object).plane[i][j][k]*inscn[k]);
                    	//test1=test1+((*object).plane[i][j][k]*face[2][k]);
                    	//cout<<inscn[k]<<"aa"<<endl;
                    	//cout<<face[k][0]<<"bb"<<face[k][1]<<"bb"<<face[k][2]<<"bb"<<endl;
                    	distance = distance + pow(inscn[k]-(*ray).Org[k],2);
                    }
                    //cout<<object.plane[i][j][0]<<"ee"<<object.plane[i][j][1]<<"ee"<<object.plane[i][j][2]<<"ee"<<object.plane[i][j][3]<<"ee"<<endl;
                    //cout<<test<<" test"<<endl;
                    //cout<<test1<<" test1"<<endl;
                    //distance=sqrt(distance);
                    //cout<<distance<<"D"<<endl;
                    c.push_back({});
                    c[temp].push_back(distance);
                    c[temp].push_back((*object).OAtn[(*object).ref[i][0]]);
                    c[temp].push_back((*object).ref[i][0]);
                    temp++;
                    //cout<<temp<<"T"<<endl;

               
        }
    }
    //cout<<c.size()<<endl;
	 //cout<<"getattune"<<endl;
    return c;
}

/*takes a getattun and returns fvec [0][0]:distance[1]:vectors of positions of each dis*/

fvec duphndl(vvec M){
	fvec a;
	a.push_back({});
	a.push_back({});
	floatvec b;
	floatvec c;
	vvec d = M;
	for (int i=0; i<d.size()-1; i++){
		if (d[i][2]>=-0.0001){
			b.push_back(d[i][0]);
			c.clear();
			c.push_back(i);
			for(int j=i+1; j<d.size(); j++){
				if (abs(d[i][0]-d[j][0])<0.0001 & d[j][2]>=-0.0001){
					/*cout<<d[i][0]<<"equal"<<d[j][0]<<endl;;*/
					if (abs(d[i][2]-d[j][2])>0.0001){
						/*cout<<d[i][2]<<"dd"<<d[j][2]<<endl;*/
						c.push_back(j);
					}
					d[j][2]=-1;
				}
				else{
				/*cout<<d[i][0]-d[j][0]<<"++"<<endl;*/
				}
			}
			a[1].push_back(c);
		}
	}
	c.clear();
	if(d[d.size()-1][2]>=-0.0001){
		/*cout<<"last"<<d[d.size()-1][2]<<endl;*/
		b.push_back(d[d.size()-1][0]);
		c.push_back(d.size()-1);
		a[1].push_back(c);
	}
	a[0].push_back(b);
	return a;
}


fvec sgmnt(object* O,ray *R){
	vvec b =getattun(O,R);
	//cout<<b.size()<<" ";
	fvec e;/*[point][i][dist,att,ON,next or -1]*/
	if (b.size()>0){
	fvec c =duphndl(b);
	intvec d =sortvec(c[0][0]);
	int tmp;
	float tmp0;
	float tmp1;
	float tmp2;
	for (int i=0; i<d.size(); i++){
		e.push_back({});
		for (int j=0; j<c[1][d[i]].size(); j++){
			tmp0 = b[c[1][d[i]][j]][0];
			tmp1 = b[c[1][d[i]][j]][1];
			tmp2 = b[c[1][d[i]][j]][2];
			e[i].push_back({tmp0,tmp1,tmp2,-1});
		}
	}
	/*for (int i=0; i<e.size(); i++){
		for (int j=0; j<e[i].size(); j++){
			cout<<"e"<<e[i][j][0]<<" "<<e[i][j][1]<<" "<<e[i][j][2]<<" "<<e[i][j][3]<<" "<<endl;
		}
    	}*/
	/*pair*/
	//cout<<endl;
	for (int i=0; i<e.size()-1; i++){
		for (int j=0; j<e[i].size(); j++){
		if (abs(e[i][j][3]+1)<0.0001){
			tmp = 0;
			tmp0 = i;
			while(tmp==0 & tmp0<e.size()-1){
				tmp0++;
				for(int jj=0; jj<e[tmp0].size(); jj++){
					if (abs(e[tmp0][jj][3]+1)<0.0001 & abs(e[i][j][2]-e[tmp0][jj][2])<0.0001){
					/*cout<<abs(e[i][j][2]-e[tmp0][jj][2])<<"p"<<tmp0<<"i"<<i<<endl;*/
					tmp=1;
					e[i][j][3]=tmp0;
					e[tmp0][jj][3]=-2;
					}
				}
			}
			if(tmp==0){
				e[i].erase(e[i].begin()+j);
			}
		}
		}
	}
	for (int i=0; i<e.size(); i++){
		if(e[i].size()==0){
			e.erase(e.begin()+i);
		}
	}
	/*pair*/}
	//cout<<e.size()<<endl;
	return e;	
}
/*[sgmnt][length,atn0,atn1...]*/
vvec dvd(object* O,ray *R){
	vvec a;
	fvec s= sgmnt(O,R);
	//cout<<s.size();
	if (s.size()>0){
	intvecvec ps;
	floatvec cs;
	intvec k;
	float l;
	int lasti;
	int lastj;
	for(int i=0; i<s.size()-1; i++){
		cs.clear();
		for (int j=0; j<s[i].size(); j++){
			/*cout<<"s"<<s[i].size()<<"i"<<j<<"ps"<<ps.size()<<endl;*/
			if (s[i][j][3]>-0.1){
				k.clear();
				k={i,j};
				ps.push_back(k);
				cs.push_back(s[i][j][1]);
				/*cout<<"ps"<<ps.size()<<endl;*/
				lasti=i;
				lastj=j;
				for(int ii=0; ii<ps.size()-1; ii++){
					/*cout<<"a"<<endl;*/
					if(s[i][j][3]>s[ps[ii][0]][ps[ii][1]][3]){
						cs.push_back(s[ps[ii][0]][ps[ii][1]][1]);
					}
				}
			}
			else{
				for(int ii=0; ii<ps.size(); ii++){
					if(abs(s[ps[ii][0]][ps[ii][1]][3]-i)<0.0001){
						/*cout<<"e"<<ii<<endl;*/
						ps.erase(ps.begin()+ii);
					}
					
				}
				if(ps.size()==0){
					cs.push_back(1);
				}
				
				for(int ii=0; ii<ps.size(); ii++){
					/*cout<<"a"<<endl;*/
					cs.push_back(s[lasti][lastj][1]);
					if(s[lasti][lastj][3]>s[ps[ii][0]][ps[ii][1]][3]){
						cs.push_back(s[ps[ii][0]][ps[ii][1]][1]);
					}
				}
			}
		}
		l=s[i+1][0][0]-s[i][0][0];
		//cout<<"l"<<l<<endl;
		a.push_back({});
		a[i].push_back(l);
		for (int j=0; j<cs.size(); j++){
			a[i].push_back(cs[j]);
		}
	}}
	//cout<<"dvd"<<endl;
	return a;
}

/*uint8_t fm(object *O, ray R,float I){
	float I1=I/(R.length()*R.length());
	vvec d= dvd(O,R);
	float temp;
	uint8_t p;
	float IH = 1;
	if (d.size()>0){
	for (int i=0; i<d.size(); i++){
		temp=0;
		for (int j=1; j<d[i].size(); j++){
			temp=d[i][j]+temp;
		}
		if(temp>0){
			//cout<<"temp "<<temp<<"d "<<d[i][0]<<"e "<<pow(1/temp,d[i][0])<<endl;
			I1=I1*pow(1/temp,d[i][0]);
		}
	}
	}
	//cout<<"i1 "<<I1<<endl;
	p=floor(256*(pow(0.5,I1/IH)));
	//cout<<"fm"<<endl;
	return p;
}*/
float fm1(object *O, ray *R,float I){
	float length = pow((*R).Des[0]-(*R).Org[0],2)+pow((*R).Des[1]-(*R).Org[1],2)+pow((*R).Des[2]-(*R).Org[2],2);
	float I1=I/length;
	vvec d= dvd(O,R);
	float temp;
	float IH = 1;
	//cout<<d.size();
	if (d.size()>0){
	for (int i=0; i<d.size(); i++){
		temp=0;
		for (int j=1; j<d[i].size(); j++){
			temp=d[i][j]+temp;
		}
		if(temp>0){
			//cout<<"temp "<<temp<<"d "<<d[i][0]<<"e "<<pow(1/temp,d[i][0])<<endl;
			I1=I1*pow(1/temp,d[i][0]);
		}
	}
	}
	//cout<<"i1 "<<I1<<endl;
	//cout<<"fm"<<endl;
	return I1;
}


vector<vector<vector<ray>>> PI(P P){
	vector<vector<vector<ray>>> a;
	float x1;
	float y1;
	float z1;
	float x2;
	float y2;
	float z2;
	float l;
	for(int j=0; j<P.res; j++){
		a.push_back({});
		for (int k=0; k<P.c1hr; k++){
			a[j].push_back({});
			for (int i=0; i<P.c2hr; i++){
				ray R;
				x1=P.radius*cos((2*PIv/6)-(j*(PIv*10/6)/P.res));
				y1=P.radius*sin((2*PIv/6)-(j*(PIv*10/6)/P.res));
				z1=P.fo+(k*P.c1h/P.c1hr);
				x2=P.radius*cos((-4*PIv/6)-(j*(PIv*10/6)/P.res));
				y2=P.radius*sin((-4*PIv/6)-(j*(PIv*10/6)/P.res));
				z2=P.c2o+i*(P.c2h/P.c2hr);
				R.Org={x1,y1,z1};
				R.Des={x2,y2,z2};
				l=sqrt(pow(x2-x1,2)+pow(y1-y2,2));
				R.XP={(y1-y2)/l,(x2-x1)/l,0};
				R.YP={0,0,1};
				a[j][k].push_back(R);
			}
		}
	}
	
	return a;
}

/*coco rndr(vector<vector<ray>> R,object O){
	coco a;
	uint8_t b;
	int c;
	O.plane = FtoP(O);
	object *OO=&O;
	for (int i=0; i<R.size(); i++){
		a.push_back({});
		for (int j=0; j<R[i].size(); j++){
			b = fm(OO,R[i][j],200);
			a[i].push_back({});
			c=b;
			cout<<"b"<<c<<" "<<i<<","<<j<<endl;
			for (int k=0; k<3; k++){
				a[i][j].push_back(b);
			}
		}
	}
	return a;
}*/


void pnttocrd(floatvec &P, floatvec &O, floatvec &X, floatvec &Y, floatvec & a){
	float x=0;
	float y=0;
	for (int i=0; i<3; i++){
		x=x+((P[i]-O[i])*X[i]);
		y=y+((P[i]-O[i])*Y[i]);
	}
	a={x,y};
}

void prjct(object *O, floatvec &Org, floatvec &X, floatvec &Y, floatvec &Src , vvec &a){
	floatvec srcp;
	pnttocrd(Src,Org,X,Y,srcp);
	floatvec srcpc={Org[0]+(X[0]*srcp[0])+(Y[0]*srcp[1]),Org[1]+(X[1]*srcp[0])+(Y[1]*srcp[1]),Org[2]+(X[2]*srcp[0])+(Y[2]*srcp[1])};
	floatvec srcpv={srcpc[0]-Src[0],srcpc[1]-Src[1],srcpc[2]-Src[2]};
	float srcpvl=0;
	for (int i=0; i<3; i++){
		srcpvl=srcpvl+pow(srcpv[i],2);
	}
	srcpvl=srcpvl;
	//cout<<"srcpv "<<srcpc[0]<<" "<<srcpc[1]<<" "<<srcpc[2]<<endl;
	floatvec crd;
	//floatvec crdc;
	for (int i=0; i<(*O).OVP.size(); i++){
		float crsp=0;
		//float l=0;
		for (int j=0; j<3; j++){
			crsp=crsp+(srcpv[j]*((*O).OVP[i][j]-Src[j]));
		}
		crsp=srcpvl/crsp;
		//crdc.clear();
		pnttocrd((*O).OVP[i],Org,X,Y,crd);
		/*crdc={Org[0]+(X[0]*crd[0])+(Y[0]*crd[1]),Org[1]+(X[1]*crd[0])+(Y[1]*crd[1]),Org[2]+(X[2]*crd[0])+(Y[2]*crd[1])};
		for (int j=0; j<3; j++){
			l=l+pow(crdc[j]-O.OVP[i][j],2);
		}
		l=sqrt(l);
		if(i==0){
			cout<<"l "<<l<<" srcpvl "<<srcpvl<<" crsp "<<crsp<<endl;
		}*/
		//l=srcpvl/(srcpvl-l);
		a[i] = {srcp[0]+((crd[0]-srcp[0])*crsp), srcp[1]+((crd[1]-srcp[1])*crsp)};
	}
}


int getsgn(float a){
	int b;
	b=0;
	if(a>0){
		b=1;
	}
	return b;
}
vector<vector<int>> trgX(vvec *P,object *O){
	vector<vector<int>> a;
	int p1;
	int p2;
	int p3;
	for(int i=0; i<(*O).OF.size(); i++){
		for(int j=0; j<(*O).OF[i].size(); j++){
			p1=getsgn((*P)[(*O).OF[i][j][0]][0]);
			p2=getsgn((*P)[(*O).OF[i][j][1]][0]);
			p3=getsgn((*P)[(*O).OF[i][j][2]][0]);
			if(0<p1+p2+p3 &p1+p2+p3<3){
				a.push_back({i,j});
			}
		}
	}
	return a;
}
vector<vector<int>> trgY(vvec *P,object *O,vector<vector<int>>*X,float Y){
	vector<vector<int>> a;
	int p1;
	int p2;
	int p3;
	for(int i=0; i<(*X).size(); i++){
		p1=getsgn((*P)[(*O).OF[(*X)[i][0]][(*X)[i][1]][0]][1]-Y);
		p2=getsgn((*P)[(*O).OF[(*X)[i][0]][(*X)[i][1]][1]][1]-Y);
		p3=getsgn((*P)[(*O).OF[(*X)[i][0]][(*X)[i][1]][2]][1]-Y);
		if(0<p1+p2+p3 &p1+p2+p3<3){
			a.push_back({(*X)[i][0],(*X)[i][1]});
		}
	}
	return a;
}

/*coco rndrp(vector<vector<vector<ray>>> R,object O){
	coco a;
	float d;
	uint8_t b;
	int c;
	float IH=1;
	O.plane = FtoP(O);
	object *OO=&O;
	for (int i=0; i<R.size(); i++){
		a.push_back({});
		for (int j=0; j<R[i].size(); j++){
			d=0;
			
			for (int k=0; k<R[i][j].size(); k++){
				d=d+fm1(OO,R[i][j][k],5);
			}
			b=floor(256*(pow(0.5,d/IH)));
			a[i].push_back({});
			c=b;
			cout<<"b"<<c<<" "<<i<<","<<j<<endl;
			for (int k=0; k<3; k++){
				a[i][j].push_back(b);
			}
		}
	}
	return a;
}*/

int PtoO(char*a,vvec C){
    fstream c;
    c.open(a,fstream::in|fstream::out|fstream::trunc);
    c<<"o "<<"123"<<endl;
	for(int i=0; i<C.size(); i++){
			c<<"v "<<C[i][0]<<" "<<C[i][1]<<" "<<0<<endl;
	}
	c.close();
	return 0;
}

int PtoOR(char*a,vvec C,intvec ref){
    fstream c;
    c.open(a,fstream::in|fstream::out|fstream::trunc);
    c<<"o "<<"123"<<endl;
	for(int i=0; i<ref.size(); i++){
			c<<"v "<<C[ref[i]][0]<<" "<<C[ref[i]][1]<<" "<<0<<endl;
	}
	c.close();
	return 0;
}

coco rndrp(vector<vector<vector<ray>>> *R,object *O){
	fvec a;
	coco e;
	float d;
	uint8_t b;
	int c;
	float IH=1;
	(*O).plane = FtoP((O));
	float temp;
	vvec f((*O).OVP.size());
	//vvec tat;
	for (int i=0; i<(*R).size(); i++){
		a.push_back({});
		for (int j=0; j<(*R)[i].size(); j++){
			a[i].push_back({});
			prjct(O, (*R)[i][j][0].Des, (*R)[i][j][0].XP, (*R)[i][j][0].YP, (*R)[i][j][0].Org,f);
			vector<vector<int>> g=trgX(&f,O);
			/*if(i==20 & j==0){
				char *n="111.obj";
    				char *m="112.obj";
    				intvec l;
    				for(int z=0; z<g.size(); z++){
    				l.push_back(O.OF[g[z][0]][g[z][1]][0]);
    				l.push_back(O.OF[g[z][0]][g[z][1]][1]);
    				l.push_back(O.OF[g[z][0]][g[z][1]][2]);
    				}
    				PtoO(n,f);
    				PtoOR(m,f,l);
				//O.temp=g;
				//tat=f;
				//cout<<R[i][j][0].Des[0]<<" "<<R[i][j][0].Des[1]<<" "<<R[i][j][0].Des[2]<<endl;
				//cout<<R[i][j][0].Org[0]<<" "<<R[i][j][0].Org[1]<<" "<<R[i][j][0].Org[2]<<endl;
			}*/
			for (int k=0; k<(*R)[i][j].size(); k++){
				vector<vector<int>> h=trgY(&f,O,&g,(*R)[i][j][k].Des[2]-(*R)[i][j][0].Des[2]);
				//cout<<"a"<<g.size();
				//cout<<"b"<<h.size()<<endl;
				(*O).ref=h;
				d=fm1(O,&(*R)[i][j][k],15);
				//cout<<d<<endl;
				a[i][j].push_back(d);
			}
		}
	}
	for (int i=0; i<a[0][0].size(); i++){
		e.push_back({});
		for (int j=0; j<a.size(); j++){
			e[i].push_back({});
			//cout<<"j "<<a.size()<<endl;
			temp=0;
			for (int k=0; k<a[0].size(); k++){
				temp=temp+a[j][k][i];
			}
			c=floor(256*(pow(0.5,temp/IH)));
			b=c;
			//cout<<"b"<<temp<<" "<<i<<","<<j<<endl;
			for (int k=0; k<3; k++){
				e[i][j].push_back(b);
			}
		}
	}
	return e;
}

int main(){
	using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();
P p;
    p.radius=2;
    p.res=128;
    p.c2wr=1;
    p.c2hr=64;
    p.c2h=3;
    p.c2o=-0.5;
    p.fo=0;
    p.c1hr=3;
    p.c1h=0.1;
    int temp=20000;
    char *c="12.obj";
    char*b="plott.bmp";
    object j=objtovec(c);
    vector<vector<vector<ray>>> R=PI(p);
	coco a=rndrp(&R,&j);
    auto t2 = high_resolution_clock::now();
	duration<double, std::milli> ms_double = t2 - t1;
	std::cout << ms_double.count() << "ms"<<endl;
    int d=plotbmp(&a,b);
    /*for (int i=6; i<8; i++){
    	for (int j=0; j<10; j++){
    		temp++;
    		string cc;
    		cc.append("./1/");
    		cc.append(to_string(i));
    		cc.append("-");
    		cc.append(to_string(j));
    		cc.append(".obj");
    		char*c = &cc[0];
    		string bb;
    		bb.append("./2/");
    		bb.append(to_string(temp));
    		bb.append(".bmp");
    		char*b=&bb[0];
    		object k=objtovec(c);
    		vector<vector<vector<ray>>> R=PI(p);
    		coco a=rndrp(&R,&k);
    		int d=plotbmp(&a,b);
    		cout<<cc<<endl;
    	}
    }
	auto t2 = high_resolution_clock::now();
	duration<double, std::milli> ms_double = t2 - t1;
	std::cout << ms_double.count() << "ms"<<endl;*/
    //cout<<"t"<<j.temp.size()<<" "<<j.temp1.size()<<endl;
    //intvec l;
    //for(int i=0; i<j.temp.size(); i++){
    	//l.push_back(j.OF[j.temp[i][0]][j.temp[i][1]][0]);
    	//l.push_back(j.OF[j.temp[i][0]][j.temp[i][1]][1]);
    	//l.push_back(j.OF[j.temp[i][0]][j.temp[i][1]][2]);
    //}
    //PtoO(f,a);
    //PtoOR(g,j.temp1,l);
    
    /*vector<vector<ray>> R;
    for (float i=0; i<60; i++){
    	R.push_back({});
    	for (float j=0; j<60; j++){
    		ray q;
    		cout<<(i*1/15)<<" "<<(j*1/15)<<endl;
    		q.Org={(i*1/15)-2,4,(j*1/15)-2};
    		q.Des={(i*1/15)-2,-4,(j*1/15)-2};
    		R[i].push_back(q);
    		
    	}
    }
    coco a =rndr(R,j);
    int d=plotbmp(a,b);*/
    /*
    ray q;
    q.Org={0.2,3,0.7};
    q.Des={0.2,-3,0.7};
    vvec a=getattun(j,q);
    uint8_t d = fm(j,q,1);
    int h=d;
    cout<<"aaa"<<h<<endl;
    for (int i=0; i<a.size(); i++){
	for (int j=0; j<a[i].size(); j++){
		cout<<a[i][j]<<" ";
	}
	cout<<endl;
    }*/
    return 0;
}
