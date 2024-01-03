//
// STARTER CODE: util.h
//
// TODO:  Write your own header and fill in all functions below.
//
#include <iostream>
#include <fstream>
#include <map>
#include <queue>          // std::priority_queue
#include <vector>         // std::vector
#include <functional>     // std::greater
#include <string>
#include "bitstream.h"
#include "hashmap.h"
#pragma once

struct HuffmanNode {
    int character;
    int count;
    HuffmanNode* zero;
    HuffmanNode* one;
};

//
// *This method frees the memory allocated for the Huffman tree.
//
void freeTree(HuffmanNode* node) {
    
    // TO DO:  Write this function here.
    if (node == nullptr) {
        return;
    }

    // Recursively free the left and right subtrees.
    freeTree(node->zero);
    freeTree(node->one);

    // Free the memory for the current node.
    delete node;
}

//
// *This function builds the frequency map.  
//  If isFile is true, read from the file with name filename.  
//  If isFile is false, read directly from string filename.
//
void buildFrequencyMap(string filename, bool isFile, hashmap &map) {
    
    // TO DO:  Write this function here.

  hashmap newMap = hashmap();
  map = newMap;
  
  char c;
  int value;
  if(isFile){
    ifstream input;
    input.open(filename);
    if (input.is_open()){
      while(input.get(c)){
        //input.get(c);
        if (!input.fail()){
          value = map.get(c);
          if (value == -1){
            value = value + 2;
          } else{
            value++;
          }
          map.put(c, value);
        }
      }
    }
    map.put(PSEUDO_EOF, 1);
    input.close();
  } else {
    for (auto ch : filename){
      value = map.get(ch);
      if (value == -1){
        value = value+2;
      } else {
        value++;
      }
      map.put(ch, value);
    }
    map.put(PSEUDO_EOF, 1);
  }
}

//
// *This function builds an encoding tree from the frequency map.
//
struct prioritize {
    bool operator()(HuffmanNode*& h1, HuffmanNode*& h2) {
        return h1->count > h2->count;
    }
};

HuffmanNode* buildEncodingTree(hashmap &map) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, prioritize> pq;
    
    // TODO: use pq to build encoding tree

    //replace this return line; only here for starter code compilation
    for (auto key: map.keys()){
        HuffmanNode* newNode = new HuffmanNode;
        newNode->character = key;
        newNode->count = map.get(key); //value
        newNode->one = nullptr;
        newNode->zero = nullptr;
        pq.push(newNode);
    }

    while (pq.size() != 1){
        HuffmanNode* nodeToAdd = new HuffmanNode; 
        nodeToAdd->zero = pq.top();
        pq.pop();
        nodeToAdd->one = pq.top();
        pq.pop();
        nodeToAdd->count = nodeToAdd->zero->count +  nodeToAdd->one->count;
        nodeToAdd->character = NOT_A_CHAR;
        pq.push(nodeToAdd);
    }
    return pq.top();
}

//Helper traversal node funtion

void traverseNode(HuffmanNode* tree, string code, map<int,string> &encodingMap){
  if (tree == nullptr){
    return;
  }

  if (tree->one == nullptr && tree->zero == nullptr){ // leaf node
    encodingMap[tree->character] = code;
  }

  traverseNode(tree->zero, code + "0", encodingMap);
  traverseNode(tree->one,code + "1",  encodingMap);
}

//
// *This function builds the encoding map from an encoding tree.
//
map<int,string> buildEncodingMap(HuffmanNode* tree) {
    map<int,string> encodingMap;
    
    // TO DO:  Write this function here.
    traverseNode(tree,"", encodingMap);
    
    return encodingMap;  // TO DO: update this return
}

//
// *This function encodes the data in the input stream into the output stream
// using the encodingMap.  This function calculates the number of bits
// written to the output stream and sets result to the size parameter, which is
// passed by reference.  This function also returns a string representation of
// the output file, which is particularly useful for testing.
//
string encode(ifstream& input, map<int,string> &encodingMap,
              ofbitstream& output, int &size, bool makeFile) {
    
    // TO DO:  Write this function here.
  string codedMessage = "";
  char c;
  while (input.get(c)){
    string code;
    code = encodingMap[c];
    codedMessage += code;
    for (char bit : code) {
      if (makeFile){
        output.writeBit(bit - '0'); // Convert the character '0' or '1' to integer 0 or 1.
        size++; // Increment the bit count.
      }
    }
  }

  string eofCode = encodingMap[PSEUDO_EOF];
  codedMessage+= eofCode;
  for (char bit : eofCode) {
      output.writeBit(bit - '0');
      size++;
    }
    
    return codedMessage;  // TO DO: update this return
}


