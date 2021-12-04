#pragma once
#include "mpir.h"
#include "mpirxx.h"
#include <iostream>
#include <fstream>
#include "windows.h"
#include <iomanip>
#include "time.h"
#include <string>
#include "locale.h"
#include <random>
using namespace std;

std::random_device rd;
ofstream out;
mpz_class pA;
mpz_class qA;
mpz_class nA; mpz_class nB;
mpz_class bA; mpz_class bB;
int c1 = 0; int c2 = 0;
int coun = 0;

int* randomNum(int len)
{
	int* number = new int[len];
	for (int i = 0; i < len; i++)
	{
		number[i] = 0;
	}
	mt19937 mersenne{ static_cast<mt19937::result_type>(time(nullptr)) };
	mersenne.seed(rd());
	uniform_int_distribution<> rand_val{ 1, 100 };
	for (int i = 0; i < len; i++)
	{
		number[i] = rand_val(mersenne) % 16;
	}
	number[0] = 1 + rand_val(mersenne) % 15;
	return number;
}

int stringTo16(char letter)
{
	switch (letter)
	{
	case '0': return 0;
	case '1':return 1;
	case '2':return 2;
	case '3':return 3;
	case '4':return 4;
	case '5':return 5;
	case '6':return 6;
	case '7':return 7;
	case '8':return 8;
	case '9':return 9;
	case 'A':return 10;
	case 'a':return 10;
	case 'B':return 11;
	case 'b':return 11;
	case 'C':return 12;
	case 'c':return 12;
	case 'D':return 13;
	case 'd':return 13;
	case 'E':return 14;
	case 'e':return 14;
	case 'F':return 15;
	case 'f':return 15;
	}
	return -1;
}
int* strToArr(string num, int len)
{
	int* arr = new int[len];
	for (int i = 0; i < len; i++)
	{
		arr[i] = 0;
	}
	for (int i = 0; i < num.length(); i++)
	{
		arr[i] = stringTo16(num[i]);
	}
	return arr;
}
string arrToStr(int* arr, int len)
{
	string init_number;
	for (int i = 0; i < len; i++)
	{
		if (arr[i] < 10)
			init_number += (char)(arr[i] + 48);
		else
			init_number += (char)(arr[i] + 55);
	}
	return init_number;
}
string initialString(int size)
{
	mt19937 mersenne{ static_cast<mt19937::result_type>(time(nullptr)) };
	mersenne.seed(rd());
	uniform_int_distribution<> rand_v{ 1, 100 };
	uniform_int_distribution<> rand_val{ 'a', 'f' };
	uniform_int_distribution<> rand_val2{ '0', '9' };
	srand(time(0));
	int rand_size = 1 + rand_v(mersenne) % (size - 2);
	string number;
	for (int i = 0; i < rand_size; i++)
	{
		int temp = rand_v(mersenne) % 15;
		//cout << temp << " ";
		if (temp < 10)
			number += rand_val2(mersenne);
		else
			number += rand_val(mersenne);
	}
	return number;
}

int* p_1(int* num, int len, int var = 2)
{
	int* diff = new int[len];
	for (int i = 0; i < len; i++)
	{
		diff[i] = 0;
	}
	int borrow = 0;
	//cout << "p-1 = ";
	for (int i = 0; i < len; i++)
	{
		int temp = 0;
		if (i == 0)
			temp = num[len - i - 1] - 1 - borrow;
		else
			temp = num[len - i - 1] - borrow;
		if (temp >= 0)
		{
			diff[len - i - 1] = temp;
			borrow = 0;
			for (int j = 0; j < len - i - 1; j++)
			{
				diff[j] = num[j];
			}
			i = len;
		}
		else
		{
			diff[len - i - 1] = temp + var;
			borrow = 1;
		}
	}
	return diff;
}

int findS(int* diff, int len, int var = 2)
{
	if (var != 2)
	{
		len = len / 4;
	}
	int s = 0;
	for (int i = 0; i < len; i++)
	{
		for (int k = 1; k < 4; k++)
		{
			if (diff[len - i - 1] % (1 << k) == 0)
				s++;
			else
			{
				i = len;
				k = 4;
			}
		}
	}
	//cout << "s = " << s << endl;
	return s;
}
mpz_class findD(int* diff, int s, int len)
{
	string differ = arrToStr(diff, len);
	mpz_class d = mpz_class(differ, 16);
	string pow2s;
	switch (s % 4)
	{
	case 0: pow2s += '1'; break;
	case 1: pow2s += '2'; break;
	case 2: pow2s += '4'; break;
	case 3: pow2s += '8'; break;
	}
	for (int i = 0; i < s / 4; i++)
	{
		pow2s += '0';
	}
	mpz_class power = mpz_class(pow2s, 16);
	mpz_divexact(d.get_mpz_t(), d.get_mpz_t(), power.get_mpz_t());
	return d;
}

