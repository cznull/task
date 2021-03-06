﻿// hpc.cpp : Defines the entry point for the console application.

#include "hpc.h"
#define PI 3.14159265358979323846

class highint {
private:
public:
	char *f;
	unsigned int length;
	int sign;
public:
	highint(int n) {
		sign = n > 0 ? 1 : -1;
		unsigned int n1 = abs(n);
		length = 15;
		f = new char[15];
		int i;
		for (i = 0; i < 15; i++) {
			f[i] = n1 % 10;
			n1 /= 10;
		}
	}
	highint(void) {
		length = 1;
		sign = 1;
		f = new char;
		*f = 0;
	}
	highint(unsigned int n,const char *fin) {
		if (n <= 0x7fffffff) {
			length = n;
			sign = 1;
			f = new char[length];
			if (fin) {
				unsigned int i, j;
				char k;
				for (i = 0, j = 0; i < n; i++) {
					k = fin[length - 1 - i];
					if (k > 47 && k < 58) {
						f[j] = k - 48;
						j++;
					}
				}
				memset(f + j, 0, i - j);
			}
			else {
				memset(f, 0, n);
			}
		}
		else {
			length = 1;
			sign = 1;
			f = new char;
			*f = 0;
		}
	}
	highint(const highint &n) {
		length = n.length;
		sign = n.sign;
		f = new char[length];
		memcpy(f, n.f, length);
	}
	void operator=(highint &n) {
		if (length < n.length||length>n.length*2) {
			char *p= new char[n.length];
			delete f;
			f = p;
			length = n.length;
			memcpy(f, n.f, length);
		}
		else {
			memcpy(f, n.f, n.length);
			memset(f + n.length, 0, length - n.length);
		}
		sign = n.sign;
	}
	unsigned int size(void) {
			char *p = f + length;
			while ((p > f) && !*(--p));
			return p - f + 1;
	}
	~highint() {
		delete f;
	}
	char* print(void);
};

char* highint::print(void) {
	unsigned long long i = size(), j;
	char *s = new char[(unsigned long long)size() + 2];
	char *s1;
	if(sign==-1&&!(i==1&&(*f==0))){
		*s = '-';
		s1 = s+1;
	}
	else {
		s1 = s;
	}
	if (i) {
		for (j = 0; j < i; j++) {
			s1[j] = 48 + f[i - j - 1];
		}
	}
	s1[i] = 0;
	return s;
}

highint operator+(highint &n1, highint &n2);
highint operator-(highint &n1, highint &n2);
highint operator*(highint &n1, highint &n2);
highint operator/(highint &n1, highint &n2);
highint operator%(highint &n1, highint &n2);
highint operator^(highint &n1, highint &n2);
int comphighintabs(highint &n1, highint &n2);
highint f(highint &n1);

highint f(highint &n1) {
	unsigned int size, exponential=0;
	unsigned int i;
	size = n1.size();
	if (size == 0 && n1.f[0] == 0) {
		return 1;
	}
	else if (n1.sign < 0) {
		printf("not defined operator");
		throw 0;
	}
	else if (size < 10)
	{
		unsigned int e;
		e = 1;
		for (i = 0; i < size; i++) {
			exponential += n1.f[i] * e;
			e *= 10;
		}
		if (exponential < 54763063) {
			unsigned int l,a,b;
			l =((exponential+0.5)*log(exponential) - exponential)/log(10) + 4;
			highint n(l, NULL);
			*n.f = 1;
			e = 1;
			a = 0;
			for (; exponential > 1; exponential--) {
				for (i = 0; i < e; i++) {
					b = n.f[i]*exponential+a;
					a = b / 10;
					n.f[i] = b % 10;
				}
				while (a) {
					n.f[e] = a % 10;
					a /= 10;
					e++;
				}
			}
			return n;
		}
		else {
			printf("The size of number is beyond calculating competence.\n");
			throw 0;
		}
	}
	else {
		printf("The size of number is beyond calculating competence.\n");
		throw 0;
	}
}

