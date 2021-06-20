#define X 100
#define X 20
#define gnu gnu is not unix
#ifdef gnu
int main(){
    printf("%d",X);
    #ifndef X
    printf("ndef\n");
    #else
    printf("X defined");
    #endif
    return 0;
}
#else
int main(){
    printf("else\n");
}
#endif