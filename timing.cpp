///////////////////////////////////////////////////////////////////////////////
// timing.cpp
//
// Example code showing how to run each algorithm while measuring
// elapsed times precisely. You should modify this program to gather
// all of your experimental data.
//
///////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "algorithms.hpp"
#include "timer.hpp"

enum class algo_choice { rle, lfs, date };

const size_t MIN_N{10},
  MAX_INPUT_PREVIEW_SIZE{80};

const unsigned LFS_K{20}; // k value for longest frequent substring

void print_bar() {
  std::cout << std::string(79, '-') << std::endl;
}

void print_usage() {
  std::cout << "usage:" << std::endl << std::endl
	    << "    timing <ALGO> <N>" << std::endl << std::endl
	    << "where" << std::endl << std::endl
	    << "    <ALGO> is one of: rle lfs date" << std::endl
	    << "    <N> is an integer string length (at least " << MIN_N << ")" << std::endl
	    << std::endl
	    << "Example:" << std::endl
	    << "    $ ./timing rle 5000" << std::endl
	    << std::endl;
}

std::string generate_input(int n, algo_choice algo){
  // build an input string
  std::string input;
  std::mt19937 rng(n); // use a deterministic seed for reproducibility between runs
  if (algo != algo_choice::date) {
    // rle and lfs can use a string of random letters
    std::uniform_int_distribution<int> rand_letter('a', 'z');
    while (input.size() < n) {
      input.push_back(rand_letter(rng));
    }
  } else {
    // date needs a properly-formatted date, padded with spaces
    // build a random "Y-M-D" string
    std::uniform_int_distribution<unsigned> rand_year(1900, 2099),
      rand_month(1, 12),
      rand_day(1, 31);
    std::stringstream ss;
    ss << rand_year(rng) << "-" << rand_month(rng) << "-" << rand_day(rng);
    std::string date_str = ss.str();
    assert(n >= date_str.size()); // the reason for MIN_N
    size_t padding_chars = n - date_str.size();
    input.assign(padding_chars, ' ');
    input += date_str;
  }
  // check that input size is correct
  assert(input.size() == n);

  return input;
}

int main(int argc, char* argv[]) {

  // Exit codes
  const int SUCCESS = 0, USAGE_ERROR = 1;

  // First, try to parse commandline arguments for algo choice and n.
  algo_choice algo;
  size_t n;
  
  if (argc != 3) {
    print_usage();
    return USAGE_ERROR;
  }

  std::string algo_str{argv[1]},
    n_str{argv[2]};

  if (algo_str == "rle") {
    algo = algo_choice::rle;
  } else if (algo_str == "lfs") {
    algo = algo_choice::lfs;
  } else if (algo_str == "date") {
    algo = algo_choice::date;
  } else {
    std::cout << "error: unknown <ALGO> \"" << algo_str << "\""
	      << std::endl << std::endl;
    print_usage();
    return USAGE_ERROR;
  }

  long n_signed;
  try {
    n_signed = std::stol(n_str);
  } catch (std::exception e) {
    std::cout << "error: <N> must be an integer"
	      << std::endl << std::endl;
    print_usage();
    return USAGE_ERROR;
  }
  if (n_signed < 0) {
    std::cout << "error: <N> must be non-negative"
	      << std::endl << std::endl;
    print_usage();
    return USAGE_ERROR;
  }
  n = n_signed;
  if (n < MIN_N) {
    std::cout << "error: <N> must be at least " << MIN_N
	      << std::endl << std::endl;
    print_usage();
    return USAGE_ERROR;
  }

  // n should be initialized
  assert(n >= MIN_N);

  // // build an input string
  // std::string input;
  // std::mt19937 rng(n); // use a deterministic seed for reproducibility between runs
  // if (algo != algo_choice::date) {
  //   // rle and lfs can use a string of random letters
  //   std::uniform_int_distribution<int> rand_letter('a', 'z');
  //   while (input.size() < n) {
  //     input.push_back(rand_letter(rng));
  //   }
  // } else {
  //   // date needs a properly-formatted date, padded with spaces
  //   // build a random "Y-M-D" string
  //   std::uniform_int_distribution<unsigned> rand_year(1900, 2099),
  //     rand_month(1, 12),
  //     rand_day(1, 31);
  //   std::stringstream ss;
  //   ss << rand_year(rng) << "-" << rand_month(rng) << "-" << rand_day(rng);
  //   std::string date_str = ss.str();
  //   assert(n >= date_str.size()); // the reason for MIN_N
  //   size_t padding_chars = n - date_str.size();
  //   input.assign(padding_chars, ' ');
  //   input += date_str;
  // }
  // // check that input size is correct
  // assert(input.size() == n);

  // prepare to run algorithm with timer
  Timer timer;    // see timer.hpp
  double elapsed; // elapsed time in seconds

  print_bar();
  std::cout << "algo = ";
  switch (algo) {
  case algo_choice::rle:
    std::cout << "rle\n";
    break;
  case algo_choice::lfs:
    std::cout << "lfs\n";
    break;
  case algo_choice::date:
    std::cout << "date\n";
    break;
  }
  
  // std::cout << std::endl
	//     << "n = " << n << std::endl;

  // size_t input_preview_size = std::min(input.size(), MAX_INPUT_PREVIEW_SIZE);
  // std::cout << "first " << input_preview_size << " characters of input:"
	//     << std::endl
	//     << std::string(input.begin(), input.begin() + input_preview_size)
	//     << std::endl;
  
  // run the algorithm
  // note that there is no input/output while the timer is running
  std::vector<std::string> test_list;
  int count = 100;
  for (int i = 0; i < n; i++)
  {
    test_list.push_back(generate_input(count, algo));
    count += 100;
  }
  
  
  switch (algo) {
  case algo_choice::rle:
    for (auto e : test_list){
      timer.reset();
      algorithms::run_length_encode(e);
      elapsed = timer.elapsed();
      std::cout << "elapsed time=" << elapsed << " seconds" << " size n=" << e.size() << std::endl;
    }
    break;
  case algo_choice::lfs:
    for (auto e : test_list){
      timer.reset();
      algorithms::longest_frequent_substring(e, LFS_K);
      elapsed = timer.elapsed();
      std::cout << "elapsed time=" << elapsed << " seconds" << " size n=" << e.size() << std::endl;
    }
    break;
  case algo_choice::date:
    for (auto e : test_list){
      timer.reset();
      algorithms::reformat_date(e);
      elapsed = timer.elapsed();
      std::cout << "elapsed time=" << elapsed << " seconds" << " size n=" << e.size() << std::endl;
    }
    break;
  }

  // elapsed = timer.elapsed();

  // // end of timing

  // std::cout << "elapsed time=" << elapsed << " seconds" << std::endl;

  print_bar();

  return SUCCESS;
}
