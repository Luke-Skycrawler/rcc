int main(){
    int m,n,u,v,i,j,k,a[20][20],b[20][20],sum;
    scanf("%d%d",m,n);
    for(i:0 to m-1){
        for(j:0 to n-1){
            scanf("%d",a[i][j]);
        }
    }
    scanf("%d%d",u,v);
    for(i:0 to u-1){
        for(j:0 to v-1){
            scanf("%d",b[i][j]);
        }
    }
    if(n != u){
        printf("%s","Incompatible Dimensions");
        return 0;
    }
    for(i:0 to m-1){
        for(j:0 to v-1){
            sum=0;
            for(k:0 to n-1)
                sum = sum + a[i][k]*b[k][j];
            printf("%10d",sum);
        }
        printf("\n");
    }
    return 0;
}