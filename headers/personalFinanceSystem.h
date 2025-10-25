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

    std::vector<std::string> category;
    double balance = 0.0;

    struct Transactions {
        int id;
        std::string date;
        std::string description;
        int categoryId;
        double amount;
        std::string type;
    };

    std::vector<Transactions> transactions;

    // optional: helper functions for per-operation DB open/close
    sqlite3* openConnection();
    void closeConnection(sqlite3* db);

public:
    // ✅ Fixed constructor signature
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

    void updateDate(int id, const std::string newDate);
    void updateDescription(int id, const std::string newDesc);
    void updateCategory(int id, const std::string newCategory);
    void updateAmount(int id, const double newAmt);

    void displayCategories();
    void deleteCategory(const std::string& cat);
    void displayTransactions();
    int findIndexById(int targetId);
    void summaryReport();
    int getLastId();
    double getBalance();
};

#endif
