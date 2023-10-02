#include <iostream>
#include <fstream>
#include <cstring>


const int MAX_BUFFER_SIZE = 500;

void Text_to_append(char*& Buffer, bool& has_text) {
    std::cout << "Enter text to append:";
    char text[MAX_BUFFER_SIZE];
    std::cin.getline(text, MAX_BUFFER_SIZE);
    if (!has_text) {
        Buffer = new char[strlen(text) + 1];
        strcpy(Buffer, text);
    } else {
        char* temp = new char[strlen(Buffer) + strlen(text) + 2];
        strcpy(temp, Buffer);
        strcat(temp, "\n");
        strcat(temp, text);
        delete[] Buffer;
        Buffer = temp;
    }
    has_text = true;
}

void New_Line(char*& Buffer) {
    char* temp = new char[strlen(Buffer) + 2];
    strcpy(temp, Buffer);
    strcat(temp, "\n");
    delete[] Buffer;
    Buffer = temp;
    std::cout << "New line is started\n";
}

void For_saving_in_file(const std::string& Buffer, std::string file_for_saving) {
    std::cout << "Enter the file name for saving:";
    std::ofstream file(file_for_saving.c_str());
    if (file.is_open()) {
        file << Buffer;
        file.close();
        std::cout << "Text has been saved successfully\n";
    } else {
        std::cout << "Error opening file for saving\n";
    }
}


void For_loading_in_life(char* file_for_loading, char*& loaded_text) {
    std::cout << "Enter the file name for loading:";
    std::cin >> file_for_loading;
    std::ifstream load_file(file_for_loading);
    if (!load_file) {
        std::cout << "Error opening file for loading\n";
    } else {
        load_file.seekg(0, std::ios::end);
        size_t file_size = load_file.tellg();
        load_file.seekg(0, std::ios::beg);
        loaded_text = new char[file_size + 1];
        load_file.read(loaded_text, file_size);
        loaded_text[file_size] = '\0';
        std::cout << "Text has been loaded successfully\n";
        load_file.close();
    }
}

void Print_loading_text(const char* loaded_text) {
    std::cout << loaded_text;
}

void Insert_an_index(int index1, int index2, const char* word_for_adding, char*& Buffer) {
    std::cout << "Choose line and index:";
    std::cin >> index1 >> index2;
    std::cout << "Enter text to insert:\n";
    char text[MAX_BUFFER_SIZE];
    std::cin.ignore();
    std::cin.getline(text, MAX_BUFFER_SIZE);

    char* lines[20];
    int num_lines = 0;
    char* line = strtok(Buffer, "\n");
    while (line != NULL) {
        lines[num_lines] = line;
        num_lines++;
        line = strtok(NULL, "\n");
    }

    if (index1 >= 0 && index1 < num_lines && index2 >= 0 && index2 <= strlen(lines[index1])) {
        char new_line[MAX_BUFFER_SIZE];
        strcpy(new_line, lines[index1]);
        strcpy(new_line + index2, word_for_adding);
        strcat(new_line, lines[index1] + index2);
        delete[] Buffer;
        Buffer = new char[MAX_BUFFER_SIZE];
        for (int i = 0; i < num_lines; i++) {
            if (i == index1) {
                strcat(Buffer, new_line);
            } else {
                strcat(Buffer, lines[i]);
            }
            if (i < num_lines - 1) {
                strcat(Buffer, "\n");
            }
        }
        std::cout << Buffer;
    } else {
        std::cout << "Invalid index values\n";
    }
}

void Text_to_search(char*& Buffer, const char* text_to_search) {
    std::cout << "Enter text to search: ";
    char search_text[MAX_BUFFER_SIZE];
    std::cin >> search_text;
    int row = 0;
    char* line = strtok(Buffer, "\n");
    while (line != NULL) {
        int position = 0;
        char* line_start = line;
        while (1) {
            char* word_in_line = strstr(line_start, search_text);
            if (word_in_line != NULL) {
                position = word_in_line - line_start;
                std::cout << "Text '" << search_text << "' found in line " << row << " at position " << position << std::endl;
                line_start = word_in_line + 1;
            } else {
                break;
            }
        }
        row++;
        line = strtok(NULL, "\n");
    }
    if (row == 0) {
        std::cout << "The word '" << search_text << "' is not found in the Buffer." << std::endl;
    }
}

int main() {
    int command;
    char* Buffer = nullptr;
    char file_for_saving[MAX_BUFFER_SIZE];
    char file_for_loading[MAX_BUFFER_SIZE];
    char word_for_adding[MAX_BUFFER_SIZE];
    char text_to_search[MAX_BUFFER_SIZE];
    char* loaded_text = nullptr;
    bool has_text = false;

    while (true) {
        std::cout << "\nChoose the command!\n";
        std::cin >> command;
        std::cin.ignore();
        switch (command) {
            case 1:
                Text_to_append(Buffer, has_text);
                break;
            case 2:
                New_Line(Buffer);
                break;
            case 3:
                For_saving_in_file(Buffer, file_for_saving);
                break;
            case 4:
                For_loading_in_life(file_for_loading, loaded_text);
                break;
            case 5:
                Print_loading_text(loaded_text);
                break;
            case 6:
                Insert_an_index(0, 0, word_for_adding, Buffer);
                break;
            case 7:
                Text_to_search(Buffer, text_to_search);
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