int pseudo(mpz_class x_num, mpz_class p, mpz_class d, int s)
{
	mpz_class res;
	mpz_powm(res.get_mpz_t(), x_num.get_mpz_t(), d.get_mpz_t(), p.get_mpz_t());
	if (res.get_ui() == 1 || res.get_ui() == p.get_ui() - 1)
	{
		return 1;
	}
	else
	{
		mpz_class pow2 = mpz_class("2", 16);
		for (int i = 1; i < s; i++)
		{
			mpz_powm(res.get_mpz_t(), res.get_mpz_t(), pow2.get_mpz_t(), p.get_mpz_t());
			if (res.get_ui() == 1)
			{
				return 0;
			}
			if (res.get_ui() == p.get_ui() - 1)
			{
				return 1;
			}
		}
	}
	//cout << res.get_ui() << endl;
	return 0;
}
int MR(int* num, int len, int var = 2)
{
	int result = 1;
	int* arr = new int[4];
	int* diff = new int[len];
	for (int i = 0; i < len; i++)
	{
		diff[i] = 0;
	}

	//считаем р-1
	//cout << endl;
	diff = p_1(num, len, var);

	//разделяем р-1=d*2^{s} и находим d, s
	int s = findS(diff, len);

	mpz_class d_num = findD(diff, s, len);
	//cout << "d = " << d_num.get_mpz_t() << endl;
	string init_number_p = arrToStr(num, len);
	mpz_class p_num = mpz_class(init_number_p, 16);
	//cout << "p = " <<  p_num.get_mpz_t() << endl;

	for (int i = 0; i < 50; i++)
	{
		string init_number_x = initialString(len);
		mpz_class x_num = mpz_class(init_number_x, 16);
		mpz_class gcd;
		mpz_gcd(gcd.get_mpz_t(), p_num.get_mpz_t(), x_num.get_mpz_t());
		if (gcd.get_ui() == 1)
		{
			result = pseudo(x_num, p_num, d_num, s);
			if (result == 0)
				return 0;
		}
		else
			return 0;
	}
	return 1;
}

int* numToBlum(int* num, int len)
{
	int len2 = len + 1;
	int* blum = new int[len2];
	blum[0] = 0;
	int temp = 0;
	for (int i = 0; i < len; i++)
	{
		blum[len - i] = 0;
		blum[len - i] = num[len - i - 1] * 4 + temp;
		temp = blum[len - i] / 16;
		blum[len - i] %= 16;
	}
	blum[0] = temp;
	temp = 0;
	for (int i = 0; i < len; i++)
	{
		if (i == 0)
			blum[len] += 3;
		else 
			blum[len - i] += temp;
		if (blum[len - i] / 16 == 0)
			i = len;
		temp = blum[len - i] / 16;
		blum[len - i] %= 16;
	}
	blum[0] += temp;
	return blum;
}
mpz_class primeBlumGen(int len)
{
	int* primeNum = new int[len];
	int* primeBlumNum = new int[len];
	for (int i = 0; i < len; i++)
	{
		primeNum[i] = 0;
		primeBlumNum[i] = 0;
	}
	int resMR = 0;
	while (resMR == 0)
	{
		primeNum = randomNum(len);
		primeBlumNum = numToBlum(primeNum, len);
		if (primeBlumNum[0]==0)
		{
			for (int i = 0; i < len; i++)
			{
				primeNum[i] = primeBlumNum[i + 1];
			}
			resMR = MR(primeNum, len, 16);
		}
		else
		{
			resMR = MR(primeBlumNum, len+1, 16);
		}
		coun++;
		
	}
	string num;
	if (primeBlumNum[0] == 0)
	{
		num = arrToStr(primeNum, len);
	}
	else
	{
		num = arrToStr(primeBlumNum, len+1);
	}
	mpz_class primeNumber = mpz_class(num, 16);
	return primeNumber;
}

