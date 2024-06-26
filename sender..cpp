#include <iostream>
#include <fstream>
using namespace std;
const int MAX_CHAR = 256;

class Node {
public:
    char data;    
    int freq;     
    Node* left;    
    Node* right;   

 
    Node(char data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

Node* Tree(int freq[]) {
    // Array to store pointers to nodes
    Node* nodes[MAX_CHAR] = {nullptr}; 

    // Create leaf nodes for characters with non-zero frequency
    for (int i = 0; i < MAX_CHAR; ++i) {
        if (freq[i] > 0) {
            nodes[i] = new Node(static_cast<char>(i), freq[i]);
        }
    }

    
    while (true) {
        int min1 = INT_MAX, min2 = INT_MAX;
        int minindex1 = -1, minindex2 = -1; 

        // Find two nodes with minimum frequencies
        for (int i = 0; i < MAX_CHAR; ++i) {
            if (nodes[i] != nullptr) {
                if (nodes[i]->freq < min1) {
                    // Update minimum frequencies and corresponding indices
                    min2 = min1;
                    minindex2 = minindex1;
                    min1 = nodes[i]->freq;
                    minindex1 = i;
                } else if (nodes[i]->freq < min2) {
                    // Update the second minimum frequency and its index
                    min2 = nodes[i]->freq;
                    minindex2 = i;
                }
            }
        }

        // If only one node remaining, break the loop
        if (minindex2 == -1) {
            break;
        }

        // Create a new internal node with sum of frequencies
        Node* newNode = new Node('\0', min1 + min2);
        
        newNode->left = nodes[minindex1]; 
        newNode->right = nodes[minindex2]; 

        // Update nodes array with new internal node
        nodes[minindex1] = nullptr;
        nodes[minindex2] = newNode;
    }

    // Return the root of the Huffman Tree
    for (int i = 0; i < MAX_CHAR; ++i) {
        if (nodes[i] != nullptr) {
            return nodes[i];
        }
    }

    return nullptr; 
}

void generateCodes(Node* root, string code, string codes[]) {
    if (root == nullptr) {
        return;
    }

    // If leaf node is reached, assign code to character in codes array
    if (!root->left && !root->right) {
        codes[static_cast<unsigned char>(root->data)] = code;
    }

    // Traverse left and right subtrees recursively, updating codes
    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}


string encodeText(const string& text, string codes[]) {
    string encodedText;
    for (char c : text) {
        encodedText += codes[static_cast<unsigned char>(c)];
    }
    return encodedText;
}

void writeEncodedFile(const string& encodedText) {
    ofstream outputFile("output.txt");
    if (outputFile.is_open()) {
        outputFile << encodedText;
        outputFile.close();
    }
    else {
       cout << "Unable to open output.txt";
    }
}

void writeCodesFile(string codes[]) {
    ofstream codesFile("codes.txt");
    if (codesFile.is_open()) {
        for (int i = 0; i < MAX_CHAR; ++i) {
            if (!codes[i].empty()) {
                codesFile << static_cast<char>(i) << ": " << codes[i] << "\n";
            }
        }
        codesFile.close();
    }
    else {
        cout << "Unable to open codes.txt";
    }
}

void deleteTree(Node* root) {
    if (root != nullptr) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
}

int main() {
    ifstream inputFile("sender.txt"); 
    string text;
    
    if (inputFile.is_open()) {
 
        getline(inputFile, text);
        inputFile.close();
    } else {
        cout << "Unable to open input.txt"; 
        return 1; 
    }

    int frequencies[MAX_CHAR] = {0};
    for (char c : text) {
        frequencies[static_cast<unsigned char>(c)]++;
    }

    Node* huffmanRoot = Tree(frequencies);

    string codes[MAX_CHAR];
    generateCodes(huffmanRoot, "", codes);

    string encodedText = encodeText(text, codes);

    writeEncodedFile(encodedText);
    writeCodesFile(codes);

    deleteTree(huffmanRoot);

    cout << "Encoding completed successfully." << endl;

    return 0;
}
