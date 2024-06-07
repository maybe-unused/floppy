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

struct Velocity : public traits::formattable<Velocity, char>
{
  f64 value;
  explicit Velocity(f64 value) : value(value) {}
  [[nodiscard]] auto to_string() const -> std::string override { return fmt::format("{:.1f} m/s", value); }
};

TEST(Traits, Singleton)
{
  EXPECT_EQ(TestSingleton::ref().add(1, 2), 3);
  EXPECT_EQ(TestSingleton::ref_mut().add(1, 2), 3);
  EXPECT_EQ(TestSingleton::ptr()->add(1, 2), 3);
  EXPECT_EQ(TestSingleton::ptr_mut()->add(1, 2), 3);
  EXPECT_TRUE(TestSingleton::ptr() == TestSingleton::ptr_mut());
}

TEST(Traits, Formattable)
{
  auto val = Velocity{10.0};
  EXPECT_EQ(val.to_string(), "10.0 m/s");
  auto ss = std::stringstream();
  ss << val;
  EXPECT_EQ(ss.str(), "10.0 m/s");
  EXPECT_EQ(fmt::format("{}", val), "10.0 m/s");
  EXPECT_EQ((std::string)val, "10.0 m/s");
  EXPECT_EQ(static_cast<std::string>(val), "10.0 m/s");
  EXPECT_EQ(str_cast<char>(val), "10.0 m/s");
}
