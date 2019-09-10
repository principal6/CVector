#include "CVector.h"

int main()
{
	CVector a{ 1, 3, 5, 7, 9 };

	a.pop_back();
	a.pop_back();
	a.shrink_to_fit();

	return 0;
}