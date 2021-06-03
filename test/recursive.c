int fib(int k){
    if(k == 0 | k==1){  // bugs: 1. change | to ||
        return 1;
    }
    return fib(k- 1)+fib(k- 2); // bugs 2: delete space before 1 or 2
}
int main(){
    printf("%d",fib(5));
    return 0;
}