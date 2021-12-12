// Copyright 2021 Petrova Kseniya <ksyushki5@yandex.ru>

#include <gtest/gtest.h>
#include <shared_pointer.hpp>
#include <stdexcept>

TEST(constructor_tests, default_constructor) {
  shared_pointer<int> pointer;
  EXPECT_EQ(pointer.get(), nullptr);
}
TEST(constructor_tests, parameters_constructor)
{
  int *p = new int(9);
  shared_pointer<int> pointer(p);
  EXPECT_EQ(p, pointer.get());
  delete p;
}
TEST(constructor_tests, copy_constructor)
{
  int *p = new int(9);
  shared_pointer<int> pointer1(p);
  shared_pointer<int> pointer2(pointer1);
  EXPECT_EQ(pointer1, pointer2);
  EXPECT_EQ(pointer1.use_count(), 2);
  delete p;
}
TEST(constructor_tests, move_copy_tests)
{
  EXPECT_EQ(std::is_move_constructible<shared_pointer<int>>::value, true);
  EXPECT_EQ(std::is_copy_constructible<shared_pointer<int>>::value, true);
}
TEST(method_tests, get)
{
  int *p = new int(9);
  shared_pointer<int> pointer1(p);
  EXPECT_EQ(pointer1.get(), p);
  delete p;
}
TEST(operator_tests, operator1)
{
  int *p = new int(9);
  shared_pointer<int> pointer1(p);
  shared_pointer<int> pointer2 = pointer1;
  EXPECT_EQ(pointer1.get(), pointer2.get());
  delete p;
}
TEST(operator_tests, operator2)
{
  int *p = new int(9);
  shared_pointer<int> pointer1(p);
  bool p2 = pointer1;
  EXPECT_TRUE(p2);
  delete p;
}
TEST(operator_tests, operator3)
{
  int *p = new int(9);
  shared_pointer<int> pointer1(p);
  EXPECT_EQ(*pointer1, 9);
  delete p;
}
TEST(methods_tests, reset)
{
  int *p = new int(9);
  shared_pointer<int> pointer1(p);
  pointer1.reset();
  EXPECT_EQ(pointer1.get(), nullptr);
  delete p;
}
TEST(methods_tests, reset2)
{
  int *p1 = new int(9);
  int *p2 = new int(10);
  shared_pointer<int> pointer1(p1);
  pointer1.reset(p2);
  EXPECT_EQ(*(pointer1.get()), 10);
  delete p1;
  delete p2;
}
TEST(methods_tests, swap)
{
  int *p1 = new int(9);
  int *p2 = new int(10);
  shared_pointer<int> pointer1(p1);
  shared_pointer<int> pointer2(p2);
  pointer1.swap(pointer2);
  EXPECT_EQ(pointer1.get(), p2);
  EXPECT_EQ(pointer2.get(), p1);
  delete p1;
  delete p2;
}
TEST(methods_tests, use_count)
{
  int *p1 = new int(9);
  shared_pointer<int> pointer1(p1);
  EXPECT_EQ(pointer1.use_count(), 1);
  delete p1;
}
