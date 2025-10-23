#ifndef PFS_H
#define PFS_H

#include <string>
#include <vector>

#pragma once
#include <sqlite3.h>

class PersonalFinanceSystem {
	private:
        sqlite3 *db = nullptr;
        std::string dbPath;

        std::vector<std::string> category;
        double balance;

		struct Transactions {
			int id;
			std::string date;
			std::string description;
			int categoryId;
			double amount;
            std::string type;
		};

		std::vector<Transactions> transactions;

	public:
        // Constructor
        explicit PersonalFinanceSystem(const std::string& path);

        // Destructor
        ~PersonalFinanceSystem();

		void addCategory(const std::string& str);
		void displayCategories();
		void deleteCategory(const std::string& cat);
		void addTransaction(const std::string& date, const std::string& description, int categoryId, double amount, const std::string& type);
		void displayTransactions();
		int findTransaction();
		int findIndexById(int targetId);
		void updateDate(const int id, const std::string newDate);
		void updateDescription(const int id, const std::string newDesc);
		void updateCategory(const int id, const std::string newCategory);
		void updateAmount(const int id, const double newAmt);
		int deleteTransactionById(int targetId);
		void summaryReport();
		int getLastId();
        double getBalance();
};

#endif
