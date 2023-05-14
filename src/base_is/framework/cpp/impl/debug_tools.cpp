/**
  * @file debug_tools.cpp
  * @author Artiom N.(cl)2017
  * @brief Debug routines implementation.
  *
  */

#ifdef __GNUG__
#include <cstdlib>
#endif

#include <iomanip>
#include <iostream>
#include <memory>
#include <variant>

#ifdef __GNUG__
#include <cxxabi.h>
#endif

#include <tsw/types.h>


namespace tsw
{

namespace
{

using namespace tsw;
struct ValueVisualizeVisitor
{
    static const int depth_shift = 4;
    int depth_;

    ValueVisualizeVisitor(int depth = depth_shift) : depth_(depth)
    {
    }

    void print_spaces(int depth) const
    {
        for (int i = 0; i < depth; ++i) std::cout << ' ';
    }

    void operator()(const Undefined &) const
    {
        std::cout << "Undefined" << (depth_ > depth_shift ? "," : "") << std::endl;
    }

    void operator()(const Null &) const
    {
        std::cout << "Null" << (depth_ > depth_shift ? "," : "") << std::endl;
    }

    void operator()(const ValueArray &val) const
    {
        std::cout << "[" << std::endl;
        for (auto i: val)
        {
            print_spaces(depth_);
            std::visit(ValueVisualizeVisitor(depth_ + depth_shift), static_cast<const ValueBase&>(i));
        }
        print_spaces(depth_ - depth_shift);
        std::cout << "]" << (depth_ > depth_shift ? "," : "") << std::endl;
    }

    void operator()(const BinData &val) const
    {
        //print_spaces();
        std::cout << std::hex << "0x";
        for (auto i: val) std::cout << static_cast<int>(i);
        std::cout << std::dec << (depth_ > depth_shift ? "," : "") << std::endl;
    }

    void operator()(const String &val) const
    {
        std::cout << "\"" << val << "\"" << (depth_ > depth_shift ? "," : "") << std::endl;
    }

    void operator()(const Time &val) const
    {
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(val);
        auto ms = (val - seconds).count();
        auto tm = seconds.count();

        std::cout << std::put_time(std::gmtime(&tm), "%c")
                  << " + " << ms << "ms"
                  << (depth_ > depth_shift ? "," : "")  << std::endl;
    }

    void operator()(const Object &val) const
    {
        std::cout << "{" << std::endl;
        for (auto i: val)
        {
            print_spaces(depth_);
            std::cout << i.first << ": ";
            std::visit(ValueVisualizeVisitor(depth_ + depth_shift), static_cast<const ValueBase&>(i.second));
        }
        print_spaces(depth_ - depth_shift);
        std::cout << "}" << (depth_ > depth_shift ? "," : "") << std::endl;
    }

    void operator()(const bool &val) const
    {
        std::cout << (val ? "true" : "false") << std::endl;
    }

    void operator()(const Value &val) const
    {
        std::visit(ValueVisualizeVisitor(depth_), static_cast<const ValueBase&>(val));
    }

    template<typename T>
    void operator()(T val) const
    {
        std::cout << val << std::endl;
    }
};

} // namespace


void print_value(const Value& value)
{
    std::visit(ValueVisualizeVisitor(), static_cast<const ValueBase&>(value));
}


#ifdef __GNUG__
std::string demangle(const char* name) noexcept
{

    // Some arbitrary value to eliminate the compiler warning.
    int status = 0;

    // Enable c++11 by passing the flag -std=c++11 to g++.
    std::unique_ptr<char, void(*)(void*)> res
    {
        abi::__cxa_demangle(name, nullptr, nullptr, &status), std::free
    };

    return (status == 0) ? res.get() : name;
}
#else
// Does nothing if not g++.
std::string demangle(const char* name)
{
    return name;
}
#endif

} // namespace tsw
