#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>

#include "helpers.h"
#include "personalFinanceSystem.h"

void PersonalFinanceSystem::addCategory(const std::string& str) {
	category.push_back(str);
	std::cout << "Added: " << str << std::endl;
}

void PersonalFinanceSystem::displayCategories() {
	std::cout << "\nCategories\n" << "-----------\n";
	for (size_t i = 0; i < category.size(); ++i)  {
			std::cout << category[i] << ", \n";
	}
}

void PersonalFinanceSystem::updateCategory(const int id, const std::string newCategory) {
	std::cout << "Hello from updateCategory()\n";	
}

void PersonalFinanceSystem::addTransaction(int id, const std::string& date, 
		const std::string& desc, const std::string& cat, const double amt) {

	Transactions newTransaction = {id, date, desc, cat, amt};
	transactions.push_back(newTransaction);
	std::cout << "Data saved.;";
	
 }

void PersonalFinanceSystem::displayTransactions() {

	// Header
	printf("%-8s %-12s %-25s %-15s %-10s",
			"Id", "Date", "Description", "Category", "Amount");

	// Lines
	for (const auto& t : transactions) {
		printf("\n%-8d %-12s %-25s %-15s %-10.2f",
			t.id, t.date.c_str(), t.desc.c_str(), t.cat.c_str(), t.amt);
	}
}

int PersonalFinanceSystem::searchTransaction(const int id) {
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

int PersonalFinanceSystem::findIndexById(int targetId) {

	for (size_t i = 0; i < transactions.size(); ++i) {
		if (transactions[i].id == targetId) {
			return static_cast<int>(i);
		}
	}

	return -1;
}

void PersonalFinanceSystem::updateDate(const int id, const std::string newDate) {

	int index = findIndexById(id);

	if (isValidDate(newDate)) {
		transactions[index].date = newDate;
	} else { std::cout << "Something went wrong in updateDate()\n"; }
}

void PersonalFinanceSystem::updateDescription(const int id, const std::string newDesc) {
	int i = findIndexById(id);
	transactions[i].desc = newDesc;
}

void PersonalFinanceSystem::updateAmount(const int id, const double newAmt) {
	int i = findIndexById(id);
	transactions[i].amt = newAmt;
}

int PersonalFinanceSystem::deleteTransactionById(int targetId) {
	auto it = std::remove_if(transactions.begin(), transactions.end(),
				[targetId](const Transactions& t) { return t.id == targetId; });

	if (it != transactions.end()) {
		transactions.erase(it, transactions.end());
		std::cout << "Transaction with ID " << targetId << " deleted.\n";
		return 1;
	}

	return -1;
}

void PersonalFinanceSystem::summaryReport() {
	std::cout << "Hello from summaryReport()\n";
}

int PersonalFinanceSystem::getLastId() {
  if (transactions.empty()) { 
	  return 0; 
	} else { return transactions.back().id; }
}
