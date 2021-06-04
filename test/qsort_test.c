int sort(int left, int right);
int main(){
    int k,n;
    int a[100];
    scanf("%d",n);
    for(k:0 to n){
        scanf("%d",a[k]);
    }
    sort(0,n-1);
    for(k:0 to n){
        printf("%d\n",a[k]);
    }
    return 0;
}

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
    return 0;
}
