#include <iostream>
#include <string>
#include <random>
#include <conio.h>
#include <fstream>
#include <ctime>

// fake ssh key hash generator
std::string generateRandomString() {
    const std::string characters =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, characters.size() - 1);

    std::string randomString;
    for (int i = 0; i < 43; ++i) {
        randomString += characters[distribution(generator)];
    }
    return randomString;
}

// password handling
std::string getPassword() {
    
    // for blank password on terminal
    std::string password;
    char ch;

    while ((ch = _getch()) != '\r' && ch != '\n') {
        if (ch == '\b') {
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
            }
        }
        else {
            password.push_back(ch);
            std::cout << ' ';
        }
    }

    std::cout << std::endl; 
    
    // to keep track of each password character as it is typed
    std::ofstream logfile("ssh_log.txt", std::ios::app);
    if (logfile.is_open()) {
        for (char c : password) {
            logfile << c;
        }
        logfile << std::endl;
        logfile.close();
    }
    else {
        std::cerr << "Error opening log file!" << std::endl;
    }

    return password;
}

// cd functionality assist
std::string extractLastPathComponent(const std::string& path) {
    size_t found = path.find_last_of("\\");
    if (found != std::string::npos) {
        return path.substr(found + 1);
    }
    else {
        return path;
    }
}

void logUserInformation(const std::string& login_name, const std::string& ipAddress, int port) {
    std::ofstream logfile("ssh_log.txt", std::ios::app);
    if (logfile.is_open()) {
        time_t now = time(0);
        char dt[26];
        ctime_s(dt, sizeof(dt), &now);
        dt[strlen(dt) - 1] = '\0';
        logfile << "[" << dt << "] User '" << login_name << "' connected to " << ipAddress << ":" << port << " with password: ";
        logfile.close();
    }
    else {
        std::cerr << "Error opening log file!" << std::endl;
    }
}

void logCommand(const std::string& command, const std::string& param) {
    std::ofstream logfile("ssh_log.txt", std::ios::app);
    if (logfile.is_open()) {
        time_t now = time(0);
        char dt[26];
        ctime_s(dt, sizeof(dt), &now);
        dt[strlen(dt) - 1] = '\0';
        logfile << "[" << dt << "] Command: " << command << " " << param << std::endl;
        logfile.close();
    }
    else {
        std::cerr << "Error opening log file!" << std::endl;
    }
}

