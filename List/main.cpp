#include <iostream>
#include <list>
#include "my_list.h"


int main(int argc, char** argv)
{
	MyList<int> my_list;
	//time_t start = clock();
	MyList<int> list{ };
	std::list<int> l1{ };
	for (int i = 0; i < 100000000; i++)
		//l1.push_back(i); // Time: 3.866 seconds
		list.push_back(i); // Time: 4.039 seconds 
	time_t start = clock();
	//for (int i = 0; i < 100000000; i++)
	//	l1.pop_back(); // time: 2.721 seconds
	//	list.pop_back(); // time: 2.702 seconds

	//l1.reverse(); // Time: 0.866 seconds
	//list.reverse(); // Time: 0.833 seconds
	auto it = list.begin();
	for (int i = 0; i < 100000000; i++)
		list.insert(it, i); // Time: 3.502 seconds
		//l1.insert(it, 1); // Time: 4.114 seconds

	std::cout << "Time: " << (double(clock() - start) / CLOCKS_PER_SEC) << " seconds" << std::endl;

	MyList<int> list1{ 3, -4, -8, 20, 2, 13, 2 };
	list1.reverse();
	std::cout << list1.front();

	// Тут можно проверять работоспособность методов, но для
	// каждого метода надо описать тест в проекте с тестами
}
