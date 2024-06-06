#include <gtest/gtest.h>
#include <floppy/floppy.h>
#include <floppy/traits.h>

using namespace fl;

class TestSingleton : public traits::singleton<TestSingleton>
{
  friend struct traits::singleton<TestSingleton>;

 public:
  ~TestSingleton() = default;

  [[nodiscard]] auto add(int a, int b) const -> int { return a + b; }

 private:
  TestSingleton() = default;
};

class TestPimpl : public traits::pin<TestPimpl>
{
 public:
  TestPimpl() = default;

  [[nodiscard]] auto add(int a, int b) const -> int;

 private:
  struct Impl;
  traits::pimpl<struct Impl> impl;
};

struct TestPimpl::Impl {
  auto add(int a, int b) const -> int { return a + b; }
};

auto TestPimpl::add(int a, int b) const -> int {
  return this->impl->add(a, b);
}

TEST(Traits, Singleton)
{
  EXPECT_EQ(TestSingleton::ref().add(1, 2), 3);
  EXPECT_EQ(TestSingleton::ref_mut().add(1, 2), 3);
  EXPECT_EQ(TestSingleton::ptr()->add(1, 2), 3);
  EXPECT_EQ(TestSingleton::ptr_mut()->add(1, 2), 3);
  EXPECT_TRUE(TestSingleton::ptr() == TestSingleton::ptr_mut());
}
