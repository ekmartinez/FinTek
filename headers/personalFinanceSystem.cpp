#include <string>
#include <cerrno>
#include <vector>
#include <cstdio>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <exception>

#include "sqlite3.h"
#include "helpers.h"
#include "personalFinanceSystem.h"

double balance = 0.00;

std::string PersonalFinanceSystem::csvFilePath = "./storage/ledger.csv";
std::string PersonalFinanceSystem::csvTempPath = "./storage/ledger.tmp";
std::vector<std::string> categories = {"Income", "Food", "Rent", "Utilities", "Taxes", "Investments"};

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
    ;;
}

void PersonalFinanceSystem::updateCsv() {
    ;;
}

int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    // Return successful
    return 0;

}

void PersonalFinanceSystem::addTransaction(
    const std::string& date,
    const std::string& description,
    int categoryId,
    double amount,
    const std::string& type
) {
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;

    // Open database (create if it doesn't exist)
    if (sqlite3_open_v2(dbPath.c_str(), &db,
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Minimal insert SQL
    const char* sql = "INSERT INTO Transactions (Date, Description, CategoryId, Amount, Type) "
                      "VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // Bind variables
    sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, categoryId);
    sqlite3_bind_double(stmt, 4, amount);
    sqlite3_bind_text(stmt, 5, type.c_str(), -1, SQLITE_TRANSIENT);

    // Execute insert
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Transaction added successfully.\n";
    }

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
};

int PersonalFinanceSystem::searchTransaction(const int id = 0) {
/*
    // Header
    printf("%-8s %-12s %-25s %-15s %-10s",
            "Id", "Date", "Description", "Category", "Amount");

	if (id == 0) {
		// If Id 0 print all for menu 2 - Display Transactions
		for (const auto& ts : transactions) {
            cPrintsBetterTables(ts.id, ts.date, ts.description, ts.categoryId, ts.amout);
		}
	} else {
		// Else prints transaction by id.
		for (const auto& ts : transactions) {
			if (int(id) == ts.id) {
                cPrintsBetterTables(ts.id, ts.date, ts.desc, ts.cat, ts.amt);
			}
		}
	}

	*/

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
	transactions[i].description = newDesc;
}

void PersonalFinanceSystem::updateAmount(const int id, const double newAmt) {
	int i = findIndexById(id);
	transactions[i].amount = newAmt;
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
    for (auto& t : transactions) { balance += t.amount; }
    return balance ;
}
