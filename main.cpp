#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
					 
#include "headers/helpers.h"
#include "headers/personalFinanceSystem.h"

int main(void)
{
    PersonalFinanceSystem pfs("./storage/ledger.db");
    pfs.loadTransactionFromDB();

    while (1)
    {
		printHeader();

        // Starts Dashboard
        std::cout << "\nLedger Balance: $" << pfs.getBalance() << std::endl;
        // Ends Dashboard

		std::cout << "\n"
            << "-------------Reporting--------------\n"
            << "| 1. Net Income / Loss per Period  |\n"
			<< "| 2. View Transactions             |\n" // 3. View Categories
			<< "------------------------------------\n\n"
            << "------------Transactions------------\n"
            << "| 3. Add Transaction               |\n"
			<< "| 4. Update Transaction            |\n"
			<< "| 5. Delete Transaction            |\n"
			<< "------------------------------------\n\n"
            << "-------------Categories-------------\n"
			<< "| 6. Add Category                  |\n"
			<< "| 7. Update Category               |\n"
			<< "| 8. Delete Category               |\n"
			<< "------------------------------------\n\n"
            << "------------Configurations----------\n"
			<< "| 9. Update Threshold Amount       |\n"
			<< "------------------------------------\n"
			<< "| 0. Exit                          |\n"
			<< "------------------------------------\n\n"
			<< "   Choose an option >>> ";

		std::string line = "";
        std::getline(std::cin, line);

        // TODO: Error checking
        int option = std::stoi(line);

        switch (option)
        {
            case 0:
            {
                // Exit
                printHeader();
                std::cout << "\n\nThank you for using this application!, see you later.\n";
                pressEnterToContinue();
                exit(0);
            }
            case 1:
            {
                // Historical Monthly Income / Expense
                printHeader();
                std::cout << "\n\nThis section wil contain the income/expense per month"
                    << " for the 12 of the current calender year.\n" << std::endl;
                pressEnterToContinue();
                break;
                }
            case 2:
            {
                // SEARCH TRANSACTIONS
                while (1)
                {

                    printHeader();
                    std::string id = "";
                    std::cout << "\nSearch Transaction\n------------------\n";
                    std::cout << "\nEnter Id of transaction (None for all): ";
                    std::getline(std::cin, id);

                    printf("\n%-8s %-12s %-8s %-25s %-15s %-10s\n",
                            "Id", "Date", "Type", "Description", "Category", "Amount");

                    if (id.empty()) {
                        pfs.findTransaction(0);
                        pressEnterToContinue();
                        break;
                    } else {
                        pfs.findTransaction(std::stoi(id));
                        pressEnterToContinue();
                        break;
                    }
                }
                break;
            }
            case 3:
            {
                // ADD TRANSACTION
                std::string date = getCurrentDate();
                while (1)
                {
                    printHeader();

                    std::cout << "\nEnter Transaction's date "
                        << "(leave blank for today: " << date << "): ";

                    std::getline(std::cin, date);

                    if (date.empty())
                    {
                        date = getCurrentDate();
                        break;
                    } else {
                            std::cout << "There was problem in the getCurrentDate() function." << std::endl;
                            continue;
                    }
                }

                // ENTER Limit
                // TODO: Limit size
                std::string type = "";

                while (1)
                {
                    std::string tmp = "";
                    std::cout << "Enter transaction type (1 - Income / 2 - Expense): ";
                    std::getline(std::cin, tmp);
                    int t_type = std::stoi(tmp);

                    switch (t_type)
                    {
                        case 1:
                        {
                            type = "income";
                            break;
                        }
                        case 2:
                        {
                            type = "expense";
                            break;
                        }
                        default: {
                            std::cout << "Wrong selection (1 | 2)" << std::endl;
                            pressEnterToContinue();
                            printHeader();
                            continue;
                        }
                    }
                }

                // ENTER DESCRIPTION
                // TODO: Limit size
                std::string description = "";
                std::cout << "Enter Description: ";
                std::getline(std::cin, description);

                // CATEGORIZATION
                std::string categoryName = "";

                std::cout << "Enter transaction's category: ";
                std::getline(std::cin, categoryName);

                if (pfs.getCategoryId(categoryName) == -1) {
                    std::string tmp = "";
                    std::cout << "Category does not exist, do you want to add it? (1 - Yes | 2 - No): ";
                    std::getline(std::cin, tmp);
                    int answer = std::stoi(tmp);

                    switch (answer)
                    {
                        case 1: {
                            pfs.addCategory(categoryName);
                            break;
                        }
                        case 2:
                        default:
                        {
                            std::cout << "Wrong option." << std::endl;
                            pressEnterToContinue();
                            continue;
                        }
                    }

                // ENDS CATEGORIZATION

                // AMOUNT
                // TODO: Reasonableness check
                std::string tmpAmt = "";
                std::cout << "Enter transaction's amount: ";
                std::getline(std::cin, tmpAmt);

                // TODO: Error checking
                double amount = std::stod(tmpAmt);

                // Data summary before saving
                std::cout << "\nData Entered and ready to be saved:\n\n"
                        << "Date: " << date << std::endl
                        << "Description: " << description << std::endl
                        << "Category: " << categoryName << std::endl
                        << "Amount: " << amount << std::endl
                        << "Type: " << type << std::endl;

                std::cout << "\nSave? (y|n) >> ";
                std::string input;
                std::getline(std::cin, input);

                if (input == "y")
                {
                int categoryId = pfs.getCategoryId(categoryName);
                    pfs.addTransaction(date, description, categoryId, amount, type);
                    pfs.loadTransactionFromDB();

                    std::string opts;
                    std::cout << "\nDo you wish to add another transaction? (y|n) >>> ";
                    std::getline(std::cin, opts);

                    if (opts == "y")
                    {
                        printHeader();
                        continue;
                    }
                } else if (input == "n")
                {
                    printHeader();
                    break;
                } else
                {
                    std::cout << "Wrong option.\n";
                    continue;
                }
                break;
                }
            }

            case 4:
            {
                // Update Transaction

                printHeader();
                // Search for Id of transaction

                std::string tmp = "";
                std::cout << "Enter Id of transaction: ";
                std::getline(std::cin, tmp);
                int id = std::stoi(tmp);

                std::cout << std::endl;

                // If id found
                if (pfs.findIndexById(id == 1)) {
                    std::cout << "\n\nWhat do you want to update?\n"
                    << "1 - Date\n"
                    << "2 - Type\n"
                    << "3 - Description\n"
                    << "4 - Category\n"
                    << "5 - Amount\n"
                    << "Choose an option: "
                    << std::endl;

                    std::string temp = "";
                    std::getline(std::cin, line);
                    int optn = std::stoi(line);

                    switch (optn) {
                        case 1: {
                            // update date
                            std::string date = "";
                            std::cout << "Enter new date >>> ";
                            std::getline(std::cin, date);

                            if (isValidDate(date)) {
                                pfs.updateDate(id, date);
                            }

                            std::cout << "Date has been updated. \n\n";
                            std::cout << "Press Enter to continue... \n";

                            std::string line = "";
                            std::cin.get();
                            std::getline(std::cin, line);
                            break;
                        }
                        case 2: {
                            // Update Description
                            std::string newDesc = "";
                            std::cout << "Enter new description (char limit = 10) >>> ";
                            std::getline(std::cin, line);

                            pfs.updateDescription(id, newDesc);

                            std::cout << "Description has been updated. \n\n";
                            pressEnterToContinue();
                            break;
                        }
                        case 3: {
                            // Update Category
                            std::string newCat = "";
                            std::cout << "Enter new category (char limit = 10) >>> ";
                            std::getline(std::cin, newCat);
                            pfs.updateCategory(id, newCat);

                            std::cout << "Category has been updated. \n\n";
                            std::cout << "Press Enter to continue... \n";

                            std::string tmp = "";
                            std::cin.get();
                            std::getline(std::cin, tmp);
                            break;
                        }
                        case 4: {
                            // Update Amount
                            std::string line = "";
                            std::cout << "Enter new amount >>> ";
                            std::getline(std::cin, line);
                            double newAmt = std::stod(line);

                            pfs.updateAmount(id, newAmt);

                            std::cout << "Category has been updated. \n\n";
                            pressEnterToContinue();
                            break;
                        }
                        default: {
                            std::cout << "Wrong Option";
                            break;
                        }
                    }
                }
                break;
            }

            case 5:
            {
                // DELETE TRANSACTION
                while (1)
                {
                    printHeader();
                    std::string line = "";
                    std::cout << "Enter id to delete: ";
                    std::getline(std::cin, line);

                    int idToDelete = std::stoi(line);

                    std::cout << "\nYou've chosen to delete id: "
                                << idToDelete << ". \n";

                    pfs.findTransaction(idToDelete);


                    std::cout << "\n\nAre you sure? (1 - Yes / 2 - No): ";
                    std::string tmp;
                    std::getline(std::cin, tmp);

                    int opts = std::stoi(tmp);

                    switch (opts)
                    {
                        case 1:
                        {
                            int d = pfs.deleteTransactionById(idToDelete);
                            if (d == 0)
                            {
                                std::cout << "\nTransaction has been deleted. \n\n";
                            }
                            pressEnterToContinue();
                            printHeader();
                            break;
                        }
                        case 2:
                        {
                            std::cout << "\nThere was a problem. \n";
                            pressEnterToContinue();
                            printHeader();
                            break;
                        }
                        default:
                        {
                            std::cout << "Wrong selection (1 - Yes / 2 - No)" << std::endl;
                            pressEnterToContinue();
                            printHeader();
                            continue;
                        }
                    }
                    break;
                }
                break;
            }

            case 6:
            {
                // ADD CATEGORY
                printHeader();

                std::cout << "\nEnter category >> ";

                std::string newCat = "";
                std::getline(std::cin, newCat);

                if (pfs.getCategoryId(newCat))
                {
                    std::cout << "Category already exists." << std::endl;
                } else
                {
                    pfs.addCategory(newCat);
                    std::cout << "Category added.\n";
                }

                break;
            }
            case 7:
            {
                printHeader();
                std::cout << "\nHello from updateCategory() \n";
                break;
            }
            case 8:
            {
                printHeader();
                std::cout << "\nHello from deleteCategory() \n";
                break;
            }
            case 9:
            {
                printHeader();
                std::cout << "\nHello from updateThreshold() \n";
                break;
            }

            case 23: {
                printHeader();
                std::cout << "\nYou have accessed the secret Location!\n\n";
                double balance = pfs.getBalance();

                std::cout << "If everything went well the "
                            << "ledger balance "
                            << " should be: "
                            << balance << std::endl;

                pressEnterToContinue();
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
