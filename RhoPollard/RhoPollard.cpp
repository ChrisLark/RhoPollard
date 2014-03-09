#include "RhoPollard.h"

int main(){
	Scalar a = "10101";
	Scalar b = "01101";  //01011 -> 01101
	Scalar c;
	//c = a.SCALMUL(b);
	c = a.SCALINV();
	std::cout << c.val << std::endl;
	getchar();
}

//3
//1010000
