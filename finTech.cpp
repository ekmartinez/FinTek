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

class PersonalFinanceSystem {
	private:
		// Data structure
		std::vector<std::string> category;
	public:
		
		void addCategory(const std::string& str) {
			category.push_back(str);
			std::cout << "Hello from addTransaction()\n";
		}
		
		void displayCategories() const {
			std::cout << "Categories: ";
			for (size_t i = 0; i < category.size(); ++i)  {
				std::cout << category[i] << ", \n";
			}
			std::cout << "Hello from displayCategories()\n";
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

	// To test category array
	pfs.addCategory("Groceries");
	pfs.addCategory("Medicine");
	pfs.addCategory("Health Care");
	pfs.addCategory("Contributions");
	pfs.displayCategories();

	return 0;
}

// Add permanent storage
