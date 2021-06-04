#include <stdio.h>
int main(){
    int m,n,u,v,i,j,k,a[20][20],b[20][20],sum;
    scanf("%d%d",&m,&n);
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            scanf("%d",&a[i][j]);
        }
    }
    scanf("%d%d",&u,&v);
    for(i=0;i<u;i++){
        for(j=0;j<v;j++){
            scanf("%d",&b[i][j]);
        }
    }
    if(n != u){
        printf("%s","Incompatible Dimensions");
        return 0;
    }
    for(i=0;i<m;i++){
        for(j=0;j<v;j++){
            sum=0;
            for(k=0;k<n;k++)
                sum += a[i][k]*b[k][j];
            printf("%10d",sum);
        }
        printf("\n");
    }
    return 0;
}