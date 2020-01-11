#include "flexBPE/flexBPE.h"
#include "gtest/gtest.h"

using namespace flexBPE;

bool file_exists(const char *fp) {
  ifstream fd(fp);
  return fd.good();
}

size_t file_size(const char *fp) {
  struct stat st;
  if (stat(fp, &st) != 0) {
    return 0;
  }
  return st.st_size;
}

void file_test(const char *fp) {
  EXPECT_EQ(file_exists(fp), true);
  EXPECT_GT(file_size(fp), 0);
  remove(fp);
}

const char *corpus = "assets/corpus.txt";

TEST(trainerTest, getvocab) {
  BPETrainer trainer = BPETrainer();
  trainer.getvocab(corpus, "", true);
  EXPECT_EQ(trainer.vocab.size(), 4);
  EXPECT_EQ(trainer.vocab["low"], 5);
  EXPECT_EQ(trainer.vocab["lower"], 2);
  EXPECT_EQ(trainer.vocab["newest"], 6);
  EXPECT_EQ(trainer.vocab["widest"], 3);
}

TEST(trainerTest, getvocab_twofiles) {
  BPETrainer trainer = BPETrainer();
  trainer.getvocab(corpus, corpus, true);
  EXPECT_EQ(trainer.vocab.size(), 4);
  EXPECT_EQ(trainer.vocab["low"], 10);
  EXPECT_EQ(trainer.vocab["lower"], 4);
  EXPECT_EQ(trainer.vocab["newest"], 12);
  EXPECT_EQ(trainer.vocab["widest"], 6);
}

TEST(trainerTest, learncodes) {
  BPETrainer trainer = BPETrainer();
  trainer.learncodes(10, corpus, "", false, true);
  EXPECT_EQ(trainer.codes.size(), 10);
}

TEST(trainerTest, learncodes_twofiles) {
  BPETrainer trainer = BPETrainer();
  trainer.learncodes(10, corpus, corpus, false, true);
  EXPECT_EQ(trainer.codes.size(), 10);
}

TEST(trainerTest, learncodes_shortstop) {
  BPETrainer trainer = BPETrainer();
  int num_merges = 1000000;
  trainer.learncodes(num_merges, corpus, "", false, true);
  EXPECT_LT(trainer.codes.size(), num_merges);
}

TEST(trainerTest, save_vocab) {
  const char *vocab_file = "vocab-save_vocab.txt";
  BPETrainer trainer = BPETrainer();
  trainer.getvocab(corpus, "", false);
  trainer.save_vocab(vocab_file);
  file_test(vocab_file);
}

TEST(trainerTest, save_merges) {
  const char *merges_file = "merges-save_merges.txt";
  BPETrainer trainer = BPETrainer();
  trainer.learncodes(10, corpus, "", false, false);
  trainer.save_merges(merges_file);
  file_test(merges_file);
}

TEST(trainerTest, save_trained) {
  BPETrainer trainer = BPETrainer();
  trainer.learncodes(10, corpus, "", true, false);
  trainer.save_trained(".");
  file_test("vocab.txt");
  file_test("merges.txt");
}

TEST(trainerTest, applybpe_with_vocab) {
  // output: "low low low low low l@@ o@@ w@@ e@@ r l@@ o@@ w@@ e@@ r newest
  // newest newest newest newest newest widest widest widest"
  const char *output_file = "assets/corpus_encoded_with_vocab.txt";
  BPETrainer trainer = BPETrainer();
  trainer.learncodes(10, corpus, "", true, false);
  trainer.applybpe(output_file, corpus);
  file_test(output_file);
}

TEST(trainerTest, applybpe_without_vocab) {
  // output: "low low low low low lo@@ w@@ e@@ r lo@@ w@@ e@@ r newest newest
  // newest newest newest newest widest widest widest"
  const char *output_file = "assets/corpus_encoded_without_vocab.txt";
  BPETrainer trainer = BPETrainer();
  trainer.learncodes(10, corpus, "", false, false);
  trainer.applybpe(output_file, corpus);
  file_test(output_file);
}

TEST(trainerTest, trainer_constructor_args) {
  BPETrainer trainer = BPETrainer("§§", 4, "§", 2, 2);
  trainer.learncodes(10, corpus, "", false, true);
  string input("low low low low low lower lower newest newest newest newest "
               "newest newest widest widest widest");
  string output = trainer.apply(input);
  string expected = "low low low low low lo§ w§ e§ r lo§ w§ e§ r newest newest "
                    "newest newest newest newest widest widest widest";
  EXPECT_EQ(output, expected);
}

TEST(inferenceTest, applybpe) {
  BPETrainer trainer = BPETrainer();
  trainer.learncodes(10, corpus, "", true, false);
  trainer.save_trained(".");
  vector<string> test_words({"wider", "newer", "lowest"});
  vector<string> expected(
      {"w@@ i@@ d@@ e@@ r", "n@@ e@@ w@@ e@@ r", "l@@ o@@ w@@ e@@ s@@ t"});
  BPEInference inference = BPEInference("merges.txt", "vocab.txt");
  string w0_applied = inference.apply(test_words[0]);
  string w1_applied = inference.apply(test_words[1]);
  string w2_applied = inference.apply(test_words[2]);
  vector<string> wN_applied = inference.apply(test_words);
  EXPECT_EQ(w0_applied, expected[0]);
  EXPECT_EQ(w1_applied, expected[1]);
  EXPECT_EQ(w2_applied, expected[2]);
  EXPECT_EQ(wN_applied, expected);
  file_test("vocab.txt");
  file_test("merges.txt");
}

TEST(inferenceTest, applybpe_novocab) {
  BPETrainer trainer = BPETrainer();
  trainer.learncodes(10, corpus, "", true, false);
  trainer.save_trained(".");
  vector<string> test_words({"wider", "newer", "lowest"});
  vector<string> expected({"wi@@ d@@ e@@ r", "n@@ e@@ w@@ e@@ r", "lo@@ west"});
  BPEInference inference = BPEInference("merges.txt", "");
  string w0_applied = inference.apply(test_words[0]);
  string w1_applied = inference.apply(test_words[1]);
  string w2_applied = inference.apply(test_words[2]);
  vector<string> wN_applied = inference.apply(test_words);
  EXPECT_EQ(w0_applied, expected[0]);
  EXPECT_EQ(w1_applied, expected[1]);
  EXPECT_EQ(w2_applied, expected[2]);
  EXPECT_EQ(wN_applied, expected);
  file_test("vocab.txt");
  file_test("merges.txt");
}

TEST(trainerDeathTest, input_does_not_exist) {
  string nonexistent_vocab_file("assets/nocorpus.txt");
  string nonexistent_codes_file("assets/nocorpus.txt");

  ASSERT_DEATH(
      {
        BPETrainer trainer = BPETrainer();
        trainer.getvocab(nonexistent_vocab_file.c_str(), "", true);
      },
      "");

  ASSERT_DEATH(
      {
        BPETrainer trainer = BPETrainer();
        trainer.learncodes(10, nonexistent_codes_file.c_str(), "", true, false);
      },
      "");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
