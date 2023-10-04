#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include <string>

class StringArray {
private:
    std::vector<std::string> array;
    std::stack<std::vector<std::string>> historyStack;
    std::stack<std::vector<std::string>> redoStack;
    int consecutiveUndoCount;
    std::string clipboard;

public:
    StringArray() : consecutiveUndoCount(0) {
        historyStack.push(array);
    }

    std::vector<std::string> getStrings() const {
        return array;
    }

    void setStrings(const std::vector<std::string>& data) {
        array = data;
    }

    size_t getStringCount() const {
        return array.size();
    }

    void addString(const std::string& buffer) {
        if (!array.empty()) {
            array.back() += buffer;
        } else {
            array.push_back(buffer);
        }
        historyStack.push(array);
        redoStack = std::stack<std::vector<std::string>>();
        consecutiveUndoCount = 0;
    }

    void addEmptyLine() {
        array.push_back("");
        historyStack.push(array);
        redoStack = std::stack<std::vector<std::string>>();
        consecutiveUndoCount = 0;
    }

    void printStrings() {
        for (size_t i = 0; i < array.size(); i++) {
            std::cout << i + 1 << ": " << array[i] << std::endl;
        }
    }

    void deleteSubstring(int lineIndex, int position, int length) {
        if (lineIndex < 1 || static_cast<size_t>(lineIndex) > array.size()) {
            std::cerr << "Invalid line index." << std::endl;
            return;
        }

        std::string& line = array[lineIndex - 1];

        if (position < 0 || static_cast<size_t>(position) >= line.length()) {
            std::cerr << "Invalid position." << std::endl;
            return;
        }

        if (length < 0 || static_cast<size_t>(position + length) > line.length()) {
            std::cerr << "Invalid length." << std::endl;
            return;
        }

        clipboard = line.substr(position, length);
        line.erase(position, length);
        historyStack.push(array);
        redoStack = std::stack<std::vector<std::string>>();
        consecutiveUndoCount = 0;
    }

    void undo() {
        if (historyStack.size() > 1 && consecutiveUndoCount < 3) {
            redoStack.push(array);
            historyStack.pop();
            array = historyStack.top();
            consecutiveUndoCount++;
        }
    }

    void redo() {
        if (!redoStack.empty()) {
            historyStack.push(array);
            array = redoStack.top();
            redoStack.pop();
            consecutiveUndoCount = 0;
        }
    }

    void insertSubstring(int lineIndex, int position, const std::string& substring, bool replace = false) {
        if (lineIndex < 1 || static_cast<size_t>(lineIndex) > array.size()) {
            std::cerr << "Invalid line index." << std::endl;
            return;
        }

        if (position < 0 || static_cast<size_t>(position) > array[lineIndex - 1].length()) {
            std::cerr << "Invalid position." << std::endl;
            return;
        }

        if (replace) {
            int length = substring.length();
            array[lineIndex - 1].erase(position, length);
            array[lineIndex - 1].insert(position, substring);
        } else {
            array[lineIndex - 1].insert(position, substring);
        }

        historyStack.push(array);
        redoStack = std::stack<std::vector<std::string>>();
        consecutiveUndoCount = 0;
    }

    void cut(int lineIndex, int position, int length) {
        if (lineIndex < 1 || static_cast<size_t>(lineIndex) > array.size()) {
            std::cerr << "Invalid line index." << std::endl;
            return;
        }

        std::string& line = array[lineIndex - 1];

        if (position < 0 || static_cast<size_t>(position) >= line.length()) {
            std::cerr << "Invalid position." << std::endl;
            return;
        }

        if (length < 0 || static_cast<size_t>(position + length) > line.length()) {
            std::cerr << "Invalid length." << std::endl;
            return;
        }

        clipboard = line.substr(position, length);
        line.erase(position, length);
        historyStack.push(array);
        redoStack = std::stack<std::vector<std::string>>();
        consecutiveUndoCount = 0;
    }

    void copy(int lineIndex, int position, int length) {
        if (lineIndex < 1 || static_cast<size_t>(lineIndex) > array.size()) {
            std::cerr << "Invalid line index." << std::endl;
            return;
        }

        const std::string& line = array[lineIndex - 1];

        if (position < 0 || static_cast<size_t>(position) >= line.length()) {
            std::cerr << "Invalid position." << std::endl;
            return;
        }

        if (length < 0 || static_cast<size_t>(position + length) > line.length()) {
            std::cerr << "Invalid length." << std::endl;
            return;
        }

        clipboard = line.substr(position, length);
    }

    void paste(int lineIndex, int position) {
        if (lineIndex < 1 || static_cast<size_t>(lineIndex) > array.size()) {
            std::cerr << "Invalid line index." << std::endl;
            return;
        }

        if (position < 0 || static_cast<size_t>(position) > array[lineIndex - 1].length()) {
            std::cerr << "Invalid position." << std::endl;
            return;
        }

        array[lineIndex - 1].insert(position, clipboard);
        historyStack.push(array);
        redoStack = std::stack<std::vector<std::string>>();
        consecutiveUndoCount = 0;
    }
};

