#include <iostream>
#include <string>
#include <limits>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

class Client {
    int fdIn, fdOut;

public:
    bool connect() {
        fdIn = open("/tmp/numberpipe_in", O_WRONLY);
        fdOut = open("/tmp/numberpipe_out", O_RDONLY);
        if (fdIn == -1 || fdOut == -1) {
            cerr << "Cannot connect to daemon" << endl;
            return false;
        }
        return true;
    }
    
    void disconnect() {
        if (fdIn != -1) close(fdIn);
        if (fdOut != -1) close(fdOut);
    }
    
    bool send(const string& cmd, string& response) {
        if (write(fdIn, cmd.c_str(), cmd.length()) == -1) return false;
        char buffer[4096];
        ssize_t bytesRead = read(fdOut, buffer, sizeof(buffer) - 1);
        if (bytesRead == -1) return false;
        buffer[bytesRead] = '\0';
        response = buffer;
        return true;
    }
};

void showMenu() {
    cout << "\n=== Number Manager ===\n";
    cout << "1. Insert a number\n";
    cout << "2. Delete a number\n";
    cout << "3. Print all numbers\n";
    cout << "4. Delete all numbers\n";
    cout << "5. Exit\n";
    cout << "Choice: ";
}

int getChoice() {
    int choice;
    while (!(cin >> choice) || choice < 1 || choice > 5) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid choice. Enter 1-5: ";
    }
    cin.ignore();
    return choice;
}

int getNumber() {
    int num;
    while (!(cin >> num) || num <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter a positive integer: ";
    }
    cin.ignore();
    return num;
}

int main() {
    Client client;
    if (!client.connect()) {
        cout << "Start the daemon first: ./daemon\n";
        return 1;
    }
    
    while (true) {
        showMenu();
        int choice = getChoice();
        string cmd, response;
        
        switch (choice) {
            case 1: {
                cout << "Enter a positive integer: ";
                int num = getNumber();
                cmd = "INSERT " + to_string(num);
                break;
            }
            case 2: {
                cout << "Enter number to delete: ";
                int num;
                cin >> num;
                cin.ignore();
                cmd = "DELETE " + to_string(num);
                break;
            }
            case 3:
                cmd = "PRINT";
                break;
            case 4:
                cmd = "DELETE_ALL";
                break;
            case 5:
                client.disconnect();
                cout << "Goodbye!\n";
                return 0;
        }
        
        if (client.send(cmd, response)) {
            cout << response << endl;
        } else {
            cout << "Communication error\n";
        }
    }
}