highint operator+(highint &n1, highint &n2)
{
	unsigned int i, size1, size2, size;
	size1 = n1.size();
	size2 = n2.size();
	size = (size1 > size2) ? size1 : size2;
	if (size1 > 0x7ffffffe || size2 > 0x7fffffffe)
	{
		printf("The size of number is beyond calculating competence.\n");
		throw 0;
	}
	else {
		highint n(size + 1, NULL);
		if (n1.sign == n2.sign)
		{
			if (size1 > size2)
			{
				n.sign = n1.sign;
				int a = 0;
				for (i = 0; i < size2; i++)
				{
					n.f[i] = n1.f[i] + n2.f[i] + a;
					if (n.f[i] > 9)
					{
						a = 1;
						n.f[i] -= 10;
					}
					else
						a = 0;
				}
				for (i = size2; i < size1; i++)
				{
					n.f[i] = n1.f[i] + a;
					if (n.f[i] > 9)
					{
						a = 1;
						n.f[i] -= 10;
					}
					else
						a = 0;
				}
				n.f[i] = a;
			}
			else
			{
				n.sign = n1.sign;
				int a = 0;
				for (i = 0; i < size1; i++)
				{
					n.f[i] = n1.f[i] + n2.f[i] + a;
					if (n.f[i] > 9)
					{
						a = 1;
						n.f[i] -= 10;
					}
					else
						a = 0;
				}
				for (i = size1; i < size2; i++)
				{
					n.f[i] = n2.f[i] + a;
					if (n.f[i] > 9)
					{
						a = 1;
						n.f[i] -= 10;
					}
					else
						a = 0;
				}
				n.f[i] = a;
			}
		}
		else
		{
			int i = comphighintabs(n1, n2);
			if (i > 0)
			{
				n.sign = n1.sign;
				int a = 0;
				for (i = 0; i < size2; i++)
				{
					n.f[i] = n1.f[i] - n2.f[i] + a;
					if (n.f[i] < 0)
					{
						a = -1;
						n.f[i] += 10;
					}
					else
						a = 0;
				}
				for (i = size2; i < size1; i++)
				{
					n.f[i] = n1.f[i] + a;
					if (n.f[i] < 0)
					{
						a = -1;
						n.f[i] += 10;
					}
					else
						a = 0;
				}
			}
			else
			{
				n.sign = n2.sign;
				int a = 0;
				for (i = 0; i < size1; i++)
				{
					n.f[i] = n2.f[i] - n1.f[i] + a;
					if (n.f[i] < 0)
					{
						a = -1;
						n.f[i] += 10;
					}
					else
						a = 0;
				}
				for (i = size1; i < size2; i++)
				{
					n.f[i] = n2.f[i] + a;
					if (n.f[i] < 0)
					{
						a = -1;
						n.f[i] += 10;
					}
					else
						a = 0;
				}
			}
		}
	return n;
	}
}

highint operator-(highint &n1, highint &n2)
{
	unsigned int i, size1, size2, size;
	size1 = n1.size();
	size2 = n2.size();
	size = (size1 > size2) ? size1 : size2;
	if (size1 > 0x7ffffffe || size2 > 0x7fffffffe)
	{
		printf("The size of number is beyond calculating competence.\n");
		throw 0;
	}
	else {
		highint n(size + 1, NULL);
		if (n1.sign != n2.sign)
		{
			if (size1 > size2)
			{
				n.sign = n1.sign;
				int a = 0;
				for (i = 0; i < size2; i++)
				{
					n.f[i] = n1.f[i] + n2.f[i] + a;
					if (n.f[i] > 9)
					{
						a = 1;
						n.f[i] -= 10;
					}
					else
						a = 0;
				}
				for (i = size2; i < size1; i++)
				{
					n.f[i] = n1.f[i] + a;
					if (n.f[i] > 9)
					{
						a = 1;
						n.f[i] -= 10;
					}
					else
						a = 0;
				}
				n.f[i] = a;
			}
			else
			{
				n.sign = n1.sign;
				int a = 0;
				for (i = 0; i < size1; i++)
				{
					n.f[i] = n1.f[i] + n2.f[i] + a;
					if (n.f[i] > 9)
					{
						a = 1;
						n.f[i] -= 10;
					}
					else
						a = 0;
				}
				for (i = size1; i < size2; i++)
				{
					n.f[i] = n2.f[i] + a;
					if (n.f[i] > 9)
					{
						a = 1;
						n.f[i] -= 10;
					}
					else
						a = 0;
				}
				n.f[i] = a;
			}
		}
		else
		{
			int i = comphighintabs(n1, n2);
			if (i > 0)
			{
				n.sign = n1.sign;
				int a = 0;
				for (i = 0; i < size2; i++)
				{
					n.f[i] = n1.f[i] - n2.f[i] + a;
					if (n.f[i] < 0)
					{
						a = -1;
						n.f[i] += 10;
					}
					else
						a = 0;
				}
				for (i = size2; i < size1; i++)
				{
					n.f[i] = n1.f[i] + a;
					if (n.f[i] < 0)
					{
						a = -1;
						n.f[i] += 10;
					}
					else
						a = 0;
				}
			}
			else
			{
				n.sign = -n2.sign;
				int a = 0;
				for (i = 0; i < size1; i++)
				{
					n.f[i] = n2.f[i] - n1.f[i] + a;
					if (n.f[i] < 0)
					{
						a = -1;
						n.f[i] += 10;
					}
					else
						a = 0;
				}
				for (i = size1; i < size2; i++)
				{
					n.f[i] = n2.f[i] + a;
					if (n.f[i] < 0)
					{
						a = -1;
						n.f[i] += 10;
					}
					else
						a = 0;
				}
			}
		}
		return n;
	}
}

