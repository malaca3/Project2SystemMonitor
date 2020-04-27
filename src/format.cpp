#include <string>
#include <iomanip>
#include <string>  
#include <iostream> 
#include <sstream> 

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
    std::stringstream buffer;
    int hours = seconds/3600;
    int minutes = (seconds%3600)/60;
    int seconds_el = (seconds%3600)%60;    
    buffer << std::setfill('0') << std::setw(2) << hours << ':' << std::setfill('0') << std::setw(2) << minutes
              << ':' << std::setfill('0') << std::setw(2) << seconds_el;

    return buffer.str();

}

std::string Format::to_string_with_precision(const float a_value, const int n )
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}