void GenerateKeyPair(int len)
{
	pA = primeBlumGen(len);
	/*out.open("results.txt", ios::app);
	out << "\n\n ========================================================== \n\n";
	out.close();*/
	qA = primeBlumGen(len);
	while (mpz_cmp(pA.get_mpz_t(), qA.get_mpz_t()) == 0)
	{
		qA = primeBlumGen(len);
	}
	/*out.open("results.txt", ios::app);
	out << "\n\n ========================================================== \n\n";
	out.close();*/
	mpz_mul(nA.get_mpz_t(), pA.get_mpz_t(), qA.get_mpz_t());
	int* primeNum = new int[len];
	for (int i = 0; i < len; i++)
	{
		primeNum[i] = 0;
	}
	string num;
	primeNum = randomNum(len);
	num = arrToStr(primeNum, len);
	bA = mpz_class(num, 16);
}

string format(string num, int len)
{
	string m = "ff";
	int* r = new int[16];
	r = randomNum(16);
	string r_str = arrToStr(r, 16);
	int l = 2 * (len - num.length()/2 - 10);
	for (int i = 0; i < l; i++)
	{
		m += "0";
	}
	m += num;
	m += r_str;
	return m;
}
mpz_class unformat(mpz_class m, int len)
{
	string ff = "ff";
	for (int i = 0; i < 2*len-4; i++)
	{
		ff += "0";
	}
	mpz_class m_ff = mpz_class(ff, 16);
	mpz_xor(m.get_mpz_t(), m.get_mpz_t(), m_ff.get_mpz_t());
	m = m >> 64;
	return m;
}

mpz_class Encrypt(mpz_class message, mpz_class n, mpz_class b)
{
	mpz_class y;
	mpz_class x;
	mpz_class inv2;
	mpz_class i2 = mpz_class("2", 16);
	mpz_invert(inv2.get_mpz_t(), i2.get_mpz_t(), n.get_mpz_t());
	y = message * (message + b) % n;
	x = (message + b * inv2) % n;
	c1 = x.get_ui() & 1;
	if (mpz_jacobi(x.get_mpz_t(), n.get_mpz_t()) == 1)
		c2 = 1;
	else
		c2 = 0;
	return y;
}

mpz_class Decrypt(mpz_class C, mpz_class n, mpz_class p, mpz_class q, mpz_class b)
{
	mpz_class inv2, inv4;
	mpz_class i2 = mpz_class("2",16);
	mpz_class i4 = mpz_class("4", 16);
	mpz_invert(inv2.get_mpz_t(), i2.get_mpz_t(), n.get_mpz_t());
	mpz_invert(inv4.get_mpz_t(), i4.get_mpz_t(), n.get_mpz_t());
	mpz_class y = (C + b * b * inv4) % n;
	mpz_class s1, s2;
	mpz_class deg = (p + 1) / 4;
	mpz_powm(s2.get_mpz_t(), y.get_mpz_t(), deg.get_mpz_t(), p.get_mpz_t());
	deg = (q + 1) / 4;
	mpz_powm(s1.get_mpz_t(), y.get_mpz_t(), deg.get_mpz_t(), q.get_mpz_t());
	mpz_class u, v, g;
	mpz_gcdext(g.get_mpz_t(), u.get_mpz_t(), v.get_mpz_t(), p.get_mpz_t(), q.get_mpz_t());
	mpz_class mess[4];
	mess[0] = (u * p * s1 + v * q * s2) % n;
	if (mpz_sgn(mess[0].get_mpz_t()) < 0)
	{
		mess[0] += n;
	}
	mess[1] = n - mess[0];
	mess[2] = (u * p * s1 - v * q * s2) % n;
	if (mpz_sgn(mess[2].get_mpz_t()) < 0)
	{
		mess[2] += n;
	}
	mess[3] = n - mess[2];
	for (int i = 0; i < 4; i++)
	{
		mpz_class c_prob = mess[i]%mpz_class(2);
		int c = c_prob.get_ui();
		//cout << "c = " << c << "       c1 = " << c1 << endl;
		if (c==c1)
		{

			if (mpz_jacobi(mess[i].get_mpz_t(), n.get_mpz_t()) == 1)
				c = 1;
			else
				c = 0;
			//cout << "c = " << c << "       c2 = " << c2 << endl;
			if (c == c2)
			{
				mpz_class k = (mess[i] - b * inv2) % n;
				if (mpz_sgn(k.get_mpz_t())<0)
				{
					k += n;
				}
				return k % n;
			}	
		}
	}
}

