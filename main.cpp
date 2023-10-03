#include <iostream> // для std:: count and std cin
#include <fstream> // для роботи з файлами
#include <cstring> // для strcpy strcat

const int MAX_SIZE = 100;

class TextEditor {
private:
    char* Buffer;
    bool has_text;

public:
    TextEditor() : Buffer(nullptr), has_text(false) {}
    ~TextEditor() { // деструктор метод в класі, який створюється перед звільненням буферу
        delete[] Buffer;
    }

    void TextToAppend() {
        std::cout << "Enter text to append:";
        char text[MAX_SIZE];
        std::cin.getline(text, MAX_SIZE);
        if (!has_text) {
            Buffer = new char[strlen(text) + 1];
            strcpy(Buffer, text);
        } else {
            char* temp = new char[strlen(Buffer) + strlen(text) + 1];
            strcpy(temp, Buffer);
            strcat(temp, " ");
            strcat(temp, text);
            delete[] Buffer;
            Buffer = temp;
        }
        has_text = true;
        std::cout << Buffer;
    }

    void NewLine() {
        strcat(Buffer, "\n");
        std::cout << "New line is started\n";
    }

    void ForSavingInFile() {
        std::string fileForSaving;
        std::cout << "Enter the file name for saving: ";
        std::cin >> fileForSaving;

        std::ofstream file(fileForSaving);

        if (file.is_open()) {
            file << Buffer;
            file.close();
            std::cout << "Text has been saved successfully" << std::endl;
        } else {
            std::cerr << "Error: Unable to open file for writing" << std::endl;
        }
    }

    void ForLoadingFromFile() {
        char fileForLoading[MAX_SIZE];
        std::cout << "Enter the file name for loading:";
        std::cin >> fileForLoading;
        std::ifstream loadFile(fileForLoading);
        if (!loadFile) {
            std::cout << "Error opening file for loading\n";
        } else {
            loadFile.seekg(0, std::ios::end);
            size_t fileSize = loadFile.tellg();
            loadFile.seekg(0, std::ios::beg);
            Buffer = new char[fileSize + 1];
            loadFile.read(Buffer, fileSize);
            Buffer[fileSize] = '\0';
            std::cout << "Text has been loaded successfully\n";
            loadFile.close();
            has_text = true;
        }
    }

    void PrintLoadedText() {
        std::cout << Buffer;
    }

    void InsertAtIndex(int index1, int index2, const char* wordForAdding) {
        std::cout << "Choose line and index:";
        std::cin >> index1 >> index2;
        std::cout << "Enter text to insert:\n";
        char text[MAX_SIZE];
        std::cin.ignore();
        std::cin.getline(text, MAX_SIZE);

        char* lines[20];
        int num_lines = 0;
        char* line = strtok(Buffer, "\n");
        while (line != nullptr) {
            lines[num_lines] = line;
            num_lines++;
            line = strtok(nullptr, "\n");
        }

        if (index1 >= 0 && index1 < num_lines && index2 >= 0 && index2 <= strlen(lines[index1])) {
            char new_line[MAX_SIZE];
            strcpy(new_line, lines[index1]);
            strncpy(new_line + index2, wordForAdding, MAX_SIZE - index2);
            strncat(new_line, lines[index1] + index2, MAX_SIZE - strlen(new_line));

            delete[] Buffer;
            Buffer = new char[MAX_SIZE];
            for (int i = 0; i < num_lines; i++) {
                if (i == index1) {
                    strncat(Buffer, new_line, MAX_SIZE - strlen(Buffer));
                } else {
                    strncat(Buffer, lines[i], MAX_SIZE - strlen(Buffer));
                }
                if (i < num_lines - 1) {
                    strncat(Buffer, "\n", MAX_SIZE - strlen(Buffer));
                }
            }
            std::cout << Buffer;
        } else {
            std::cout << "Invalid index values\n";
        }
    }


    void TextToSearch() {
        std::cout << "Enter text to search: ";
        char search_text[MAX_SIZE];
        std::cin >> search_text;
        int row = 0;
        char* line = strtok(Buffer, "\n");
        while (line != nullptr) {
            int position = 0;
            char* line_start = line;
            while (1) {
                char* word_in_line = strstr(line_start, search_text);
                if (word_in_line != nullptr) {
                    position = word_in_line - line_start;
                    std::cout << "Text '" << search_text << "' found in line " << row << " at position " << position << std::endl;
                    line_start = word_in_line + 1;
                } else {
                    break;
                }
            }
            row++;
            line = strtok(nullptr, "\n");
        }
        if (row == 0) {
            std::cout << "The word '" << search_text << "' is not found in the Buffer." << std::endl;
        }
    }
};

int main() {
    TextEditor textEditor;

    while (true) {
        int command;
        std::cout << "\nChoose the command!\n";
        std::cin >> command;
        std::cin.ignore();

        switch (command) {
            case 1:
                textEditor.TextToAppend();
                break;
            case 2:
                textEditor.NewLine();
                break;
            case 3:
                textEditor.ForSavingInFile();
                break;
            case 4:
                textEditor.ForLoadingFromFile();
                break;
            case 5:
                textEditor.PrintLoadedText();
                break;
            case 6:
                int index1, index2;
                char wordForAdding[MAX_SIZE];
                textEditor.InsertAtIndex(index1, index2, wordForAdding);
                break;
            case 7:
                textEditor.TextToSearch();
                break;
            case 8:
                system("clear");
                break;
            default:
                std::cout << "The command is not implemented\n";
                break;
        }
    }
}
