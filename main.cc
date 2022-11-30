#include <string>
#include <set>
#include <iostream>
#include<stdio.h>
#include<string.h>
#include<fstream>
#include<bits/stdc++.h>
#include <math.h>

using namespace std;

void StreamCipher(string InputFile, string KeyFile, string OutputFile){ // Done
    ifstream in(InputFile);
    string key;
    string line;
    string lines;

    while (in.good()) {
        getline(in, line);

        if(in.peek()!=EOF){
            lines += line + '\n';
        } else{
            lines += line;
        }
    }
    in.close();
    if(lines.empty()){
        lines = " ";
    }
    ifstream Key(KeyFile); // read key file
    getline(Key,key);

    int keylen = key.size();
    int outlen = lines.size();

    for (int i = 0; i < outlen; i++) {
        lines[i] =  lines[i] ^key[i % keylen];
    }
    ofstream out(OutputFile);
    if(out.is_open()){
        out << lines;
    }
    else{
        cerr << "Unable to open file";
    }
}

void BlockCipher_Decrypt(string InputFile, string KeyFile, string OutputFile){
    ifstream in(InputFile);
    string key;
    string line;
    string lines;

    while (in.good()) {
        getline(in, line);

        if(in.peek()!=EOF){
            lines += line + '\n';
        } else{
            lines += line;
        }
    }
    ifstream Key(KeyFile); // read key file
    getline(Key,key);

    int start = 0;
    int end = lines.length()-1;
    int key_pointer = 0;
    char temp;
    while (end >= start){
        if(key[key_pointer% key.length()] % 2 == 0){
            start ++;
        }
        else if(key[key_pointer% key.length()] % 2 == 1){
            temp= lines[end];//temp = lines[start];
            lines[end] = lines[start];
            lines[start] = temp;
            start++;
            end --;
        }
        key_pointer ++;
    }

    int line_len = lines.length();
    for(int i =0; i < line_len; i ++){
        lines[i] = lines [i] ^ key[i%key.length()];
    }

    ofstream out(OutputFile);
    if(out.is_open()){
        out << lines;
    }
    else{
        cerr << "Unable to open file";
    }
}






void BlockCipher_Encrypt(string InputFile, string KeyFile, string OutputFile){

    ifstream in(InputFile);
    string key;
    string lines;
    string line;

    while(in.good()){
        getline(in,line);
        if(in.peek() != EOF){
            lines = lines + line + "\n";
        }
        else{
            lines = lines + line;
        }
    }
    if(lines.empty()){
        lines = " ";
    }
    ifstream Key(KeyFile); // read key file
    getline(Key,key);
    int g = lines.size();
    float p = (float) g/ (float)16;
    int n = ceil(p);
    char blocks[n][16];

    for (int i =0; i < n ; i ++){
        for(int j =0; j <16; j ++){
            blocks[i][j] = ' ';
        }
    }

    int pointer = 0;
    for (int i =0; i < n ; i ++){
        for(int j =0; j <16; j ++){
            if(lines[pointer] == 0x0){
                break;
            }
            blocks[i][j] = lines[pointer];
            pointer ++;
        }
    }

    for(int i = 0 ; i <n ; i ++){
        int j = 15;
        char c = blocks[i][15];
        while (c == ' '){
            blocks[i][j] = (char) 0x81;
            j --;
            c = blocks[i][j];
        }
    }

    for (int i = 0; i < n; i++) {
        for(int j=0 ; j < 16 ; j ++){
            blocks[i][j] = blocks[i][j] ^ key[j];
        }
    }
    string output = "";
    for(int i =0; i< n; i ++){
        for(int j=0 ; j< 16; j ++){
            output.push_back(blocks[i][j]);
        }
    }
    int start = 0;
    int end = output.length()-1;
    int key_pointer = 0;
    char temp;
    while (end >= start){
        if(key[key_pointer% key.length()] % 2 == 0){
            start ++;
        }
        else if(key[key_pointer% key.length()] % 2 == 1){
            temp = output[start];
            output[start] = output[end];
            output[end] = temp;
            start++;
            end --;
        }
        key_pointer ++;
    }
    ofstream out(OutputFile);
    if(out.is_open()){
        out << output;
    }
    else{
        cerr << "Unable to open file";
    }
}
bool check_empty(ifstream& pFile)
{
    return pFile.peek() == ifstream::traits_type::eof();
}


int main(int argc, char *argv[]){

    if (argc != 6){
        cerr << "5 arguments needed\n";
        return 1 ;
    }
    char function = *argv[1]; // first argument either B or S function
    string InputFile = argv[2]; // 2nd argument input file name
    string OutputFile = argv[3]; // 3rd argument output file name
    string KeyFile = argv[4]; // 4th argument key file
    char mode = *argv[5];  // 5th argument mode either E for encrypt or D for decrypt

    ifstream input;

    input.open(InputFile);
    if(!input){
        cerr << "Can't open InputFile\n ";
        return 1;
    }
    if(check_empty(input)){
        ofstream out(OutputFile);
        if(out.is_open()){
            out << "";
        }
        else{
            cerr << "Unable to open file";
        }
        return 2;
    }

    input.close();

    ifstream key;
    key.open(KeyFile);
    if(!key){
        cerr << "Can't open Key file\n";
        return 1;
    }
    if (mode == 'E'){ //Encrypt BlockCipher
        if(function == 'B'){
            BlockCipher_Encrypt(InputFile,KeyFile,OutputFile);
        }
        if(function == 'S'){
            StreamCipher(InputFile,KeyFile,OutputFile);
        }
    }

    if (mode == 'D'){
        if (function == 'B'){
            BlockCipher_Decrypt(InputFile,KeyFile,OutputFile);
        }
        if (function == 'S'){
            StreamCipher(InputFile,KeyFile,OutputFile);
        }
    }




    return 0;

}
