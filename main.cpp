#include <cmath>
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
        pfs.showMonthlySummary();
        // Ends Dashboard


		std::cout << "\n"
            << "-------------Reporting---------------\n"
            << "|  1. Net Income / Loss per Period  |\n"
			<< "|  2. Search by Date                |\n"
			<< "|  3. View Transactions             |\n"
			<< "|  4. View Categories               |\n"
			<< "-------------------------------------\n\n"
            << "------------Transactions-------------\n"
            << "|  5. Add Transaction               |\n"
			<< "|  6. Update Transaction            |\n"
			<< "|  7. Delete Transaction            |\n"
			<< "-------------------------------------\n\n"
            << "-------------Categories--------------\n"
			<< "|  8. Add Category                  |\n"
			<< "|  9. Update Category               |\n"
			<< "| 10. Delete Category               |\n"
			<< "-------------------------------------\n\n"
            << "------------Configurations-----------\n"
			<< "| 11. Update Threshold Amount       |\n"
			<< "-------------------------------------\n"
			<< "|  0. Exit                          |\n"
			<< "-------------------------------------\n\n"
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
                // Monthly Income / Expense
                printHeader();
                pfs.showYearlySummary();
                pressEnterToContinue();
                break;
                }

            case 2: {
                // Search by Date
                printHeader();
                std::cout << "\nSearch by Date\n--------------\n\n";

                std::string startDate = "";
                std::cout << "\nEnter start date (Blank for None) >> ";
                std::getline(std::cin, startDate);
                if (startDate.empty()) { startDate = "0000-01-01"; }

                std::string endDate = "";
                std::cout << "\nEnter end date (Blank for None) >> ";
                std::getline(std::cin, endDate);
                if (endDate.empty()) { endDate = "9999-12-31"; }

                pfs.queryTransactionsByDateRange(startDate, endDate);
                pfs.printDateRangeResults();

                pressEnterToContinue();

                break;
            }

            case 3:
            {
                // SEARCH TRANSACTIONS
                while (1)
                {

                    printHeader();
                    std::string id = "";
                    std::cout << "\nView Transaction\n------------------\n";
                    std::cout << "\nEnter Id of transaction (None for all): ";
                    std::getline(std::cin, id);

                    if (id.empty()) {
                        pfs.viewTransaction(0);
                        pressEnterToContinue();
                        break;
                    } else {
                        pfs.viewTransaction(std::stoi(id));
                        pressEnterToContinue();
                        break;
                    }
                }
                break;
            }

            case 4:
              {
                printHeader();

                std::cout << "----------------\n";
                std::cout << "|  Categories  |\n";
                std::cout << "----------------\n\n";

                // Income Categories
                std::cout << "-------------------------------\n";
                std::cout << "|      Income Categories      |\n";
                std::cout << "-------------------------------\n";
                for (const auto& c : pfs.viewCategories())
                {
                    if (c.Type == "Income")
                    {
                        printf("| %-14s | %-10.2f |\n", c.categoryDescription.c_str(), c.totalAmount);
                    }
                }
                std::cout << "-------------------------------\n\n";

                // Expense Categories
                std::cout << "-------------------------------\n";
                std::cout << "|      Expense Categories     |\n";
                std::cout << "-------------------------------\n";
                for (const auto& c : pfs.viewCategories())
                {
                  if (c.Type == "Expense")
                    {
                        printf("| %-14s | %-10.2f |\n", c.categoryDescription.c_str(),
                        c.totalAmount);
                    }
                }
                std::cout << "-------------------------------\n";

                pressEnterToContinue();
                break;
            }

            case 5:
                {
                while(1) {
                    // ADD TRANSACTION

                    std::string date = "";

                    while (1)
                    {
                        date = getCurrentDate();
                        printHeader();

                        std::cout << "\nEnter Transaction's date "
                            << "(leave blank for today: " << date << "): ";

                        std::getline(std::cin, date);

                        if (date.empty())
                        {
                            date = getCurrentDate();
                            break;
                        }
                        break;
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
                                break;
                            }
                        }
                        break;
                    }

                    // ENTER DESCRIPTION
                    // TODO: Limit size

                    std::string description = "";
                    std::cout << "Enter Description: ";
                    std::getline(std::cin, description);
                    // ----------------------------------

                    // CATEGORIZATION
                    // -------------------------------------------
                    int categoryId = 0;
                    std::string categoryName = "";
                    while (1) {
                        std::cout << "Enter category: ";
                        std::getline(std::cin, categoryName);
                        // ----------------------------------

                        if (pfs.getCategoryId(categoryName) == -1)
                        {
                            std::cout << "Category does not exist, do you want to add it? (1 - Yes | 2 - No): ";

                            std::string tmp = "";
                            std::getline(std::cin, tmp);
                            int opts = std::stoi(tmp);

                            if (opts == 1)
                            {
                                pfs.addCategory(categoryName);
                                categoryId = pfs.getCategoryId(categoryName);
                                break;
                            } else
                            {
                                std::cout << "You can't progress unless you provide a category.\n\n";
                                std::cout << "Do you want to exit? (1 - Yes | 2 - No): ";
                                std::getline(std::cin, tmp);
                                opts = std::stoi(tmp);
                                if (opts == 1) { break; } else { continue; }
                            }
                        }
                        break;
                    }

                    categoryId = pfs.getCategoryId(categoryName);
                    // ENDS CATEGORIZATION

                    // AMOUNT
                    std::string tmpAmt = "";
                    std::cout << "Enter transaction's amount: ";
                    std::getline(std::cin, tmpAmt);
                    double amount = std::stod(tmpAmt);
                    // Threshold stored in db - Todo: check.

                    // Just to summarize

                    // Data summary before saving
                    std::cout << "\nData Entered and ready to be saved:\n\n"
                            << "Date: " << date << std::endl
                            << "Description: " << description << std::endl
                            << "CategoryId: " << categoryId << std::endl
                            << "CategoryName: " << categoryName << std::endl
                            << "Amount: " << amount << std::endl
                            << "Type: " << type << std::endl;

                    std::cout << "\nSave? (y|n) >> ";
                    std::string input;
                    std::getline(std::cin, input);

                    double absAmount = std::abs(amount);

                    if (input == "y")
                    {
                        // income / expense
                        if (type == "expense") {
                            amount = -absAmount;

                        } else if (type == "income") {
                            amount = absAmount;
                        } else {
                            std::cout << "Something went wrong.\n";
                            break;
                        }

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
                        } else if (input == "n")
                            printHeader();
                            break;
                    } else
                    {
                        std::cout << "Wrong option.\n";
                        break;
                    }
                }
                break;
            }

            case 6:
                // update
            {
              while (1)
              {
                    printHeader();
                    std::string tmp = "";
                    std::cout << "Enter Id of transaction: ";
                    std::getline(std::cin, tmp);
                    int id = std::stoi(tmp);


                    std::cout << "\nYou've chosen to update id: " << id << ". \n";
                    pfs.viewTransaction(id);

                    int i = 1;

                    std::vector<std::string> fields = {
                        "Date",     "Type",   "Description",
                        "Category", "Amount", "Exit"};

                    if (pfs.findIndexById(id == 1))
                    {
                        std::cout << "\n\n------------------------\n";
                        std::cout << "|       Change?        |\n";
                        std::cout << "------------------------\n";

                        printf("| %-4s |  %-10s   |\n%23s", "Opt", "Field", "------------------------\n");

                        for (auto& field : fields)
                        {
                            printf("| %-4d | ", i++);
                            printf(" %-12s |\n", field.c_str());
                        }

                        std::cout << "------------------------\n\n";
                        std::cout << "Choose an option: ";
                        std::getline(std::cin, tmp);
                        int choice = std::stoi(tmp);

                        switch (choice)
                        {
                            case 1:
                            {
                                std::cout << "Enter new Date: ";
                                std::getline(std::cin, tmp);
                                pfs.updateRecord(id, "Date", tmp);

                                std::cout << "Date has been updated!.\n";
                                pressEnterToContinue();
                                break;
                            }
                            case 2:
                            {
                                std::cout << "Enter new Type: ";
                                std::getline(std::cin, tmp);
                                pfs.updateRecord(id, "Type", tmp);

                                std::cout << "Type has been updated!.\n";
                                pressEnterToContinue();

                                break;
                            }
                            case 3:
                            {
                                std::cout << "Enter new Description: ";
                                std::getline(std::cin, tmp);
                                pfs.updateRecord(id, "Description", tmp);

                                std::cout << "Descripton has been updated!.\n";
                                pressEnterToContinue();

                                break;
                            }
                            case 4:
                            {
                                std::cout << "Enter new Category: ";
                                std::getline(std::cin, tmp);
                                pfs.updateRecord(id, "Category", tmp);

                                std::cout << "Category has been updated!.\n";
                                pressEnterToContinue();

                                break;
                            }
                            case 5:
                            {
                                std::cout << "Enter new Amount: ";
                                std::getline(std::cin, tmp);
                                pfs.updateRecord(id, "Amount", tmp);

                                std::cout << "Amount has been updated!.\n";
                                pressEnterToContinue();

                                break;
                            }
                            case 6:
                            {
                                std::cout << "Exiting.\n";
                                pressEnterToContinue();
                                break;
                            }
                            default:
                            {
                                std::cout << "Wrong Option.";
                                break;
                            }
                        }
                      pfs.loadTransactionFromDB();
                    } else
                    {
                        std::cout << "Id not found.\n";
                        pressEnterToContinue();
                        continue;
                    }
                break;
              }
              break;
            }
            case 7:
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

                    pfs.viewTransaction(idToDelete);

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

                            pfs.loadTransactionFromDB();
                            pressEnterToContinue();
                            printHeader();
                            break;
                        }
                        case 2:
                        {
                            std::cout << "\nThere was a problem. \n";

                            pfs.loadTransactionFromDB();
                            pressEnterToContinue();
                            printHeader();
                            break;
                        }
                        default:
                        {
                            std::cout << "Wrong selection (1 - Yes / 2 - No)" << std::endl;
                            pressEnterToContinue();
                            printHeader();
                            break;
                        }
                    }
                    break;
                }
                break;
            }

            case 8:
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
            case 9:
            {
                printHeader();
                std::cout << "\nHello from updateCategory() \n";
                break;
            }
            case 10:
            {
                printHeader();
                std::cout << "\nHello from deleteCategory() \n";
                break;
            }
            case 11:
            {
                printHeader();
                std::cout << "\nHello from updateThreshold() \n";
                break;
   }

            case 23: {
                printHeader();
                std::cout << "\nYou have accessed the secret Location!\n\n";

                pfs.showYearlySummary();

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
