#include "gtest/gtest.h"

#include "examples.h"
#include "vm.h"
#include "parser.h"


TEST(VM,Examples) {
  int n = Example::size();
  for (int k=0; k<n; ++k) {
    Example::Ptr ex = Example::example(k);
    std::cout << ex->input << std::endl;
    VM vm;
    JSON expect = ex->ans;
    std::cout << expect << std::endl;
    auto result = vm.run(ex->ast);
    std::cout << result << std::endl;
    ASSERT_EQ(result,expect);
  }
}
