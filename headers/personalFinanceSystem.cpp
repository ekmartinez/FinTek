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

// ---------- Start DB Init -------------------------------

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
// -------  Ends DB Init -------------------------------------


// DashBoard
// ------------------------------------------------------------

void PersonalFinanceSystem::showMonthlySummary() {
    if (!openDB()) {                          // must set db (sqlite3*) in openDB()
        std::cerr << "Cannot open database.\n";
        return;
    }

    // This SQL returns income and expense sums in one row (0 if no rows)
    const std::string sql =
        "SELECT "
        "COALESCE(SUM(CASE WHEN Type = 'income' THEN Amount END), 0.0) AS income_sum, "
        "COALESCE(SUM(CASE WHEN Type = 'expense' THEN Amount END), 0.0) AS expense_sum "
        "FROM Transactions "
        "WHERE strftime('%Y-%m', Date) = ?;";   // Date column from your schema

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << '\n';
        closeDB();
        return;
    }

    std::string yearMonth = getCurrentYearMonth();
    if (sqlite3_bind_text(stmt, 1, yearMonth.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        std::cerr << "Failed to bind parameter: " << sqlite3_errmsg(db) << '\n';
        sqlite3_finalize(stmt);
        closeDB();
        return;
    }

    double income = 0.0, expense = 0.0;
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        income  = sqlite3_column_double(stmt, 0); // income_sum
        expense = sqlite3_column_double(stmt, 1); // expense_sum
    } else if (rc != SQLITE_DONE) {
        std::cerr << "Query error: " << sqlite3_errmsg(db) << '\n';
    }

    sqlite3_finalize(stmt);
    closeDB();

    double net = income + expense;

    std::cout << "\nFor the month (" << yearMonth << "):\n";
    std::cout << "-------------------------------------\n";

    // C Prints better tables
    printf("|  %-10s    |     %8.2f     |\n", "Income", income);
    printf("|  %-10s    |     %8.2f     |\n", "Expense", expense);
    std::cout << "-------------------------------------\n";
    printf("|  %-10s    |     %8.2f     | \n", "Net", net);
    std::cout << "-------------------------------------\n";
} // --- Ends Dashboard --------------------------------------------------


// Reporting
// -----------------------------------------------------------------------

void PersonalFinanceSystem::showYearlySummary() {
  if (!openDB()) {
      std::cerr << "Cannot open database.\n";
      return;
  }

  const char *sql = R"(
    SELECT
        strftime('%m', Date) AS month,
        SUM(CASE WHEN Type = 'income' THEN Amount ELSE 0 END) AS income,
        SUM(CASE WHEN Type = 'expense' THEN Amount ELSE 0 END) AS expense
    FROM Transactions
    WHERE strftime('%Y', Date) = strftime('%Y', 'now')
    GROUP BY month
    ORDER BY month;
    )";

  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
      std::cerr << "Failed to prepare statement.\n";
      closeDB();
      return;
  }

  std::vector<std::string> monthNames = {"Jan", "Feb", "Mar", "Apr",
                                         "May", "Jun", "Jul", "Aug",
                                         "Sep", "Oct", "Nov", "Dec"};

  std::vector<double> income(12, 0.0);
  std::vector<double> expense(12, 0.0);

  while (sqlite3_step(stmt) == SQLITE_ROW) {
      int monthIndex = std::stoi(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))) -1;
      income[monthIndex] = sqlite3_column_double(stmt, 1);
      expense[monthIndex] = sqlite3_column_double(stmt, 2);
  }

  sqlite3_finalize(stmt);
  closeDB();

  // Print table header
  printf("%-10s %12s %12s %12s\n", "Month", "Income", "Expense", "Net");
  printf("------------------------------------------------------------\n");

  double totalIncome = 0.00;
  double totalExpense = 0.00;

  for (int i = 0; i < 12; ++i) {
    double net = income[i] + expense[i];
    totalIncome += income[i];
    totalExpense += expense[i];
    printf("%-10s %12.2f %12.2f %12.2f\n",
    monthNames[i].c_str(), income[i], expense[i], net);
  }

  printf("------------------------------------------------------------\n");
  printf("%-10s %12.2f %12.2f %12.2f\n",
    "Total", totalIncome, totalExpense, totalIncome + totalExpense);
}

std::vector<PersonalFinanceSystem::QueryObject>
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
        PersonalFinanceSystem::QueryObject tx;
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

std::vector<PersonalFinanceSystem::CategorySummary>
PersonalFinanceSystem::viewCategories() {
    std::vector<CategorySummary> results;

    if (!openDB()) { return results; }

    const char *sql = R"(
        SELECT
            c.CategoryId,
            c.CategoryDescription,
            IFNULL(SUM(t.Amount), 0) AS TotalAmount,
            c.Type
        FROM Category c
        LEFT JOIN Transactions t ON c.CategoryId = t.CategoryId
        GROUP BY c.CategoryId, c.CategoryDescription, c.Type
        ORDER BY c.CategoryDescription;
    )";

    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        closeDB();
        return results;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        CategorySummary record;
        record.categoryId = sqlite3_column_int(stmt, 0);

        const unsigned char *desc = sqlite3_column_text(stmt, 1);
        record.categoryDescription = desc ? reinterpret_cast<const char*>(desc) : "";

        record.totalAmount = sqlite3_column_double(stmt, 2);

        const unsigned char *type = sqlite3_column_text(stmt, 3);
        record.Type = type ? reinterpret_cast<const char*>(type) : "";

        results.push_back(record);
    }

    sqlite3_finalize(stmt);
    closeDB();
    return results;
}

int PersonalFinanceSystem::viewTransaction(int id = 0)
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
            printf("\n| %-8d | %-12s | %-8s | %-25s | %-11d | %-25s | %10.2f |",
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
              printf("\n| %-8d | %-12s | %-8s | %-25s | %-11d | %-25s | %10.2f |",
                ts.id, ts.date.c_str(), ts.type.c_str(), ts.description.c_str(), ts.categoryId, ts.categoryName.c_str(), ts.amount);
            }
		}

        std::cout << "\n";

        w = 120;
        do { std::cout << "-"; } while ( w-- > 0 );
	}

	return 0;
}

  // // -----Ends Reporting---------------------------------------------------------

// Adding Records
// ----------------------------------------------------------------------------
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
} // Ends Adding Records

// Updating Records
// -------------------------------------------------------------------------

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

// Deleting Records
// ---------------------------------------------------------------------------

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

// Helpers
// ------------------------------------------------------------------

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
