/*
'########:'####:'##::: ##:'########:'########:'##:::'##::::::::::'##::::'##::::
 ##.....::. ##:: ###:: ##:... ##..:: ##.....:: ##::'##:::::::::::. ##:::. ##:::
 ##:::::::: ##:: ####: ##:::: ##:::: ##::::::: ##:'##:::::::::::::. ##:::. ##::
 ######:::: ##:: ## ## ##:::: ##:::: ######::: #####::::'#######:::. ##:::. ##:
 ##...::::: ##:: ##. ####:::: ##:::: ##...:::: ##. ##:::........::: ##:::: ##::
 ##:::::::: ##:: ##:. ###:::: ##:::: ##::::::: ##:. ##:::::::::::: ##:::: ##:::
 ##:::::::'####: ##::. ##:::: ##:::: ########: ##::. ##:::::::::: ##:::: ##::::
..::::::::....::..::::..:::::..:::::........::..::::..:::::::::::..:::::..:::::
*/
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

class PersonalFinanceSystem {
	private:
		std::vector<std::string> category;
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

		void addTransaction() {
			std::cout << "Hello from addTransaction()\n";
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
	std:: cout << "FinTek - Personal Finance System\n" << std::endl;

	// To test category array
	pfs.addCategory("Groceries");
	pfs.addCategory("Medicine");
	pfs.addCategory("Health Care");
	pfs.addCategory("Contributions");
	pfs.displayCategories();
	pfs.deleteCategory("Groceries");
	pfs.displayCategories();
	return 0;
}

// Add permanent storage