highint operator*(highint &n1, highint &n2)
{
	unsigned int size1 = n1.size();
	unsigned int size2 = n2.size();
	if (size1 + size2 <= 0x7fffffff ) {
		if (size1 <= 0x4000000 && size2 <= 0x4000000 && size1 >= 2048 && size2 >= 2048) {
			unsigned long long size, Nf;
			size = size1 > size2 ? size1 : size2;
			Nf = 16;
			while (Nf < size) {
				Nf *= 2;
			}
			double *f1 = NULL, *f2 = NULL, *w = NULL;
			double tr1, ti1, tr2, ti2;
			highint n(size1 + size2, NULL);
			try {
				f1 = new double[Nf * 4];
				f2 = new double[Nf * 4];
				w = new double[Nf * 2];
			}
			catch (...) {
				if (f1)
					delete f1;
				if (f2)
					delete f2;
				if (w)
					delete w;
				printf("no enough memory.\n");
				throw 0;
			}
			memset(f1, 0, Nf * 32);
			memset(f2, 0, Nf * 32);
			unsigned int i, j, k, l;
			j = 0;
			for (i = 0; i < Nf; i++)
			{
				w[i * 2] = cos((double)i / Nf*PI);
				w[i * 2 + 1] = sin((double)i / Nf*PI);
				if (i < size1)
					f1[2 * j] = n1.f[i];
				if (i < size2)
					f2[2 * j] = n2.f[i];
				k = Nf;
				while (j >= k) {
					j -= k;
					k >>= 1;
				}
				j += k;
			}
			i = 1;
			while (i <= Nf) {
				j = Nf / i;
				for (k = 0; k < Nf * 2; k += 2 * i) {
					for (l = 0; l < i; l++) {
						tr1 = f1[(k + l) * 2] + f1[(k + l + i) * 2] * w[l*j * 2] - f1[(k + l + i) * 2 + 1] * w[l*j * 2 + 1];
						ti1 = f1[(k + l) * 2 + 1] + f1[(k + l + i) * 2 + 1] * w[l*j * 2] + f1[(k + l + i) * 2] * w[l*j * 2 + 1];
						tr2 = f1[(k + l) * 2] - f1[(k + l + i) * 2] * w[l*j * 2] + f1[(k + l + i) * 2 + 1] * w[l*j * 2 + 1];
						ti2 = f1[(k + l) * 2 + 1] - f1[(k + l + i) * 2 + 1] * w[l*j * 2] - f1[(k + l + i) * 2] * w[l*j * 2 + 1];
						f1[(k + l) * 2] = tr1;
						f1[(k + l) * 2 + 1] = ti1;
						f1[(k + l + i) * 2] = tr2;
						f1[(k + l + i) * 2 + 1] = ti2;
						tr1 = f2[(k + l) * 2] + f2[(k + l + i) * 2] * w[l*j * 2] - f2[(k + l + i) * 2 + 1] * w[l*j * 2 + 1];
						ti1 = f2[(k + l) * 2 + 1] + f2[(k + l + i) * 2 + 1] * w[l*j * 2] + f2[(k + l + i) * 2] * w[l*j * 2 + 1];
						tr2 = f2[(k + l) * 2] - f2[(k + l + i) * 2] * w[l*j * 2] + f2[(k + l + i) * 2 + 1] * w[l*j * 2 + 1];
						ti2 = f2[(k + l) * 2 + 1] - f2[(k + l + i) * 2 + 1] * w[l*j * 2] - f2[(k + l + i) * 2] * w[l*j * 2 + 1];
						f2[(k + l) * 2] = tr1;
						f2[(k + l) * 2 + 1] = ti1;
						f2[(k + l + i) * 2] = tr2;
						f2[(k + l + i) * 2 + 1] = ti2;
					}
				}
				i *= 2;
			}
			for (i = 0; i < Nf * 2; i++) {
				tr1 = f1[i * 2] * f2[i * 2] - f1[i * 2 + 1] * f2[i * 2 + 1];
				f2[i * 2 + 1] = f1[i * 2] * f2[i * 2 + 1] + f1[i * 2 + 1] * f2[i * 2];
				f2[i * 2] = tr1;
			}
			j = 0;
			for (i = 0; i < Nf * 2 - 1; i++)
			{
				f1[2 * j] = f2[2 * i];
				f1[2 * j + 1] = f2[2 * i + 1];
				k = Nf;
				while (j >= k) {
					j -= k;
					k >>= 1;
				}
				j += k;
			}
			f1[2 * j] = f2[2 * i];
			f1[2 * j + 1] = f2[2 * i + 1];
			i = 1;
			while (i <= Nf) {
				j = Nf / i;
				for (k = 0; k < Nf * 2; k += 2 * i) {
					for (l = 0; l < i; l++) {
						tr1 = f1[(k + l) * 2] + f1[(k + l + i) * 2] * w[l*j * 2] + f1[(k + l + i) * 2 + 1] * w[l*j * 2 + 1];
						ti1 = f1[(k + l) * 2 + 1] + f1[(k + l + i) * 2 + 1] * w[l*j * 2] - f1[(k + l + i) * 2] * w[l*j * 2 + 1];
						tr2 = f1[(k + l) * 2] - f1[(k + l + i) * 2] * w[l*j * 2] - f1[(k + l + i) * 2 + 1] * w[l*j * 2 + 1];
						ti2 = f1[(k + l) * 2 + 1] - f1[(k + l + i) * 2 + 1] * w[l*j * 2] + f1[(k + l + i) * 2] * w[l*j * 2 + 1];
						f1[(k + l) * 2] = tr1;
						f1[(k + l) * 2 + 1] = ti1;
						f1[(k + l + i) * 2] = tr2;
						f1[(k + l + i) * 2 + 1] = ti2;
					}
				}
				i *= 2;
			}
			k = 0;
			for (i = 0; i < n.length; i++) {
				k = int(f1[i * 2] / Nf / 2 + 0.5) + k;
				n.f[i] = k % 10;
				k /= 10;
			}
			delete f1;
			delete f2;
			delete w;
			return n;
		}
		else if (size1<0x2c00000 || size2<0x2c00000) {
			int i, j, ten;
			highint n(size1 + size2, NULL);
			unsigned int *p = new unsigned int[size1 + size2];
			n.length = size1 + size2;
			n.sign = n1.sign*n2.sign;
			memset(p, 0, (size_t)(size1 + size2) * 4);
			for (i = 0; i < size1; i++)
			{
				for (j = 0; j < size2; j++)
				{
					p[i + j] += n1.f[i] * n2.f[j];
				}
			}
			for (i = 0; i < n.length; i++)
			{
				if (p[i] > 9)
				{
					ten = p[i] / 10;
					p[i + 1] += ten;
					n.f[i] = p[i] - 10 * ten;
				}
				else
				{
					n.f[i] = p[i];
				}
			}
			delete p;
			return n;
		}
		else{
			printf("The size of number is beyond calculating competence.\n");
			throw 0;
		}
	}
	else {
		printf("The size of number is beyond calculating competence.\n");
		throw 0;
	}
}

