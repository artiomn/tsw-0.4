/**
  * @file error.h
  * @author Artiom N.(cl)2017
  * @brief Exception class definition, assert's defines and other error-dealing stuff.
  *
  */

#ifndef _TSW_ERROR_H
#define _TSW_ERROR_H

#include <cassert>
#include <exception>
#include <string>
#include <typeinfo>

// Thanks to Alexey Konovalov.
#include <boost/current_function.hpp>


#ifndef NDEBUG
#   define TSW_ASSERT(expression) assert(expression)
#else
#   define TSW_ASSERT(expression) (void)(expression)
#endif

#define TSW_THROW(E, ...) (throw E(__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, E(__VA_ARGS__)))

namespace tsw
{

std::string demangle(const char* name) noexcept;

template<typename T>
class Exception : public std::exception
{
public:
    Exception() {}
    Exception(int error_code) : error_code_(error_code) {}
    Exception(int error_code, const std::string &error_message) :
        error_code_(error_code), message_(error_message) {}
    Exception(const std::string &error_message) : message_(error_message) {}
    Exception(const char *file, int line, const char *function, const T& e) :
        Exception(demangle(typeid(e).name()) +
                  " with a message: \"" + std::string(T(e).what()) + "\""
                  " in the function \"" + function + "\"" +
                  " [" + std::string(file) + ":" + std::to_string(line) + "]")
    {}
    virtual ~Exception() {}
public:
    virtual const char* what() const noexcept { return message_.c_str(); }
    virtual int error_code() const noexcept { return error_code_; }

private:
    int error_code_;
    std::string message_;
};

}
#endif // _TSW_ERROR_H
