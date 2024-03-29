﻿#include <list>
#include <vector>
#include "CppUnitTest.h"
#include "my_list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ListTests
{
	class Elem {
		int data;
		uint32_t watch;
		static size_t elem_counter;
		void check() const { Assert::IsTrue(watch == 0xDEADBEEF, L"Повреждение памяти!! (Обращение к неинициализированному экземпляру класса Elem)"); }
	public:
		Elem(const Elem&) = delete;
		Elem& operator=(const Elem&) = delete;
		explicit Elem(int value) : data(value), watch(0xDEADBEEF) { ++elem_counter; }
		Elem(Elem&& e) noexcept { e.check(); data = e.data; watch = e.watch; ++elem_counter; }
		Elem& operator=(Elem&& e) noexcept { check(); e.check(); data = e.data; watch = e.watch; }
		bool operator==(const Elem& e) const { check(); e.check(); return data == e.data; }
		~Elem() { check(); watch = 0; --elem_counter; }
		static size_t count() { return elem_counter; }
	};
	size_t Elem::elem_counter = 0;

	//  Для того, чтобы выполнить тестирование одного из указанных контейнеров (std::list или MyList),
	//  должна быть раскомментирована одна из следующих строк:
	//template<typename T> using ContainerTemplate = std::list<T>;
	template<typename T> using ContainerTemplate = MyList<T>;

	TEST_CLASS(ComplexTypeTests)
	{

	public:

		TEST_METHOD(IteratorTests)
		{
			ContainerTemplate<int> list{ 3, 4, 8, 5, 2, 13, 20 };
			list.push_back(10);
			Assert::IsTrue(list.end() == ++--list.end(), L"++--list.end() должен возвращать list.end()");
			Assert::AreEqual(10, *(--list.end()), L"--list.end() должен возвращать последний элемент");
		}

		TEST_METHOD(StringTests)
		{
			ContainerTemplate<std::string> list{ "123", "AAAAAAAA", "abc", "cde" };
			std::vector<std::string> expected1{ "123", "AAAAAAAA", "abc", "cde" };

			Assert::AreEqual(4ull, list.size(), L"Неправильно считается количество строковых элементов");
			Assert::IsTrue(std::equal(list.begin(), list.end(), expected1.begin(), expected1.end()), L"Неправильный порядок строковых элементов");
			Assert::IsTrue(std::equal(list.rbegin(), list.rend(), expected1.rbegin(), expected1.rend(), [](std::string s1, std::string s2) { return s1 == s2; }), L"Неправильный порядок строковых элементов");

			for (const auto& str : expected1)
				list.erase(std::find(list.begin(), list.end(), str));

			Assert::AreEqual(0ull, list.size(), L"Список не пуст после удаления всех элементов");
			for (const auto& elem : list)
				Assert::Fail(L"Что-то лежит в списке после удаления всех элементов");

			list.push_back("test-1");
			list.push_back("test-2");
			list.push_back("test-4");
			std::vector<std::string> expected2{ "test-1", "test-2", "test-4" };
			Assert::IsTrue(std::equal(list.begin(), list.end(), expected2.begin(), expected2.end()), L"Неправильный порядок строковых элементов");
			Assert::IsTrue(std::equal(list.rbegin(), list.rend(), expected2.rbegin(), expected2.rend()), L"Неправильный порядок строковых элементов");
		}

		TEST_METHOD(ElemTests)
		{
			size_t init_count = Elem::count();
			{
				ContainerTemplate<Elem> list;
				Assert::AreEqual(0ull, Elem::count() - init_count, L"Создан лишний экземпляр класса Elem");

				for (const auto& elem : list)
					Assert::Fail(L"Что-то лежит в пустом списке");

				list.push_back(Elem(40));
				list.push_back(Elem(75));
				list.push_front(Elem(50));
				list.push_back(Elem(23));
				list.push_back(Elem(87));
				Assert::IsTrue(Elem::count() - init_count == 5, L"Создан лишний экземпляр класса Elem");
				list.erase(std::find(list.begin(), list.end(), Elem(75)));
				Assert::IsTrue(Elem::count() - init_count == 4, L"Неправильно работает удаление элементов списка");
			}
			Assert::IsTrue(Elem::count() - init_count == 0, L"Утечка памяти!!");
		}

		TEST_METHOD(CountPosElemTest)
		{
			MyList<int>list{ 3, -4, -8, 5, 2, 13, 20 };
			Assert::IsTrue(list.CountPosElems() == 5);
		}

		TEST_METHOD(MaxElems)
		{
			MyList<int>list{ 3, -4, -8, 20, 2, 13, 20 };
			std::map<int, int> m{ {20, 2}};
			Assert::IsTrue(list.SearchMaxElemAndCountEntry() == m);
		}

		TEST_METHOD(Split)
		{
			MyList<int>list{ 3, -4, -8, 20, 2, 13, 20 };
			MyList<int>listNN{ 3, 20, 2, 13, 20 };
			MyList<int>listN{ -4, -8 };
			MyList<int>l1, l2;
			list.SplitNegativeAndNonNegative(l1, l2);
			Assert::IsTrue(listNN.size() == 5);
			Assert::IsTrue(listN.size() == 2);
		}

		TEST_METHOD(ReverseTest)
		{
			MyList<int>list{ 3, -4, -8, 20, 2, 13, 20 };
			list.reverse();
			Assert::IsTrue(list.front() == 20);
			Assert::IsTrue(list.back() == 3);
		}

		TEST_METHOD(PopFrontTest)
		{
			MyList<int>list{ 3, -4, -8, 20, 2, 13, 20 };
			list.pop_front();
			Assert::IsTrue(list.front() == -4);
		}
		TEST_METHOD(PopBackTest)
		{
			MyList<int>list{ 3, -4, -8, 20, 2, 13, 20 };
			list.pop_back();
			Assert::IsTrue(list.back() == 13);
		}

		TEST_METHOD(SwapTest)
		{
			MyList<int>list{ 3, -4, -8, 20, 2, 13, 20 };
			MyList<int>list1{ 5, 4, 3, 7, 1, 5, 7, 0, 0 };
			list.swap(list1);
			Assert::IsTrue(list1.back() == 20);
			Assert::IsTrue(list.back() == 0);
		}

		TEST_METHOD(PushTest)
		{
			MyList<int>list{ 3, -4, -8, 20, 2, 13, 20 };
			list.push_back(25);
			list.push_front(2);
			Assert::IsTrue(list.back() == 25);
			Assert::IsTrue(list.front() == 2);
		}
		
		TEST_METHOD(EraseTest)
		{
			MyList<int>list{ 3, -4, -8, 20, 2, 13, 20 };
			auto it = list.begin();
			++it;
			it = list.erase(it);
			Assert::IsTrue(*it == -8);
		}

		TEST_METHOD(InsertTest)
		{
			MyList<int>list{ 3, -4, -8, 20, 2, 13, 20 };
			auto it = list.begin();
			++it;
			it = list.insert(it, 155);
			Assert::IsTrue(*it == 155);
		}




	};
}