//
// *This function decodes the input stream and writes the result to the output
// stream using the encodingTree.  This function also returns a string
// representation of the output file, which is particularly useful for testing.
//

string decode(ifbitstream& input, HuffmanNode* encodingTree, ofstream& output) {
    string decodedData;

    HuffmanNode* current = encodingTree;

    // Read bits one by one from the input stream.
    while (true) {
        int bit = input.readBit();

        // Check if we have reached the end of the input stream (EOF).
        if (bit == -1) {
            break;
        }

        // Traverse the Huffman tree based on the read bit.
        if (bit == 0) {
            current = current->zero;
        } else {
            current = current->one;
        }

        // If a leaf node is reached, output the corresponding character.
        if (current->zero == nullptr && current->one == nullptr) {
            // If the character is PSEUDO_EOF, we have reached the end of the data.
            if (current->character == PSEUDO_EOF) {
                break;
            }

            char c = static_cast<char>(current->character);
            output << c;
            decodedData += c;

            // Reset the current node to the root of the tree for the next character.
            current = encodingTree;
        }
    }

    // Close the output stream.
    output.close();

    return decodedData; // Return the string representation of the output data.
}



//
// *This function completes the entire compression process.  Given a file,
// filename, this function (1) builds a frequency map; (2) builds an encoding
// tree; (3) builds an encoding map; (4) encodes the file (don't forget to
// include the frequency map in the header of the output file).  This function
// should create a compressed file named (filename + ".huf") and should also
// return a string version of the bit pattern.
//
// string compress(string filename) {
    
//     // TO DO:  Write this function here.
    
//     return "";  // TO DO: update this return
// }
// Function to compress the given input file into the output file.
string compress(string inputFilename) {
    ifstream input(inputFilename, ios::binary);

    // Build the frequency map.
    hashmap frequencyMap;
    buildFrequencyMap(inputFilename, true, frequencyMap);

    // Build the encoding tree.
    HuffmanNode* encodingTree = buildEncodingTree(frequencyMap);

    // Build the encoding map.
    map<int, std::string> encodingMap = buildEncodingMap(encodingTree);

    // Create the output file with the ".huf" extension.
    string outputFilename = inputFilename + ".huf";
    ofbitstream output(outputFilename);

    // Write the frequency map (header) into the bit output stream.
    output << frequencyMap;

    // Reset the input file to read from the beginning.
    input.clear();
    input.seekg(0);

    // Compress the input file by encoding and writing the data to the output stream.
    int size;
    string encodedData = encode(input, encodingMap, output, size, true);

    // Free the memory allocated for the encoding tree.
    freeTree(encodingTree);

    return encodedData; // Return the string representation of the encoded data.
}



//
// *This function completes the entire decompression process.  Given the file,
// filename (which should end with ".huf"), (1) extract the header and build
// the frequency map; (2) build an encoding tree from the frequency map; (3)
// using the encoding tree to decode the file.  This function should create a
// compressed file using the following convention.
// If filename = "example.txt.huf", then the uncompressed file should be named
// "example_unc.txt".  The function should return a string version of the
// uncompressed file.  Note: this function should reverse what the compress
// function does.
//
// Function to decompress the compressed input file and write the original data to the output file.
string decompress(string inputFilename) {
    ifbitstream input(inputFilename);
    hashmap frequencyMap;
    input >> frequencyMap;

    // Build the encoding tree.
    HuffmanNode* encodingTree = buildEncodingTree(frequencyMap);


    size_t pos = inputFilename.find_last_of('.');
    // Extract the base filename without the ".huf" extension.
    string baseFilename = inputFilename.substr(0, pos);
    string outputFilename = baseFilename.substr(0, pos-4) + "_unc.txt";
    ofstream output(outputFilename, ios::binary);

    string decodedData = decode(input, encodingTree, output);
    // Free the memory allocated for the encoding tree.
    freeTree(encodingTree);
    return decodedData;
}


