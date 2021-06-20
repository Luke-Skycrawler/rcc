int main()
{
    int x, y, z; // int declaration
    char a = 'c'; // char declaration
    double d = -3.14, e; // double declaration
                         // error: unary expression like -3.14 not supported! (but will continue)
    int array1[30]; // an array
    int array2[30][30][30]; // 3D array
    3 + 4; // int + int, OK
    3.14 + 4; // double + int, OK
    d + 4; // error, but will continue
    3.14 + 4.14; // double + double, OK
    z + 3; // int + int, OK
    a + 3; // char + int, OK
    x + y; // int + int, OK
    x | 0; // int | int, OK
}