highint operator/(highint &n1, highint &n2)
{
	if (n2.size() == 1 && n2.f[0] == 0) {
		printf("error:divided by 0.\n");
		throw 0;
	}
	else
	{
		unsigned int size1, size2, size, i, j, k;
		size1 = n1.size();
		size2 = n2.size();
		size = size1 - size2;
		if (size1 < size2)
			return (highint)0;
		else
		{
			int a;
			highint n(size + 1, NULL);
			highint n3(size1 + 1, NULL);
			highint n4(size2 + 1, NULL);
			n.sign = n1.sign*n2.sign;
			memcpy(n3.f, n1.f, size1);
			memcpy(n4.f, n2.f, size2);
			for (k = 0; k < size + 1; k++)
			{
				for (j = 0; n3.f[size1 - k] >= 0; j++)
				{
					a = 0;
					for (i = 0; i < size2; i++)
					{
						n3.f[size + i - k] = n3.f[size + i - k] - n4.f[i] + a;
						if (n3.f[size + i - k] < 0)
						{
							a = -1;
							n3.f[size + i - k] += 10;
						}
						else a = 0;
					}
					n3.f[size + i - k] += a;
				}
				n.f[size - k] = j - 1;
				a = 0;
				for (i = 0; i < size2 + 1; i++)
				{
					n3.f[size + i - k] = n3.f[size + i - k] + n4.f[i] + a;
					if (n3.f[size + i - k] > 9)
					{
						a = 1;
						n3.f[size + i - k] -= 10;
					}
					else a = 0;
				}
			}
			return n;
		}
	}
}

