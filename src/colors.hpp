#ifndef _COLORS_HPP__
#define _COLORS_HPP__ 1 

#include <string>

class Color {
public:
    static std::string set_red(std::string s);
    static std::string set_green(std::string s);
    static std::string set_yellow(std::string s);
    static std::string set_blue(std::string s);
    static std::string set_magenta(std::string s);
    static std::string set_cyan(std::string s);
private:
    Color() = default;
};

#endif  
