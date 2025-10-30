#include <string>
#include <cerrno>
#include <vector>
#include <cstdio>
#include <iostream>
#include <algorithm>

#include "sqlite3.h"
#include "helpers.h"
#include "queries.h"
#include "personalFinanceSystem.h"

// Constructor
PersonalFinanceSystem::PersonalFinanceSystem(const std::string &path)
    : dbPath(path), db(nullptr)
{
        std::cout << "Initialized\n";
}

// Destructor
PersonalFinanceSystem::~PersonalFinanceSystem() {
  if (db) {
    sqlite3_close(db);
    db = nullptr;
  }
}

bool PersonalFinanceSystem::openDB() {
    if (sqlite3_open_v2(dbPath.c_str(), &db,
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                        nullptr) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    return true;
}

void PersonalFinanceSystem::closeDB() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

int PersonalFinanceSystem::addCategory(const std::string& categoryName)
{
    // Open DB for this operation
    if (!openDB()) {
        std::cerr << "Failed to open database in addCategory().\n";
        return -1;
    }

    sqlite3_stmt* stmt = nullptr;
    int categoryId = -1;

    // 1) Try to find existing category id
    const char* sqlSelect = R"SQL(
        SELECT CategoryId FROM Category WHERE CategoryDescription = ?;
    )SQL";

    if (sqlite3_prepare_v2(db, sqlSelect, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare SELECT failed: " << sqlite3_errmsg(db) << std::endl;
        closeDB();
        return -1;
    }

    if (sqlite3_bind_text(stmt, 1, categoryName.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        std::cerr << "Bind SELECT failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        closeDB();
        return -1;
    }

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        categoryId = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        closeDB();
        return categoryId;
    } else if (rc != SQLITE_DONE) {
        std::cerr << "SELECT step error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        closeDB();
        return -1;
    }
    sqlite3_finalize(stmt);

    // 2) Not found -> INSERT the category
    const char* sqlInsert = R"SQL(
        INSERT INTO Category (CategoryDescription) VALUES (?);
    )SQL";

    if (sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare INSERT failed: " << sqlite3_errmsg(db) << std::endl;
        closeDB();
        return -1;
    }

    if (sqlite3_bind_text(stmt, 1, categoryName.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        std::cerr << "Bind INSERT failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        closeDB();
        return -1;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "INSERT failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        closeDB();
        return -1;
    }

    // get the last inserted row id (CategoryId)
    sqlite3_finalize(stmt);
    categoryId = static_cast<int>(sqlite3_last_insert_rowid(db));

    closeDB();
    return categoryId;
}

int PersonalFinanceSystem::getCategoryId(const std::string& categoryName)
{
    // Open database connection
    if (!openDB())
    {
        std::cerr << "Failed to open database in getCategoryId().\n";
        return -1;
    }

    const char* sql = SQL_GET_CATEGORY_ID;
    sqlite3_stmt* stmt = nullptr;

    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
        closeDB();
        return -1;
    }

    // Bind parameter (category name)
    if (sqlite3_bind_text(stmt, 1, categoryName.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        std::cerr << "Bind failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        closeDB();
        return -1;
    }

    // Execute the query
    int categoryId = -1;
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        categoryId = sqlite3_column_int(stmt, 0);
    } else if (rc != SQLITE_DONE) {
        std::cerr << "Step failed: " << sqlite3_errmsg(db) << std::endl;
    }

    // Cleanup
    sqlite3_finalize(stmt);
    closeDB();

    return categoryId;
}

void PersonalFinanceSystem::addTransaction(
    const std::string& date,
    const std::string& description,
    int categoryId,
    double amount,
    const std::string& type
) {
    // Open DB connection
    if (!openDB()) {
        std::cerr << "Failed to open database in addTransaction().\n";
        return;
    }

    sqlite3_stmt* stmt = nullptr;

    // Prepare the SQL
    if (sqlite3_prepare_v2(db, SQL_INSERT_TRANSACTION, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        closeDB();
        return;
    }

    // Bind parameters
    if (sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 3, categoryId) != SQLITE_OK ||
        sqlite3_bind_double(stmt, 4, amount) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 5, type.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
    {
        std::cerr << "Failed to bind parameter: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        closeDB();
        return;
    }

    // Execute insert
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Transaction added successfully.\n";
    }

    // Cleanup
    sqlite3_finalize(stmt);
    closeDB();
}

