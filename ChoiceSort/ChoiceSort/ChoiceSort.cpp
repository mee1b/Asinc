#include <algorithm>
#include <iostream>
#include <list>
#include <iterator>
#include <future>
#include <thread>

static void findMinimum(std::list<int>& l, std::promise<int>& min_elem) 
{
    min_elem.set_value(*std::min_element(begin(l), end(l)));
}

static void choiceSort(std::list<int>& l, std::promise<std::string> value) 
{
    for (int j = 0; j < l.size(); j++) 
    {
        auto begin = std::next(l.begin(), j);
        auto end = l.end();
        std::promise<int> min_elem;
        std::future<int> result = min_elem.get_future();
        std::list<int> subList;
        subList.splice(subList.begin(), l, begin, end);
        
        std::async(std::launch::async, findMinimum, ref(subList), ref(min_elem));
        int m = result.get();

        if (m != *subList.begin()) 
        {
            subList.insert(std::find(subList.begin(), subList.end(), m), *subList.begin());
            subList.pop_front();
            subList.remove(m);
            subList.push_front(m);
        }
        l.splice(end, subList);
    }
    value.set_value("Sorting is completed");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

static void printList(std::list<int>& l) 
{
    for (const auto& elem : l) std::cout << elem << " ";
    std::cout << std::endl;
}

int main()
{

    std::list<int> list1 = { 2, 7, 5, 17, 26, 32, 1, 4, 8 };
    std::promise<std::string> value;
    std::future<std::string> valueFuture = value.get_future();

    std::thread Asyncthread1(std::move(choiceSort), std::ref(list1), std::move(value));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << valueFuture.get() << std::endl;

    Asyncthread1.join();
    printList(list1);
    return 0;
}