highint operator%(highint &n1, highint &n2)
{
	if (n2.size() == 1 && n2.f[0] == 0) {
		printf("error:divided by 0.\n");
		throw 0;
	}
	else
	{
		unsigned int size1, size2, size, i, j, k;
		size1 = n1.size();
		size2 = n2.size();
		size = size1 - size2;
		if (size1 < size2)
			return (highint)0;
		else
		{
			int a;
			highint n3(size1 + 1, NULL);
			highint n4(size2 + 1, NULL);
			memcpy(n3.f, n1.f, size1);
			n3.sign = n1.sign;
			memcpy(n4.f, n2.f, size2);
			for (k = 0; k < size + 1; k++)
			{
				for (j = 0; n3.f[size1 - k] >= 0; j++)
				{
					a = 0;
					for (i = 0; i < size2; i++)
					{
						n3.f[size + i - k] = n3.f[size + i - k] - n4.f[i] + a;
						if (n3.f[size + i - k] < 0)
						{
							a = -1;
							n3.f[size + i - k] += 10;
						}
						else a = 0;
					}
					n3.f[size + i - k] += a;
				}
				a = 0;
				for (i = 0; i < size2 + 1; i++)
				{
					n3.f[size + i - k] = n3.f[size + i - k] + n4.f[i] + a;
					if (n3.f[size + i - k] > 9)
					{
						a = 1;
						n3.f[size + i - k] -= 10;
					}
					else a = 0;
				}
			}
			return n3;
		}
	}
}

highint operator^(highint &n1, highint &n2)
{
	int exponential=0;
	unsigned int size2 = n2.size();
	unsigned int size1 = n1.size();
	if ((size1 == 1) && (n1.f[0] == 1)) {
		return (n1.sign) > 0 ? 1 : (n2.f[0] % 2 ? -1 : 1);
	}
	else if ((size2 == 1) && (n2.f[0] == 0)) {
		if ((size1 == 1) && (n1.f[0] == 0)) {
			printf("not defined operating.\n");
			throw 0;
		}
		else {
			return 1;
		}
	}
	else if ((size1 == 1) && (n1.f[0] == 0)||n2.sign==-1) {
		return 0;
	}
	else if (size2 < 10)
	{
		unsigned int i, e,j;
		e = 1;
		for (i = 0; i < size2; i++) {
			exponential += n2.f[i] * e;
			e *= 10;
		}
		if ((size1 - 1)*exponential < 0x8000000) {
			highint n(1), t(n1);
			if (exponential & 1) {
				n = n1;
			}
			exponential >>= 1;
			while (exponential) {
				t = t*t;
				if (exponential & 1) {
					n = n*t;
				}
				exponential>>=1;
			}
			return n;
		}
		else {
			printf("The size of number is beyond calculating competence.\n");
			throw 0;
		}
	}
	else {
		printf("The size of number is beyond calculating competence.\n");
		throw 0;
	}
}

int comphighintabs(highint &n1, highint &n2) {
	unsigned int s1, s2;
	s1 = n1.size();
	s2 = n2.size();
	if (s1 > s2) {
		return 1;
	}
	else if (s1 < s2) {
		return -1;
	}
	else {
		unsigned int i;
		char f1, f2;
		for (i = 0; i < s1; i++) {
			f1 = n1.f[s1 - i - 1];
			f2 = n2.f[s1 - i - 1];
			if (f1 > f2) {
				return 1;
			}
			else if (f1 < f2) {
				return -1;
			}
		}
		return 0;
	}
}

struct ele {
	char t;
	highint *p;
};

struct store {
	char *s;
	highint *p;
};

store *ps, *ps1;
int ns,ens;

