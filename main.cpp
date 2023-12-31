#include <iostream>
#include <fstream>
#include <cstring>
#include <stack>
const int MAX_SIZE = 1000;

class Cursor {
public:
    int row;
    int position;

    Cursor(int startRow, int startPosition) : row(startRow), position(startPosition) {}
};

class TextEditor {
private:
    char *Buffer;
    bool has_text;
    std::stack<char*> undoStack;
    std::stack<char*> redoStack;
    Cursor cursor;

public:
    TextEditor() : Buffer(nullptr), has_text(false), cursor(0, 0) {}

    ~TextEditor() {
        delete[] Buffer;
    }

    char* getBuffer() const {
        return Buffer;
    }

    void moveCursor(int row, int position) {
        cursor.row = row;
        cursor.position = position;
    }

    friend class substringEditor;
    friend class CutCopyPaste;

    void TextToAppend() {
        undoStack.push(Buffer);
        std::cout << "Enter text to append: ";
        char text[MAX_SIZE];
        std::cin.getline(text, MAX_SIZE);

        if (!has_text) {
            Buffer = new char[strlen(text) + 1];
            strcpy(Buffer, text);
        } else {
            char *temp = new char[strlen(Buffer) + strlen(text) + 2];
            strcpy(temp, Buffer);
            strcat(temp, " ");
            strcat(temp, text);
            Buffer = temp;
        }

        std::cout << Buffer;

        has_text = true;
    }

    void NewLine() {
        undoStack.push(Buffer);
        char *lines[20];
        int num_lines = 0;
        char *line = strtok(Buffer, "\n");

        while (line != nullptr) {
            lines[num_lines] = line;
            num_lines++;
            line = strtok(nullptr, "\n");
        }

        if (cursor.row >= 0 && cursor.row < num_lines) {
            char *new_lines = new char[strlen(Buffer) + 2];
            new_lines[0] = '\0';

            for (int i = 0; i < num_lines; i++) {
                if (i == cursor.row) {
                    strcat(new_lines, "\n");
                }

                strcat(new_lines, lines[i]);

                if (i < num_lines - 1) {
                    strcat(new_lines, "\n");
                }
            }

            delete[] Buffer;
            Buffer = new_lines;
            cursor.row++;
            cursor.position = 0;
        } else {
            std::cout << "Invalid cursor position." << std::endl;
        }

        std::cout << "New line started." << std::endl;
    }

    void ForSavingInFile() {
        std::string fileForSaving;
        std::cout << "Enter the file name for saving: ";
        std::cin >> fileForSaving;

        std::ofstream file(fileForSaving);

        if (file.is_open()) {
            file << Buffer;
            file.close();
            std::cout << "Text has been saved successfully." << std::endl;
        } else {
            std::cerr << "Error: Unable to open file for writing." << std::endl;
        }
    }

    void ForLoadingFromFile() {
        char fileForLoading[MAX_SIZE];
        std::cout << "Enter the file name for loading: ";
        std::cin >> fileForLoading;
        std::ifstream loadFile(fileForLoading);

        if (!loadFile) {
            std::cout << "Error opening file for loading." << std::endl;
        } else {
            loadFile.seekg(0, std::ios::end);
            size_t fileSize = loadFile.tellg();
            loadFile.seekg(0, std::ios::beg);
            Buffer = new char[fileSize + 1];
            loadFile.read(Buffer, fileSize);
            Buffer[fileSize] = '\0';
            std::cout << "Text has been loaded successfully." << std::endl;
            loadFile.close();
            has_text = true;
            cursor.row = 0;
            cursor.position = 0;
        }
    }

    void PrintLoadedText() {
        std::cout << Buffer;
    }

    void Deletecommand() {
        undoStack.push(Buffer);
        char *lines[20];
        int num_lines = 0;
        char *line = strtok(Buffer, "\n");

        while (line != nullptr) {
            lines[num_lines] = line;
            num_lines++;
            line = strtok(nullptr, "\n");
        }

        if (cursor.row >= 0 && cursor.row < num_lines) {
            char *current_line = lines[cursor.row];
            int lineLength = strlen(current_line);

            if (cursor.position >= 0 && cursor.position < lineLength) {
                int numSymbols = 1;

                if (cursor.position + numSymbols <= lineLength) {
                    memmove(&current_line[cursor.position], &current_line[cursor.position + numSymbols], lineLength - cursor.position - numSymbols + 1);
                    std::cout << current_line << std::endl;
                }
            }
        } else {
            std::cout << "Invalid cursor position." << std::endl;
        }
    }

    void Undocommand(){
        if (!undoStack.empty()){
            undoStack.pop();
            if (!undoStack.empty()){
                Buffer = undoStack.top();
            }
        }
    }
};

class substringEditor{
public:
    static void InsertAtIndex(int index1, int index2, const char *wordForAdding, char* Buffer, TextEditor& textEditor) {
        std::stack<char*>& undoStack = textEditor.undoStack;
        undoStack.push(textEditor.getBuffer());
        std::cout << "Choose line and index: ";
        std::cin >> index1 >> index2;
        std::cin.ignore();
        std::cout << "Enter text to insert:\n";
        char text[MAX_SIZE];
        std::cin.getline(text, MAX_SIZE);

        char *lines[20];
        int num_lines = 0;
        char *line = strtok(Buffer, "\n");

        while (line != nullptr) {
            lines[num_lines] = line;
            num_lines++;
            line = strtok(nullptr, "\n");
        }

        if (index1 >= 0 && index1 < num_lines && index2 >= 0 && index2 <= strlen(lines[index1])) {
            char new_line[MAX_SIZE];
            strcpy(new_line, lines[index1]);
            strncpy(new_line + index2, text, MAX_SIZE - index2);
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
            std::cout << "Invalid index values." << std::endl;
        }
    }

