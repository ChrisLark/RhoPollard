#include <bitset>
#include <vector>
#include <iostream>
#include <math.h>
#include <time.h>


class RhoPollard{
public:
	RhoPollard();
	static const int m = 5;  //163-4 //5 - 2
	static const int T = 2;
	static const int p = T * m + 1;
	int F[p];
	int dF[p];
};

class Scalar : public RhoPollard{
public:
	std::bitset<m> val;
	Scalar SCALMUL(Scalar b);
	Scalar SCALADD(Scalar b);
	Scalar SCALSQ();
	Scalar SCALPOWn(int n);
	Scalar SCALINV();
	Scalar& operator=(Scalar const &a){
		this->val = a.val;
		return *this;
	}
	Scalar(){};
	Scalar(std::string hexval);
};

class Point : public RhoPollard{
public:
	Scalar x;
	Scalar y;
	Point ECCMUL(Scalar k, Point P);
	Point ECCADD(Point P, Point Q);
	Point ECCDOUB(Point P);
	Point& operator=(Point const &a){
		this->x.val = a.x.val;
		this->y.val = a.y.val;
		return *this;
	};
	Point(){};
	Point(std::string hexval_x, std::string hexval_y);
};



//Scalar Scalar::SCALMUL(Scalar b){  //NIST multiplication
//	Scalar c;
//	std::cout << "c: " << c.val << std::endl;
//
////	std::cout << "F: " << c.F[3] << std::endl;
//	for (int i = 0; i <= m - 1; i++){
//		for (int n = 1; n <= p - 2; n++) c.val[i] = c.val[i] ^ this->val[F[n + 1]] | b.val[F[p - n]];
//		this->val = this->val << 1;
//		b.val = b.val << 1;
//	}
//	return c;
//}

Scalar Scalar::SCALMUL(Scalar b){ 
	Scalar Sa = *this;
	Scalar Sb = b;
	Scalar C,R;
	for (int n = 1; n <= p - 2; n++){
		Sa.val = (Sa.val << dF[n]) | (Sa.val >> (Sa.val.size() - dF[n]));
		R.val = Sa.val & Sb.val;
		C.val = C.val ^ R.val;
		Sb.val = (Sb.val << dF[n]) | (Sb.val >> (Sb.val.size() - dF[n]));
		}
	return C;
}

Scalar Scalar::SCALADD(Scalar b){
	Scalar c;
	c.val = this->val ^ b.val;
	return c;
}

Scalar Scalar::SCALSQ(){
	this->val = (this->val >> 1) | (this->val << (this->val.size() - 1));
	return *this;
}

Scalar Scalar::SCALPOWn(int n){
	this->val = (this->val >> n) | (this->val << (this->val.size() - n));
	return *this;
}

Scalar Scalar::SCALINV(){
	Scalar b = "1";
	*this = this->SCALSQ();
	int x = (m - 1) / 2;
	while (x != 0){
		*this = this->SCALMUL(this->SCALPOWn(x));
		if (x % 2 == 0)	x = x / 2;
		else{
			b = b.SCALMUL(*this);
			*this = this->SCALSQ();
			x = (x - 1) / 2;
		}
	}
	return b;
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
	Scalar lambda, temp1, temp2;
	if (P.x.val == 0 && P.y.val == 0) S = Q;
	if (Q.x.val == 0 && Q.y.val == 0) S = P;
	if (P.x.val != Q.x.val){
		temp1.val = P.y.val & ~Q.y.val;
		temp1 = temp1.SCALINV();
		temp2.val = P.y.val & ~Q.y.val;
		lambda = temp2.SCALMUL(temp1); 
	}
	return S;
}

Point Point::ECCDOUB(Point P){
	Point S;
	return S;
}

RhoPollard::RhoPollard(){
	int n, g, b, j, u;
	srand(time(NULL));
	for (int i = 0; i < p; i++) F[i] = 0;
	while (true){
		g = rand() % (p-1) + 2;
		//b = g;
		//j = 1;
		//while (true){
		//	b = (g * b) % p;
		//	j = j + 1;
		//	if (!(b>1)) break;
		//}
		//if (j%T == 0){
		//	u = (int)pow((double)g, double(j / T)) % p;
		//	break;
		//}
		if ((int)pow((double)g, double(T)) % p == 1){
			u = g;
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
	for (int z = 1; z <= p; z++);
	for (int n = 1; n <= p - 2; n++) dF[n] = ((F[n + 1] - F[n] % m)+m)%m;  //bo liczby ujemne :<

	//for (int z = 1; z <= p - 1; z++)
	//	std::cout << "F[" << z << "]: " << F[z] << std::endl;
	//for (int z = 1; z <= p - 1; z++)
	//	std::cout << "dF[" << z << "]: " << dF[z] << std::endl;
		
	//}
}

Point::Point(std::string hexval_x, std::string hexval_y){
	this->x.val = std::bitset<m>(hexval_x);
	this->y.val = std::bitset<m>(hexval_y);
}

Scalar::Scalar(std::string hexval){
	this->val = std::bitset<m>(hexval);
}