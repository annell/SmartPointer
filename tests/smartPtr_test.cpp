//
// Created by Stefan Annell on 2022-05-17.
//

#include "SmartPointer.h"
#include <gtest/gtest.h>
#include <type_traits>

TEST(Auto, CompileCheck) {
  using ptrType = smartPtr::Auto<int>;
  ASSERT_TRUE(std::is_constructible<ptrType>::value);
  ASSERT_TRUE(std::is_destructible<ptrType>::value);
  ASSERT_TRUE((std::is_assignable<ptrType, ptrType>::value));
  ASSERT_TRUE(std::is_copy_assignable<ptrType>::value);
  ASSERT_TRUE(std::is_copy_constructible<ptrType>::value);
  ASSERT_TRUE(std::is_move_assignable<ptrType>::value);
  ASSERT_TRUE(std::is_move_constructible<ptrType>::value);
}

TEST(Auto, Nullptr) {
  smartPtr::Auto<int> sp;
  EXPECT_FALSE(sp);
}

TEST(Auto, NotNullptr) {
  auto sp = smartPtr::MakeAuto<int>(5);
  EXPECT_TRUE(sp);
}

TEST(Auto, Assignment) {
  smartPtr::Auto<int> sp;
  auto sp2 = smartPtr::MakeAuto<int>(5);
  EXPECT_FALSE(sp);
  EXPECT_TRUE(sp2);

  sp = sp2;

  ASSERT_NE(sp, sp2);
  ASSERT_EQ(*sp, *sp2);
}

TEST(Auto, emptyReset) {
  smartPtr::Auto<int> sp;
  EXPECT_FALSE(sp);

  sp.reset();
  EXPECT_FALSE(sp);
}

TEST(Auto, reset) {
  auto sp = smartPtr::MakeAuto<int>(5);
  EXPECT_TRUE(sp);

  sp.reset();
  EXPECT_FALSE(sp);
}

TEST(Auto, resetToValue) {
  smartPtr::Auto<int> sp;
  EXPECT_FALSE(sp);

  sp.reset((new int(5)));
  EXPECT_TRUE(sp);
}

TEST(Auto, CopyReset) {
  auto sp = smartPtr::MakeAuto<int>(5);
  {
    EXPECT_TRUE(sp);

    auto sp2 = sp;
    ASSERT_NE(sp, sp2);
    ASSERT_EQ(*sp, *sp2);

    sp.reset();
    EXPECT_FALSE(sp);
    EXPECT_TRUE(sp2);
  }
}

TEST(Auto, GetRaw) {
  auto sp = smartPtr::MakeAuto<int>(5);
  EXPECT_NE(sp.get(), nullptr);

  smartPtr::Auto<int> sp2;
  EXPECT_EQ(sp2.get(), nullptr);
}

TEST(Auto, PushbackVector) {
  auto sp = smartPtr::MakeAuto<int>(5);
  std::vector<smartPtr::Auto<int>> vector;
  vector.push_back(std::move(sp));
}

TEST(Shared, CompileCheck) {
  using ptrType = smartPtr::Shared<int>;
  ASSERT_TRUE(std::is_constructible<ptrType>::value);
  ASSERT_TRUE(std::is_destructible<ptrType>::value);
  ASSERT_TRUE((std::is_assignable<ptrType, ptrType>::value));
  ASSERT_TRUE(std::is_copy_assignable<ptrType>::value);
  ASSERT_TRUE(std::is_copy_constructible<ptrType>::value);
  ASSERT_TRUE(std::is_move_assignable<ptrType>::value);
  ASSERT_TRUE(std::is_move_constructible<ptrType>::value);
}

TEST(Shared, Nullptr) {
  smartPtr::Shared<int> sp;
  EXPECT_FALSE(sp);
}

TEST(Shared, NotNullptr) {
  smartPtr::Shared<int> sp(new int(5));
  EXPECT_TRUE(sp);
}

TEST(Shared, Assignment) {
  smartPtr::Shared<int> sp;
  auto sp2 = smartPtr::MakeShared<int>(5);
  EXPECT_FALSE(sp);
  EXPECT_TRUE(sp2);

  sp = sp2;

  ASSERT_EQ(sp.get(), sp2.get());
  ASSERT_EQ(*sp, *sp2);
}

TEST(Shared, AssignmentDelete) {
  auto sp2 = smartPtr::MakeShared<int>(5);
  EXPECT_TRUE(sp2);
  ASSERT_EQ(sp2.references(), 1);

  {
    smartPtr::Shared<int> sp;
    sp = sp2;

    ASSERT_EQ(sp.get(), sp2.get());
    ASSERT_EQ(*sp, *sp2);
    ASSERT_EQ(sp.references(), 2);
    ASSERT_EQ(sp2.references(), 2);
  }

  ASSERT_EQ(sp2.references(), 1);
  ASSERT_NE(sp2.get(), nullptr);
  ASSERT_EQ(*sp2, 5);
}

static int constructDeleteCounter;

struct dummyData {
  dummyData() { constructDeleteCounter++; }
  ~dummyData() { constructDeleteCounter--; }
};

