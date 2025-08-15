#include <bits/stdc++.h>

using namespace std;

struct Expense {
    int id;
    string date;      // Format: YYYY-MM-DD
    string category;
    double amount;
    string note;
};

// Validate date format YYYY-MM-DD
bool isValidDate(const string &date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;

    string yearStr = date.substr(0, 4);
    string monthStr = date.substr(5, 2);
    string dayStr = date.substr(8, 2);

    if (!all_of(yearStr.begin(), yearStr.end(), ::isdigit) ||
        !all_of(monthStr.begin(), monthStr.end(), ::isdigit) ||
        !all_of(dayStr.begin(), dayStr.end(), ::isdigit))
        return false;

    int year = stoi(yearStr);
    int month = stoi(monthStr);
    int day = stoi(dayStr);

    if (month < 1 || month > 12 || day < 1 || day > 31)
        return false;

    return true;
}

void addExpense(vector<Expense> &expenses, int &nextId) {
    Expense e;
    e.id = nextId++;

    cout << "Enter date (YYYY-MM-DD): ";
    cin >> e.date;
    while (!isValidDate(e.date)) {
        cout << "Invalid date format! Try again: ";
        cin >> e.date;
    }

    cout << "Enter category: ";
    cin >> e.category;

    cout << "Enter amount: ";
    cin >> e.amount;

    cin.ignore();
    cout << "Enter note: ";
    getline(cin, e.note);

    expenses.push_back(e);
    cout << "✅ Expense added successfully!\n";
}

void viewExpenses(const vector<Expense> &expenses) {
    cout << left << setw(5) << "ID" << setw(12) << "Date"
         << setw(15) << "Category" << setw(10) << "Amount"
         << "Note\n";
    cout << string(50, '-') << "\n";

    for (const auto &e : expenses) {
        cout << left << setw(5) << e.id
             << setw(12) << e.date
             << setw(15) << e.category
             << setw(10) << fixed << setprecision(2) << e.amount
             << e.note << "\n";
    }
}

void deleteExpenses(vector<Expense> &expenses) {
    cout << "Enter IDs to delete (comma separated): ";
    string input;
    cin.ignore();
    getline(cin, input);

    stringstream ss(input);
    string token;
    vector<int> idsToDelete;

    while (getline(ss, token, ',')) {
        try {
            idsToDelete.push_back(stoi(token));
        } catch (...) {
            cout << "⚠ Invalid ID ignored: " << token << "\n";
        }
    }

    expenses.erase(remove_if(expenses.begin(), expenses.end(),
                             [&](const Expense &e) {
                                 return find(idsToDelete.begin(), idsToDelete.end(), e.id) != idsToDelete.end();
                             }),
                   expenses.end());

    cout << "🗑 Selected expenses deleted.\n";
}

int main() {
    vector<Expense> expenses;
    int nextId = 1;
    int choice;

    do {
        cout << "\n===== Expense Tracker =====\n";
        cout << "1. Add Expense\n";
        cout << "2. View Expenses\n";
        cout << "3. Delete Multiple Expenses\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addExpense(expenses, nextId);
                break;
            case 2:
                viewExpenses(expenses);
                break;
            case 3:
                deleteExpenses(expenses);
                break;
            case 4:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 4);

    return 0;
}
