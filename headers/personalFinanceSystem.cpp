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

int PersonalFinanceSystem::addCategory(const std::string& categoryName)
{
    if (!db) { return -1; }

    const char* sql = SQL_GET_CATEGORY_ID;
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    sqlite3_bind_text(stmt, 1, categoryName.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    return static_cast<int>(sqlite3_last_insert_rowid(db));
}

int PersonalFinanceSystem::getCategoryId(const std::string &categoryName)
{
    if (!db) { return -1; }

    const char* sql = SQL_GET_CATEGORY_ID;
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    sqlite3_bind_text(stmt, 1, categoryName.c_str(), -1, SQLITE_TRANSIENT);

    int categoryId = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        categoryId = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    return categoryId;
}

void PersonalFinanceSystem::updateCategory(const int id, const std::string newCategory)
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

int PersonalFinanceSystem::findTransaction(int id)
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

int PersonalFinanceSystem::deleteTransactionById(int idToDelete)
{
    sqlite3_stmt* stmt = nullptr;


    if (sqlite3_prepare_v2(db, SQL_DELETE_TRANSACTION, -1, &stmt, nullptr) !=
        SQLITE_OK)
    {
        std::cerr << "Failed to prepare delete statement:" << sqlite3_errmsg(db) << std::endl;
    }

    // Bind the ID
    sqlite3_bind_int(stmt, 1, idToDelete);

    // Execute
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Error deleting transaction: %s\n", sqlite3_errmsg(db));
        std::cerr << "Error deleting transaction." << sqlite3_errmsg(db) << std::endl;
    } else
    {
      if (sqlite3_changes(db) > 0)
      {
         std::cout << "\nTransaction with ID " << idToDelete << " deleted successfully.\n";
      } else {
          std::cout << "\nNo transaction found wit ID " << idToDelete << "." << std::endl;
      }

    }
    sqlite3_finalize(stmt);

	return 0;
}

void PersonalFinanceSystem::loadTransactionFromDB()
{
    sqlite3_stmt *stmt = nullptr;

    transactions.clear();
    int rc = sqlite3_prepare_v2(db, SQL_GET_ALL, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare SELECT statement: "
            << sqlite3_errmsg(db) << std::endl;

        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
      Transactions t;
      t.id = sqlite3_column_int(stmt, 0);
      t.date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
      t.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
      t.categoryId = sqlite3_column_int(stmt, 3);
      t.amount = sqlite3_column_double(stmt, 4);
      t.type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

      transactions.push_back(t);
    }

    if (rc != SQLITE_DONE)
    {
        std::cerr << "Error retrieving transactions: "
            << sqlite3_errmsg(db) << std::endl;
    } else {
      std::cout << "Loaded "<< transactions.size()
                << " transactions from database." << std::endl;
    }
    sqlite3_finalize(stmt);
}

int PersonalFinanceSystem::findIndexById(int targetId)
{
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
