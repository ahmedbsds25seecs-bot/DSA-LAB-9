#include <iostream>
#include <string>

using namespace std;
//Node structure for Word Indexing BST
struct Node {
    string word;
    int count;
    Node* left;
    Node* right;
    
    Node(string w) {
        word = w;
        count = 1;
        left = nullptr;
        right = nullptr;
    }
};
class WordBST {
private:
    Node* root;
    int totalWordCount;
    int uniqueWordCount;

    //function to insert or update word frequency
    Node* insertRec(Node* node, string w, bool& isNew) {
        if (node == nullptr) {
            uniqueWordCount++;
            isNew = true;
            return new Node(w);
        }
        if (w < node->word) {
            node->left = insertRec(node->left, w, isNew);
        } else if (w > node->word) {
            node->right = insertRec(node->right, w, isNew);
        } else {
            //if word already exists, increment frequency
            node->count++;
            isNew = false;
        }
        return node;
    }
    //function for alphabetical traversal(inorder) with numbering
    void inorderAlphabetical(Node* node, int& index) {
        if (node == nullptr) return;
        inorderAlphabetical(node->left, index);
        cout << index++ << ". " << node->word << ": " << node->count << endl;
        inorderAlphabetical(node->right, index);
    }
    //function to collect all nodes into an array for frequency sorting
    void collectNodes(Node* node, Node* arr[], int& index) {
        if (node == nullptr) return;
        collectNodes(node->left, arr, index);
        arr[index++] = node;
        collectNodes(node->right, arr, index);
    }
public:
    WordBST() {
        root = nullptr;
        totalWordCount = 0;
        uniqueWordCount = 0;
    }
    void insertWord(string w) {
        //case-insensitivity and cleaning 
        string lowerWord = "";
        for (int i = 0; i < w.length(); i++) {
            char c = w[i];
            //Check if it's an alphabet letter (A-Z or a-z) or number (0-9)
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
                // Convert uppercase to lowercase manually if needed
                if (c >= 'A' && c <= 'Z') {
                    c = c + 32;
                }
                lowerWord += c;
            }
        }
        if (!lowerWord.empty()) {
            totalWordCount++;
            bool isNew = false;
            root = insertRec(root, lowerWord, isNew);
        }
    }
    //1. Display Alphabetical Order (Now Numbered)
    void displayAlphabetical() {
        cout << "\nWords in Alphabetical Order" << endl;
        int index = 1;
        inorderAlphabetical(root, index);
    }
    //2. Display By Frequency (Highest to Lowest)
    void displayByFrequency() {
        Node* arr[200]; // Fixed-size array to avoid <vector>
        int size = 0;
        collectNodes(root, arr, size);

        //Simple Bubble Sort by frequency (descending)
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (arr[j]->count < arr[j + 1]->count) {
                    Node* temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
        cout << "\nWords Sorted by Frequency (Highest to Lowest)" << endl;
        for (int i = 0; i < size; i++) {
            cout << i+1 << ". " << arr[i]->word << ": " << arr[i]->count << endl;
        }
    }
    //3. Find Most Frequent Word(s)
    void findMostFrequentWord() {
        Node* arr[200];
        int size = 0;
        collectNodes(root, arr, size);

        if (size == 0) {
            cout << "\nNo words found." << endl;
            return;
        }
        //Find the maximum frequency
        int maxFreq = arr[0]->count;
        for (int i = 1; i < size; i++) {
            if (arr[i]->count > maxFreq) {
                maxFreq = arr[i]->count;
            }
        }
        cout << "\nMost Frequent Word(s) \nFrequency: " << maxFreq << "\tWord: ";
        for (int i = 0; i < size; i++) {
            if (arr[i]->count == maxFreq) {
                cout << arr[i]->word << endl;
            }
        }
    }
    //4. Summary Information
    void displaySummary() {
        cout << "\n--- Summary Information ---" << endl;
        cout << "Total Word Count: " << totalWordCount << endl;
        cout << "Total Unique Words: " << uniqueWordCount << endl;
    }
};
int main() {
    WordBST wordBst;
    string paragraph;
    //User Input for Paragraph
    cout << "Enter a paragraph: ";
    getline(cin, paragraph);

    //string parsing 
    string currentWord = "";
    for (int i = 0; i < paragraph.length(); i++) {
        char c = paragraph[i];
        //If it's a space or tab, it marks the end of a word
        if (c == ' ' || c == '\t' || c == '\n') {
            if (!currentWord.empty()) {
                wordBst.insertWord(currentWord);
                currentWord = "";
            }
        } else {
            currentWord += c;
        }
    }
    //Insert the last word if string doesn't end with a space
    if (!currentWord.empty()) {
        wordBst.insertWord(currentWord);
    }

    //Display Operations & Summary
    wordBst.displayAlphabetical();
    wordBst.displayByFrequency();
    wordBst.findMostFrequentWord();
    wordBst.displaySummary();

    return 0;
}