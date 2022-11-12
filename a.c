#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
void bitonic_seq_generate(int *a, int n, int up)
{
if (n > 1)
{
int m = n / 2;
for (int i = 0; i < m; i++)
{
if (up == (a[i] > a[i + m]))
{
int temp = a[i];
a[i] = a[i + m];
a[i + m] = temp;
}
}
// parallelize the recursive calls
#pragma omp task
bitonic_seq_generate(a, m, up);
#pragma omp task
bitonic_seq_generate(a + m, m, up);
#pragma omp taskwait
}
}
void bitonic_sort(int *a, int n, int up)
{
if (n > 1)
{
int m = n / 2;
#pragma omp task
bitonic_sort(a, m, 1);
#pragma omp task
bitonic_sort(a + m, m, 0);
#pragma omp taskwait
bitonic_seq_generate(a, n, up);
}

}
// bitonic sort starts with master thread then broken into individual threads
void printArray(int *a, int n)
{
for (int i = 0; i < n; i++)
printf("%d ", a[i]);
printf("\n");
}
int main()
{
int a[] = {4,1,5,7,6,8,1,2};
int n = 8;
printArray(a, n);
bitonic_sort(a, n, 1);
printf("\nSorted array: ");
printArray(a, n);
return 0;
}
