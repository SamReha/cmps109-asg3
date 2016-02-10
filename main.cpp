// $Id: main.cpp,v 1.8 2015-04-28 19:23:13-07 - - $

#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            traceflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}

void trim_whitespace(string& str) {
   // Trim left-hand whitespace
   size_t left_index = str.find_first_not_of(" ");
   if (left_index != 0 and left_index != string::npos) {
      str = str.substr(left_index, str.size()-left_index);
   }

   // Trim right-hand whitespace
   size_t right_index = str.find_last_not_of(" ");
   if (right_index != str.size()-1 and right_index != string::npos) {
      str = str.substr(0, right_index+1);
   }

   if (right_index == string::npos or left_index == string::npos) {
      str = "";
   }
}

int main (int argc, char** argv) {
   sys_info::set_execname (argv[0]);
   scan_options (argc, argv);

   // Process file arguments
   str_str_map test;
   ifstream infile;
   for (char** argp = &argv[optind]; argp != &argv[argc]; ++argp) {
      /*
      str_str_pair pair (*argp, to_string<int> (argp - argv));
      cout << "Before insert: " << pair << endl;
      test.insert (pair);
      */

      // Try and open the file
      infile.open(*argp);
      if (infile.fail()) {
         cerr << "Couldn't open file: " << *argp << endl;
         return EXIT_FAILURE;
      } else {
         // Parse the file for commands
         string line;
         while (getline(infile, line)) {
            trim_whitespace(line);
            string key("");
            string value("");
            size_t index_of_equals = line.find_first_of("=");

            // If the line has any data at all...
            if (line.size() > 0 and line.at(0) != '#') {
               // If there's no '=' in the line:
               if (index_of_equals == string::npos) {
                  cout << "Command: key" << endl;
                  key = line;
               } else {
                  // If there's only 1 character, it MUST be
                  // the '=' command
                  if (line.size() == 1) {
                     cout << "Command: =" << endl;
                  } else if (index_of_equals == line.size()-1) {
                     // If the = is at the very end, it must
                     // be the delete command
                     cout << "Command: key =" << endl;
                     key = line.substr(0, index_of_equals);
                  } else if (index_of_equals == 0) {
                     // If the = is at the front, it must be
                     // the value search command
                     cout << "Command: = value" << endl;
                  } else {
                     // Else, it must be the key assignment
                     // command
                     cout << "Command: key = value" << endl;
                     key = line.substr(0, index_of_equals);
                     value = line.substr(index_of_equals+1,
                                         line.size());
                  }
               }
               cout << "-" << key << ": " << value << endl;
            }
         }

         infile.close();
      }
   }

   /*
   for (str_str_map::iterator itor = test.begin();
        itor != test.end(); ++itor) {
      cout << "During iteration: " << *itor << endl;
   }

   str_str_map::iterator itor = test.begin();
   test.erase (itor);
   */

   cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}

