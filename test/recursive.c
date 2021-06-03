int fib(int k){
    if(k == 0 || k==1){
        return 1;
    }
    return fib(k-1)+fib(k-2);
}
int main(){
    printf("%d",fib(5));
    return 0;
}