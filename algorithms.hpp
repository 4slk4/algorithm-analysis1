///////////////////////////////////////////////////////////////////////////////
// algorithms.hpp
//
// Definitions for four algorithms:
//
// run_length_encode
// longest_frequent_substring
// reformat_date
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>


namespace algorithms {

  //RUN LENGTH ENCODE
  void append_run(std::string &c, char run_char, int run_length){
      if (run_length > 1)
          c.append(std::to_string(run_length));
      c.push_back(run_char);
  }

  std::string run_length_encode(const std::string& uncompressed) {
    for (int i = 0; i < uncompressed.size(); i++){
        if (isspace(uncompressed[i])){
          if (isspace(uncompressed[i+1]))
            throw std::invalid_argument("The input string contains trailing spaces.");
        }
    }
    int size = uncompressed.size();
    std::string result;
    if (uncompressed.empty()){
        return result;
    }
    char run_char = uncompressed[0];
    int run_length = 1;

    for (int i = 1; i < size; i++){
        if (uncompressed[i] == run_char)
            run_length++;
        else{
            append_run(result, run_char, run_length);
            run_char = uncompressed[i];
            run_length = 1;
        }
    }

    append_run(result, run_char, run_length);
    return result;  
  }

  //RETURN THE LONGEST SUBSTRING OF TEXT
  std::string longest_frequent_substring(const std::string& text, unsigned k) {
    int n = text.size();
    
    //Check for special cases
    if (n == 0 || k == 1)
        return text;
    if (k > n)
        return "";
    
    //Create and initialize a dictionary
    std::map<char, int> freq;
    for (auto const& c : text)
        freq.insert(std::pair<char, int>(c, 0));

    for (auto const& c: text)
        freq[c] = (freq.find(c) == freq.end()) ? 1 : freq[c] + 1;

    std::string best = "";

    for (int b = 0; b < n-1; b++){
        for (int e = 1; e <= n; e++){
            std::string cand = text.substr(b, e);
            for (auto const& elem : cand)
                if (freq[elem] < k) cand = "";
            if (cand.size() > best.size())
                best = cand;
        }
        }

    return best;
  }
  

  //REFORMAT DATE

  //Helper function for matching letter month
  std::string lookup_month(const std::string& m){
      if (m == "jan" || m == "january") return "01";
      else if (m == "feb" || m == "february") return "02";
      else if (m == "mar" || m == "march") return "03";
      else if (m == "apr" || m == "april") return "04";
      else if (m == "may" || m == "may") return "05";
      else if (m == "jun" || m == "june") return "06";
      else if (m == "jul" || m == "july") return "07";
      else if (m == "aug" || m == "august") return "08";
      else if (m == "sep" || m == "september") return "09";
      else if (m == "oct" || m == "october") return "10";
      else if (m == "nov" || m == "november") return "11";
      else if (m == "dec" || m == "december") return "12";
      else
        return "";

  }

  //Main function
  std::string reformat_date(const std::string& input) {
    //Check for special cases
    if (input.empty()){
        throw std::invalid_argument("Empty input");
    }
    if (input.find_first_not_of(' ') == std::string::npos){
        throw std::invalid_argument("The input only has spaces");
    }
    
    //Trim string
    std::string clean = "";
    std::size_t start = input.find_first_not_of(" ");
    std::size_t len = input.find_last_not_of(" ") - start + 1;
    clean = input.substr(start, len);
    
    //Erase commas
    if (input.find(',') != std::string::npos){
        int comma = clean.find(',');
        clean = clean.erase(comma, 1);
    }
    
    //Find the delimeter to split the input string
    char delim;
    
    if (input.find('-') != std::string::npos){
        delim = '-';
    }
    else if (input.find('/') != std::string::npos){
        delim = '/';
    }
    else if (input.find(' ') != std::string::npos){
        delim = ' ';
    }
    else
        throw std::invalid_argument("Invalid format");

    //Split string into tokens and store them in the tokens<vector>
    std::stringstream ss(clean);
    std::string iter;
    std::vector<std::string> tokens;
    while (getline(ss, iter, delim)){
        tokens.push_back(iter);
    }

    //Check for invalid string
    if (tokens.size() != 3)
        throw std::invalid_argument("Invalid input");

    //Build result
    std::string result = "";
    std::string y;
    std::string m;
    std::string d;

    for (auto str: tokens){
        if (isalpha(str[0])){
            //Convert the string to lowercase
            for (int i = 0; i < str.length(); i++){
                str[i] = towlower(str[i]);
            }

            m = lookup_month(str);
            if (m.empty()) //The function lookup_month() return an empty string if it doesn't match any month
                throw std::invalid_argument("Invalid month");
        }
        else{
            if (str.size() == 4){
                if (std::stoi(str) >= 1900 && std::stoi(str) <= 2099)
                    y.append(str);
                else
                    throw std::invalid_argument("Year is out of range.");
            }
            else if (str.size() == 2 || str.size() == 1){
                if (m.empty()){
                    if (std::stoi(str) >= 1 && std::stoi(str) <= 12){
                        if (str.size() == 1)
                            m.append("0");
                        m.append(str);
                    }
                    else
                        throw std::invalid_argument("Month is out of range");
                }
                else{
                    if (std::stoi(str) >= 1 && std::stoi(str) <= 31){
                        if (str.size() == 1)
                            d.append("0");
                        d.append(str);
                    }
                    else
                        throw std::invalid_argument("Date is out of range");
                }
            }
            else
                throw std::invalid_argument("Invalid format");
        }
    }
    
    std::stringstream parse;   
    parse << y << "-" << m << "-" << d;
    result = parse.str();
    return result;
  }

}
