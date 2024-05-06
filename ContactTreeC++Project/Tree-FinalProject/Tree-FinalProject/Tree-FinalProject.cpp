// Tree-FinalProject.cpp : This file contains the "main" function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <list>
#include <queue>

using namespace std;

struct TreeNode {
    string medicareId; // Unique identifier cannot be duplicated
    int directContacts = 0; // Number of children a node has (default 0)
    int totalCases = 1; // Number of nodes rooted at this tree node, including self
    TreeNode* parentPtr; // Pointer to the parent node
    list<TreeNode*> directContactsPtrList; // List of pointers to children nodes (similar to ArrayList in Java)
    // Easy to traverse

    TreeNode(const string& medicareId) {
        this->medicareId = medicareId;
        this->parentPtr = nullptr;
    }

    TreeNode(const string& medicareId, TreeNode* parentPtr)
    {
        this->medicareId = medicareId;
        this->parentPtr = parentPtr;
    }
};

// FOR THE LOOKUP CONTACT: Use a queue. Each children node will be put in the qeue, to wait its turn to be searched.
//Breadth First Search
class ContactTree {
private:
    TreeNode* root;
    int numberOfNodes;

    void PrintHierarchicalTree(TreeNode* node, const string& indent) //Method private so the user can call without parameters
    {
        if (this->IsEmpty()) return;
        
        if (node == nullptr) return; //base case: node is null, so nothing to print

        cout << indent << node->medicareId << endl; //print the medId fromt he current node with a certain indent

        const string childIndent = indent + "  "; //Increase the indent for the children

        for (TreeNode* children : node->directContactsPtrList) //Call the method on the children of the current node
        {
            this->PrintHierarchicalTree(children, childIndent); //recursive method
        }
    }

public:
    ContactTree() {
        this->root = nullptr;
        this->numberOfNodes = 0;
    }

    ~ContactTree() 
    {
        this->DeleteContact(this->root->medicareId);
    }

    bool IsEmpty()
    {
        return this->numberOfNodes == 0 || this->root == nullptr;
    }

    int GetSize()
    {
        return this->numberOfNodes;
    }

    void DisplayRoot() // This is just for testing
    {
        cout << this->root->medicareId << endl;
        cout << this->root->directContacts << endl;
        cout << this->numberOfNodes << endl;
    }

    void AddPatient0(const string& medicareId) {
        if (!this->IsEmpty()) 
        {
            cout << "The tree already has a patient 0, try AddContact method";
            return;
        }

        this->root = new TreeNode(medicareId);
        this->numberOfNodes++;
    }
    
    TreeNode* LookUpContact(const string& targetMedicareId) //try and search for this method. 
        //a method in a Tree which uses Breadth First Search (BFS) to return a specific node in the tree. We use a string to indentify the target
    {
        if (this->IsEmpty()) return nullptr;

        queue<TreeNode*> visitQueue;
        visitQueue.push(this->root);

        while (!visitQueue.empty()) {
            TreeNode* current = visitQueue.front();
            visitQueue.pop();

            if (current->medicareId == targetMedicareId) {
                return current; // Found the node with the target Medicare ID
            }

            // Enqueue all children of the current node
            for (TreeNode* child : current->directContactsPtrList) {
                visitQueue.push(child);
            }
        }

        return nullptr; // Target Medicare ID not found in the tree
    }

    void AddContact(const string& parentMedicareId, const string& medicareId) {
        TreeNode* parentNode = this->LookUpContact(parentMedicareId);
        if (!parentNode) {
            cout << "Parent node not found." << endl;
            return;
        }

        TreeNode* newNode = new TreeNode(medicareId, parentNode);
        parentNode->directContactsPtrList.push_back(newNode);
        parentNode->directContacts++;
        for (TreeNode* currentNode = parentNode; currentNode != nullptr; currentNode = currentNode->parentPtr) {
            currentNode->totalCases++;
        }
        this->numberOfNodes++;
    }

