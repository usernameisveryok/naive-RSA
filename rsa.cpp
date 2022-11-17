#include "rsa.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <iomanip>
using namespace std;
#include <random>
#include <limits>
std::random_device rd;
std::mt19937_64 eng(rd());
std::uniform_int_distribution<unsigned int> distr;
LL mulmod(LL a, LL b, LL mod)
{
    LL x = 0, y = a % mod;
    while (b > 0)
    {
        if (b % 2 == 1)
        {
            x = (x + y) % mod;
        }
        y = (y * 2) % mod;
        b /= 2;
    }
    return x % mod;
}
LL ext_euclid(long long a, long long b, long long &x, long long &y)
{

    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }
    LL ret = ext_euclid(b, a % b, x, y);
    LL tmp = x;
    x = y;
    y = (tmp - a * y / b);
    return ret;
}
LL modular_exponentitation(LL a, LL b, LL n)
{
    LL ans = 1;
    while (b)
    {
        if (b & 1)
            ans = mulmod(a, ans, n);
        ans %= n;
        a = mulmod(a, a, n);
        b >>= 1;
    }
    return ans;
}
LL exponentitation(LL a, LL b)
{
    LL ans = 1;
    while (b)
    {
        if (b & 1)
            ans *= a;
    }
    a *= a;
    b >>= 1;
    return ans;
}
bool is_prime(LL n)
{

    for (LL i = 2; i < n; i++)
    {
        if (n % i == 0)
        {
            return false;
        }
    }
    return true;
}
LL euler_fun(LL a)
{
    vector<LL> prime;
    LL i = 1;
    for (LL i = 2; i < 1001; i++)
    {
        if (is_prime(i))
        {
            prime.push_back(i);
        }
    }
    LL ans = 1;
    for (auto &&i : prime)
    {
        LL ex = 0;
        while (a % i == 0)
        {
            a /= i;
            ex++;
        }
        if (ex > 0)
        {
            ans *= (LL)pow(i, ex - 1);
            ans *= (i - 1);
        }
        if (a == 1)
        {
            break;
        }
    }
    if (a != 1)
    {
        ans *= (a - 1);
    }
    return ans;
}

