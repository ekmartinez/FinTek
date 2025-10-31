#ifndef PFS_H
#define PFS_H

#include <string>
#include <vector>
#include <algorithm>  // for std::find
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
    struct SearchObject {
        int id;
        std::string date;
        std::string description;
        int categoryId;
        std::string categoryName;
        double amount;
        std::string type;
    };

    double balance = 0.0;

    std::vector<std::string> categories;
    std::vector<Transactions> transactions;
    std::vector<SearchObject> searchResults;

    sqlite3* openConnection();
    void closeConnection(sqlite3* db);

public:
    explicit PersonalFinanceSystem(const std::string &path);
    ~PersonalFinanceSystem();

    // CRUD and utility functions
    void addTransaction(const std::string& date, const std::string& description, int categoryId, double amount, const std::string& type);
    int findTransaction(int id);
    int getCategoryId(const std::string& categoryName);
    int addCategory(const std::string& categoryName);
    int deleteTransactionById(int targetId);
    void loadTransactionFromDB();
    bool updateRecord(int transactionId, const std::string& field, const std::string& newValue);

    void printCategories();

    void deleteCategory(const std::string& cat);

    void displayTransactions();

    int findIndexById(int targetId);
    void summaryReport();
    int getLastId();
    double getBalance();

    void loadCategoriesFromDB();
    void tryCat();
    void printDateRangeResults();

     std::vector<SearchObject> queryTransactionsByDateRange(
        const std::string& startDate,
        const std::string& endDate);

    void showMonthlySummary();

    void showYearlySummary();

    };


#endif
