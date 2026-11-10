#include <iostream>
#include <string>

using namespace std;

//StarSystem class representing a node in the M-ary tree
class StarSystem {
public:
    int system_id;
    string name;
    int num_planets;
    float x, y, z;
    bool is_habitable;
    
    int M;                 
    StarSystem** children;
    float* routes;         
    int childCount;       

    //Constructor
    StarSystem(int id, string n, int planets, float px, float py, float pz, bool habitable, int maxM) {
        system_id = id;
        name = n;
        num_planets = planets;
        x = px;
        y = py;
        z = pz;
        is_habitable = habitable;
        M = maxM;
        childCount = 0;
   
        children = new StarSystem*[M];
        routes = new float[M];
        for (int i = 0; i < M; i++) {
            children[i] = nullptr;
            routes[i] = 0.0f;
        }
    }
    //Destructor to free dynamically allocated memory
    ~StarSystem() {
        delete[] children;
        delete[] routes;
    }
    //Method to add a child star system and its route cost
    bool addChild(StarSystem* childSystem, float cost) {
        if (childCount < M) {
            children[childCount] = childSystem;
            routes[childCount] = cost;
            childCount++;
            return true;
        }
        return false; // Exceeds M branching factor limit
    }
};
//Galaxy class managing the M-ary tree structure
class Galaxy {
private:
    int M; //Maximum branching factor
    StarSystem* root;

    //function to calculate the total size of a subtree
    int getSubtreeSize(StarSystem* node) const {
        if (node == nullptr) return 0;
        int size = 1; // Count the current node
        for (int i = 0; i < node->childCount; i++) {
            size += getSubtreeSize(node->children[i]);
        }
        return size;
    }
public:
    //Overloaded Constructor 1: Initialize with given M and empty root
    Galaxy(int m) {
        M = m;
        root = nullptr;
    }
    //Overloaded Constructor 2: Initialize with given M and a specified root node
    Galaxy(int m, StarSystem* r) {
        M = m;
        root = r;
    }
    //Set root node
    void setRoot(StarSystem* r) {
        root = r;
    }
    //Get root node
    StarSystem* getRoot() const {
        return root;
    }
    // 2. Check if M-ary Tree is Balanced
    //A tree is balanced if, for every node, the sizes of all child subtrees differ by at most 1
    bool is_balanced(StarSystem* x) const {
        if (x == nullptr) return true;
        if (x->childCount == 0) return true;

        int min_size = -1;
        int max_size = -1;

        for (int i = 0; i < x->childCount; i++) {
            int subtreeSize = getSubtreeSize(x->children[i]);
            
            if (min_size == -1 || subtreeSize < min_size) {
                min_size = subtreeSize;
            }
            if (max_size == -1 || subtreeSize > max_size) {
                max_size = subtreeSize;
            }
            //Recursively check if the child's subtree is also balanced
            if (!is_balanced(x->children[i])) {
                return false;
            }
        }
        //Balanced tree condition
        return (max_size - min_size <= 1);
    }
    //Overload wrapper for the root node
    bool is_balanced() const {
        return is_balanced(root);
    }
    // 3.Count Leaf Nodes
    int count_leaves(StarSystem* x) const {
        if (x == nullptr) return 0;
        
        // If the node has no children, it is a leaf node
        if (x->childCount == 0) {
            return 1;
        }
        int leafCount = 0;
        for (int i = 0; i < x->childCount; i++) {
            leafCount += count_leaves(x->children[i]);
        }
        return leafCount;
    }
    //Overload wrapper for the root node
    int count_leaves() const {
        return count_leaves(root);
    }
};
int main() {
    int mFactor;
    cout << "Enter the maximum branching factor (M) for the M-ary Tree: ";
    cin >> mFactor;

    //Initialize Galaxy 
    Galaxy galaxy(mFactor);

    cout << "\n--- Setup Root Star System ---" << endl;
    int id, planets;
    string name;
    float x, y, z;
    bool habitable;

    cout << "Enter Root ID, Name, Planets, X, Y, Z coords, Habitable (1 for Yes, 0 for No):\n";
    cin >> id >> name >> planets >> x >> y >> z >> habitable;

    StarSystem* rootSystem = new StarSystem(id, name, planets, x, y, z, habitable, mFactor);
    galaxy.setRoot(rootSystem);

    //Simple interactive addition of child nodes 
    char choice;
    cout << "\nAdd child systems to the Root (Yes:y, No:any other key): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        int numChildren;
        cout << "Give number of children of " << rootSystem->name << ":\t Max: " << mFactor << endl;
        cin >> numChildren;

        if (numChildren > mFactor) numChildren = mFactor;

        for (int i = 0; i < numChildren; i++) {
            cout << "\nEnter details for Child " << (i + 1) << ":" << endl;
            cout << "ID, Name, Planets, X, Y, Z coords, Habitable (1/0):\n";
            cin >> id >> name >> planets >> x >> y >> z >> habitable;
            
            float cost;
            cout << "Enter travel route cost from parent to " << name << ": ";
            cin >> cost;

            StarSystem* childSys = new StarSystem(id, name, planets, x, y, z, habitable, mFactor);
            rootSystem->addChild(childSys, cost);
        }
    }
    //Test and Display Results 
    cout << "\n---------------------------------------" << endl;
    cout << "          GALAXY ANALYSIS REPORT         " << endl;
    cout << "Root System Name: " << galaxy.getRoot()->name << endl;
    cout << "Total Leaf Star Systems (Task c): " << galaxy.count_leaves() << endl;
    cout << "Is the Galaxy Balanced? (Task b): " << (galaxy.is_balanced() ? "Yes" : "No") << endl;

    return 0;
}