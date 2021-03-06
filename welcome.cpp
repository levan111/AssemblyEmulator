#include "welcome.h"


using namespace std;

unordered_map<string, long> registers;

extern long *sp;

vector<string> assembler_code;

unordered_map<string, vector<string> > functions;

void welcome(string &filename) {
    cout << "this emulator takes assembly code and executes it\n"
         << "i have already included some examples and you can run them \n"
         << "by using specific numbers below or you can execute your own file\n"
         << "by inputting your file path" << endl;

    cout << "choose 0 if you want to input your own filepath\n"
         << "choose 1 if you want to run factorial program\n"
         << "choose 2 if you want to run fibonacci program\n"
         << "choose 3 if you want to run sum program" << endl;

    cout << "remember that you can declare your functions anywhere you want in file\n"
         << "but you must put your main code in START/..../END uppercase is not necessary\n"
         << "also you can use tab and space anywhere you want but consider that your line shouldn't contain\n"
         << "alu store or load at the same time, otherwise it won't be executed and program will crash" << endl;


    long choice;
    cout << "enter your choice" << endl;
    cin >> choice;
    switch (choice) {
        case 0:
            cout << "enter file name below" << endl;
            cin >> filename;
            break;
        case 1:
            filename = "../factorial.txt";
            break;
        case 2:
            filename = "../fibonacci.txt";
            break;
        case 3:
            filename = "../sum.txt";
            break;
    }
}

void start() {
    for (int i = 0; i < assembler_code.size(); ++i) {
        if (assembler_code[i] == "START") {
            int index = i + 1;
            while (assembler_code[index] != "END") {
                functions["START"].push_back(assembler_code[index]);
                index++;
            }
        }
    }
}

void init_funcs() {
    for (int i = 0; i < assembler_code.size(); ++i) {
        if (assembler_code[i][0] == 'F') {
            string name = assembler_code[i].substr(1);
            i++;
            while (assembler_code[i] != "ENDF") {
                functions[name].push_back(assembler_code[i]);
                i++;
            }
        }
    }
}

string filter(string line) {
    string temp = line;
    for (int i = temp.length() - 1; i >= 0; i--) {
        if (temp[i] == ' ' || temp[i] == '  ' || temp[i] == '!') {
            temp.erase(i, 1);
        }
    }
    return temp;
}

void validate_lines() {
    for (int i = 0; i < assembler_code.size(); ++i) {
        string line = assembler_code[i];
        assembler_code[i] = filter(line);
    }
}

string upper_code(string code) {
    string temp = code;
    for (int i = 0; i < temp.length(); i++) {
        if (temp[i] >= 'a' && temp[i] <= 'z') {
            char a = temp[i];
            temp[i] = toupper(a);
        }
    }
    return temp;
}

void upload_code(ifstream &input) {
    string info;
    while (getline(input, info)) {
        if (info != "") {
            string forUpload = upper_code(info);
            assembler_code.push_back(forUpload);
        }
    }
    input.close();
}

void startup(string &file_name) {
    ifstream input(file_name);
    if (input.is_open()) {
        upload_code(input);
        validate_lines();
        registers["SP"] = (int64_t) (sp);
        start();
        init_funcs();
        exec();

    } else {
        cout << "ERROR: couldn't open file" << endl;
    }
}