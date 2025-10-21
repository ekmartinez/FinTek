#include <exception>
#include <string>
#include <cerrno>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <cstdio>


#include "helpers.h"
#include "personalFinanceSystem.h"

double balance = 0.00;

std::string PersonalFinanceSystem::csvFilePath = "./storage/ledger.csv";
std::string PersonalFinanceSystem::csvTempPath = "./storage/ledger.tmp";

void PersonalFinanceSystem::addCategory(const std::string& str) {
	category.push_back(str);
	std::cout << "Added: " << str << std::endl;
}

void PersonalFinanceSystem::displayCategories() {
	std::cout << "\nCategories\n" << "-----------\n";
	for (size_t i = 0; i < category.size(); ++i)  {
			std::cout << category[i] << ", \n";
	}
}

void PersonalFinanceSystem::updateCategory(const int id, const std::string newCategory) {
	std::cout << "Hello from updateCategory()\n" << id << " " << newCategory << " " << std::endl;
}

void PersonalFinanceSystem::AddToCsv(const int id, const std::string date, const std::string desc, const std::string cat, double amt) {

    std::fstream fout;
    fout.open(csvFilePath, std::ios::out | std::ios::app);
    fout << id << ", " << date << ", " << desc << ", " << cat << ", " << amt << std::endl;

}

void PersonalFinanceSystem::loadFromCsv() {
    transactions.clear();

    std::ifstream file("storage/ledger.csv");
    if (!file.is_open()) {
        std::cerr << "loadFromCsv: ERROR — could not open file \"storage/ledger.csv\". errno=" << errno << '\n';
        return;
    }

    auto trim_cr = [](std::string &s) {
        if (!s.empty() && s.back() == '\r') s.pop_back();
        // optionally trim BOM at start
        if (s.size() >= 3 && (unsigned char)s[0] == 0xEF && (unsigned char)s[1] == 0xBB && (unsigned char)s[2] == 0xBF) {
            s.erase(0, 3);
        }
    };

    std::string line;
    // Read header (if there is one) and show it
    if (!std::getline(file, line)) {
        std::cerr << "loadFromCsv: file is empty\n";
        return;
    }
    trim_cr(line);
    std::cerr << "loadFromCsv: header: \"" << line << "\"\n";

    int lineno = 1;
    while (std::getline(file, line)) {
        ++lineno;
        trim_cr(line);
        if (line.empty()) {
            std::cerr << "loadFromCsv: skipping empty line " << lineno << '\n';
            continue;
        }

        std::istringstream iss(line);
        std::string id_str, amt_str;
        Transactions t;                // use a short local name, not 'transactions'

        // id
        if (!std::getline(iss, id_str, ',')) {
            std::cerr << "loadFromCsv: malformed line " << lineno << " (missing id): \"" << line << "\"\n";
            continue;
        }
        try {
            t.id = std::stoi(id_str);
        } catch (const std::exception &e) {
            std::cerr << "loadFromCsv: stoi failed on line " << lineno << " id=\"" << id_str << "\" -> " << e.what() << '\n';
            continue;
        }

        // date, desc, cat (these may be empty)
        if (!std::getline(iss, t.date, ',')) t.date.clear();
        if (!std::getline(iss, t.desc, ',')) t.desc.clear();
        if (!std::getline(iss, t.cat, ',')) t.cat.clear();

        // amount
        if (!std::getline(iss, amt_str, ',')) amt_str.clear();
        try {
            t.amt = amt_str.empty() ? 0.0 : std::stod(amt_str);
        } catch (const std::exception &e) {
            std::cerr << "loadFromCsv: stod failed on line " << lineno << " amt=\"" << amt_str << "\" -> " << e.what() << '\n';
            t.amt = 0.0; // fallback
        }

        transactions.push_back(std::move(t));
        std::cerr << "loadFromCsv: loaded line " << lineno << " id=" << transactions.back().id << '\n';
    }

    std::cerr << "loadFromCsv: finished. total loaded = " << transactions.size() << '\n';
}

void PersonalFinanceSystem::updateCsv() {

    std::ofstream file(csvTempPath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open temporary file for writing.\n";
        return;
    }

    file << "Id,Date,Description,Category,Amount\n";

    for (const auto &t : transactions) {
        // Escape any commans or quotes
        file << t.id << ','
            << t.date << ','
            << t.desc << ','
            << t.cat << ','
            << t.amt << '\n';
    }

    file.close();

    if (std::rename(csvTempPath.c_str(), csvFilePath.c_str()) != 0) {
        perror("Error replacing CSV file");
    } else {
        std::cout << "CSV successfully updated.\n";
    }

}

void PersonalFinanceSystem::addTransaction(int id, const std::string& date,
	const std::string& desc, const std::string& cat, const double amt) {

	Transactions newTransaction = {id, date, desc, cat, amt};
	transactions.push_back(newTransaction);
    //AddToCsv(id, date, desc, cat, amt); // use update csv instead
	std::cout << "Data saved.;";
 }

int PersonalFinanceSystem::searchTransaction(const int id = 0) {

    // Header
    printf("%-8s %-12s %-25s %-15s %-10s",
            "Id", "Date", "Description", "Category", "Amount");

	if (id == 0) {
		// If Id 0 print all for menu 2 - Display Transactions
		for (const auto& ts : transactions) {
            cPrintsBetterTables(ts.id, ts.date, ts.desc, ts.cat, ts.amt);
		}
	} else {
		// Else prints transaction by id.
		for (const auto& ts : transactions) {
			if (int(id) == ts.id) {
                cPrintsBetterTables(ts.id, ts.date, ts.desc, ts.cat, ts.amt);
			}
		}
	}

	return 0;
}

int PersonalFinanceSystem::findIndexById(int targetId) {

	for (size_t i = 0; i < transactions.size(); ++i) {
		if (transactions[i].id == targetId) {
			return static_cast<int>(i);
		}
	}

	return -1;
}

void PersonalFinanceSystem::updateDate(const int id, const std::string newDate) {

	int index = findIndexById(id);

	if (isValidDate(newDate)) {
		transactions[index].date = newDate;
	} else { std::cout << "Something went wrong in updateDate()\n"; }
}

void PersonalFinanceSystem::updateDescription(const int id, const std::string newDesc) {
	int i = findIndexById(id);
	transactions[i].desc = newDesc;
}

void PersonalFinanceSystem::updateAmount(const int id, const double newAmt) {
	int i = findIndexById(id);
	transactions[i].amt = newAmt;
}

int PersonalFinanceSystem::deleteTransactionById(int targetId) {
	auto it = std::remove_if(transactions.begin(), transactions.end(),
				[targetId](const Transactions& t) { return t.id == targetId; });

	if (it != transactions.end()) {
		transactions.erase(it, transactions.end());
		std::cout << "Transaction with ID " << targetId << " deleted.\n";
		return 1;
	}

	return -1;
}

void PersonalFinanceSystem::summaryReport() {
	std::cout << "Hello from summaryReport()\n";
}

int PersonalFinanceSystem::getLastId() {
    if (transactions.empty()) {
        return 0;
	} else { return transactions.back().id; }
}

double PersonalFinanceSystem::getBalance() {
    balance = 0;
    for (auto &t : transactions) { balance += t.amt; }
    return balance ;
}
