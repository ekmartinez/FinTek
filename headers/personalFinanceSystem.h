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

		void addTransaction(const std::string& date, const std::string& description, int categoryId, double amount, const std::string& type);
		int findTransaction(int id = 0);
        int getCategoryId(const std::string& categoryName);
		int addCategory(const std::string& categoryName);
		int deleteTransactionById(int targetId);
        void loadTransactionFromDB();





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
