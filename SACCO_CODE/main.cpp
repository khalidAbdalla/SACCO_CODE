#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

string generatePassword() {
    const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    string password;
    srand(time(0));

    for (int i = 0; i < 8; ++i) {
        password += chars[rand() % chars.length()];
    }
    return password;
}

class User {
private:
    string firstName;
    string lastName;
    string phoneNumber;
    string email;
    string password;

public:
    User(string firstName, string lastName, string phoneNumber, string email, string password)
        : firstName(firstName), lastName(lastName), phoneNumber(phoneNumber), email(email), password(password) {}

    string getFirstName() const { return firstName; }
    string getPhoneNumber() const { return phoneNumber; }
    string getPassword() const { return password; }
    void setPassword(const string& newPassword) { password = newPassword; }
};

class Account {
private:
    double balance = 0.0;
    vector<string> transactions;

public:
    void deposit(double amount) {
        balance += amount;
        transactions.push_back("Deposited: " + to_string(amount));
    }

    void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            transactions.push_back("Withdrew: " + to_string(amount));
        } else {
            throw runtime_error("Insufficient funds!");
        }
    }

    void printStatement() const {
        cout << "\n=== Account Statement ===\n";
        cout << "Current Balance: " << balance << "\n\n";
        cout << "Transaction History:\n";
        for (const auto& t : transactions) {
            cout << t << "\n";
        }
    }
};

class AuthManager {
private:
    vector<User> users;
    User* currentUser = nullptr;
    Account currentAccount;

public:
    bool signUp(const string& firstName, const string& lastName,
                const string& phoneNumber, const string& email) {
        for (const auto& user : users) {
            if (user.getPhoneNumber() == phoneNumber) {
                return false;
            }
        }
        string tempPassword = generatePassword();
        users.emplace_back(firstName, lastName, phoneNumber, email, tempPassword);
        cout << "\nYour temporary password is: " << tempPassword << endl;
        cout << "Please change it after login for security.\n";
        return true;
    }

    bool signIn(const string& phoneNumber, const string& password) {
        for (auto& user : users) {
            if (user.getPhoneNumber() == phoneNumber && user.getPassword() == password) {
                currentUser = &user;
                cout << "\nWelcome Farmer " << user.getFirstName() << "!\n";
                return true;
            }
        }
        return false;
    }

    bool isAuthenticated() const { return currentUser != nullptr; }

    void changePassword(const string& newPassword) {
        if (currentUser) {
            currentUser->setPassword(newPassword);
            cout << "Password changed successfully!\n";
        }
    }

    Account& getAccount() { return currentAccount; }

    void promptPasswordChange() {
        if (currentUser) {
            string newPassword;
            cout << "\nFor security, please change your password:\n";
            cout << "Enter new password: ";
            cin >> newPassword;
            changePassword(newPassword);
        }
    }
};

class SACCOSystem {
private:
    AuthManager auth;
    int attemptsLeft = 2;

    bool authenticate() {
        while (attemptsLeft > 0) {
            cout << "\n1. Sign Up\n2. Sign In\nChoice: ";
            int choice;
            cin >> choice;

            if (choice == 1) {
                string fn, ln, phone, email;
                cout << "First Name: "; cin >> fn;
                cout << "Last Name: "; cin >> ln;
                cout << "Phone: "; cin >> phone;
                cout << "Email: "; cin >> email;

                if (auth.signUp(fn, ln, phone, email)) {
                    cout << "Sign up successful! Please sign in.\n";
                } else {
                    cout << "Phone number already exists!\n";
                }
            }
            else if (choice == 2) {
                string phone, pw;
                cout << "Phone: "; cin >> phone;
                cout << "Password: "; cin >> pw;

                if (auth.signIn(phone, pw)) {
                    auth.promptPasswordChange();
                    return true;
                } else {
                    cout << "Invalid credentials. Attempts left: " << --attemptsLeft << "\n";
                }
            }
            else {
                cout << "Invalid choice. Attempts left: " << --attemptsLeft << "\n";
            }
        }
        return false;
    }

public:
    void run() {
        cout << "=== Welcome to Farmers SACCO System ===\n";

        if (!authenticate()) {
            cout << "Too many failed attempts. Exiting...\n";
            return;
        }

        while (true) {
            cout << "\n1. Deposit\n2. Withdraw\n3. Statement\n4. Change Password\n5. Exit\nChoice: ";
            int choice;
            cin >> choice;

            try {
                switch (choice) {
                    case 1: {
                        double amount;
                        cout << "Amount: "; cin >> amount;
                        auth.getAccount().deposit(amount);
                        cout << "Deposit successful!\n";
                        break;
                    }
                    case 2: {
                        double amount;
                        cout << "Amount: "; cin >> amount;
                        auth.getAccount().withdraw(amount);
                        cout << "Withdrawal successful!\n";
                        break;
                    }
                    case 3:
                        auth.getAccount().printStatement();
                        break;
                    case 4: {
                        string newPw;
                        cout << "New Password: "; cin >> newPw;
                        auth.changePassword(newPw);
                        break;
                    }
                    case 5:
                        cout << "Goodbye!\n";
                        return;
                    default:
                        cout << "Invalid choice!\n";
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
    }
};

int main() {
    SACCOSystem system;
    system.run();
    return 0;
}
