#include <ctime> 
#include <regex>				 
#include <iomanip>				 
#include <sstream>
#include <iostream>

#include "helpers.h"

void printHeader() {
	system("clear");

	std::cout <<"\n"
	<< "########:'####:'##::: ##:'########:'########:'##:::'##::::::::::'##::::'##::::\n"
	<< "##.....::. ##:: ###:: ##:... ##..:: ##.....:: ##::'##:::::::::::. ##:::. ##:::\n"
	<< "##:::::::: ##:: ####: ##:::: ##:::: ##::::::: ##:'##:::::::::::::. ##:::. ##::\n"
	<< "######:::: ##:: ## ## ##:::: ##:::: ######::: #####::::'#######:::. ##:::. ##:\n"
	<< "##...::::: ##:: ##. ####:::: ##:::: ##...:::: ##. ##:::........::: ##:::: ##::\n"
	<< "##:::::::: ##:: ##:. ###:::: ##:::: ##::::::: ##:. ##:::::::::::: ##:::: ##:::\n"
	<< "##:::::::'####: ##::. ##:::: ##:::: ########: ##::. ##:::::::::: ##:::: ##::::\n"
	<< "E..::::::::...::..::::..:::::..:::::.......Z:..::::..::::::::::..:::::..:::::W\n";

	std::cout << "\nFinTek - Personal Finance System\n" << std::endl;

}

void cPrintsBetterTables(const int id, const std::string dt, const std::string desc, const std::string cat, const double amt)
{

	printf("\n%-8d %-12s %-25s %-15s %-10.2f",
		id, dt.c_str(), desc.c_str(), cat.c_str(), amt);

}

std::string getCurrentDate() {
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(now, "%Y-%m-%d");

	return oss.str();
}

int daysInMonth(int year, int month) {
	static const int baseDays[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	if (month < 1 || month > 12) { return 0; }

	int days = baseDays[month - 1]; // why months

	// Adjust February for leap years
	bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	if (month == 2 && isLeap) { days = 29; }

	return days;
}

// Validate date string in "YYYY-MM-DD"
bool isValidDate(const std::string& date) {
	static const std::regex pattern(R"(^(\d{4})-(\d{2})-(\d{2}))");
	std::smatch match;

	// Check basic format
	if (!std::regex_match(date, match, pattern)) { return false; }

	int year = std::stoi(match[1]);
	int month = std::stoi(match[2]);
	int day = std::stoi(match[3]);

	// Get days in the given month/year
	int maxDay = daysInMonth(year, month);

	// If month or day are invalid, return false
	if (maxDay == 0 || day < 1 || day > maxDay) { return false; }

	return true;
}

void safeInput() {
    // Sanitize txt input    
	;;
}


