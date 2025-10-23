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
#include "queries.h"
#include "personalFinanceSystem.h"

PersonalFinanceSystem::PersonalFinanceSystem(const std::string &path)
    : dbPath(path)
{
  if (sqlite3_open_v2(dbPath.c_str(), &db,
                      SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                      nullptr) != SQLITE_OK) {
      std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
      db = nullptr;
  } else {
      std::cout << "Database opened successfully.\n";
  }
}

PersonalFinanceSystem::~PersonalFinanceSystem()
{
  if (db)
  {
      sqlite3_close(db);
      std::cout << "Database closed.\n";
  }
}

void PersonalFinanceSystem::addCategory(const std::string &str)
{
	category.push_back(str);
	std::cout << "Added: " << str << std::endl;
}

void PersonalFinanceSystem::displayCategories()
{
	std::cout << "\nCategories\n" << "-----------\n";
	for (size_t i = 0; i < category.size(); ++i)  {
			std::cout << category[i] << ", \n";
	}
}

void PersonalFinanceSystem::updateCategory(const int id,
                                           const std::string newCategory)
{
	std::cout << "Hello from updateCategory()\n" << id << " " << newCategory << " " << std::endl;
}

void PersonalFinanceSystem::addTransaction(
    const std::string& date,
    const std::string& description,
    int categoryId,
    double amount,
    const std::string& type
) {
  if (!db)
  {
      std::cerr << "Database not initialized.\n";
      return;
  }
    // Minimal insert SQL
    std::string sql = SQL_INSERT_TRANSACTION;

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
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
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
    } else
    {
        std::cout << "Transaction added successfully.\n";
    }

    // exec
    sqlite3_finalize(stmt);
};

int PersonalFinanceSystem::findTransaction()
{
  sqlite3_stmt* stmt = nullptr;

  std::string sql = SQL_GET_ALL;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
  {
    std::cerr << "Failed to prepare query: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);

  }
  printf("%-8s %-12s %-25s %-15s %-10s %-10s\n",
         "Id", "Date", "Description", "Category", "Amount", "Type");

  while (sqlite3_step(stmt) == SQLITE_ROW)
  {
      int id = sqlite3_column_int(stmt, 0);
      const unsigned char* date = sqlite3_column_text(stmt, 1);
      const unsigned char* description = sqlite3_column_text(stmt, 2);
      const unsigned char* category = sqlite3_column_text(stmt, 3);
      double amount = sqlite3_column_double(stmt, 4);
      const unsigned char* type = sqlite3_column_text(stmt, 5);


        printf("%-8d %-12s %-25s %-15s %-10.2f %-10s\n",
            id,
            date ? (const char*)date : "",
            description ? (const char*)description : "",
            category ? (const char*)category : "",
            amount,
            type ? (const char*)type : "");
    }

    sqlite3_finalize(stmt);

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
