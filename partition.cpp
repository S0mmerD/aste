#include <boost/python.hpp>
#include <iostream>

void func(){
    std::cout << "Hello World\n";
}

BOOST_PYTHON_MODULE(part)
{
    using namespace boost::python;
    def("func", func);
}
