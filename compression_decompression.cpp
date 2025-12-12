#include "compression_decompression.h"
#include <iostream>
#include <fstream>

// compressor functions implementations
void compressor::compress_file(const string& input_path, const string& output_path)
{
    vector<unsigned char> data = read_binary_file(input_path);
    if(data.size() <= 2)
    {
        cout << "File too small to compress" << endl;
        return;
    }
    stack<unsigned char> unused_char;
    build_unused_char(data,unused_char);
    vector<unsigned char> changed_data;
    vector<pair<pair<unsigned char, unsigned char>, unsigned char>> changes;
    map<pair<unsigned char, unsigned char>, int> freq;
    pair<unsigned char, unsigned char> most_freq_pair;
    int max_freq;
    unsigned char new_char;
    while(true)
    {
        freq = build_Pair_Frequencies(data);
        if(freq.empty())
            break;
        most_freq_pair = find_mostFreq_pair(freq);
        
        max_freq = freq[most_freq_pair];
        if(max_freq <= 3) break;
        
        if(unused_char.empty())
        {
            // cout << "Warning: No more unused characters available. Stopping compression." << endl;
            break;
        }
        new_char = new_unused_char(unused_char);
        changed_data.clear();
        apply_change(data, changed_data, most_freq_pair, new_char);
        data.swap(changed_data);
        
        save_change(changes, most_freq_pair, new_char);
    }

    write_binary_file(data, changes,output_path);
}

vector<unsigned char> compressor::read_binary_file(const string& path)
{
    ifstream input(path, ios::binary);
    
    if(!input)
    {
        cout << "cannot open the file";
        return {};
    }

    vector<unsigned char> data((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));
    input.close();
    return data;
}

void compressor::write_binary_file(const vector<unsigned char>& compressed_data, const vector<pair<pair<unsigned char, unsigned char>, unsigned char>>& changes, const string& output_path)
{
    ofstream output(output_path, ios::binary);
    if(!output)
    {
        cout << "cannot open the file for writing";
        return;
    }

    int n_changes = changes.size();
    output.write((const char*) &n_changes, sizeof(n_changes));

    for(const auto& p : changes)
    {
        output.write((const char*)(&p.first.first),1);
        output.write((const char*)(&p.first.second),1);
        output.write((const char*)(&p.second),1);
    }

    output.write((const char*)compressed_data.data() , compressed_data.size());
    
    if(!output)
        cout << "write failed";
    
    output.close();
}

map<pair<unsigned char, unsigned char>, int> compressor::build_Pair_Frequencies(const vector<unsigned char>& data)
{
    map<pair<unsigned char, unsigned char>, int> freq;
    // string s;
    pair<unsigned char, unsigned char> s; 
    for(int i = 1 ; i < data.size() ; i++)
    {
        s = {data[i-1],data[i]};
        freq[s]++;
        s = {};
    }
    return freq;
}

pair<unsigned char, unsigned char> compressor::find_mostFreq_pair(map<pair<unsigned char, unsigned char>, int>& freq)
{
    pair<unsigned char, unsigned char> most_freq_pair;
    int max_freq = 0;
    for(const auto& p : freq)
    {
        if(p.second > max_freq)
        {
            most_freq_pair = p.first;
            max_freq = p.second;
        }
    }
    return most_freq_pair;
}

void compressor::build_unused_char(const vector<unsigned char>& data, stack<unsigned char>& unused_char)
{
    unordered_set<unsigned char> used_char(data.begin(), data.end());
    for(int i = 0 ; i < 256 ; i++)
    {
        if(used_char.find(i) == used_char.end())
            unused_char.push(i);
    }
}

unsigned char compressor::new_unused_char(stack <unsigned char>& unused_char)
{
    unsigned char output = unused_char.top();
    unused_char.pop();
    return output;
}

void compressor::apply_change(const vector<unsigned char>& data, vector<unsigned char>& output, pair<unsigned char, unsigned char>& most_freq, unsigned char new_char)
{
    int i = 0;
    while (i < data.size())
    {
        if ((i < data.size() - 1) && (data[i] == most_freq.first) && (data[i+1] == most_freq.second) )
        {
            output.push_back(new_char);  
            i += 2; //skip the pair
        }
        else
        {
            output.push_back(data[i]);
            i += 1;
        }
    }
}

void compressor::save_change(vector<pair<pair<unsigned char, unsigned char>, unsigned char>>& changes ,const pair<unsigned char, unsigned char>& changed_pair,const unsigned char& new_char)
{
    changes.push_back({changed_pair,new_char});
}


// decompressor functions implementations
void decompressor::retrieve_data_changes(const string& input_path, vector<unsigned char>& compressed_data, vector<pair<pair<unsigned char, unsigned char>, unsigned char>>& changes)
{
    ifstream input(input_path, ios::binary);
    
    if(!input)
    {
        cout << "cannot open the file";
        return;
    }

    int no_of_changes;
    input.read((char*) &no_of_changes, sizeof(no_of_changes));
    
    unsigned char temp_first;
    unsigned char temp_second;
    unsigned char change;
    for(int i = 0 ; i < no_of_changes ; i++)
    {
        input.read((char*) &temp_first,1);
        input.read((char*) &temp_second,1);
        input.read((char*) &change,1);

        changes.push_back({{temp_first, temp_second},change});
    }

    compressed_data.assign((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));
    input.close();
}

void decompressor::decompress_file(const string& input_path, const string& output_path)
{
    vector<unsigned char> compressed_data;
    vector<pair<pair<unsigned char, unsigned char>, unsigned char>> changes;
    retrieve_data_changes(input_path, compressed_data, changes);

    vector<unsigned char> data;
    for(int i = changes.size()-1 ; i >= 0 ; i--)
    {
        for(int j = 0 ; j < compressed_data.size() ; j++)
        {
            if(compressed_data[j] == changes[i].second)
            {
                data.push_back(changes[i].first.first);
                data.push_back(changes[i].first.second);
            }
            else
            {
                data.push_back(compressed_data[j]);
            }
        }
        if(i != 0)
        {
            data.swap(compressed_data);
            data.clear();
        }
    }

    ofstream output(output_path, ios::binary);
    if(!output)
    {
        cout << "cannot open the file for writing";
        return;
    }

    output.write((const char*)data.data() , data.size());
    
    if(!output)
        cout << "write failed";
    
    output.close();
}

