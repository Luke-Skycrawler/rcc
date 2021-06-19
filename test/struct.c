struct A{
    int x;
    double y;
    char z;
};
struct B{
    int a;
    double b;
    char c;
    struct A d;
    int array[10][20];
};
struct B fool;

int main()
{
    fool.a = 123;
    fool.b = 3.14;
    fool.c = 'h';
    fool.d.x = 233;
    fool.d.y = 6.66;
    fool.d.z = 'i';
    fool.array[3][12] = 321;
    printf("fool.a = %d\n", fool.a);
    printf("fool.b = %f\n", fool.b);
    printf("fool.c = %c\n", fool.c);
    printf("fool.d.x = %d\n", fool.d.x);
    printf("fool.d.y = %f\n", fool.d.y);
    printf("fool.d.z = %c\n", fool.d.z);
    printf("fool.array[3][12] = %d\n", fool.array[3][12]);
}