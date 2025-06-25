#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h> //make sure openSSL is installed

using namespace std;

// Hash function using SHA-256
string hashPassword(const string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)password.c_str(), password.size(), hash);

    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    return ss.str();
}

// Load users from file into map
unordered_map<string, string> loadUsers(const string& filename) {
    unordered_map<string, string> users;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string username, password;
        if (getline(iss, username, ':') && getline(iss, password)) {
            users[username] = password;
        }
    }
    return users;
}

// Register function
void registerUser(const string& filename) {
    unordered_map<string, string> users = loadUsers(filename);
    string username, password;

    cout << "Enter new username: ";
    cin >> username;
    if (users.find(username) != users.end()) {
        cout << "Username already exists.\n";
        return;
    }

    cout << "Enter new password: ";
    cin >> password;

    string hashed = hashPassword(password);
    ofstream file(filename, ios::app);
    file << username << ":" << hashed << endl;

    cout << "Registration successful.\n";
}

// Login function
void loginUser(const string& filename) {
    unordered_map<string, string> users = loadUsers(filename);
    string username, password;

    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    string hashed = hashPassword(password);

    if (users.find(username) != users.end() && users[username] == hashed) {
        cout << "Login successful.\n";
    } else {
        cout << "Invalid username or password.\n";
    }
}

// Main menu
int main() {
    const string filename = "users.txt";
    int choice;

    do {
        cout << "\n1. Register\n2. Login\n3. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                registerUser(filename);
                break;
            case 2:
                loginUser(filename);
                break;
            case 3:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 3);

    return 0;
}