bool Miller(LL p, int iteration)
{
    if (p < 2)
    {
        return false;
    }
    if (p != 2 && p % 2 == 0)
    {
        return false;
    }

    LL s = p - 1;
    while (s % 2 == 0)
    {
        s /= 2;
    }
    for (int i = 0; i < iteration; i++)
    {
        LL a = rand() % (p - 1) + 1, temp = s;
        LL mod = modular_exponentitation(a, temp, p);
        while (temp != p - 1 && mod != 1 && mod != p - 1)
        {
            mod = mulmod(mod, mod, p);
            temp *= 2;
        }
        if (mod != p - 1 && temp % 2 == 0)
        {
            return false;
        }
    }
    return true;
}
void give_Prime(LL &p)
{
    p = 1;
    while (p < 65536 || p > UINT32_MAX || !Miller(p, 10))
    {
        p = distr(eng);
        if (p % 2 == 0)
            p++;
    }
}
void key_gen(LL &p, LL &q, LL &phi, LL &e, LL &n, LL &k)
{

    cout << "key-generating\n";
    do
    {
        give_Prime(p);
        give_Prime(q);
        n = p * q;
        phi = (p - 1) * (q - 1);
        e = 13;
        long long tmp;
        long long kk;
        ext_euclid(e, phi, kk, tmp);
        k = (kk + phi) % phi;
    } while ((e * k) % phi != 1);

    cout << setiosflags(ios::uppercase) << hex
         << "p:  " << setfill('0') << setw(16) << p << '\n'
         << "q:  " << setfill('0') << setw(16) << q << '\n'
         << "phi:" << setfill('0') << setw(16) << phi << '\n'
         << "k:  " << setfill('0') << setw(16) << k << '\n'
         << "n:  " << setfill('0') << setw(16) << n << '\n'
         << "e:  " << setfill('0') << setw(16) << e << '\n';
}
void change_key(LL &p, LL &q, LL &phi, LL &e, LL &n, LL &k)
{
    cout << "enter p(hex)\n";
    cin.setf(ios_base::hex, ios_base::basefield);
    cin >> p;
    if (!is_prime(p))
    {
        cerr << "p is not a prime!";
        return;
    }
    cout << "enter q(hex)\n";
    cin >> q;
    if (!is_prime(q))
    {
        cerr << "q is not a prime!";
        return;
    }
    cout << "enter e(hex)\n";
    cin >> e;
    if (!is_prime(e))
    {
        cerr << "e is not a prime!";
        return;
    }
    long long tmp;
    long long kk;
    ext_euclid(e, phi, kk, tmp);
    k = (kk + phi) % phi;
    if ((e * k) % phi != 1)
    {
        cerr << "runtime error";
        return;
    }
    cout << setiosflags(ios::uppercase) << hex << "change sucess\n"
         << "p:  " << setfill('0') << setw(16) << p << '\n'
         << "q:  " << setfill('0') << setw(16) << q << '\n'
         << "phi:" << setfill('0') << setw(16) << phi << '\n'
         << "k:  " << setfill('0') << setw(16) << k << '\n'
         << "n:  " << setfill('0') << setw(16) << n << '\n'
         << "e:  " << setfill('0') << setw(16) << e << '\n';
}
LL encrypt(string s, LL e, LL n)
{
    int i = 0;
    LL plain = 0;
    string out;
    for (int i = 3; i >= 0; i--)
    {
        LL tmp = s[i];
        tmp <<= 8 * i;
        plain |= tmp;
    }
    LL cipher = modular_exponentitation(plain, e, n);

    return cipher;
}
string decrypt(LL c, LL k, LL n)
{
    LL mask = 0xFF000000;
    LL cipher = modular_exponentitation(c, k, n);
    string out;
    for (int i = 3; i >= 0; i--)
    {
        out.push_back((char)((cipher & (mask >> 8 * i)) >> 8 * (3 - i)));
    }
    return out;
}
vector<LL> encrypt_all(string a, LL e, LL n)
{
    vector<LL> out;

    // padding '0'

    int remain = a.size() % 4;
    if (remain != 0)
    {

        for (int i = 0; i < (4 - remain); i++)
        {
            a.push_back(' ');
        }
    }
    for (int i = 4; i <= a.size(); i += 4)
    {
        out.push_back(encrypt(a.substr(i - 4, 4), e, n));
    }
    return out;
}
string decrypt_all(vector<LL> c, LL k, LL n)
{
    string p;
    for (auto &&i : c)
    {
        p += decrypt(i, k, n);
    }
    return p;
}
void print_cipher(vector<LL> a)
{
    cout << "cipher:";
    for (auto &&i : a)
    {
        cout << setfill('0') << setw(16) << setiosflags(ios::uppercase) << hex << i;
    }
    cout << '\n';
}
int main()
{
    LL a, b;
    long long x, y;
    LL p, q, e, phi, n, k;

    // cout << euler_fun(9987) << '\n';
    // ofstream ofile("./out.txt");

    // for (size_t i = 100; i < 10000; i++)
    // {
    //     ofile << i << " " << euler_fun(i) << '\n';
    // };
    // ofile.close();
    int flag = 1;

    do
    {
        cout << "0:exit\n1:auto\n2:manual\n";
        cin >> flag;
        if (flag == 1)
        {
            key_gen(p, q, phi, e, n, k);
        }
        else
        {
            change_key(p, q, phi, e, n, k);
        }
        cout << "please enter plain text" << endl;
        string plain;
        cin >> plain;
        auto cc = encrypt_all(plain, e, n);
        print_cipher(cc);
        cout << "decrypted:\n"
             << decrypt_all(cc, k, n) << endl;
    } while (flag);

    return 0;
}