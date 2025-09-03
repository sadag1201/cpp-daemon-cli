#include<bits/stdc++.h>
using namespace std;

class NumberManager {
    map<int, time_t> numbers;
    mutex mtx;
    
public:
    string insert(int num) {
        lock_guard<mutex> lock(mtx);
        if (numbers.count(num)) return "ERROR: Number already exists";
        
        time_t now = time(nullptr);
        numbers[num] = now;
        cout << "Inserted: " << num << " at " << now << endl;
        return "Number " + to_string(num) + " inserted at " + to_string(now);
    }
    
    string remove(int num) {
        lock_guard<mutex> lock(mtx);
        auto it = numbers.find(num);
        if (it == numbers.end()) return "ERROR: Number not found";
        
        numbers.erase(it);
        cout << "Deleted: " << num << endl;
        return "Number " + to_string(num) + " deleted";
    }
    
    string printAll() {
        lock_guard<mutex> lock(mtx);
        if (numbers.empty()) return "No numbers stored";
        
        string result = "Numbers:\n";
        for (auto& p : numbers) {
            result += to_string(p.first) + " -> " + to_string(p.second) + "\n";
        }
        return result;
    }
    
    string deleteAll() {
        lock_guard<mutex> lock(mtx);
        size_t count = numbers.size();
        numbers.clear();
        cout << "Deleted all " << count << " numbers" << endl;
        return "Deleted " + to_string(count) + " numbers";
    }
};

string processCommand(NumberManager& mgr, const string& cmd) {
    istringstream iss(cmd);
    string command;
    iss >> command;
    
    if (command == "INSERT") {
        int num;
        if (iss >> num && num > 0) return mgr.insert(num);
        return "ERROR: Invalid number";
    } else if (command == "DELETE") {
        int num;
        if (iss >> num) return mgr.remove(num);
        return "ERROR: Invalid number";
    } else if (command == "PRINT") {
        return mgr.printAll();
    } else if (command == "DELETE_ALL") {
        return mgr.deleteAll();
    }
    return "ERROR: Unknown command";
}

int main() {
    NumberManager mgr;
    const char* pipePath = "/tmp/numberpipe";
    
    unlink(pipePath);
    if (mkfifo(pipePath, 0666) != 0) {
        cerr << "Failed to create pipe" << endl;
        return 1;
    }
    
    cout << "Daemon started on " << pipePath << endl;
    
    while (true) {
        int fd = open(pipePath, O_RDWR);
        if (fd != -1) {
            char buffer[1024];
            ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                string response = processCommand(mgr, buffer);
                write(fd, response.c_str(), response.length());
            }
            close(fd);
        }
    }
    
    return 0;
}
