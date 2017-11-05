/************************************************************************

    File: list.cc
    Author: Steven Cunden
    Date: November 1st, 2017

    Randwrite replicates the writing style of a certain book, 
    by determining the probability with which each character follows
    every possible sequence of characters of length k. It reads from a 
    given source file and redirects its results in an output file, named
    in the command line.

************************************************************************/

#include "list.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <stdio.h>

using namespace std;

list source_char_list(ifstream & source_stream) {
  // Converts the input stream into a list of char

  char c;
  list source_list;

  while (source_stream.get(c)) 
    source_list.add(c,source_list.size());

  return source_list;
}

string initial_seed(int k, list & source_list) {
  // Creates an initial seed of length k by
  // Picking index within range and getting the corresponsding char
  // Then the following k-1 chars added to create the seed

  string seed = "";
  int n = rand() % (source_list.size()-k);

  for (int i = 0; i < k; i++) 
    seed += source_list.get(n+i);
  
  return seed;
}

list followers(size_t k, string seed, list & source_list) {
  // Finds all the occurrences of the seed in the list
  // Creates a list of all the chars that follow the seed

  list fol;

  for (size_t i = 0; i < source_list.size()-k; i++) {

    if (source_list.get(i) == seed[0]) {

      string seed_check = "";

      for (size_t j = 0; j < k; j++) 
        seed_check += source_list.get(i+j);

      if (seed_check == seed) 
        fol.add(source_list.get(i+k),fol.size());      
    }
  }
  return fol;
}

string new_seed(string init_seed, list fol, list & output, int k,
  list & source_list) {
  // Creates a new seed by removing the first char of the initial one
  // and adding to it a random char from the followers of the original seed
  // If there are no followers, then it re initializes a new seed

  if (fol.size()==0) {

    string init_seed =  initial_seed(k,source_list);
    fol = followers(k,init_seed,source_list);
  }

  char rand_fol = fol.get(rand() % fol.size());
  output.add(rand_fol, output.size());
  string seed = init_seed.substr(1)+rand_fol;

  return seed;
 }

void refresh (ifstream & source_file, char** argv) {
  // Refreshes the stream to allow it to re used

  source_file.close();
  source_file.open(argv[3]);
}

int main(int argc, char** argv)
{
  // Randomizes sequence every time program is ran
  srand(time(NULL)); 

  if (argc!=5) {
    // Checks for the correct number of arguments

    cout <<"Error: "<< argc <<" arguments detected. Exactly 5 needed."<< endl;
    exit(1);
  }

  // Initialization of k, length, source_file, 
  // and output_file from command line input
  // Corresponding validity check following initiliazation. 

  int k = atoi(argv[1]);

  if (k <= 0) {
    // Checks for a positive value of k

    cout << "Error: Invalid k detected."<< endl;
    exit(1);
  }

  int len = atoi(argv[2]);

  if (len < 0) {
    // Checks for a valid length

    cout << "Error: Invalid length detected."<< endl;
    exit(1);
  }

  ifstream source_file(argv[3]);

  if (!source_file.good()) {
    // Check that the source file exists

    cout << "Error: Input file doesn't exist." << endl;
    exit(1);
  }

  string fname = argv[4];
  ofstream output_file(fname.c_str());

  if (!output_file.good()) {
    // Checks if the ouptut file exists

    cout << "Error: Unable to access output file" << endl;
    exit(1);
  }

  list source_list = source_char_list(source_file);
  int list_size = source_list.size();

  if (list_size < k) {
    // Checks that the input file has at least k chars

    cout << "Error: Input file doesnt contain eough characters!"<< endl;
    exit(1);
  }

  // Initializes output list, initial seed and seed followers

  list output;
  string seed = initial_seed(k,source_list);
  refresh(source_file,argv);
  list fol = followers(k,seed,source_list);

  // Creates a new seed using the new_seed method
  // Creates a new list of followers for that seed and outputs a char
  // Repeat the process until an output of size length is obtained.
  
  for (int i = 0; i < len; i++) {

    seed = new_seed(seed,fol,output,k,source_list);
    refresh (source_file,argv);
    fol = followers(k,seed,source_list);
    output_file << output.get(i); 
  } 
}