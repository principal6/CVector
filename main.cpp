#include "CVector.h"

int main()
{
	CVector a{ 1, 3, 5, 7, 9 };
	a.pop_back();
	a.pop_back();
	a.shrink_to_fit();

	CVector b{ a.rbegin(), a.rend() };

	for (auto& i : a)
	{
		std::cout << "[" << i << "]";
	}

	for (auto it = a.rbegin(); it != a.rend(); ++it)
	{
		std::cout << "[" << *it << "]";
	}

	for (auto it = a.cbegin(); it != a.cend(); ++it)
	{
		std::cout << "[" << *it << "]";
	}

	for (auto it = a.crbegin(); it != a.crend(); ++it)
	{
		std::cout << "[" << *it << "]";
	}

	return 0;
}