#include <ctime>
#include <cstdio>

class TimeProfiler
{
public:
    TimeProfiler(const char *funcName = "anonymous")
        : name(funcName)
    {
        printf("Function %s started\n", name);
        fflush(stdout);

        start = clock();
    }

    ~TimeProfiler()
    {
        clock_t finish = clock();

        printf("Function %s finished at %.2f sec.\n",
               name, (double)(finish - start) / CLOCKS_PER_SEC);
        fflush(stdout);
    }

private:
    const char *name;
    clock_t start;
};

int fibbonacci(int n)
{
    if (n == 0 || n == 1)
        return n;

    return fibbonacci(n - 1) + fibbonacci(n - 2);
}

void fibbonacciLauncher(int n)
{
    TimeProfiler tp;

    printf("F(%d) = %d\n", n, fibbonacci(n));
}

int main()
{
    fibbonacciLauncher(38);

    return 0;
}