class SearchFunctions {
public:
    static void searchSubstringInArray(const std::vector<std::string>& array, const std::string& substring) {
        int foundCount = 0;

        for (size_t i = 0; i < array.size(); i++) {
            size_t found = array[i].find(substring);
            if (found != std::string::npos) {
                std::cout << "Substring found in line " << i + 1 << " at position " << found << ": " << substring << std::endl;
                foundCount++;
            }
        }

        if (foundCount == 0) {
            std::cout << "Substring not found in any line." << std::endl;
        }
    }
};

class FilesSL {
public:
    static void saveToFile(const std::string& fileName, const std::vector<std::string>& data) {
        std::ofstream file(fileName);
        if (file.is_open()) {
            for (const std::string& line : data) {
                file << line << std::endl;
            }
            file.close();
            std::cout << "Array saved to " << fileName << std::endl;
        } else {
            std::cerr << "Error opening the file." << std::endl;
        }
    }

    static std::vector<std::string> loadFromFile(const std::string& fileName) {
        std::vector<std::string> loadedData;
        std::ifstream file(fileName);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                loadedData.push_back(line);
            }
            file.close();
            std::cout << "Array loaded from " << fileName << std::endl;
        } else {
            std::cerr << "Error opening the file." << std::endl;
        }
        return loadedData;
    }
};


int main() {
    int command = 0;
    StringArray stringArray;
    std::string fileName;
    std::cout << "Commands:\n"
                 "1 - Append text\n"
                 "2 - Add empty line\n"
                 "3 - Print all text\n"
                 "4 - Save to file\n"
                 "5 - Load from file\n"
                 "6 - Search\n"
                 "7 - Insert\n"
                 "8 - Delete\n"
                 "9 - Undo\n"
                 "10 - Redo\n"
                 "11 - Cut\n"
                 "12 - Copy\n"
                 "13 - Paste\n";

    while (true) {
        std::cout << "Write command 1-13: ";
        std::cin >> command;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (command) {
            case 1: {
                std::string buffer;
                std::cout << "Write text to append: ";
                std::getline(std::cin, buffer);
                stringArray.addString(buffer);
                break;
            }
            case 2: {
                stringArray.addEmptyLine();
                break;
            }
            case 3: {
                stringArray.printStrings();
                break;
            }
            case 4: {
                std::cout << "Write file name to SAVE: ";
                std::cin >> fileName;
                FilesSL::saveToFile(fileName, stringArray.getStrings());
                break;
            }
            case 5: {
                std::cout << "Write file name to LOAD: ";
                std::cin >> fileName;
                stringArray.setStrings(FilesSL::loadFromFile(fileName));
                break;
            }
            case 6: {
                std::string substring;

                std::cout << "Enter substring to search for: ";
                std::cin >> substring;

                SearchFunctions::searchSubstringInArray(stringArray.getStrings(), substring);
                break;
            }
            case 7: {
                int lineIndex, position;
                std::string substring;
                bool replaceMode;

                std::cout << "Enter line index for insertion: ";
                std::cin >> lineIndex;

                std::cout << "Enter position for insertion (0-" << stringArray.getStringCount() << "): ";
                std::cin >> position;

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "Enter substring to insert: ";
                std::getline(std::cin, substring);

                std::cout << "Replace existing text (1 for yes, 0 for no): ";
                std::cin >> replaceMode;

                stringArray.insertSubstring(lineIndex, position, substring, replaceMode);
                break;
            }
            case 8: {
                int lineIndex, position, length;

                std::cout << "Choose line, index, and number of symbols to delete: ";
                std::cin >> lineIndex >> position >> length;
                stringArray.deleteSubstring(lineIndex, position, length);
                break;
            }
            case 9: {
                stringArray.undo();
                break;
            }
            case 10: {
                stringArray.redo();
                break;
            }
            case 11: {
                int cutLine, cutPos, cutLen;
                std::cout << "Choose line, position, and length to cut: ";
                std::cin >> cutLine >> cutPos >> cutLen;
                stringArray.cut(cutLine, cutPos, cutLen);
                break;
            }
            case 12: {
                int copyLine, copyPos, copyLen;
                std::cout << "Choose line, position, and length to copy: ";
                std::cin >> copyLine >> copyPos >> copyLen;
                stringArray.copy(copyLine, copyPos, copyLen);
                break;
            }
            case 13: {
                int pasteLine, pastePos;
                std::cout << "Choose line and position to paste: ";
                std::cin >> pasteLine >> pastePos;
                stringArray.paste(pasteLine, pastePos);
                break;
            }
            default: {
                if (command < 0 || command > 13) {
                    std::cout << "The command is not implemented." << std::endl;
                }
                break;
            }
        }
    }
}
