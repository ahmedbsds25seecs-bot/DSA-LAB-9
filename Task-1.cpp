#include <iostream>
using namespace std;

//Node structure for BST
struct Node {
    int data;
    Node* left;
    Node* right;
    
    Node(int val) {
        data = val;
        left = nullptr;
        right = nullptr;
    }
};
class BST {
private:
    Node* root;

    //function to insert a node (recursive)
    Node* insertRec(Node* node, int val) {
        if (node == nullptr) {
            return new Node(val);
        }
        if (val < node->data) {
            node->left = insertRec(node->left, val);
        } else if (val > node->data) {
            node->right = insertRec(node->right, val);
        }
        return node;
    }
    //function for inorder traversal
    void inorderRec(Node* node) {
        if (node == nullptr) return;
        inorderRec(node->left);
        cout << node->data << " ";
        inorderRec(node->right);
    }
    //Recursive Range Query with comparison counter 
    void rangeQueryRec(Node* node, int low, int high, int& comparisons) {
        if (node == nullptr) return;

        comparisons++; //Comparing node data with range bounds
        if (node->data >= low && node->data <= high) {
            cout << node->data << " ";
            rangeQueryRec(node->left, low, high, comparisons);
            rangeQueryRec(node->right, low, high, comparisons);
        } 
        else if (node->data > high) {
            rangeQueryRec(node->left, low, high, comparisons);
        } 
        else {
            rangeQueryRec(node->right, low, high, comparisons);
        }
    }
public:
    BST() {
        root = nullptr;
    }
    void insert(int val) {
        root = insertRec(root, val);
    }
    void displayInorder() {
        cout << "\nInorder Traversal: ";
        inorderRec(root);
        cout << endl;
    }
    //Finding Predecessor and Successor for a given key
    void findPredecessorSuccessor(int key) {
        Node* current = root;
        Node* p = nullptr;
        Node* s = nullptr;

        while (current != nullptr) {
            if (current->data == key) {
                //Predecessor = max value in left subtree
                if (current->left != nullptr) {
                    Node* temp = current->left;
                    while (temp->right != nullptr) temp = temp->right;
                    p = temp;
                }
                //Successor = min value in right subtree
                if (current->right != nullptr) {
                    Node* temp = current->right;
                    while (temp->left != nullptr) temp = temp->left;
                    s = temp;
                }
                break;
            } else if (key < current->data) {
                s = current; // Potential successor
                current = current->left;
            } else {
                p = current; // Potential predecessor
                current = current->right;
            }
        }
        cout << "\nPredecessor and Successor for Key = " << key << endl;
        if (p != nullptr) cout << "Predecessor: " << p->data << endl;
        else cout << "Predecessor: None (Key is minimum or not found)" << endl;

        if (s != nullptr) cout << "Successor: " << s->data << endl;
        else cout << "Successor: None (Key is maximum or not found)" << endl;
    }
    void rangeQueryRecursive(int low, int high) {
        int comparisons = 0;
        cout << "\nRecursive Range Query [" << low << ", " << high << "]" << endl;
        cout << "Keys found: ";
        rangeQueryRec(root, low, high, comparisons);
        cout << "\nTotal Comparisons: " << comparisons << endl;
    }
    //Iterative Range Query 
    void rangeQueryIterative(int low, int high) {
        int comparisons = 0;
        Node* stack[100]; // Manual stack array
        int top = -1;
        Node* curr = root;

        cout << "\nIterative Range Query [" << low << ", " << high << "]" << endl;
        cout << "Keys found: ";

        while (curr != nullptr || top != -1) {
            while (curr != nullptr) {
                stack[++top] = curr;
                curr = curr->left;
            }
            curr = stack[top--];
            comparisons++;
            if (curr->data >= low && curr->data <= high) {
                cout << curr->data << " ";
            }
            curr = curr->right;
        }
        cout << "\nTotal Comparisons: " << comparisons << endl;
    }
};

int main() {
    BST bst;
    int n, val;

    //1. User input for building the BST
    cout << "Enter the number of elements to insert into the BST: ";
    cin >> n;

    cout << "Enter " << n << " integers:\n";
    for (int i = 0; i < n; i++) {
        cin >> val;
        bst.insert(val);
    }

    //2. Display Inorder Traversal
    bst.displayInorder();

    //3. User input for Successor and Predecessor finder
    int key;
    cout << "\nEnter a key to find its Predecessor and Successor: ";
    cin >> key;
    bst.findPredecessorSuccessor(key);

    //4. User input for Range Query
    int low, high;
    cout << "\nEnter the range bounds(low / high): ";
    cin >> low >> high;

    bst.rangeQueryRecursive(low, high);
    bst.rangeQueryIterative(low, high);

    return 0;
}