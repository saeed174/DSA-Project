#ifndef COMPRESSION_DECOMPRESSION_H
#define COMPRESSION_DECOMPRESSION_H

#include <string>
#include <vector>
#include <unordered_set>
#include<map>
#include <stack>
using namespace std;

// Compressor class handles compressing a binary file using a BPE-like algorithm
class compressor
{
    public:
        // Compress the input file and write the result to the output file
        // input_path: path to the original file
        // output_path: path to write the compressed file
        void compress_file(const string& input_path, const string& output_path);
   
    private:
        // Read an entire binary file into a vector of unsigned chars
        // Returns a vector containing the file's bytes
        vector<unsigned char> read_binary_file(const string& path);
        
        // Write the compressed data and the list of changes (pair replacements) to a binary file
        // compressed_data: vector of bytes after compression
        // changes: list of pair substitutions made during compression
        // output_path: path to write the compressed file
        void write_binary_file(const vector<unsigned char>& compressed_data, const vector<pair<pair<unsigned char, unsigned char>, unsigned char>>& changes, const string& output_path);
       
        // Build frequency map of all consecutive byte pairs in the data
        // data: input vector of bytes
        // Returns a map: key = pair of bytes, value = count of occurrences
        map<pair<unsigned char, unsigned char>, int> build_Pair_Frequencies(const vector<unsigned char>& data);
        
        // Find the most frequent pair of bytes from the frequency map
        // freq: map of pair frequencies
        // Returns the pair of bytes that appears most frequently
        pair<unsigned char, unsigned char> find_mostFreq_pair(map<pair<unsigned char, unsigned char>, int>& freq);
        
        // Build a stack of unused bytes that do not appear in the original data
        // data: input bytes
        // unused_char: stack to store available replacement bytes
        void build_unused_char (const vector<unsigned char>& data, stack<unsigned char>& unused_char);
       
        // Get a new unused byte from the stack and remove it from available pool       
        unsigned char new_unused_char(stack <unsigned char>& unused_char);
        
        // Apply a replacement: replace occurrences of the most frequent pair with a new byte
        // data: input bytes
        // output: output bytes after applying this change
        // most_freq: pair of bytes to replace
        // new_char: byte used to replace the pair
        void apply_change(const vector<unsigned char>& data, vector<unsigned char>& output, pair<unsigned char, unsigned char>& most_freq, unsigned char new_char);
        
        // Save a pair replacement to the list of changes
        // changes: list of replacements
        // changed_pair: pair that was replaced
        // new_char: byte that replaced the pair
        void save_change(vector<pair<pair<unsigned char, unsigned char>, unsigned char>>& changes ,const pair<unsigned char, unsigned char>& changed_pair,const unsigned char& new_char);
};


// Decompressor class handles restoring a compressed file to its original form
class decompressor
{
    public:
        // Decompress a file compressed with compress_file
        // input_path: path to the compressed file
        // output_path: path to write the decompressed original file
        void decompress_file(const string& input_path, const string& output_path);
    
    private:
        // Retrieve the compressed data and the list of changes (pair replacements) from a file
        // input_path: path to the compressed file
        // compressed_data: vector to store the compressed bytes
        // changes: vector to store the list of pair substitutions
        void retrieve_data_changes(const string& input_path, vector<unsigned char>& compressed_data, vector<pair<pair<unsigned char, unsigned char>, unsigned char>>& changes );
};

#endif