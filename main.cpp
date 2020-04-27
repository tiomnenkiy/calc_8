#include <iostream>
#include <limits>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>

using namespace std;

class Eint {
    char* str;
    bool minus;
    int size;
public:
    explicit Eint () { //ready
        str = new char[100];
        minus = 0;
        size = 0;
    }
    explicit Eint (char* sr) : str(sr) { //ready
        size = strlen(sr);
        minus = 0;
        if (sr[0] == '-') {
            ++sr;
            minus = 1;
            --size;
        }
        str = new char[size];
        strcpy(str, sr);
    }
    explicit Eint (char* sr, bool ms, int sz) : str(sr), minus(ms), size(sz) {
        size = sz;
        str = new char[size];
        strcpy(str, sr);
        minus = ms;
    }
    Eint operator ++ () {
        int count = size-1; //!!!
        bool ready = 1;
        while (ready) {
            if (minus) {
                if (str[count] == '0') {
                    str[count] = '7';
                    --count;
                } else {
                    str[count] = str[count]-1;
                    ready = 0;
                }
            } else {
                if (count == -1) {
                    str[0] = '1';
                    count = strlen(str);
                    str[count] = '0';
                    ++count;
                    str[count] = '\0';
                    ready = 0;
                } else if (str[count] == '7') {
                    str[count] = '0';
                    --count;
                } else {
                    str[count] = str[count]+1;
                    ready = 0;
                }
            }
        }
        if (str[0] == '0' && size == 1 && minus)
            minus = 0;
        if (str[0] == '0' && size > 1) {
            int i = 0;
            while(str[i] != '\0') {
                str[i] = str[i+1];
                ++i;
            }
        }
        if (str[0] == '-' && strlen(str) == 2) {
            str[0] = '0';
        }
        size = strlen(str);
        return Eint(str, minus, size);
    }

    char* strrev (char* s) { //ok
        int size = strlen(s);
        char* rev = new char[size];
        int i = 0;
        if (s[0] == '-') {
            rev[i] = '-';
            ++i;
            while (i < size) { //cместим, чтобы минус не оказался в конце
                rev[i] = s[size - (i)];
                ++i;
            }
        } else {
            while (i < size) {
                rev[i] = s[size - (i+1)];
                ++i;
            }
        }
        rev[size] = '\0';
        return rev;
    }

    char* OctToDec(char *s) { //ok
        int size = strlen(s);
        char* temp = new char[size];
        int num = atoi(s);
        int rez = 0;
        int i = 0;
        while(num != 0) { // перевод в нужную сс
            rez += (num%10)*pow(8, i);
            num/=10;
            ++i;
        }
        i = 0;
        while(rez!=0) { //перевод в строку
            temp[i] = rez%10 + '0';
            rez /= 10;
            ++i;
        }
        temp[i] = '\0';
        return strrev(temp);
    }

    char* DecToOct(char* s) { //ok
        int size = strlen(s);
        char* temp = new char[size];
        int num = atoi(s);

        if (num == 0) {
            temp[0] = '0';
            temp[1] = '\0';
            return temp;
        }

        int rez = 0;
        int i = 0;
        while(num != 0) { //перевод в нужную сс
            rez += (num%8)*pow(10, i);
            num/=8;
            ++i;
        }

        i = 0;
        if (rez < 0) { //перевод в строку
            temp[i] = '-';
            ++i;
        }
        while(rez!=0) {
            temp[i] = abs(rez%10) + '0';
            rez /= 10;
            ++i;
        }
        temp[i] = '\0';
        return strrev(temp);
    }

    Eint operator + (const Eint& c) { //в десятиную и обратно
        int num1, num2, sum;
        char* temp = new char[100];
        num1 = atoi(OctToDec(str));
        if (minus)
            num1*=-1;
        num2 = atoi(OctToDec(c.str));
        if (c.minus)
            num2*=-1;
        sum = num1 + num2;

        int i = 0;  // переводим в строку
        if (sum < 0) {
            temp[i] = '-';
            ++i;
        }
        if (sum == 0) {
            temp[i] = '0';
            ++i;
        } else {
            while (sum != 0) {
                temp[i] = abs(sum%10) + '0';
                sum/=10;
                ++i;
            }
        }
        temp[i] = '\0';
        return Eint(DecToOct(strrev(temp))); // норм, т.к. конструктор преобразует минус в bool
    }

    Eint operator += (const Eint& c) { // в десятичную и обратно
        int num1, num2;
        num1 = atoi(OctToDec(str));
        if (minus)
            num1*=-1;
        num2 = atoi(OctToDec(c.str));
        if (c.minus)
            num2*=-1;
        num1+=num2;
        int i = 0;  // переводим в строку
        delete str;
        str = new char[100];
        if (num1 < 0) {
            str[i] = '-';
            ++i;
        }
        if (num1 == 0) {
            str[i] = '0';
            ++i;
        } else {
            while (num1 != 0) {
                str[i] = abs(num1%10) + '0';
                num1/=10;
                ++i;
            }
        }
        str[i] = '\0';
        str = DecToOct(strrev(str));
        size = strlen(str);
        if (str[0] == '-') {
            minus = 1;
            ++str;
            --size;
        }
        return Eint(str); // норм, т.к. конструктор преобразует минус в bool
    }

    friend ostream& operator << (ostream& output, const Eint& c);
    friend istream& operator >> (istream& intput, Eint& c);
};

ostream& operator << (ostream& output, const Eint& c) {
    int i = 0;
    if (c.minus) {
        putchar('-');
    }
    while(c.str[i] != '\0') {
        putchar(c.str[i]);
        ++i;
    }
    return output;
}

istream& operator >> (istream& input, Eint& c) {
    char sym = 'a';
    bool kek = 0;
    sym = getchar();
    if (sym == '-') {
        c.minus = 1;
        sym = getchar();
    }
    while (sym != '\n') {
        if (sym >= '0' && sym < '8') {
            c.str[c.size] = sym;
            ++c.size;
        } else {
            kek = 1;
            break;
        }
        sym = getchar();
    }
    if (c.minus && c.str[0] == '0')
        kek = 1;
    if (kek) {
        printf("Vvedite eshe raz\n");
        delete c.str;
        c.str = new char[100];
        c.minus = 0;
        c.size = 0;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return cin >> c;
    }
    c.str[c.size] = '\0';
    return input;
}

int main() {
    Eint kek, prekek;
    cout << "Input kek:\n"; //126
    cin >> kek;
    cout << "Input prekek:\n"; //456
    cin >> prekek;
    cout << "++kek: " << ++kek << endl;
    cout << "prekek+kek+1: " << prekek+kek << endl;
    prekek+=kek;
    cout << "prekek+=(kek+1): " << prekek << endl;
    return 0;
}