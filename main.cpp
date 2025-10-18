#include <cstdlib>
#include <iostream>
					 
#include "headers/helpers.h"
#include "headers/csvOperations.h"
#include "headers/personalFinanceSystem.h"

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
						std::cout << "\nDo you wish to add another transaction? (y|n) >>> ";
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
// std::cout << "Press Enter to continue... " <<< std::endl;
// // std::cin.get();
// // std::cin.get();