int PersonalFinanceSystem::findTransaction(int id = 0)
{
  if (id == 0) {
      int w = 120;
      do { std::cout << "-"; } while ( w-- > 0 );
      // C Prints Better tables - Headers
      printf("\n%-8s %-12s %-8s %-25s %-8s %-25s %-10s\n", "| Id      ",
             "| Date        ", "| Type    ", "| Description              ",
             "| CategoryId",
             " | CategoryName             ", "| Amount     |");

        w = 120;
        do { std::cout << "-"; } while ( w-- > 0 );

        // If Id 0 print all transactions
        for (const auto& ts : transactions)
        {
            printf("\n| %-8d | %-12s | %-8s | %-25s | %-11d | %-25s | %-10.2f |",
                ts.id, ts.date.c_str(), ts.type.c_str(), ts.description.c_str(), ts.categoryId, ts.categoryName.c_str(), ts.amount);
        }

        std::cout << "\n";

        w = 120;
        do { std::cout << "-"; } while ( w-- > 0 );

  } else {
      int w = 120;
      do { std::cout << "-"; } while ( w-- > 0 );
      // C Prints Better tables - Headers
      printf("\n%-8s %-12s %-8s %-25s %-8s %-25s %-10s\n", "| Id      ",
             "| Date        ", "| Type    ", "| Description              ",
             "| CategoryId",
             " | CategoryName             ", "| Amount     |");

        w = 120;
        do { std::cout << "-"; } while ( w-- > 0 );

		// Else prints transaction by id.
        for (const auto& ts : transactions)
        {
          if (int(id) == ts.id) {
              printf("\n| %-8d | %-12s | %-8s | %-25s | %-11d | %-25s | %-10.2f |",
                ts.id, ts.date.c_str(), ts.type.c_str(), ts.description.c_str(), ts.categoryId, ts.categoryName.c_str(), ts.amount);
            }
		}

        std::cout << "\n";

        w = 120;
        do { std::cout << "-"; } while ( w-- > 0 );
	}

	return 0;
}

int PersonalFinanceSystem::deleteTransactionById(int idToDelete)
{
    // Open the database connection
    if (!openDB())
    {
        std::cerr << "Failed to open database in deleteTransactionById().\n";
        return -1;
    }

    const char *sql = "DELETE FROM Transactions WHERE TransactionId = ?;";

    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare delete statement: "
                  << sqlite3_errmsg(db) << std::endl;
        closeDB();
        return -1;
    }

    // Bind the TransactionId parameter
    sqlite3_bind_int(stmt, 1, idToDelete);

    // Execute
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Error deleting transaction: "
                  << sqlite3_errmsg(db) << std::endl;
    } else {
        int changes = sqlite3_changes(db);
        if (changes > 0) {
            std::cout << "\nTransaction with ID " << idToDelete
                      << " deleted successfully.\n";
        } else {
            std::cout << "\nNo transaction found with ID "
                      << idToDelete << ".\n";
        }
    }

    // Cleanup
    sqlite3_finalize(stmt);
    closeDB();
    return 0;
}

bool PersonalFinanceSystem::updateRecord(int transactionId,
                                         const std::string& field,
                                         const std::string& newValue)
{
    if (!openDB()) { return -1; }

    sqlite3_stmt* stmt = nullptr;

    // Validate Field Name
    const std::vector<std::string> validFields = {"Date", "Type", "Description", "CategoryId", "Amount"};
    if (std::find(validFields.begin(), validFields.end(), field) ==
        validFields.end())
    {
        std::cerr << "Error: Invalid field '" << field << "'." << std::endl;
        return false;
    }

    std::string specialSql = "UPDATE Transactions SET " + field + " = ? WHERE TransactionId = ?;";

    if (sqlite3_prepare_v2(db, specialSql.c_str(), -1, &stmt, nullptr) !=
        SQLITE_OK)
    {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
    }

    // Bind Values
    if (field == "Amount")
    {
      double value = std::stod(newValue);
      sqlite3_bind_double(stmt, 1, value);
    } else if (field == "CategoryId")
    {
      int value = std::stoi(newValue);
      sqlite3_bind_int(stmt, 1, value);
    } else
    {
        sqlite3_bind_text(stmt, 1, newValue.c_str(), -1, SQLITE_TRANSIENT);
    }

    sqlite3_bind_int(stmt, 2, transactionId);

    int rc = sqlite3_step(stmt);
    bool success = (rc == SQLITE_DONE);

    if (!success)
    {
        std::cerr << "SQL execution error: " << sqlite3_errmsg(db) << std::endl;
    } else
    {
      std::cout << "Record with TransactionId " << transactionId
                << " updated successfully."
                << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return true;
}

void PersonalFinanceSystem::loadCategoriesFromDB()
{
    categories.clear();

    if (!openDB()) {
        std::cerr << "Failed to open database in loadTransactionFromDB().\n";
        return;
    }

    static const char *SQL_GET_CATEGORIES = R"(SELECT * FROM Category;)";

    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, SQL_GET_CATEGORIES, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare SELECT statement: "
                  << sqlite3_errmsg(db) << std::endl;
        closeDB();
        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::string tryVar = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) ?: "";

    categories.push_back(std::move(tryVar));
}

