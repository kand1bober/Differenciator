

#include <stdio.h>
#include <stdlib.h>

void func( int** ptr);

int main()
{
    int* ptr = (int*)calloc( 10, sizeof(int) );

    func( &ptr );

    return 0;
}

void func( int** ptr)
{
    printf("%p\n", *ptr);

    free( *ptr );

    printf("%p\n", *ptr);
}