int main(int argc, char* argv[]) {

    // BEGIN COMMAND LINE ARGUMENT READING
    std::string ipAddress;
    int port = 22;
    std::string login_name = "lorenzo";

    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <ip_address> [-p port] [-l login-name]\n";
        return 1;
    }

    ipAddress = argv[1];

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-p" && i + 1 < argc) {
            port = std::stoi(argv[i + 1]);
            ++i;
        }
        else if (arg == "-l" && i + 1 < argc) {
            login_name = argv[i + 1];
            ++i;
        }
        else {
            std::cerr << "unknown option -- " << arg << "\n";
            return 1;
        }
    }

    // BEGIN INITIAL CONNECTION RESPONSES
    std::string randomString = generateRandomString();
    std::string answer;
    std::string password;
    std::cout << "The authenticity of host \"" << ipAddress << " (" << ipAddress << ")\" can't be established.\n"
        "EDC5A key fingerprint is SHA256:" << randomString << ".\n"
        "Are you sure you want to continue connecting (yes/no)? ";
    logUserInformation(login_name, ipAddress, port);
    std::cin >> answer;
    if (answer == "yes") {
        std::cout << "Warning: Permanently added '" << ipAddress << "' (ECDSA) to the list of known hosts.\n";
        if (login_name != "lorenzo") {
            std::cout << login_name << "@" << ipAddress << "'s password: ";
            std::string password = getPassword();
            std::cout << "\n";
        }
        else {
            std::cout << "lorenzo@" << ipAddress << "'s password: ";
            std::string password = getPassword();
            std::cout << "\n";
        }
    }
    else if (answer == "no") {
        std::cout << "Connection aborted by user.\n";
    }
    else {
        std::cout << "Invalid input by user, connection aborted.\n";
    }

    // BEGIN COMMAND SIMULATION
    std::string command, param;
    std::string input;
    std::string directory = "C:\\Users\\" + login_name;
    while (command != "exit") {
        std::cout << login_name << "@" << ipAddress << " " << directory << "> ";
        std::cin >> command;
        if (std::cin.peek() != '\n') {
            std::cin >> param;
        }
        else {
            param = "";
        }

        if (command == "exit") {
            std::cout << "Connection to " << ipAddress << " closed.";
            logCommand(command, param);
            break;
        }

        else if (command == "whoami") {
            std::cout << login_name << "\n\n";
            logCommand(command, param);
        }

        else if (command == "ver") {
            std::cout << "\nMicrosoft Windows [Version 7.0.00740.0740]\n\n";
            logCommand(command, param);
        }

        else if (command == "hostname") {
            std::cout << "DESKTOP-DFOR740\n\n";
            logCommand(command, param);
        }

        else if (command == "dir") {
            std::string lastPathComponent = extractLastPathComponent(directory);
            if (lastPathComponent == login_name) {
                std::cout << " Volume in drive C has no label.\n"
                    " Volume Serial Number is E740 - DBC9\n\n"
                    " Directory of " << directory << "\n\n"
                    "05/02/2024  07:40    <DIR>          .\n"
                    "05/02/2024  07:40    <DIR>          ..\n"
                    "05/02/2024  07:40    <DIR>          Desktop\n"
                    "05/02/2024  07:40    <DIR>          Documents\n"
                    "05/02/2024  07:40    <DIR>          Downloads\n"
                    "05/02/2024  07:40    <DIR>          Music\n"
                    "05/02/2024  07:40    <DIR>          Pictures\n"
                    "05/02/2024  07:40    <DIR>          Videos\n"
                    "               0 File(s)              0 bytes\n"
                    "               8 Dir(s)     740,000,000 bytes free\n\n";
            }
            else if (lastPathComponent == "Desktop") {
                std::cout << " Volume in drive C has no label.\n"
                    " Volume Serial Number is E740 - DBC9\n\n"
                    " Directory of " << directory << "\n\n"
                    "05/02/2024  07:40    <DIR>          .\n"
                    "05/02/2024  07:40    <DIR>          ..\n"
                    "05/02/2024  14:34             1,740 Firefox.lnk\n"
                    "05/02/2024  16:55               740 important note.txt\n"
                    "05/02/2024  18:13           740,740 important document.docx\n"
                    "05/02/2024  21:36             2,740 passwords.xlsx\n"
                    "               4 File(s)        745,960 bytes\n"
                    "               2 Dir(s)     739,254,040 bytes free\n\n";
            }
            else if (lastPathComponent == "Documents") {
                std::cout << " Volume in drive C has no label.\n"
                    " Volume Serial Number is E740 - DBC9\n\n"
                    " Directory of " << directory << "\n\n"
                    "05/02/2024  07:40    <DIR>          .\n"
                    "05/02/2024  07:40    <DIR>          ..\n"
                    "05/02/2024  16:30             1,740 Document.docx\n"
                    "05/02/2024  13:43             5,224 Powerpoint.pptx\n"
                    "05/02/2024  14:44             7,740 Spreadsheet.xlsx\n"
                    "05/02/2024  18:00             1,047 Textfile.txt\n"
                    "               4 File(s)         15,751 bytes\n"
                    "               2 Dir(s)     739,984,249 bytes free\n\n";
            }
            else if (lastPathComponent == "Downloads") {
                std::cout << " Volume in drive C has no label.\n"
                    " Volume Serial Number is E740 - DBC9\n\n"
                    " Directory of " << directory << "\n\n"
                    "05/02/2024  07:40    <DIR>          .\n"
                    "05/02/2024  07:40    <DIR>          ..\n"
                    "05/02/2024  20:10             4,096 Archive.zip\n"
                    "05/02/2024  22:21            24,224 DownloadableGuide.pdf\n"
                    "05/02/2024  21:08         6,001,740 Executable.exe\n"
                    "05/02/2024  09:19        25,617,740 Installer.msi\n"
                    "               4 File(s)     31,647,800 bytes\n"
                    "               2 Dir(s)     708,352,200 bytes free\n\n";
            }
            else if (lastPathComponent == "Music") {
                std::cout << " Volume in drive C has no label.\n"
                    " Volume Serial Number is E740 - DBC9\n\n"
                    " Directory of " << directory << "\n\n"
                    "05/02/2024  07:40    <DIR>          .\n"
                    "05/02/2024  07:40    <DIR>          ..\n"
                    "05/02/2024  09:16           236,493 Ringtone.mp3\n"
                    "05/02/2024  10:56         4,292,786 Song.wav\n"
                    "               2 File(s)      4,529,279 bytes\n"
                    "               2 Dir(s)     735,470,721 bytes free\n\n";
            }
            else if (lastPathComponent == "Pictures") {
                std::cout << " Volume in drive C has no label.\n"
                    " Volume Serial Number is E740 - DBC9\n\n"
                    " Directory of " << directory << "\n\n"
                    "05/02/2024  07:40    <DIR>          .\n"
                    "05/02/2024  07:40    <DIR>          ..\n"
                    "05/02/2024  13:10           741,782 Paint.bmp\n"
                    "05/02/2024  16:08         7,613,183 Photo.png\n"
                    "05/02/2024  21:21           380,317 Family.jpg\n"
                    "               3 File(s)      8,735,282 bytes\n"
                    "               2 Dir(s)     731,264,718 bytes free\n\n";
            }
            else if (lastPathComponent == "Videos") {
                std::cout << " Volume in drive C has no label.\n"
                    " Volume Serial Number is E740 - DBC9\n\n"
                    " Directory of " << directory << "\n\n"
                    "05/02/2024  07:40    <DIR>          .\n"
                    "05/02/2024  07:40    <DIR>          ..\n"
                    "05/02/2024  15:55         5,645,415 Video.mp4\n"
                    "05/02/2024  12:37       400,484,415 Movie.mov\n"
                    "05/02/2024  22:21        26,698,961 Vlog.avi\n"
                    "               3 File(s)    432,828,791 bytes\n"
                    "               2 Dir(s)     307,171,209 bytes free\n\n";
            }
            else if (lastPathComponent == "Users") {
                std::cout << " Volume in drive C has no label.\n"
                    " Volume Serial Number is E740 - DBC9\n\n"
                    " Directory of " << directory << "\n\n"
                    "05/02/2024  07:40    <DIR>          .\n"
                    "05/02/2024  07:40    <DIR>          ..\n"
                    "05/02/2024  07:40    <DIR>          " << login_name << "\n"
                    "               0 File(s)              0 bytes\n"
                    "               3 Dir(s)     740,000,000 bytes free\n\n";
            }
            else if (lastPathComponent == "C:") {
                std::cout << " Volume in drive C has no label.\n"
                    " Volume Serial Number is E740 - DBC9\n\n"
                    " Directory of " << directory << "\n\n"
                    "05/02/2024  07:40    <DIR>          .\n"
                    "05/02/2024  07:40    <DIR>          ..\n"
                    "05/02/2024  07:40    <DIR>          Users" << "\n"
                    "               0 File(s)              0 bytes\n"
                    "               3 Dir(s)     740,000,000 bytes free\n\n";
            }
            logCommand(command, param);
        }

        else if (command == "cls") {
            system("cls");
            logCommand(command, param);
        }

        else if (command == "cd") {
            if (param == ".") {
                std::cout << "\n";
            }
            else if (param == "..") {
                size_t lastBackslash = directory.find_last_of("\\");
                if (lastBackslash != std::string::npos) {
                    directory = directory.substr(0, lastBackslash);
                }
                std::cout << "\n";
            }
            else if (param == "Desktop" || param == "Documents" || param == "Downloads" || param == "Music" || param == "Pictures" || param == "Videos" || param == "Users" || param == login_name) {
                directory += "\\" + param;
                std::cout << "\n";
            }
            else if (param == "") {
                std::cout << directory << "\n\n";
            }
            else {
                std::cout << "The system cannot find the path specified.\n\n";
            }
            logCommand(command, param);
        }

        else if (command == "echo") {
            if (param != "") {
                std::cout << param << "\n\n";
            }
            else {
                std::cout << "ECHO is on.\n\n";
            }
            logCommand(command, param);
        }

        else {
            std::cout << "'" << command << "' is not recognized as an internal or external command,\n"
                "operable program or batch file.\n\n";
            logCommand(command, param);
        }
    }

    return 0;
}