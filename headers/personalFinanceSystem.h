#ifndef PFS
#define PFS

#include <string>
#include <vector>

class PersonalFinanceSystem {
	private:
		std::vector<std::string> category;

		struct Transactions {
			int id;
			std::string date;
			std::string desc;
			std::string cat;
			double amt;
		};

		std::vector<Transactions> transactions;
		static std::string csvFilePath;
		static std::string csvTempPath;

	public:
		void addCategory(const std::string& str);
		void displayCategories();
		void deleteCategory(const std::string& cat);
		void addTransaction(int id, const std::string& date, const std::string& desc, const std::string& cat, const double amt);
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
};

#endif
