#include <map>
#include <string>
#include <vector>
// #include <iomanip> // for setw
#include <cstdlib> // for system
#include <iostream>
#include <algorithm> // for remove

void printHeader() {
	system("clear");
	std::cout <<"\n" 
			<< "########:'####:'##::: ##:'########:'########:'##:::'##::::::::::'##::::'##::::\n"
			<< "##.....::. ##:: ###:: ##:... ##..:: ##.....:: ##::'##:::::::::::. ##:::. ##:::\n"
			<< "##:::::::: ##:: ####: ##:::: ##:::: ##::::::: ##:'##:::::::::::::. ##:::. ##::\n"
			<< "######:::: ##:: ## ## ##:::: ##:::: ######::: #####::::'#######:::. ##:::. ##:\n"
			<< "##...::::: ##:: ##. ####:::: ##:::: ##...:::: ##. ##:::........::: ##:::: ##::\n"
			<< "##:::::::: ##:: ##:. ###:::: ##:::: ##::::::: ##:. ##:::::::::::: ##:::: ##:::\n"
			<< "##:::::::'####: ##::. ##:::: ##:::: ########: ##::. ##:::::::::: ##:::: ##::::\n"
			<< "E..::::::::...::..::::..:::::..:::::.......Z:..::::..::::::::::..:::::..:::::W\n";

	std::cout << "\nFinTek - Personal Finance System\n" << std::endl;

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

			std::cout << "Added: \n" << id << "\n" 
									<< date << "\n"
									<< desc << "\n"
									<< cat << "\n"
									<< amt << std::endl;
		}
		
		void displayTransaction() const {
			for (const auto& transaction : transactions) {
				std::cout << transaction.id << "\t"; 
				std::cout << transaction.date << "\t"; 
				std::cout << transaction.desc << "\t"; 
				std::cout << transaction.cat << "\t"; 
				std::cout << transaction.amt << "\t"; 
				std::cout << "\n";
			}
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
		int getLastId() {
			if (transactions.empty()) { return 0; }
			return transactions.back().id;
		}
};

int main(void) {

	PersonalFinanceSystem pfs;

	while(1) {
		printHeader();
		int option = 0;	
		std::cout << "\nWelcome to the Personal Finance Tracker!\n" 
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
			case 1: { 
				while(1) {
					int id = pfs.getLastId() + 1;
					
					std::string date = "";
					std::cout << "Enter transaction's date: ";
					std::cin >> date;

					std::string description = "";
					std::cout << "Enter transaction's description: ";
					std::cin >> description;

					std::string cat = "";
					std::cout << "Enter transaction's category: ";
					std::cin >> cat;

					double amount = 0.00;
					std::cout << "Enter transaction's amount: ";
					std::cin >> amount;

					std::cout << "\nData Entered and ready to be saved: \n"
								<< "Id: " << id << std::endl
								<< "Date: "<< date << std::endl
								<< "Description: " << description << std::endl
								<< "Category: " << cat << std::endl
								<< "Amount: " << (double)amount << std::endl;

					std::cout << "\nSave? (y|n) >> ";
					std::string input;
					std::cin >> input;
										
					if (input == "y") {
						pfs.addTransaction(id, date, description, cat, amount);
						char opts;
						std::cout << "Do you wish to add another transaction? (y|n) >>> ";
						std::cin >> opts;
						if (opts == 'y') { continue; }
					} else if (input == "n") {
						printHeader();
						break;
					} else { 
						std::cout << "Wrong option." << std::endl; 
						continue;
						}
					break;
				}
				break;
			}
			case 2: {
				printHeader();
				pfs.displayTransaction();
				std::cout << "Press Enter to continue... " << std::endl;
				std::cin.get(); std::cin.get(); // One wont work.
				break;
			}
			case 3: {
				printHeader();
				pfs.updateTransaction();
				std::cout << "You ordered a transaction update" << std::endl;
				break;
			}
			case 4: {
				printHeader();
				pfs.deleteTransaction();
				std::cout << "You ordered to delete a transaction" << std::endl;
				break;
			}
			case 5: {
				printHeader();
				pfs.summaryReport();
				std::cout << "You ordered a summary report" << std::endl;
				break;
			}
			case 6: {
				printHeader();
				std::cout << "\nThank you for using this application!, see you later." << std::endl;
				exit(0);
			}
			default: {
				std::cout << "Nevermind" << std::endl;
				break;
			}
			
		}
	}
	return 0;
}

// Add permanent storage

/*
// Press Enter to Continue...
// Double cin.get() needed to account for prev
// Enters.
std::cout << "Press Enter to continue... " <<< std::endl;
std::cin.get();
std::cin.get();
*/

