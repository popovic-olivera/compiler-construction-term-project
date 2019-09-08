#include "colors.hpp"

#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"

std::string Color::set_red(std::string s)  
{
    return RED + s + RESET;
}
    
std::string Color::set_green(std::string s)  
{
    return GREEN + s + RESET;
}
    
std::string Color::set_yellow(std::string s)  
{
    return YELLOW + s + RESET;
}

std::string Color::set_blue(std::string s)  
{
    return BLUE + s + RESET;
}

std::string Color::set_magenta(std::string s)  
{
    return MAGENTA + s + RESET;
}

std::string Color::set_cyan(std::string s)  
{
    return CYAN + s + RESET;
}
