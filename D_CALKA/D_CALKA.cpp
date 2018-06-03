/**
 * D_CALKA_Niedoba_Dariusz
 */

#include <cmath>
#include <iostream>
#include <tuple>
#include <functional>
#include <map>

double f(double x)
{
    return std::sin(x);
}

double SumTrapeze(double a, int m, double h)
{
    double s = 0;
    for (int i = 1; i < m; ++i)
        s += f(a + i * h);
    return s;
}

std::tuple<double, int> TrapezeMethod(double e, double a, double b)
{
    int k = 0;
    int m = 1;
    double Tp = 0;
    double T = 0;
    double h = b - a;

    do
    {
        std::swap(Tp, T);

        T = (h / 2) * (f(a) + f(b) + 2 * SumTrapeze(a, m, h));
        h /= 2;
        m *= 2;
        k++;
    } while (std::fabs(T - Tp) >= e);

    return std::make_tuple(T, k);
}

double SumSimpson(double a, int m, double h)
{
    double s1 = 0;
    double s2 = 0;
    for (int i = 0; i < m; i += 2)
    {
        s1 += f(a + i * h);
        s2 += f(a + (i + 1) * h);
    }
    return 4.0 * s1 + 2.0 * s2;
}

std::tuple<double, int> SimpsonMethod(double e, double a, double b)
{
    int k = 1;
    int m = 2;
    double Sp = 0;
    double S = 0;
    double h = (b - a) / m;

    do
    {
        std::swap(Sp, S);

        S = (h / 3.0) * (f(a) + f(a + m * h) + SumSimpson(a, m, h));
        h /= 2;
        m *= 2;
        k++;
    } while (std::fabs(S - Sp) >= e);

    return std::make_tuple(S, k);

}

double SumRomberg(double a, int k, double h)
{
    double s = 0;
    for (int i = 1; i <= std::pow(2.0f, k - 1); i++)
        s += f(a + (2 * i - 1) * h);

    return s;
}

std::tuple<double, int> RombergMethod(double e, double a, double b)
{
    int k = 2;
    double Rp = 0;
    double R = 0.5 * (f(a) + f(b)) * (b - a);
    double h = (b - a) / 4;

    do
    {
        std::swap(Rp, R);

        R = 0.5 * Rp + h * SumRomberg(a, k, h);
        h /= 2;
        k++;
    } while (std::fabs(R - Rp) >= e);

    return std::make_tuple(R, k);
}

int main()
{
    const double PI = 3.14159265359;
    const double a = 0;
    const double b = PI / 2.0;
    const double e = 10e-5;

    std::map<std::string, std::function<std::tuple<double, int>(double, double, double)>> methods{
        {"Metoda trapezow", TrapezeMethod},
        {"Metoda Simpsona", SimpsonMethod},
        {"Metoda Romberga", RombergMethod}
    };

    for (const auto& pair : methods)
    {
        auto t = pair.second(e, a, b);
        std::cout << pair.first << "\t|\t" << std::get<0>(t) << "\t|\t" << std::get<1>(t) << " iteracji\n";
    }

    return 0;
}