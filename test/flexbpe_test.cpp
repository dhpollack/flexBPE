#include "flexBPE/flexBPE.h"
#include "gtest/gtest.h"

using namespace flexBPE;

TEST(trainerTest, trainer) {
	BPETrainer trainer = BPETrainer();
	vector<string> test_vocab = trainer.getvocab_vec("assets/corpus.txt", "");
	EXPECT_EQ(test_vocab.size(), 4);
}
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