mpz_class Sign(string message, mpz_class n, mpz_class p, mpz_class q, int len)
{
	string messageF = format(message, len-1);
	mpz_class m = mpz_class(messageF, 16);
	while (mpz_legendre(m.get_mpz_t(), p.get_mpz_t())!=1 || mpz_legendre(m.get_mpz_t(), q.get_mpz_t()) != 1)
	{
		messageF = format(message, len-1);
		m = mpz_class(messageF, 16);
	}
	mpz_class s1, s2;
	mpz_class deg = (p + 1) / 4;
	mpz_powm(s2.get_mpz_t(), m.get_mpz_t(), deg.get_mpz_t(), p.get_mpz_t());
	deg = (q + 1) / 4;
	mpz_powm(s1.get_mpz_t(), m.get_mpz_t(), deg.get_mpz_t(), q.get_mpz_t());
	mpz_class u, v, g;
	mpz_gcdext(g.get_mpz_t(), u.get_mpz_t(), v.get_mpz_t(), p.get_mpz_t(), q.get_mpz_t());
	mpz_class mess[4];
	mess[0] = (u * p * s1 + v * q * s2) % n;
	if (mpz_sgn(mess[0].get_mpz_t()) < 0)
	{
		mess[0] += n;
	}
	mess[1] = n - mess[0];
	mess[2] = (u * p * s1 - v * q * s2) % n;
	if (mpz_sgn(mess[2].get_mpz_t()) < 0)
	{
		mess[2] += n;
	}
	mess[3] = n - mess[2];
	mpz_class s = mess[coun % 4];
	return s;
}

void Verify(mpz_class M, mpz_class S, mpz_class n, int len)
{
	mpz_class x = (S * S) % n;
	cout << endl << "x_format = " << x.get_mpz_t() << endl;
	x = unformat(x, len);
	cout << endl << "x = " << x.get_mpz_t() << endl;
	if (mpz_cmp(M.get_mpz_t(), x.get_mpz_t()) == 0)
	{
		cout << endl << "Верификация прошла успешно!" << endl << endl << endl;
	}
}

void ZeroKnowledge()
{
	string n;
	cout << "Возьмите с сайта модуль" << endl;
	cin >> n;
	int l = n.length() - 1;
	cout << l << endl;
	nA = mpz_class(n, 16);
	int* r = new int[l];
	int stopper = 0;
	coun = 0;
	while (stopper==0)
	{
		r = randomNum(l);
		string r_str = arrToStr(r, l);
		mpz_class t = mpz_class(r_str, 16);
		cout << endl <<  "t = " << hex << t.get_mpz_t() << endl;
		mpz_class y = (t * t) % nA;
		cout << "y = " << hex << y.get_mpz_t() << endl << endl;
		cout << "Возьмите с сайта корень" << endl;
		cin >> n;
		bA = mpz_class(n, 16);
		cout << "z1 = " << bA.get_mpz_t() << endl;
		bB = nA - bA;
		cout << "z2 = " << bB.get_mpz_t() << endl << endl;
		if (mpz_cmp(t.get_mpz_t(), bA.get_mpz_t()) != 0 && mpz_cmp(t.get_mpz_t(), bB.get_mpz_t()) != 0)
		{
			cout << endl  << "Мы можем разложить n!" << endl;
			bA = (t + bA) % nA;
			mpz_gcd(pA.get_mpz_t(), bA.get_mpz_t(), nA.get_mpz_t());
			cout << "p = " << pA.get_mpz_t() << endl;
			mpz_divexact(qA.get_mpz_t(), nA.get_mpz_t(), pA.get_mpz_t());
			cout << "q = " << qA.get_mpz_t() << endl;
			cout << endl << "Проверка" << endl;
			bB = pA * qA;
			cout << "n = " << bB.get_mpz_t() << endl;
			if (mpz_cmp(nA.get_mpz_t(), bB.get_mpz_t()) == 0)
			{
				cout << endl << "Проверка пройдена успешно!" << endl << endl;
			}
			stopper = 1;
		}
		coun++;
	}
	cout << "Потребовалось " << coun << " попыток" << endl;
}