TEST(Shared, AssignmentDelete2) {
  constructDeleteCounter = 0;

  {
    ASSERT_EQ(constructDeleteCounter, 0);
    auto sp2 = smartPtr::MakeShared<dummyData>();
    EXPECT_TRUE(sp2);
    ASSERT_EQ(sp2.references(), 1);
    ASSERT_EQ(constructDeleteCounter, 1);
    {
      smartPtr::Shared<dummyData> sp;
      sp = sp2;
      ASSERT_EQ(constructDeleteCounter, 1);

      ASSERT_EQ(sp.get(), sp2.get());
      ASSERT_EQ(sp.references(), 2);
      ASSERT_EQ(sp2.references(), 2);
    }
    ASSERT_EQ(sp2.references(), 1);
    ASSERT_NE(sp2.get(), nullptr);
  }
  ASSERT_EQ(constructDeleteCounter, 0);
}

TEST(Shared, CopyDelete) {
  constructDeleteCounter = 0;

  {
    ASSERT_EQ(constructDeleteCounter, 0);
    auto sp2 = smartPtr::MakeShared<dummyData>();
    EXPECT_TRUE(sp2);
    ASSERT_EQ(sp2.references(), 1);
    ASSERT_EQ(constructDeleteCounter, 1);
    {
      auto sp = sp2;
      ASSERT_EQ(constructDeleteCounter, 1);

      ASSERT_EQ(sp.get(), sp2.get());
      ASSERT_EQ(sp.references(), 2);
      ASSERT_EQ(sp2.references(), 2);
    }
    ASSERT_EQ(sp2.references(), 1);
    ASSERT_NE(sp2.get(), nullptr);
  }
  ASSERT_EQ(constructDeleteCounter, 0);
}

TEST(Shared, PushbackVector) {
  auto sp = smartPtr::MakeShared<int>(5);
  std::vector<smartPtr::Shared<int>> vector;
  vector.push_back(std::move(sp));
}

TEST(Unique, CompileCheck) {
  using ptrType = smartPtr::Unique<int>;
  ASSERT_TRUE(std::is_constructible<ptrType>::value);
  ASSERT_TRUE(std::is_destructible<ptrType>::value);
  ASSERT_FALSE((std::is_assignable<ptrType, ptrType>::value));
  ASSERT_FALSE(std::is_copy_assignable<ptrType>::value);
  ASSERT_FALSE(std::is_copy_constructible<ptrType>::value);
  ASSERT_FALSE(std::is_move_assignable<ptrType>::value);
  ASSERT_TRUE(std::is_move_constructible<ptrType>::value);
}

TEST(Unique, Nullptr) {
  smartPtr::Unique<int> sp;
  EXPECT_FALSE(sp);
}

TEST(Unique, NotNullptr) {
  auto sp = smartPtr::MakeUnique<int>(5);
  EXPECT_TRUE(sp);
}

TEST(Unique, Assignment) {
  smartPtr::Unique<int> sp;
  auto sp2 = smartPtr::MakeUnique<int>(5);
  EXPECT_FALSE(sp);
  EXPECT_TRUE(sp2);

  // sp = sp2; <--- Won't compile
}

TEST(Unique, Copy) {
  auto sp = smartPtr::MakeUnique<int>(5);
  EXPECT_TRUE(sp);

  // auto sp2 = sp; <--- Won't compile
}

TEST(Unique, Reset) {
  auto sp = smartPtr::MakeUnique<int>(5);
  EXPECT_TRUE(sp);

  sp.reset();
  EXPECT_FALSE(sp);
}

TEST(Unique, Move) {
  auto sp = smartPtr::MakeUnique<int>(5);
  EXPECT_TRUE(sp);

  auto movePtr = [](smartPtr::Unique<int> &&ptr) { EXPECT_TRUE(ptr); };
  movePtr(std::move(sp));
}

TEST(Unique, emptyReset) {
  smartPtr::Unique<int> sp;
  EXPECT_FALSE(sp);

  sp.reset();
  EXPECT_FALSE(sp);
}

TEST(Unique, reset) {
  auto sp = smartPtr::MakeUnique<int>(5);
  EXPECT_TRUE(sp);

  sp.reset();
  EXPECT_FALSE(sp);
}

TEST(Unique, resetToValue) {
  smartPtr::Unique<int> sp;
  EXPECT_FALSE(sp);

  sp.reset((new int(5)));
  EXPECT_TRUE(sp);
}

TEST(Unique, PushbackVector) {
  auto sp = smartPtr::MakeUnique<int>(5);
  std::vector<smartPtr::Unique<int>> vector;
  vector.push_back(std::move(sp));
}

struct leakyClass {
  leakyClass() { data = new int(5); }

  int *data = nullptr;
};

TEST(Unique, LeakyData) {
  int copyData = 0;
  int *copyPtr = nullptr;
  {
    auto sp = smartPtr::MakeUnique<leakyClass>();
    copyData = *sp->data;
    EXPECT_EQ(copyPtr, nullptr);
    copyPtr = sp->data;
    EXPECT_EQ(copyData, 5);
    EXPECT_EQ(*copyPtr, 5);
  }
  EXPECT_EQ(copyData, 5);
  EXPECT_EQ(*copyPtr, 5);
}

struct notLeakyClass {
  notLeakyClass() { data = new int(5); }
  ~notLeakyClass() { delete data; }

  int *data = nullptr;
};

TEST(Unique, notLeakyData) {
  int copyData = 0;
  int *copyPtr = nullptr;
  {
    auto sp = smartPtr::MakeUnique<notLeakyClass>();
    copyData = *sp->data;
    EXPECT_EQ(copyPtr, nullptr);
    copyPtr = sp->data;
    EXPECT_EQ(copyData, 5);
    EXPECT_EQ(*copyPtr, 5);
  }
  EXPECT_EQ(copyData, 5);
  EXPECT_NE(*copyPtr, 5);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}