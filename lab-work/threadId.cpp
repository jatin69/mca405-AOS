#include <iostream>
#include <thread>
#include <string>

using namespace std;

void thread_function(string s){
    std::cout << "thread function : \n" << s << "\n";
}

int main()
{
    std::string s = "Kathy Perry";
    std::thread t(&thread_function, std::move(s));
    std::cout << "main thread message = " << s << std::endl;

    std::cout << "main thread id = " << std::this_thread::get_id() << std::endl;
    std::cout << "child thread id = " << t.get_id() << std::endl;

    t.join();
    return 0;
}