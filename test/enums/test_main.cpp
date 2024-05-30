#include <sstream>

#include "io_Foo.hpp"

#include "ICGTestFixture.hpp"


TEST_F(ICGTest, example_restore) {
    // ARRANGE
    
    Foonum foo_enum;
    
    foo_enum = Foonum::THREE;
    memoryManager.declare_var("Foonum foo_enum;", &foo_enum);


    memoryManager.write_checkpoint("checkpoint.txt");

    // ACT
    memoryManager.restore_checkpoint("checkpoint.txt");

    // ASSERT
    EXPECT_EQ(foo_enum, 2);
}

