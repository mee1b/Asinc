#include <algorithm>
#include <iostream>
#include <list>
#include <iterator>
#include <future>
#include <thread>

typedef unsigned int ui;

static int increment_vector(int value) 
{
	return value * 2;
}

template<typename It, typename Func>
void async_for_each(It first, It last, Func f) 
{
	const ui length = std::distance(first, last);
	if (!length)
		return;
	const ui min_per_thread = 10;
	if (length < (2 * min_per_thread)) 
	{
		std::for_each(first, last, f);
	}
	else 
	{
		It const mid_point = first + length / 2;
		std::future<void> first_half = std::async(&async_for_each<It, Func>, first, mid_point, f);
		std::future<void> last_half = std::async(&async_for_each<It, Func>, mid_point, last, f);
		first_half.get();
		last_half.get();
	}
}
static void printVector(std::vector<int>& l)
{
	for (const auto& elem : l) std::cout << elem << " ";
	std::cout << std::endl;
}
int main() 
{
	std::vector<int> v{ 1,4,6,9,12,20 };
	printVector(v);
	async_for_each(v.begin(), v.end(), [](int value) {std::cout << increment_vector(value) << " "; });

	return 0;
}