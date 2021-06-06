// #include <stdio.h>
int a[10005];
int sort(int left, int right)
{
    int i = left;
    int j = right;
    int key = a[left],tmp;

    // printf("left: %d, right: %d, key: %d\n",i,j,key);
    if(left >= right)
    {
        return 0;
    }
    while(i < j)
    {
        while(i < j & key <= a[j])
        {
            j=j - 1;
        }
        tmp=a[i];
        a[i]=a[j];
        a[j]=tmp;

        while(i < j & key >= a[i])
        {
            i=i + 1;
        }
        tmp=a[i];
        a[i]=a[j];
        a[j]=tmp;
    }
    a[i] = key;
    sort(left, i - 1);
    sort(i + 1, right);
    return 0;
}

int main(){
    int k,n;
    scanf("%d",n);
    for(k:0 to n-1){
        scanf("%d",a[k]);
    }
    sort(0,n-1);
    for(k:0 to n-1){
        printf("%d\n",a[k]);
    }
    return 0;
}