int countele(const char *s) {
	const char *p;
	int i,state=0;
	p = s;
	i = 0;
	while (*p) {
		switch (*p) {
		case 48: case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57: 
			if (state == 0) {
				state = 1;
				i++;
			}
			break;
		case ' ':
			break;
		case 'a':case'b':case 'c':case 'd':case'e':case 'f':case 'g':case'h':case 'i':case 'j':case'k':case 'l':case 'm':case'n':case 'o':case 'p':case'q':case 'r':case 's':case't':case 'u':case 'v':case'w':case 'x':case 'y':case'z':
		case 'A':case 'B':case'C':case 'D':case 'E':case'F':case 'G':case 'H':case'I':case 'J':case 'K':case'L':case 'M':case 'N':case'O':case 'P':case 'Q':case'R':case 'S':case 'T':case'U':case 'V':case 'W':case'X':case 'Y':case 'Z':
			if (state == 0) {
				i++;
				state = 2;
			}
			else if (state == 1) {
				i += 2;
				state = 2;
			}
			break;
		default:
				i += 1;
				state = 0;
			break;
		}
		p++;
	}
	return i;
}

void pretreat(ele* pele, const char *s) {
	int state = 0, count, i, j;
	const char *p;
	p = s;
	i = 0;
	try {
		while (*p) {
			switch (*p) {
			case 48: case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57:
				if (state == 0) {
					state = 1;
					count = 1;
				}
				else {
					count++;
				}
				break;
			case ' ':
				count++;
				break;
			case 'a':case'b':case 'c':case 'd':case'e':case 'f':case 'g':case'h':case 'i':case 'j':case'k':case 'l':case 'm':case'n':case 'o':case 'p':case'q':case 'r':case 's':case't':case 'u':case 'v':case'w':case 'x':case 'y':case'z':
			case 'A':case 'B':case'C':case 'D':case 'E':case'F':case 'G':case 'H':case'I':case 'J':case 'K':case'L':case 'M':case 'N':case'O':case 'P':case 'Q':case'R':case 'S':case 'T':case'U':case 'V':case 'W':case'X':case 'Y':case 'Z':
				if (state == 0) {
					state = 2;
					count = 1;
				}
				else if (state == 1) {
					pele[i].t = -1;
					pele[i].p = new highint(count, p - count);
					i++;
					pele[i].t = '*';
					pele[i].p = NULL;
					i++;
					count = 1;
					state = 2;
				}
				else {
					count++;
				}
				break;
			default:
				if (state == 0) {
					pele[i].t = *p;
					pele[i].p = NULL;
					i++;
				}
				else if (state == 1) {
					pele[i].t = -1;
					pele[i].p = new highint(count, p - count);
					i++;
					pele[i].t = *p;
					pele[i].p = NULL;
					i++;
					state = 0;
				}
				else {
					for (j = 0; j < ens; j++) {
						int e, k;
						e = 1;
						for (k = 0; k < count; k++) {
							if (ps[j].s[k] != p[k - count]) {
								e = 0;
								break;
							}
						}
						if (ps[j].s[k]) {
							e = 0;
						}
						if (e) {
							pele[i].t = -1;
							pele[i].p = new highint(*ps[j].p);
							i++;
							break;
						}
					}
					if (j == ens) {
						pele[i].t = -1;
						pele[i].p = new highint;
						i++;
					}
					pele[i].t = *p;
					pele[i].p = NULL;
					i++;
					state = 0;
				}
				break;
			}
			p++;
		}
		if (state == 1) {
			pele[i].t = -1;
			pele[i].p = new highint(count, p - count);
		}
		else if (state == 2) {
			for (j = 0; j < ens; j++) {
				int e, k;
				e = 1;
				for (k = 0; k < count; k++) {
					if (ps[j].s[k] != p[k - count]) {
						e = 0;
						break;
					}
				}
				if (ps[j].s[k]) {
					e = 0;
				}
				if (e) {
					pele[i].t = -1;
					pele[i].p = new highint(*ps[j].p);
					i++;
					break;
				}
			}
			if (j == ens) {
				pele[i].t = -1;
				pele[i].p = new highint;
				i++;
			}
		}
	}
	catch (...) {
		for (j = 0; j < i; j++) {
			if (pele[j].p) {
				delete pele[j].p;
			}
		}
		throw 0;
	}
}

