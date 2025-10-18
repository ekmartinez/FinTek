#include <string>

#ifndef HELP
#define HELP

int daysInMonth();
void printHeader();
std::string getCurrentDate();
bool isValidDate(const std::string& date);
std::string  safeInput(std::size_t maxLen, const std::string&); // NW 


#endif
