#include <map>
#include <ctime>
#include <limits>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <cstdlib> // for system
#include <iostream>
#include <algorithm> // for remove
#include "csvOperations.h"

// Prototypes for needed functions
bool isValidDate(const std::string& date);
std::string  safeInput(std::size_t maxLen, const std::string&);

class PersonalFinanceSystem {
	/* Personal Ledger */
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

			// Header
			printf("%-8s %-12s %-25s %-15s %-10s",
					"Id", "Date", "Description", "Category", "Amount");

			// Lines
			for (const auto& t : transactions) {
				printf("\n%-8d %-12s %-25s %-15s %-10.2f", 
						t.id, t.date.c_str(), t.desc.c_str(), t.cat.c_str(), t.amt);
			}
		}

		 int searchTransaction(const int id) { 
		 	for (const auto& transaction : transactions) {
		 		if (transaction.id == id) {
		 			std::cout << "\nId found and ready for updating\n";
		 			std::cout << "Id\tDate\tDesc\tCategory\tAmount\n";
		 			std::cout << transaction.id << "\t"; 
		 			std::cout << transaction.date << "\t"; 
		 			std::cout << transaction.desc << "\t"; 
		 			std::cout << transaction.cat << "\t"; 
		 			std::cout << transaction.amt << "\t"; 

		 			return 1;

		 			} else { std::cout << "\nId not found.\n"; }
		 	}
		 	return 0;
		 }

		int findIndexById(int targetId) {

			for (size_t i = 0; i < transactions.size(); ++i) {
				if (transactions[i].id == targetId) { 
					return static_cast<int>(i); 
				}
			}

			return -1;
		}

		void updateDate(const int id, const std::string newDate) {
			int index = findIndexById(id);
			if (isValidDate(newDate)) {
				transactions[index].date = newDate;
			} else { 
				std::cout << "Something went wrong in updateDate()\n";
			} 
		}
		
		void updateDescription(const int id, const std::string newDesc) { 
			int i = findIndexById(id);
			transactions[i].desc = newDesc;
		}
		void updateCategory(const int id, const std::string newCategory) { 
			int i = findIndexById(id);
			transactions[i].cat = newCategory;
		}

		void updateAmount(const int id, const double newAmt) { 
			int i = findIndexById(id);
			transactions[i].amt = newAmt;
		}

		int deleteTransactionById(int targetId) {
			auto it = std::remove_if(transactions.begin(), transactions.end(), 
					[targetId](const Transactions& t) { return t.id == targetId; });

			if (it != transactions.end()) {
				transactions.erase(it, transactions.end());
				std::cout << "Transaction with ID " << targetId << " deleted.\n";
				return 1;
			}

			return -1;

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

std::string getCurrentDate() {
	std::time_t t = std::time(nullptr);
	std::tm* now =std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(now, "%Y-%m-%d");
	
	return oss.str();
}

int daysInMonth(int year, int month) {
	static const int baseDays[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	if (month < 1 || month > 12) { return 0; }

	int days = baseDays[month - 1]; // why months
									
		// Adjust February for leap years
		bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
		if (month == 2 && isLeap) { days = 29; }
		
		return days; 
}

// Validate date string in "YYYY-MM-DD"
bool isValidDate(const std::string& date) {
	static const std::regex pattern(R"(^(\d{4})-(\d{2})-(\d{2}))");
	std::smatch match;

	// Check basic format
	if (!std::regex_match(date, match, pattern)) { return false; }
	
	int year = std::stoi(match[1]);
	int month = std::stoi(match[2]);
	int day = std::stoi(match[3]);

	// Get days in the given month/year
	int maxDay = daysInMonth(year, month);

	// If month or day are invalid, return false
	if (maxDay == 0 || day < 1 || day > maxDay) { return false; }

	return true;
}
std::string safeInput(std::size_t maxLen, const std::string& txt) {
	// NOT WORKING

	std::string input;
	input.reserve(maxLen); // avoid reallocations
	
	char c;
	while(std::cin.get(c)) {
		if (c == '\n') { break; }
		if (input.size() < maxLen) { input += c; }
	}
	
	// 
	// Clear remaining characters
	if (!std::cin.eof()) {
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return input;
}

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
				printHeader();
				while(1) {
					int id = pfs.getLastId() + 1;
					
					std::string date = getCurrentDate();
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

					std::cout << "\nData Entered and ready to be saved:\n\n"
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
						
						if (opts == 'y') { 
							printHeader();
							continue; }
					} else if (input == "n") {
						printHeader();
						break;
					} else { 
						std::cout << "Wrong option.\n"; 
						continue;
					}
					break;
				}
				break;
			}
			case 2: {
				printHeader();
				pfs.displayTransactions();
				std::cout << "\n\nPress Enter to continue... \n";
				std::cin.get(); std::cin.get();
				break;
			}
			case 3: {
				// Update Transaction 

				printHeader();
				// Search for Id of transaction
				int idOfTrans = 0;
				std::cout << "Enter Id of transaction: ";
				std::cin >> idOfTrans;
				std::cout << std::endl;

				// If id found
				if (pfs.searchTransaction(idOfTrans) == 1) { 
					std::cout << "\n\nWhat do you want to change?: "
						<< "\n\t1-" << "Date\n"
						<< "\t2-" << "Description\n"
						<< "\t3-" << "Category\n"
						<< "\t4-" << "Amount\n"
						<< "Choose an option: " 
						<< std::endl;

					int optn = 0;
					std::cin >> optn;

					switch (optn) {
						case 1: { 
							// update date 
							std::string date;
							std::cout << "Enter new date >>> "; 
							std::cin >> date;

							if (isValidDate(date)) {
								pfs.updateDate(idOfTrans, date);
							}

							std::cout << "Date has been updated. \n\n";
							std::cout << "Press Enter to continue... \n";
							std::cin.get();
							std::cin.get();
							break;
						}
						case 2: {
							// Update Description
							std::string newDesc;
							std::cout << "Enter new description (char limit = 10) >>> "; 
							std::cin >> newDesc;

							pfs.updateDescription(idOfTrans, newDesc);

							std::cout << "Description has been updated. \n\n";
							std::cout << "Press Enter to continue... \n";
							std::cin.get();
							std::cin.get();
							break;
						}

						case 3: {
							// Update Category
							std::string newCat;
							std::cout << "Enter new category (char limit = 10) >>> "; 
							std::cin >> newCat;

							pfs.updateCategory(idOfTrans, newCat);
							std::cout << "Category has been updated. \n\n";
							std::cout << "Press Enter to continue... \n";
							std::cin.get();
							std::cin.get();
							break;
						}
						case 4: {
							// Update Amount

							double newAmt;
							std::cout << "Enter new amount >>> "; 
							std::cin >> newAmt;
							pfs.updateAmount(idOfTrans, newAmt);

							std::cout << "Category has been updated. \n\n";
							std::cout << "Press Enter to continue... \n";
							std::cin.get();
							std::cin.get();
							break;
						}
						default: {
							std::cout << "Wrong Option"; 
							break;
						}
					}
				}
			}

			case 4: {
				printHeader();
				int idToDelete;
				std::cout << "Enter id to delete: "; 
				std::cin >> idToDelete;

				std::cout << "You've chosen to delete id" 
							<< idToDelete << ": \n";

				pfs.searchTransaction(idToDelete);

				std::cout << "\n\nAre you sure? (y|n): ";

				char c;
				std::cin >> c;

				if (c == 'y') { 
					
					int d = pfs.deleteTransactionById(idToDelete); 
					if (d != -1) {
						std::cout << "\nTransaction has been deleted. \n\n";
					} else { std::cout << "\nThere was a problem. \n\n";} 
				} else { std::cout << "Nevermind.\n"; }

				std::cout << "Press Enter to continue... \n";
				std::cin.get();
				std::cin.get();
				break;
			}
			case 5: {
				printHeader();
				pfs.summaryReport();
				std::cout << "You ordered a summary report\n";
				break;
			}
			case 6: {
				printHeader();
				std::cout << "\nThank you for using this application!, see you later.\n";
				exit(0);
			}
			case 7: {
				printHeader();
				std::cout << "\nYou have accessed the secret Location!\n\n";

				int id_ = 0;
				std::cout << "\nEnter id:\n";
				std::cin >> id_;
				readRecord(id_);
				updateRecord(id_);

				std::cout << "\n\nPress Enter to continue... \n";
				std::cin.get();
				std::cin.get();
				break;
			}

			default: {
				std::cout << "Nevermind.\n";
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

// int id = 0;
				// std::cout << "Search By Id\n" << "------------\n";
				// std::cout << "\nEnter Id >> ";
				// std::cin >> id;

				// pfs.searchTransaction((int)id);


