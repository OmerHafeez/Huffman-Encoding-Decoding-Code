#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

const int MAX_CHAR = 256;

class Node {
public:
    char data;
    Node* left;
    Node* right;

    Node(char data) : data(data), left(nullptr), right(nullptr) {}
};

Node* buildHuffmanTree(const string codes[MAX_CHAR]) {

    Node* root = new Node('\0');

    // Iterate through each character in the codes array
    for (int i = 0; i < MAX_CHAR; ++i) {
        // Check if the code for the character is not empty
        if (!codes[i].empty()) {
            Node* currentNode = root;

            for (char c : codes[i]) {
                if (c == '0') {
                    // If '0' encountered, create a left child if it doesn't exist
                    if (currentNode->left == nullptr) {
                        currentNode->left = new Node('\0');
                    }

                    currentNode = currentNode->left;
                } else {
                    // If '1' encountered, create a right child if it doesn't exist
                    if (currentNode->right == nullptr) {
                        currentNode->right = new Node('\0');
                    }
                    // Move to the right child
                    currentNode = currentNode->right;
                }
            }
            // Assign the character to the leaf node
            currentNode->data = static_cast<char>(i);
        }
    }
    // Return the root of the constructed Huffman Tree
    return root;
}

string decodeText(Node* root, const string& encodedText) {
    string decodedText = "";
    Node* currentNode = root;

    // Traverse through the encoded text
    for (char bit : encodedText) {
        if (bit == '0') {
            // Move to the left child if '0' encountered
            currentNode = currentNode->left;
        } else {
            // Move to the right child if '1' encountered
            currentNode = currentNode->right;
        }

        if (currentNode->left == nullptr && currentNode->right == nullptr) {
            decodedText += currentNode->data;
            // Reset to the root for decoding the next character
            currentNode = root;
        }
    }

    return decodedText;
}


int main() {
    ifstream codesFile("codes.txt");
    ifstream encodedFile("output.txt");

    string codeLine;
    string codes[MAX_CHAR];
    while (getline(codesFile, codeLine)) {
        char character = codeLine[0];
        string code = codeLine.substr(3); 
        codes[static_cast<unsigned char>(character)] = code;
    }
    codesFile.close();

    string encodedText;
    if (encodedFile.is_open()) {
        encodedText.assign((istreambuf_iterator<char>(encodedFile)), (istreambuf_iterator<char>()));
        encodedFile.close();
    } else {
        cout << "Unable to open encoded file.";
        return 1;
    }

    Node* huffmanRoot = buildHuffmanTree(codes);

    string decodedText = decodeText(huffmanRoot, encodedText);

    ofstream decodedFile("receiver.txt");
    if (decodedFile.is_open()) {
        decodedFile << decodedText;
        decodedFile.close();
    } else {
        cout << "Unable to write to receiver.txt";
        return 1;
    }

    cout << "Decoding completed successfully." << endl;

    return 0;
}
