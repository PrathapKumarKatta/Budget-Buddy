#include <bits/stdc++.h>
using namespace std;
struct Expense {
    int id;
    string date;
    string category;
    double amount;
    string note;
};


void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool parseExpense(const string& line, Expense& e) {
    if (line.empty()) return false;
    size_t p1 = line.find(',');
    size_t p2 = line.find(',', p1 + 1);
    size_t p3 = line.find(',', p2 + 1);
    size_t p4 = line.find(',', p3 + 1);

    if (p1 == string::npos || p2 == string::npos || p3 == string::npos || p4 == string::npos) {
        return false;
    }
    e.id = stoi(line.substr(0, p1));
    e.date = line.substr(p1 + 1, p2 - p1 - 1);
    e.category = line.substr(p2 + 1, p3 - p2 - 1);
    e.amount = stod(line.substr(p3 + 1, p4 - p3 - 1));
    e.note = line.substr(p4 + 1);
    return true;
}

vector<Expense> readExpenses() {
    vector<Expense> expenses;
    ifstream file("expenses.csv");
    if (!file) return expenses;
    string line;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        Expense e;
        if (parseExpense(line, e)) {
            expenses.push_back(e);
        }
    }
    file.close();
    return expenses;
}

int getNextID() {
    vector<Expense> expenses = readExpenses();
    if (expenses.empty()) {
        return 1;
    }
    return expenses.back().id + 1;
}

bool isValidDateFormat(const string& date) {
    if (date.length() != 10) return false;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            if (date[i] != '-') return false;
        } else if (!isdigit(date[i])) {
            return false;
        }
    }
    return true;
}

void addExpense() {
    bool fileExists = false;
    ifstream checkFile("expenses.csv");
    if (checkFile.good()) {
        fileExists = true;
    }
    checkFile.close();
    ofstream file("expenses.csv", ios::app);
    if (!file) {
        cout << "Error: Unable to open file.\n";
        return;
    }
    if (!fileExists) {
        file << "ID,Date,Category,Amount,Note\n";
    }
    Expense e;
    e.id = getNextID();

    string input;
    do {
        cout << "Enter date (YYYY-MM-DD): ";
        cin >> input;
        e.date = input;
        if (!isValidDateFormat(e.date)) {
            cout << "Invalid date format! Please use YYYY-MM-DD.\n";
        }
    } while (!isValidDateFormat(e.date));
    clearInputBuffer();
    cout << "Enter category: ";
    getline(cin, e.category);
    cout << "Enter amount: ";
    while (!(cin >> e.amount) || e.amount < 0) {
        cout << "Invalid amount. Please enter again: ";
        cin.clear();
        clearInputBuffer();
    }
    clearInputBuffer();

    cout << "Enter note: ";
    getline(cin, e.note);

    file << e.id << "," << e.date << "," << e.category << ","
         << fixed << setprecision(2) << e.amount << "," << e.note << "\n";

    file.close();
    cout << "Expense added successfully!\n";
}

void printExpenses(const vector<Expense>& expenses, const string& header = "All Expenses") {
    if (expenses.empty()) {
        cout << "No expenses found.\n";
        return;
    }
    cout << "\n--- " << header << " ---\n";
    cout << left << setw(5) << "ID"
         << setw(15) << "Date"
         << setw(15) << "Category"
         << setw(12) << "Amount"
         << "Note" << endl;
    cout << "-------------------------------------------------------------\n";

    double total = 0.0;
    for (const auto& e : expenses) {
        cout << left << setw(5) << e.id
             << setw(15) << e.date
             << setw(15) << e.category
             << setw(12) << fixed << setprecision(2) << e.amount
             << e.note << endl;
        total += e.amount;
    }
    cout << "-------------------------------------------------------------\n";
    cout << "Total Expenses: " << fixed << setprecision(2) << total << endl;
}


void viewExpenses() {
    vector<Expense> expenses = readExpenses();
    printExpenses(expenses);
}


void clearExpenses() {
    ofstream file("expenses.csv", ios::trunc);
    file.close();
    cout << "All expenses cleared!\n";
}


void searchExpenses(const string& searchPrompt, const function<bool(const Expense&)>& predicate) {
    vector<Expense> allExpenses = readExpenses();
    vector<Expense> searchResults;
    for (const auto& e : allExpenses) {
        if (predicate(e)) {
            searchResults.push_back(e);
        }
    }
    printExpenses(searchResults, searchPrompt);
}

int main() {
    int choice;
    do {
        cout << "\n============================================\n";
        cout << "           EXPENSE TRACKER MENU             \n";
        cout << "============================================\n";
        cout << "1. Add Expense\n";
        cout << "2. View All Expenses\n";
        cout << "3. Clear All Expenses\n";
        cout << "4. Search by Date\n";
        cout << "5. Search by Category\n";
        cout << "6. Exit\n";
        cout << "--------------------------------------------\n";
        cout << "Enter choice: ";

        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number (1-6): ";
            cin.clear();
            clearInputBuffer();
        }
        clearInputBuffer();

        switch (choice) {
            case 1: addExpense(); break;
            case 2: viewExpenses(); break;
            case 3: clearExpenses(); break;
            case 4: {
                string searchDate;
                do {
                    cout << "Enter date to search (YYYY-MM-DD): ";
                    cin >> searchDate;
                    if (!isValidDateFormat(searchDate)) {
                        cout << "Invalid date format! Please use YYYY-MM-DD.\n";
                    }
                } while (!isValidDateFormat(searchDate));
                searchExpenses("Expenses for " + searchDate, [=](const Expense& e) {
                    return e.date == searchDate;
                });
                break;
            }
            case 5: {
                string searchCategory;
                cout << "Enter category to search: ";
                getline(cin, searchCategory);
                searchExpenses("Expenses for " + searchCategory, [=](const Expense& e) {
                    return e.category == searchCategory;
                });
                break;
            }
            case 6:
                cout << "\nGoodbye! Have a great day!\n";
                cout << "Press Enter to exit...";
                cin.get();
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 6);

    return 0;
}