highint* calc(ele* pele, const int count) {
	int b = 0;
	ele *p1, *p2;
	p1 = pele;
	p2 = pele + 1;
	try {
		while (p1 != pele || p2 != pele + count) {
			switch (p1->t) {
			case -1:
				switch (p1[-1].t) {
				case 0:
					switch (p2->t) {
					case '+':
					case '-':
					case '*':
					case '/':
					case '%':
					case '^':
						p1++;
						*p1 = *p2;
						p2++;
						p1++;
						*p1 = *p2;
						p2++;
						break;
					case ')':
						printf("need a \'(\'\n");
						throw 0;
					case '!':
						*p1->p=f(*p1->p);
						p2++;
						break;
					default:
						printf("need an operator\n");
						throw 0;
					}
					break;
				case '(':
					switch (p2->t) {
					case '+':
					case '-':
					case '*':
					case '/':
					case '%':
					case '^':
						p1++;
						*p1 = *p2;
						p2++;
						p1++;
						*p1 = *p2;
						p2++;
						break;
					case ')':
						p1[-1] = *p1;
						p1--;
						p2++;
						break;
					case '!':
						*p1->p = f(*p1->p);
						p2++;
						break;
					default:
						printf("need a \')\'\n");
						throw 0;
					}
					break;
				case '-':
					switch (p2->t) {
					case '+':
					case '-':
					case ')':
					case 0:
						switch (p1[-2].t) {
						case -1:
							*p1[-2].p = *p1[-2].p - *p1->p;
							delete p1->p;
							p1 -= 2;
							break;
						default:
							p1->p->sign = -p1->p->sign;
							p1[-1] = *p1;
							p1--;
							break;
						}
						break;
					case '*':
					case '/':
					case '%':
					case '^':
						p1++;
						*p1 = *p2;
						p2++;
						p1++;
						*p1 = *p2;
						p2++;
						break;
					case '!':
						*p1->p = f(*p1->p);
						p2++;
						break;
					default:
						printf("need an operator\n");
						throw 0;
					}
					break;
				case '+':
					switch (p2->t) {
					case '+':
					case '-':
					case ')':
					case 0:
						switch (p1[-2].t) {
						case -1:
							*p1[-2].p = *p1[-2].p + *p1->p;
							delete p1->p;
							p1 -= 2;
							break;
						default:
							p1[-1] = *p1;
							p1--;
							break;
						}
						break;
					case '*':
					case '/':
					case '%':
					case '^':
						p1++;
						*p1 = *p2;
						p2++;
						p1++;
						*p1 = *p2;
						p2++;
						break;
					case '!':
						*p1->p = f(*p1->p);
						p2++;
						break;
					default:
						printf("need an operator\n");
						throw 0;
					}
					break;
				case '/':
					switch (p2->t) {
					case '+':
					case '-':
					case ')':
					case '*':
					case '/':
					case '%':
					case 0:
						*p1[-2].p = *p1[-2].p / *p1->p;
						delete p1->p;
						p1 -= 2;
						break;
					case '^':
						p1++;
						*p1 = *p2;
						p2++;
						p1++;
						*p1 = *p2;
						p2++;
						break;
					case '!':
						*p1->p = f(*p1->p);
						p2++;
						break;
					default:
						printf("need an operator\n");
						throw 0;
					}
					break;
				case '%':
					switch (p2->t) {
					case '+':
					case '-':
					case ')':
					case '*':
					case '/':
					case '%':
					case 0:
						*p1[-2].p = *p1[-2].p % *p1->p;
						delete p1->p;
						p1 -= 2;
						break;
					case '^':
						p1++;
						*p1 = *p2;
						p2++;
						p1++;
						*p1 = *p2;
						p2++;
						break;
					case '!':
						*p1->p = f(*p1->p);
						p2++;
						break;
					default:
						printf("need an operator\n");
						throw 0;
					}
					break;
				case '*':
					switch (p2->t) {
					case '+':
					case '-':
					case ')':
					case '*':
					case '/':
					case '%':
					case 0:
						*p1[-2].p = *p1[-2].p * *p1->p;
						delete p1->p;
						p1 -= 2;
						break;
					case '^':
						p1++;
						*p1 = *p2;
						p2++;
						p1++;
						*p1 = *p2;
						p2++;
						break;
					case '!':
						*p1->p = f(*p1->p);
						p2++;
						break;
					default:
						printf("need an operator\n");
						throw 0;
					}
					break;
				case '^':
					switch (p2->t) {
					case '+':
					case '-':
					case ')':
					case '*':
					case '/':
					case '%':
					case 0:
						*p1[-2].p = *p1[-2].p ^ *p1->p;
						delete p1->p;
						p1 -= 2;
						break;
					case '^':
						p1++;
						*p1 = *p2;
						p2++;
						p1++;
						*p1 = *p2;
						p2++;
						break;
					case '!':
						*p1->p = f(*p1->p);
						p2++;
						break;
					default:
						printf("need an operator\n");
						throw 0;
					}
					break;
				}
				break;
			case '(':
			case '+':
			case '-':
				p1++;
				*p1 = *p2;
				p2++;
				break;
			default:
				printf("need a number\n");
				throw 0;
			}
		}
		if (p1->p == NULL) {
			printf("need a number\n");
			throw 0;
		}
	}
	catch (...) {
		std::cout << "error\n";
		for (ele *p=pele; p<=p1; p++) {
			if (p->p) {
				delete p->p;
			}
		}
		for (ele *p = p2; p < pele+count; p++) {
			if (p->p) {
				delete p->p;
			}
		}
		return NULL;
	}
	return p1->p;
}

