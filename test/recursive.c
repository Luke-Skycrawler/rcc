// #include <stdio.h>
int fib(int k){
    int t;
    printf("k = %d\n", k);
    if(k == 0 | k == 1){
        return 1;
    }
    t= fib(k - 1) + fib(k - 2);
    // printf("f(%d)=%d\n",k,t);
    return t;
}
int main(){
    int k = 5;
    printf("k = %d\n", k);
    printf("fib(5) = %d\n", fib(5));
    return 0;
}