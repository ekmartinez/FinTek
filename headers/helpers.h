#include <string>

#ifndef HELPERS_H
#define HELPERS_H

int daysInMonth();
void printHeader();
void cPrintsBetterTables(const int id, const std::string dt, const std::string desc, const std::string cat, const double amt);
std::string getCurrentDate();
bool isValidDate(const std::string& date);
std::string  safeInput(std::size_t maxLen, const std::string&); // NW 


#endif