int main()
{
	size_t i,k;
	int j;
	char *s,s1;
	ele *pele;
	std::string sst;
	FILE *fi;
	struct stat fs;
	highint *n;
	ps = new store[2];
	ps->p = new highint;
	ps->s = "ans";
	ps[1].p = NULL;
	ps[1].s = NULL;
	ns = 2;
	ens = 1;
	for (;;) {
		j = -1;
		k = 0;
		std::cin >> sst;
		i = sst.size();
		try {
			s = new char[i + 1];
		}
		catch (...) {
			printf("no enough memory.\n");
			continue;
		}
		memcpy(s, sst.data(), i);
		s[i] = 0;
		sst.clear();
		if (!strcmp(s, "list")) {
			delete s;
			for (i = 1; i < ens; i++) {
				s = ps[i].p->print();
				printf("%s:%s\n", ps[i].s, s);
				delete s;
			}
			continue;
		}
		if (!strcmp(s, "quit")) {
			delete s;
			return 0;
		}
		if (i < 200) {
			if (!fopen_s(&fi, s, "r")) {
				stat(s, &fs);
				if (fs.st_size < 0x8001000) {
					delete s;
					s = new char[fs.st_size + 1];
					i = fread(s, 1, fs.st_size, fi);
					s[i] = 0;
				}
				fclose(fi);
			}
		}
		if ((*s <= 'z'&&*s >= 'a') || (*s <= 'Z'&&*s >= 'A')) {
			for (j = 0; j < i; j++) {
				s1 = s[j];
				if (s1 == '=') {
					s[j] = 0;
					for (k = 0; k < ens; k++) {
						if (!strcmp(s, ps[k].s)) {
							break;
						}
					}
					break;
				}
				if (!((s1 <= '9'&&s1 >= '0') || (s1 <= 'z'&&s1 >= 'a') || (s1 <= 'Z'&&s1 >= 'A'))) {
					j = -1;
					break;
				}
			}
			if (j == i) {
				j = -1;
			}
		}
		i = countele(s+j+1);
		try {
			pele = new ele[i + 2];
		}
		catch (...) {
			printf("no enough memory.\n");
			delete s;
			continue;
		}
		try {
			pretreat(pele + 1, s+j+1);
		}
		catch (...) {
			printf("no enough memory.\n");
			delete s;
			delete pele;
			continue;
		}
		pele[i + 1].p = NULL;
		pele[i + 1].t = 0;
		pele[0].p = NULL;
		pele[0].t = 0;
		n = calc(pele + 1, i);
		if (n) {
			delete ps->p;
			ps->p = n;
			if (k < ens) {
				if (k) {
					try {
						*ps[k].p = *n;
					}
					catch (...) {
						printf("updata failed,no enough memory");
					}
				}
			}
			else {
				try {
					ps[k].s = new char[j + 1];
				}
				catch (...) {
					printf("storing failed,no enough memory");
					goto next;
				}
				memcpy(ps[k].s, s, j);
				ps[k].s[j] = 0;
				try {
					ps[k].p = new highint(*n);
					ens++;
				}
				catch(...){
					printf("storing failed,no enough memory");
					delete ps[k].s;
				}
			}
			if (ens == ns) {
				try {
					ps1 = new store[ns * 2];
					memcpy(ps1, ps, ns * sizeof(store));
					delete ps;
					ps = ps1;
					ns += ns;
				}
				catch (...) {
					printf("storing failed,no enough memory");
					delete ps[k].s;
					delete ps[k].p;
					ens--;
				}
			}
			next:
			try {
				char* s1 = n->print();
				printf("%s\n", s1);
				delete s1;
			}
			catch (...) {
				printf("no enough memory to print");
			}
		}
		delete s;
		delete pele;
	}
	return 0;
}
