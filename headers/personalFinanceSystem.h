#ifndef PFS_H
#define PFS_H

#include <string>
#include <vector>
#include <sqlite3.h>

class PersonalFinanceSystem {
private:
    sqlite3* db = nullptr;
    std::string dbPath;
    bool openDB();
    void closeDB();

     // Used to load all transactions from db
    struct Transactions {
        int id;
        std::string date;
        std::string description;
        int categoryId;
        std::string categoryName;
        double amount;
        std::string type;
    };

    // Used to store query results.
    struct QueryObject {
        int id;
        std::string date;
        std::string description;
        int categoryId;
        std::string categoryName;
        double amount;
        std::string type;
    };

    // Used to Store Category results
    struct CategorySummary
    {
        int categoryId;
        std::string categoryDescription;
        double totalAmount;
        std::string Type;
    };

    std::vector<Transactions> transactions; // Primary Data structure
    std::vector<QueryObject> searchResults; // Used in sql daterange search
    std::vector<std::string> categories;
public:
    explicit PersonalFinanceSystem(const std::string &path);
    ~PersonalFinanceSystem();

    // Dashboard Net Income for the month.
    void showMonthlySummary();

   // Reporting
    void showYearlySummary(); // YTD Report
    void printDateRangeResults(); // Print helper for query
    std::vector<QueryObject> queryTransactionsByDateRange( // Search by date
        const std::string& startDate,
        const std::string& endDate);

    int viewTransaction(int id);
    std::vector<CategorySummary> viewCategories();

    // Adding Records
    // -----------------
    void addTransaction(const std::string &date, const std::string &description,
                        int categoryId, double amount, const std::string &type);

    int addCategory(const std::string& categoryName);

    // Updating Records
    // -----------------
    bool updateRecord(int transactionId, const std::string &field,
                      const std::string& newValue);


    // Deleting Records
    // --------------------
    int deleteTransactionById(int idToDelete);


    // Helpers
    // ----------------------------------------------------
    int getCategoryId(const std::string& categoryName); // if exists
    int findIndexById(int targetId);

    // Loads Data from database into structs
    void loadTransactionFromDB();
    void loadCategoriesFromDB();

    };


#endif
