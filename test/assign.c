int main(){
    int x=0;
    double y;
    y=0.0;
    x+=2;
    printf("%d ",x);
    x-=6;
    printf("%d ",x);
    x*=3;
    printf("%d ",x);
    x/=4;
    printf("%d ",x);
    x|=7;
    printf("%d ",x);
    x&=10;
    printf("%d ",x);
    x%=-3; // unary expr is not supported, so an error would be triggered; try "0 - 3" instead...
    printf("%d ",x);

    y+=2.0;
    printf("%f ",x);
    y-=6.0;
    printf("%f ",x);
    y*=3.0;
    printf("%f ",x);
    y/=4.0;
    printf("%f ",x);
    return 0;
}