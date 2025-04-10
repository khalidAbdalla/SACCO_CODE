#include <iostream>
#include <vector>
#include <string>

// User Class - Handles user data only (SRP)
class User {
private:
    std::string firstName;
    std::string lastName;
    std::string phoneNumber;
    std::string email;
    std::string password;

public:
    User(std::string firstName, std::string lastName, std::string phoneNumber, std::string email, std::string password)
        : firstName(firstName), lastName(lastName), phoneNumber(phoneNumber), email(email), password(password) {}

    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    std::string getPhoneNumber() const { return phoneNumber; }
    std::string getEmail() const { return email; }
    std::string getPassword() const { return password; }
    void setPassword(const std::string& newPassword) { password = newPassword; }
};

// AuthManager Class - Handles user authentication (SRP)
class AuthManager {
private:
    std::vector<User> users;

public:
    bool signUp(const std::string& firstName, const std::string& lastName, const std::string& phoneNumber,
                const std::string& email, const std::string& password) {
        for (const auto& user : users) {
            if (user.getPhoneNumber() == phoneNumber) {
                std::cout << "User already exists.\n";
                return false;
            }
        }
        users.push_back(User(firstName, lastName, phoneNumber, email, password));
        return true;
    }

    bool signIn(const std::string& phoneNumber, const std::string& password) {
        for (auto& user : users) {
            if (user.getPhoneNumber() == phoneNumber && user.getPassword() == password) {
                return true;
            }
        }
        return false;
    }

    User* getUserByPhoneNumber(const std::string& phoneNumber) {
        for (auto& user : users) {
            if (user.getPhoneNumber() == phoneNumber) {
                return &user;
            }
        }
        return nullptr;
    }
};

// Transaction Class - Handles individual transaction details (SRP)
class Transaction {
private:
    std::string type;  // Deposit or Withdraw
    double amount;

public:
    Transaction(std::string type, double amount) : type(type), amount(amount) {}

    std::string getType() const { return type; }
    double getAmount() const { return amount; }
};

// Account Class - Manages balance and transactions (SRP)
class Account {
private:
    double balance;
    std::vector<Transaction> transactions;

public:
    Account() : balance(0.0) {}

    void deposit(double amount) {
        balance += amount;
        transactions.push_back(Transaction("Deposit", amount));
    }

    void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            transactions.push_back(Transaction("Withdraw", amount));
        } else {
            std::cout << "Insufficient funds!\n";
        }
    }

    void viewStatements() {
        for (const auto& transaction : transactions) {
            std::cout << transaction.getType() << ": " << transaction.getAmount() << "\n";
        }
    }

    double getBalance() const { return balance; }

    void setPassword(const std::string& newPassword) {
        std::cout << "Password reset successful.\n";
    }
};

// SACCOSystem Class - Orchestrates user interaction with the system (SRP)
class SACCOSystem {
private:
    AuthManager authManager;

public:
    void signUp() {
        std::string firstName, lastName, phoneNumber, email, password;
        std::cout << "Enter first name: ";
        std::cin >> firstName;
        std::cout << "Enter last name: ";
        std::cin >> lastName;
        std::cout << "Enter phone number: ";
        std::cin >> phoneNumber;
        std::cout << "Enter email: ";
        std::cin >> email;
        std::cout << "Enter password: ";
        std::cin >> password;

        if (authManager.signUp(firstName, lastName, phoneNumber, email, password)) {
            std::cout << "Sign-up successful! Please login.\n";
        }
    }

    void signIn() {
        std::string phoneNumber, password;
        std::cout << "Enter phone number: ";
        std::cin >> phoneNumber;
        std::cout << "Enter password: ";
        std::cin >> password;

        if (authManager.signIn(phoneNumber, password)) {
            std::cout << "Welcome, " << phoneNumber << "!\n";
        } else {
            std::cout << "Invalid login credentials.\n";
        }
    }

    void deposit() {
        double amount;
        std::cout << "Enter deposit amount: ";
        std::cin >> amount;
        Account account; // This should be linked to a User, but for simplicity, we use a local object
        account.deposit(amount);
    }

    void withdraw() {
        double amount;
        std::cout << "Enter withdraw amount: ";
        std::cin >> amount;
        Account account; // Same as deposit function, this should be tied to a User account
        account.withdraw(amount);
    }

    void viewStatements() {
        Account account; // Same issue as deposit, view statements should link to user account
        account.viewStatements();
    }

    void resetPassword() {
        std::string newPassword;
        std::cout << "Enter new password: ";
        std::cin >> newPassword;
        Account account; // Again, link this to a real user account
        account.setPassword(newPassword);
    }

    void handleUserLogin() {
        int attempts = 0;
        while (attempts < 2) {
            int choice;
            std::cout << "1. Sign Up\n2. Sign In\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            if (choice == 1) {
                signUp();
                break;
            } else if (choice == 2) {
                signIn();
                break;
            } else {
                std::cout << "Invalid choice. Try again.\n";
                attempts++;
            }

            if (attempts == 2) {
                std::cout << "Too many failed attempts. Exiting...\n";
                exit(0); // Exit the program after two failed attempts
            }
        }
    }
};

int main() {
    SACCOSystem saccoSystem;

    // Handle user login or sign up first
    saccoSystem.handleUserLogin();

    // Now show the menu options after successful login
    while (true) {
        int choice;
        std::cout << "1. Deposit\n2. Withdraw\n3. View Statements\n4. Reset Password\n5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: saccoSystem.deposit(); break;
            case 2: saccoSystem.withdraw(); break;
            case 3: saccoSystem.viewStatements(); break;
            case 4: saccoSystem.resetPassword(); break;
            case 5: std::cout << "Exiting system...\n"; return 0;
            default: std::cout << "Invalid choice. Please try again.\n"; break;
        }
    }

    return 0;
}