if (rc != SQLITE_DONE)
{
    std::cerr << "Error retrieving transactions: "
        << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Loaded " << categories.size()
                  << " categories.\n\n";
    }

    sqlite3_finalize(stmt);
    closeDB();
}

void PersonalFinanceSystem::loadTransactionFromDB()
{
    transactions.clear();

    if (!openDB()) {
        std::cerr << "Failed to open database in loadTransactionFromDB().\n";
        return;
    }

    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, SQL_GET_ALL, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare SELECT statement: "
                  << sqlite3_errmsg(db) << std::endl;
        closeDB();
        return;
    }

   while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    Transactions t;

    t.id           = sqlite3_column_int(stmt, 0);
    t.date         = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) ?: "";
    t.description  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) ?: "";
    t.categoryId   = sqlite3_column_int(stmt, 3);
    t.categoryName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) ?: "";
    t.amount       = sqlite3_column_double(stmt, 5);
    t.type         = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)) ?: "";

    transactions.push_back(std::move(t));
}

if (rc != SQLITE_DONE)
{
    std::cerr << "Error retrieving transactions: "
        << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Loaded " << transactions.size()
                  << " transactions from database.\n\n";
    }

    sqlite3_finalize(stmt);
    closeDB();
}


std::vector<PersonalFinanceSystem::SearchObject>
PersonalFinanceSystem::queryTransactionsByDateRange(
    const std::string& startDate,
    const std::string& endDate)
{
    searchResults.clear();

    if (!openDB()) {
        std::cerr << "Error opening database.\n";
        return searchResults;
    }

    static const char* sql = R"(
        SELECT
            t.TransactionId,
            t.Date,
            t.Description,
            c.CategoryId,
            c.CategoryDescription,
            t.Amount,
            t.Type
        FROM Transactions t
        LEFT JOIN Category c ON t.CategoryId = c.CategoryId
        WHERE t.Date BETWEEN ? AND ?
        ORDER BY t.Date ASC;
    )";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        closeDB();
        return searchResults;
    }

    sqlite3_bind_text(stmt, 1, startDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, endDate.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        PersonalFinanceSystem::SearchObject tx;
        tx.id           = sqlite3_column_int(stmt, 0);
        tx.date         = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) ?: "";
        tx.description  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) ?: "";
        tx.categoryId   = sqlite3_column_int(stmt, 3);
        tx.categoryName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) ?: "";
        tx.amount       = sqlite3_column_double(stmt, 5);
        tx.type         = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)) ?: "";

        searchResults.push_back(tx);
    }

    sqlite3_finalize(stmt);
    closeDB();

    return searchResults;
}

int PersonalFinanceSystem::findIndexById(int targetId)
{
  for (size_t i = 0; i < transactions.size(); ++i)
  {
    if (transactions[i].id == targetId)
    {
        return static_cast<int>(i);
    }
  }
	return -1;
  }

void PersonalFinanceSystem::summaryReport() {
	std::cout << "Hello from summaryReport()\n";
}

double PersonalFinanceSystem::getBalance()
{
    balance = 0;
    for (auto& t : transactions) { balance += t.amount; }
    return balance ;
}

void PersonalFinanceSystem::printDateRangeResults() {

      int w = 120;
      do { std::cout << "-"; } while ( w-- > 0 );
      // C Prints Better tables - Headers
      printf("\n%-8s %-12s %-8s %-25s %-8s %-25s %-10s\n", "| Id      ",
             "| Date        ", "| Type    ", "| Description              ",
             "| CategoryId",
             " | CategoryName             ", "| Amount     |");

        w = 120;
        do { std::cout << "-"; } while ( w-- > 0 );

        // If Id 0 print all transactions
        for (const auto& ts : searchResults)
        {
            printf("\n| %-8d | %-12s | %-8s | %-25s | %-11d | %-25s | %-10.2f |",
                ts.id, ts.date.c_str(), ts.type.c_str(), ts.description.c_str(), ts.categoryId, ts.categoryName.c_str(), ts.amount);
        }

        std::cout << "\n";

        w = 120;
        do { std::cout << "-"; } while ( w-- > 0 );
  }

void PersonalFinanceSystem::tryCat()
{
    int columns = 0;
    int max_columns = 4;

    for (auto &t : categories)
    {
      if (columns < max_columns) {
        std::cout  << " | " << t;
      } else { std::cout << t; }

        columns++;
        if (columns == max_columns)
        {
            // four column table
            std::cout << " |\n";
            columns = 0; // reset
        }
    }
}