    static void TextToSearch(char* Buffer) {
        std::cout << "Enter text to search: ";
        char search_text[MAX_SIZE];
        std::cin.ignore();
        std::cin.getline(search_text, MAX_SIZE);

        int row = 0;
        char *line = strtok(Buffer, "\n");

        while (line != nullptr) {
            int position = 0;
            char *line_start = line;

            while (1) {
                char *word_in_line = strstr(line_start, search_text);

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

class CutCopyPaste{
private:
    static char clipboard[MAX_SIZE];

public:
    static void PasteCommand(char* Buffer, TextEditor& textEditor) {
        std::stack<char*>& undoStack = textEditor.undoStack;
        undoStack.push(textEditor.getBuffer());
        int row, index;
        std::cout << "Choose line and index to paste: ";
        std::cin >> row >> index;
        std::cin.ignore();

        char *lines[20];
        int num_lines = 0;
        char *line = strtok(Buffer, "\n");

        while (line != nullptr) {
            lines[num_lines] = line;
            num_lines++;
            line = strtok(nullptr, "\n");
        }

        if (row >= 0 && row < num_lines && index >= 0 && index <= strlen(lines[row])) {
            char new_line[MAX_SIZE];
            strcpy(new_line, lines[row]);
            strncpy(new_line + index, clipboard, MAX_SIZE - index);
            strncat(new_line, lines[row] + index, MAX_SIZE - strlen(new_line));

            delete[] Buffer;
            Buffer = new char[MAX_SIZE];

            for (int i = 0; i < num_lines; i++) {
                if (i == row) {
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
            std::cout << "Invalid index values." << std::endl;
        }
    }

    static void CutCommand(char* Buffer, TextEditor& textEditor) {
        std::stack<char*>& undoStack = textEditor.undoStack;
        undoStack.push(textEditor.getBuffer());
        int row, index, numSymbols;
        std::cout << "Choose line, index, and number of symbols: ";
        std::cin >> row >> index >> numSymbols;

        char *lines[20];
        int num_lines = 0;
        char *line = strtok(Buffer, "\n");

        while (line != nullptr) {
            lines[num_lines] = line;
            num_lines++;
            line = strtok(nullptr, "\n");
        }

        if (row >= 0 && row < num_lines) {
            char *current_line = lines[row];
            int lineLength = strlen(current_line);

            if (index >= 0 && index < lineLength) {
                if (numSymbols > 0) {
                    if (index + numSymbols <= lineLength) {
                        strncpy(clipboard, &current_line[index], numSymbols);
                        clipboard[numSymbols] = '\0';

                        memmove(&current_line[index], &current_line[index + numSymbols], lineLength - index - numSymbols + 1);
                        std::cout << "Cutted Text: " << clipboard << std::endl;
                    }
                }
            }
        } else {
            std::cout << "Invalid index values." << std::endl;
        }
    }

    static void CopyCommand(char* Buffer) {
        int row, index, numSymbols;
        std::cout << "Choose line, index, and number of symbols: ";
        std::cin >> row >> index >> numSymbols;

        char *lines[20];
        int num_lines = 0;
        char *line = strtok(Buffer, "\n");

        while (line != nullptr) {
            lines[num_lines] = line;
            num_lines++;
            line = strtok(nullptr, "\n");
        }

        if (row >= 0 && row < num_lines) {
            char *current_line = lines[row];
            int lineLength = strlen(current_line);

            if (index >= 0 && index < lineLength) {
                if (numSymbols > 0) {
                    if (index + numSymbols <= lineLength) {
                        strncpy(clipboard, &current_line[index], numSymbols);
                        clipboard[numSymbols] = '\0';

                        std::cout << "Copied Text: " << clipboard << std::endl;
                    }
                }
            }
        } else {
            std::cout << "Invalid index values." << std::endl;
        }
    }
};

char CutCopyPaste::clipboard[MAX_SIZE] = "";

int main() {
    TextEditor textEditor;

    while (true) {
        int command;
        std::cout << "\nChoose the command:\n";
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
                substringEditor::InsertAtIndex(index1, index2, wordForAdding, textEditor.getBuffer(), textEditor);
                break;

            case 7:
                substringEditor::TextToSearch(textEditor.getBuffer());
                break;

            case 8:
                textEditor.Deletecommand();
                break;

            case 9:
                textEditor.Undocommand();
                break;

            case 11:
                CutCopyPaste::CutCommand(textEditor.getBuffer(), textEditor);
                break;

            case 12:
                CutCopyPaste::PasteCommand(textEditor.getBuffer(), textEditor);
                break;

            case 13:
                CutCopyPaste::CopyCommand(textEditor.getBuffer());
                break;

            default:
                std::cout << "The command is not implemented." << std::endl;
                break;
        }
    }

    return 0;
}
