#ifndef RANDOMNUMBER_H
#define RANDOMNUMBER_H
#include <random>
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;

class math{
    public:
    static int gcd(int n,int m){
        if(!m)return n;
        else return gcd(m,n%m);
    }
};
class RandomNumber{
    public:
        int aval_num[101];
        int total;
        int difficulty;
        int amount;
        RandomNumber(int k=15):difficulty(k),amount(0){
            srand(time(NULL));
            int tmp[111];
            memset(tmp,0,sizeof(tmp));
            for(int i=2;i<=10;++i){
                for(int j=i;j*i<=100;++j){
                    tmp[i*j]++;
                }
            }
            for(int i=difficulty+1;i<=100;++i){
                if(tmp[i]==0){
                    for(int j=1;j*i<=100;++j){
                        tmp[i*j]=0;
                    }
                }
            }
            int num=0;
            for(int i=1;i<=100;++i){
                if(tmp[i]||i<=k){
                    aval_num[num]=i;
                    num++;
                }
            }
            total=num;
        }
        void change_difficulty(int k){
            difficulty=k;
            int tmp[111];
            memset(tmp,0,sizeof(tmp));
            for(int i=2;i<=10;++i){
                for(int j=i;j*i<=100;++j){
                    tmp[i*j]++;
                }
            }
            for(int i=difficulty+1;i<=100;++i){
                if(tmp[i]==0){
                    for(int j=1;j*i<=100;++j){
                        tmp[i*j]=0;
                    }
                }
            }
            int num=0;
            memset(aval_num,0,sizeof(aval_num));
            for(int i=1;i<=100;++i){
                if(tmp[i]||i<=k){
                    aval_num[num]=i;
                    num++;
                }
            }
            total=num;
        }
        int get(int num[]){
            int score=amount;
            int try_time=0;int sample=2;int limit_size=10;
            limit_size=min(10+(score*difficulty)/20,total);
            for(;try_time<10;++try_time){
                sample=aval_num[rand()%(limit_size)];
                int estimate_score=0;
                for(int i=0;i<=9;++i){
                    if(num[i]==0){
                        estimate_score+=1;
                    }
                    else if(sample%num[i]==0||num[i]%sample==0){
                        estimate_score+=2;
                    }
                    else if(math::gcd(num[i],sample)!=1){
                        estimate_score+=1;
                    }
                }
                estimate_score+=try_time;
                int p=rand()%(10+difficulty);
                if(p<estimate_score){
                    break;
                }
            }
            if (sample==1)
                return sample=get(num);
            amount++;
            return sample;
        }
};
#endif // RANDOMNUMBER_H