    void DisplayContact(const string& medicareId) {
        TreeNode* foundNode = this->LookUpContact(medicareId);
        if (foundNode) {
            cout << "Medicare ID: " << foundNode->medicareId << endl;
            cout << "Direct Contacts: " << foundNode->directContacts << endl;
            cout << "Total Cases: " << foundNode->totalCases << endl;
        }
        else {
            cout << "Node not found." << endl;
        }
    }

    void DisplayContact(TreeNode* foundNode) {
        if (foundNode) {
            cout << "Medicare ID: " << foundNode->medicareId << endl;
            cout << "Direct Contacts: " << foundNode->directContacts << endl;
            cout << "Total Cases: " << foundNode->totalCases << endl;
        }
        else {
            cout << "Node not found." << endl;
        }
    }


    void DeleteContact(const string& medicareId) {
        TreeNode* node = this->LookUpContact(medicareId);
        if (!node) 
        {
            "Cannot find the node";
            return;
        }

        // Perform BFS to collect all nodes in the subtree
        std::queue<TreeNode*> queue;
        std::list<TreeNode*> nodesToDelete;
        queue.push(node);

        while (!queue.empty()) {
            TreeNode* current = queue.front();
            queue.pop();
            nodesToDelete.push_front(current);

            for (TreeNode* child : current->directContactsPtrList) {
                queue.push(child);
            }
        }

        for (TreeNode* node : nodesToDelete) {
            if (node->parentPtr) 
            {
                node->parentPtr->directContactsPtrList.remove(node);
                node->parentPtr->directContacts--;
                for (TreeNode* currentNode = node->parentPtr; currentNode != nullptr; currentNode = currentNode->parentPtr) 
                {
                    currentNode->totalCases--;
                }
            }
            this->numberOfNodes--;
            delete node;
        }
    }



    void PrintHierarchicalTree() //the user will only call without giving any arguments, always display the whole tree from the  root
    {
        this->PrintHierarchicalTree(this->root, "");
    }

    void TraceSource(const string& medicareId)
    {
        TreeNode* currentNode = this->LookUpContact(medicareId);

        if (!currentNode)
        {
            cout << "Node not found" << endl;
            return;
        }

        if (currentNode == this->root) 
        {
            cout << "No parent, this is the root" << endl;
            return;
        }

        cout << "parent of " + medicareId << endl;
        cout << "+++++++++++++++++++++++" << endl;
        this->DisplayContact(currentNode->parentPtr);
        cout << "======================" << endl;

        string parentId = currentNode->parentPtr->medicareId;
        this->TraceSource(parentId);
    }
    
    void PrintContactCases(const string& medicareId) 
    {
        TreeNode* currentNode = this->LookUpContact(medicareId);

        if (!currentNode) 
        {
            cout << "Node not found" << endl;
            return;
        }

        if (currentNode->directContacts == 0) 
        {
            cout << "This node has no contacts" << endl;
            return;
        }


        cout << "All the children of " + medicareId << endl;
        cout << "=============================" << endl;
        for (TreeNode* children : currentNode->directContactsPtrList) 
        {
            this->DisplayContact(children);
            cout << "*****************************" << endl;
        }
    }

    void PrintContactTree() //Using the exact same structure as lookup, only that instead of returning a node it prints it
    {
        if (this->IsEmpty()) return;

        queue<TreeNode*> visitQueue;
        visitQueue.push(this->root);

        while (!visitQueue.empty()) {
            TreeNode* current = visitQueue.front();
            visitQueue.pop();

            this->DisplayContact(current);

            // Enqueue all children of the current node
            for (TreeNode* child : current->directContactsPtrList) {
                visitQueue.push(child);
            }
        }
    }
};


int main() {

    ContactTree* tree1 = new ContactTree();

    tree1->AddPatient0("patient 0");
    
    tree1->AddContact("patient 0", "child1");
    tree1->AddContact("patient 0", "child2");
    tree1->AddContact("child1", "child11");
    tree1->AddContact("child1", "child12");
    tree1->AddContact("child1", "child13");
    tree1->AddContact("child11", "child111");

    tree1->PrintContactTree();
    
    tree1->DeleteContact("child1");

    cout << "===============================================" << endl;

    tree1->PrintContactTree();
}