#ifndef PFS
#define PFS

#include <string>
#include <vector>

class PersonalFinanceSystem {
	private:
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
		static std::string csvFilePath;
		static std::string csvTempPath;

        std::string dbPath = "./storage/ledger.db";

	public:
		void addCategory(const std::string& str);
		void displayCategories();
		void deleteCategory(const std::string& cat);
		void addTransaction(const std::string& date, const std::string& description, int categoryId, double amount, const std::string& type);
		void displayTransactions();
		int searchTransaction(const int id);
		int findIndexById(int targetId);
		void updateDate(const int id, const std::string newDate);
		void updateDescription(const int id, const std::string newDesc);
		void updateCategory(const int id, const std::string newCategory);
		void updateAmount(const int id, const double newAmt);
		int deleteTransactionById(int targetId);
		void summaryReport();
		int getLastId();
        void AddToCsv(const int id, const std::string date, const std::string desc, const std::string cat, double amt);
        void loadFromCsv();
        void updateCsv();
        double getBalance();
};

#endif
