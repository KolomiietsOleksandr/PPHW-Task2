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
    std::string clipboard; // Clipboard for Cut/Copy operations

public:
    StringArray() : consecutiveUndoCount(0) {
        historyStack.push(array);
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

        clipboard = line.substr(position, length); // Copy deleted text to clipboard
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

    void saveToFile(const std::string& fileName) {
        std::ofstream file(fileName);
        if (file.is_open()) {
            for (const std::string& line : array) {
                file << line << std::endl;
            }
            file.close();
            std::cout << "Array saved to " << fileName << std::endl;
        } else {
            std::cerr << "Error opening the file." << std::endl;
        }
    }

    void loadFromFile(const std::string& fileName) {
        std::ifstream file(fileName);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                array.push_back(line);
            }
            file.close();
            std::cout << "Array loaded from " << fileName << std::endl;
        } else {
            std::cerr << "Error opening the file." << std::endl;
        }
    }

    void searchSubstringInLine(int lineIndex, const std::string& substring) {
        int found = 0;
        int position = -1;
        int firstLetterPos = -1;
        int lastLetterPos = -1;
        size_t substringIndex = 0;

        const std::string& line = array[lineIndex - 1];

        for (size_t j = 0; j < line.length(); j++) {
            if (line[j] == substring[substringIndex]) {
                if (firstLetterPos == -1) {
                    firstLetterPos = j;
                }
                lastLetterPos = j;
                substringIndex++;

                if (substringIndex == substring.length()) {
                    position = firstLetterPos;
                    std::cout << "Word found in line " << lineIndex << ", positions " << position + 1 << " to " << lastLetterPos + 1 << ": " << substring << std::endl;
                    found = 1;
                    substringIndex = 0;
                    firstLetterPos = -1;
                }
            } else {
                substringIndex = 0;
                firstLetterPos = -1;
                lastLetterPos = -1;
            }
        }

        if (!found) {
            std::cout << "Substring not found in line " << lineIndex << "." << std::endl;
        }
    }

    void searchSubstringInArray(const std::string& substring) {
        int found = 0;

        for (size_t i = 0; i < array.size(); i++) {
            searchSubstringInLine(i + 1, substring);
            found = 1;
        }

        if (!found) {
            std::cout << "Substring not found in any line." << std::endl;
        }
    }

    void insertSubstring(int lineIndex, int position, const std::string& substring) {
        if (lineIndex < 1 || static_cast<size_t>(lineIndex) > array.size()) {
            std::cerr << "Invalid line index." << std::endl;
            return;
        }

        if (position < 0 || static_cast<size_t>(position) > array[lineIndex - 1].length()) {
            std::cerr << "Invalid position." << std::endl;
            return;
        }

        array[lineIndex - 1].insert(position, substring);
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

        clipboard = line.substr(position, length); // Copy text to clipboard
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

        clipboard = line.substr(position, length); // Copy text to clipboard
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

        array[lineIndex - 1].insert(position, clipboard); // Paste text from clipboard
        historyStack.push(array);
        redoStack = std::stack<std::vector<std::string>>();
        consecutiveUndoCount = 0;
    }
};

int main() {
    int command = 0;
    StringArray stringArray;
    std::string fileName;

    while (true) {
        std::cout << "Write command 1-12: ";
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
                stringArray.saveToFile(fileName);
                break;
            }
            case 5: {
                std::cout << "Write file name to LOAD: ";
                std::cin >> fileName;
                stringArray.loadFromFile(fileName);
                break;
            }
            case 6: {
                std::string substring;
                std::cout << "Enter the substring to search for: ";
                std::cin >> substring;
                stringArray.searchSubstringInArray(substring);
                break;
            }
            case 7: {
                int lineIndex, position;
                std::string substring;

                std::cout << "Enter line index for insertion: ";
                std::cin >> lineIndex;

                std::cout << "Enter position for insertion (0-" << stringArray.getStringCount() << "): ";
                std::cin >> position;

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "Enter substring to insert: ";
                std::getline(std::cin, substring);

                stringArray.insertSubstring(lineIndex, position, substring);
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
    return 0;
}
