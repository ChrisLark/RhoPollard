#include <bitset>
#include <iostream>
#include <math.h>
#include <time.h>


class RhoPollard{
public:
	RhoPollard();
	static const int m = 4;
	static const int T = 3;
	static const int p = T * m + 1;
	int F[p];
};

class Scalar : public RhoPollard{
public:
	std::bitset<m> val;
	Scalar SCALMUL(Scalar a, Scalar b);
	Scalar SCALADD(Scalar a, Scalar b);
	Scalar SCALSQ(Scalar a);
};

class Point : public RhoPollard{
public:
	Scalar x;
	Scalar y;
	Point ECCMUL(Scalar k, Point P);
	Point ECCADD(Point P, Point Q);
	Point ECCDOUB(Point P);
	Point(){};
	Point(std::string hexval_x, std::string hexval_y);
};



Scalar Scalar::SCALMUL(Scalar a, Scalar b){  //NIST multiplication
	Scalar c;
	for (int i = 0; i <= m - 1; i++){
		for (int n = 1; n <= p - 2; n++) c.val[i] = c.val[i] ^ a.val[F[n + 1]] | b.val[F[p - n]];
		a.val = a.val << 1;
		b.val = b.val << 1;
	}
	return c;
}

Scalar SCALADD(Scalar a, Scalar b){
	Scalar c;
	c.val = a.val ^ b.val;
	return c;
}

Scalar SCALSQ(Scalar a){
	a.val = (a.val >> 1) | (a.val << (a.val.size() - 1));
	return a;
}

Point Point::ECCMUL(Scalar k, Point P){
	Point S = P;
	Point T = P;
	int i;
	for (i = m - 2; i >= 0; i--){
		if (k.val[i] == 1){
			S = ECCADD(S, T);
			T = ECCDOUB(T);
		}
		else{
			S = ECCDOUB(S);
			T = ECCADD(S, T);
		}
	}
	return S;
}

Point Point::ECCADD(Point P, Point Q){
	Point S;

	return S;
}

Point Point::ECCDOUB(Point P){
	Point S;
	return S;
}

RhoPollard::RhoPollard(){
	int n, g, b, j, u;
	srand(time(NULL));
	while (true){
		g = rand() % p + 1;
		b = g;
		j = 1;
		while (true){
			b = (g * b) % p;
			j = j + 1;
			if (!(b>1)) break;
		}
		if (j%T == 0){
			u = (int)pow((double)g, double(j / T)) % p;
			break;
		}
	}
	int w = 1;
	for (int j = 0; j <= T - 1; j++){
		n = w;
		for (int i = 0; i <= m - 1; i++){
			F[n] = i;
			n = (2 * n) % p;
		}
		w = (u * w) % p;
	}
	for (int z = 1; z < p; z++);
}

Point::Point(std::string hexval_x, std::string hexval_y){
	x.val = std::bitset<m>(hexval_x);
	y.val = std::bitset<m>(hexval_y);
}