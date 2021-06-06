#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
// vector<string> courses;
string courses[50];
static int bit[50],size_l1[50],size_l2[50][3];
string prerequisites[50][5][5];
// vector<vector<vector<string>>> prerequisites;
int cnt=0,tot_credit=0,tot_score=0,attempt_credit=0,left_credit=0;
int read(){
    char c,tmp1,tmp2;
    int i=0,j,credit,t;
    t=scanf("%c",&c);
    if(t==EOF)return 0;
    while(c=='\n')scanf("%c",&c);
    while(c!='|'){
        courses[cnt]+=c;
        scanf("%c",&c);    
    }
    // courses[cnt]+='\0';
    scanf("%d",&credit);
    tot_credit+=credit;

    scanf("%c",&c);
    scanf("%c",&c);
    i=j=0;
    while(c!='|'){
        switch (c){
            case ';':/*prerequisites[cnt][i++][j]+='\0';*/size_l2[cnt][i++]=j;break;
            case ',':/*prerequisites[cnt][i][j++]+='\0';*/j++;break;
            default:prerequisites[cnt][i][j]+=c;
        }
        scanf("%c",&c);
    }
    size_l1[cnt]=i;
    size_l2[cnt][i]=j;
    t=scanf("%c",&c);
    if(t==-1){
        left_credit+=credit;
        bit[cnt++]=0;
        return 1;
    }
    switch(c){
        case 'A':case 'B':case 'C':case 'D':
            tot_score+=credit*(4-(c-'A'));
            attempt_credit+=credit;
            bit[cnt]=1;
            scanf("%c",&c);
            break;
        case 'F':
            attempt_credit+=credit;
        default:
            left_credit+=credit;
            bit[cnt]=0;
    }
    // printf("end of line %d",cnt);
    cnt++;
    return 0;
}
int main(void){
    while(read()==0){}
    printf("GPA: %.1f\n",(float)tot_score/(tot_credit-left_credit));
    printf("Hours Attempted: %d\n",attempt_credit);
    printf("Hours Completed: %d\n",tot_credit-left_credit);
    printf("Credits Remaining: %d\n\n",left_credit);
    printf("Possible Courses to Take Next\n");
    for(int i=0;i<cnt;i++)
        if(!bit[i]){        // failed classes
            if(int n=size_l1[i]){
                for(int j=0;j<n;j++){
                    int k;
                    if(size_l2[i][j]==0)continue;
                    for(k=0;k<size_l2[i][j];k++){
                        int u;
                        for(u=0;u<cnt;u++)if(courses[u]==prerequisites[i][j][k])
                            break;
                        if(bit[u])continue; // not trimmed
                            else break;     // branch not taken
                    }
                    if(k==size_l2[i][j]){
                        printf("  %s\n",courses[i].data()); // end of the branch, qualified
                        break;
                    }
                }
            }
            else printf("  %s\n",courses[i].data());
        }
    return 0;
}