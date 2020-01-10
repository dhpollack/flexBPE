#include "flexBPE/flexBPE.h"
#include "gtest/gtest.h"

using namespace flexBPE;

TEST(trainerTest, getvocab) {
	BPETrainer trainer = BPETrainer();
	trainer.getvocab("assets/corpus.txt", "", true);
	EXPECT_EQ(trainer.vocab.size(), 4);
	EXPECT_EQ(trainer.vocab["low"], 5);
	EXPECT_EQ(trainer.vocab["lower"], 2);
	EXPECT_EQ(trainer.vocab["newest"], 6);
	EXPECT_EQ(trainer.vocab["widest"], 3);
}

TEST(trainerTest, learncodes) {
	BPETrainer trainer = BPETrainer();
	trainer.learncodes(10, "assets/corpus.txt", "", true);
	EXPECT_EQ(trainer.codes.size(), 10);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
