#include <stdio.h>
int a[100];
int sort(int left, int right)
{
    int i = left;
    int j = right;
    int key = a[left],tmp;
    if(left >= right)
    {
        return 0;
    }
     
    while(i < j)
    {
        while(i < j && key <= a[j])
        {
            j--;
        }
         
        tmp=a[i];
        a[i]=a[j];
        a[j]=tmp;

        while(i < j && key >= a[i])
        {
            i++;
        }
        tmp=a[i];
        a[i]=a[j];
        a[j]=tmp;
    }
     
    a[i] = key;
    sort(left, i - 1);
    sort(i + 1, right);
}
int main(){
    int i,n;
    scanf("%d",&n);
    for(i=0;i<n;i++)scanf("%d",&a[i]);
    sort(0,n-1);
    for(i=0;i<n;i++)printf("%d\n",a[i]);
    return 0;
}