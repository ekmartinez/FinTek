#include <string>

#ifndef HELPERS_H
#define HELPERS_H

int daysInMonth();
std::string getCurrentYearMonth();
void printHeader();
void pressEnterToContinue();
void cPrintsBetterTables(int id, const std::string date, const std::string type, const std::string description, int categoryId, std::string categoryName, double amount);
std::string getCurrentDate();
bool isValidDate(const std::string& date);

int doubleLength(double value, int precision);


#endif
