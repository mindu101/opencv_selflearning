#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
typedef struct student{
    int a[7];
    string name;
    int p;
    
}S;
int main(){
    int max=-1;
    int N,max_index;
    S b[15];
    scanf("%d",&N);
    for (int i=0;i<N;i++){
        for (int j=0;j<5;j++){
            scanf("%d",&(b[i].a[j]));
        }
        cin>>b[i].name;
        int j,k,l;
        scanf ("%d %d %d",&j,&k,&l);
        b[i].p=j+k+l;
        if (b[i].p>max){
            max_index=b[i].p;
            max=b[i].p;
        }
    }
    cout<<b[max_index].name<<" "<<b[max_index].a<<" "<<b[max_index].p;
    return 0;
}