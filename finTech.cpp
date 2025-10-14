#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

void printHeader() {

	std::cout <<"" 
			<< "########:'####:'##::: ##:'########:'########:'##:::'##::::::::::'##::::'##::::\n"
			<< "##.....::. ##:: ###:: ##:... ##..:: ##.....:: ##::'##:::::::::::. ##:::. ##:::\n"
			<< "##:::::::: ##:: ####: ##:::: ##:::: ##::::::: ##:'##:::::::::::::. ##:::. ##::\n"
			<< "######:::: ##:: ## ## ##:::: ##:::: ######::: #####::::'#######:::. ##:::. ##:\n"
			<< "##...::::: ##:: ##. ####:::: ##:::: ##...:::: ##. ##:::........::: ##:::: ##::\n"
			<< "##:::::::: ##:: ##:. ###:::: ##:::: ##::::::: ##:. ##:::::::::::: ##:::: ##:::\n"
			<< "##:::::::'####: ##::. ##:::: ##:::: ########: ##::. ##:::::::::: ##:::: ##::::\n"
			<< "e..::::::::...::..::::..:::::..:::::.......z:..::::..::::::::::..:::::..:::::w\n";

	std::cout << "FinTek - Personal Finance System\n" << std::endl;
}

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

	public:
		
		void addCategory(const std::string& str) {
			category.push_back(str);
			std::cout << "Added: " << str << std::endl;
		}
		
		void displayCategories() const {
			std::cout << "\nCategories\n" << "-----------\n";
			for (size_t i = 0; i < category.size(); ++i)  {
				std::cout << category[i] << ", \n";
			}
		}

		void deleteCategory(const std::string& cat) {
			category.erase(
					std::remove(category.begin(), category.end(), cat),
					category.end()
			);

			std::cout << "Deleted: " << cat << std::endl;
		}

		void addTransaction(int id, const std::string& date, const std::string& desc, 
							const std::string& cat, const double amt) {

			Transactions newTransaction = {id, date, desc, cat, amt};
			transactions.push_back(newTransaction);
			std::cout << "Added: " << id << date << desc << cat << amt << std::endl;
		}

		void displayTransaction() {
			std::cout << "Hello from displayTransaction()\n";
		}

		void updateTransaction() {
			std::cout << "Hello from updateTransaction()\n";
		}

		void deleteTransaction() {
			std::cout << "Hello from deleteTransaction()\n";
		}

		void summaryReport() {
			std::cout << "Hello from summaryReport()\n";
		}
		void exit() {
			std::cout << "Exiting Application ... \n";
		}
};


int main(void) {

	PersonalFinanceSystem pfs;
	printHeader();

	int option = 0;	
	std::cout << "Welcome to the Personal Finance Tracker!\n" 
				<< "---------------------------------------\n" 
				<< "\t1. Add Transaction\n"
				<< "\t2. Display Transactions\n"
				<< "\t3. Update Transaction\n"
				<< "\t4. Delete Transaction\n"
				<< "\t5. Summary Report\n"
				<< "\t6. Exit\n"
				<< "\nChoose an option: ";

	std::cin >> option; 
	switch (option) {
		case 1: 
			std::cout << "Where now ready to add a transactions from the menu" << std::endl;
			break;
		default:
			std::cout << "Nevermind" << std::endl;
		
	}

	return 0;
}

// Add permanent storage
