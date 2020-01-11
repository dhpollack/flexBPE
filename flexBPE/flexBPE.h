#pragma once
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h> // ftruncate

#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace flexBPE {

using namespace std;

struct pair_hash {
  template <class T1, class T2> size_t operator()(const pair<T1, T2> &p) const {
    auto h1 = hash<T1>{}(p.first);
    auto h2 = hash<T2>{}(p.second);
    size_t seed = h1;
    // boost::hash_combine
    return h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
};

using tp = pair<uint32_t, uint32_t>;
using tps = pair<string, string>;
using pc = unordered_map<tp, pair<int32_t, tp> *, pair_hash>;

auto compFunctor = [](pair<string, int> elem1, pair<string, int> elem2) {
  return elem1.second > elem2.second ||
         (elem1.second == elem2.second && elem1.first < elem2.first);
};

class BPETrainer {
public:
  explicit BPETrainer(const char *jEndWord = "</w>",
                      const size_t jEndWordLength = 4,
                      const char *jTokenDelim = "@@",
                      const size_t jTokenDelimLength = 2,
                      const size_t jThreads =
                          max(1, min(10, int(thread::hardware_concurrency()))));

  void learncodes(const uint32_t kNPairs, const char *inputFile1,
                  const char *inputFile2, const bool replace_vocab = false,
                  const bool output_codes = false);
  void printcodes();

  void getvocab(const char *inputFile1, const char *inputFile2,
                const bool = false);

  void applybpe(const char *outputFile, const char *inputFile);

  void applybpe_stream();

  void readVocab(const char *fp, unordered_map<string, uint32_t> &vocab);

  void readCodes(const char *fp, unordered_map<tps, uint32_t, pair_hash> &codes,
                 unordered_map<string, tps> &reversed_codes);

  void save_vocab(const char *outputFile);
  void save_merges(const char *outputFile);
  void save_trained(const char *outputDir);

  string apply(string &sentence);

  vector<string> apply(vector<string> &sentences);

  // Previously serialized to a file
  unordered_map<string, uint32_t> vocab;
  unordered_map<tps, uint32_t, pair_hash> codes;
  unordered_map<string, tps> reversed_codes;

private:
  // previous global variables
  const char *jEndWord;
  const size_t jEndWordLength;
  const char *jTokenDelim;
  const size_t jTokenDelimLength;
  const size_t jThreads;
  // storage for serializing codes
  vector<pair<string, string>> merges;
  // private functions
  int safeOpen(const char *file_path, int flags, mode_t mode);
  void readText(const char *fp, unordered_map<string, uint32_t> &word_count);
  std::pair<size_t, uint64_t>
  output_or_count(unordered_map<string, string> &bpe, size_t size, char *f,
                  char *fo);
  void outputText(const char *fpo, const char *fp,
                  unordered_map<string, string> &bpe);
  void tokenize(const unordered_map<string, uint32_t> &word_count,
                unordered_map<string, uint32_t> &token_to_int,
                vector<string> &int_to_token, vector<list<uint32_t>> &words,
                vector<int32_t> &counts);
  void tokenize_str(const unordered_map<string, uint32_t> &word_count,
                    unordered_map<string, vector<string>> &words);
  void
  count_in_word(list<uint32_t> &word, uint32_t wi, uint32_t count,
                pc &pair_counts, vector<pair<int32_t, tp>> &contiguous_counts,
                unordered_map<tp, unordered_set<uint32_t>, pair_hash> &where);
  void find_maxp(vector<pair<int32_t, tp>> &contiguous_counts, tp &maxp,
                 int32_t &max_c);
  void split(vector<string> &splits, const string &text, char sep);
  void decompose(const string s, vector<string> &newSubwords, bool isFinal);
  void limitVocab(const vector<string> &subwords, vector<string> &newSubwords);
  string process_bpe(vector<string> &subwords);
  set<pair<string, int>, decltype(compFunctor)> get_sortedvocab();
};

class BPEInference : public BPETrainer {
public:
  explicit BPEInference(
      const char *codesPath, const char *vocabPath,
      const char *jEndWord = "</w>", const size_t jEndWordLength = 4,
      const char *jTokenDelim = "@@", const size_t jTokenDelimLength = 2,
      const size_t jThreads = max(1, min(10,
                                         int(thread::hardware_concurrency()))));
};

} // end namespace flexBPE
