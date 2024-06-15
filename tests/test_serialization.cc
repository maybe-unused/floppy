#include <gtest/gtest.h>
#include <floppy/serialization.h>

struct TestStruct
{
  int a = 1;
  int b = 2;
};

template <>
auto fl::serialization::serialize<fl::serialization::format::json>(TestStruct const& value) -> std::basic_string<char>
{
  return fmt::format(R"({{"a": {}, "b": {}}})", value.a, value.b);
}

template <>
auto fl::serialization::deserialize<fl::serialization::format::json>(std::basic_string<char> const& value) -> TestStruct
{
  return {
    .a = 1,
    .b = 2
  };
}

TEST(Serialization, Test)
{
  using fl::serialization::format;

  TestStruct test = { .a = 1, .b = 2 };
  auto const expected = test;
  auto json = fl::serialization::serialize<format::json>(test);

  auto test2 = fl::serialization::deserialize<format::json, TestStruct>(json);
  ASSERT_EQ(expected.a, test2.a);
  ASSERT_EQ(expected.b, test2.b);
  ASSERT_EQ(json, R"({"a": 1, "b": 2})");
}