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

			std::cout << "Data saved." << std::endl;
		}
		
		void displayTransactions() const {
			for (const auto& transaction : transactions) {
				std::cout << transaction.id << "\t"; 
				std::cout << transaction.date << "\t"; 
				std::cout << transaction.desc << "\t"; 
				std::cout << transaction.cat << "\t"; 
				std::cout << transaction.amt << "\t"; 
				std::cout << "\n";
			}
		}

		int searchTransaction(const int id) { 
			for (const auto& transaction : transactions) {
				if (transaction.id == id) {
					std::cout << "Id found and ready for updating\n";
					std::cout << "Id\tDate\tDesc\tCategory\tAmount\n";
					std::cout << transaction.id << "\t"; 
					std::cout << transaction.date << "\t"; 
					std::cout << transaction.desc << "\t"; 
					std::cout << transaction.cat << "\t"; 
					std::cout << transaction.amt << "\t"; 

					return 1;

					} else { std::cout << "\nId not found." << std::endl; }
			}
			return 0;
		}
		void updateTransaction(int id, int fieldToEdit, std::string date) {
			for (const auto& transaction : transactions) {
				if (transaction.id == id) {
					switch(fieldToEdit) {
						case 0: { 
							std::cout << "Edit Date" << std::endl;
							transaction.date = date;
							break;
						}
						case 1: { 
							std::cout << "Edit Description" << std::endl;
							break;
						}
						case 2: { 
							std::cout << "Edit Category" << std::endl;
							break;
						}
						case 3: { 
							std::cout << "Edit Amount" << std::endl;
							break;
						}
					}
				}else { std::cout << "\nId not found." << std::endl; }
			}
			
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
					std::string cat1 = "";
					std::cout << "Enter transaction's category: ";
					std::cin >> cat1;

					double amount = 0.00;
					std::cout << "Enter transaction's amount: ";
					std::cin >> amount;

					std::cout << "\nData Entered and ready to be saved: \n"
								<< "Id: " << id << std::endl
								<< "Date: "<< date << std::endl
								<< "Description: " << description << std::endl
								<< "Category: " << cat1 << std::endl
								<< "Amount: " << (double)amount << std::endl;

					std::cout << "\nSave? (y|n) >> ";
					std::string input;
					std::cin >> input;
										
					if (input == "y") {
						pfs.addTransaction(id, date, description, cat1, amount);
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
				pfs.displayTransactions();
				std::cout << "Press Enter to continue... " << std::endl;
				std::cin.get(); std::cin.get();
				break;
			}
			case 3: {
				printHeader();
				// Search for Id of transaction
				int idOfTrans = 0;
				std::cout << "Enter Id of transaction: ";
				std::cin >> idOfTrans;

				// If id found
				if (pfs.searchTransaction(idOfTrans) == 1) { 
					std::cout << "What do you want to change?"
						<< "\t1-" << "Date\n"
						<< "\t2-" << "Description\n"
						<< "\t3-" << "Category\n"
						<< "\t4-" << "Amount\n"
						<< "Choose an option: " 
						<< std::endl;

					int optn = 0;
					std::cin >> optn;

					switch (optn) {
						case 1: { 
							std::string date;
							std::cout << "Enter new date >>> "; 
							std::cin >> date;
							// update transaction 
							break;
						}
						case 2: {}
						case 3: {}
						case 4: {}
						case 5: {}
						default: {}
					}
				}
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

