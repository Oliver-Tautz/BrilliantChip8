
#include <gtest/gtest.h>

#include "brilliant_chip8/BrilliantChip8.hpp"
#include "brilliant_chip8/Renderer.hpp"

class BrilliantChip8OpcodeStepTest : public ::testing::Test {
   protected:
    BrilliantChip8 chip;

    void SetUp() override {
        std::vector<uint8_t> testProgram = {
            // 0x200:
            0x22, 0x0C,  // CALL 0x20C (subroutine)

            // 0x202:
            0x60, 0x01,  // LD V0, 0x01

            // 0x204:
            0x61, 0x02,  // LD V1, 0x02

            // 0x206:
            0x62, 0x03,  // LD V2, 0x03

            // 0x208:
            0xA3, 0x00,  // LD I, 0x300

            // 0x20A:
            0xF2, 0x55,  // LD [I], V0..V2

            // ---- SUBROUTINE at 0x20C ----
            0x60, 0x05,  // LD V0, 5 (digit)
            0xF0, 0x29,  // LD F, V0 (I = font address of digit in V0)
            0x61, 0x00,  // LD V1, 0 (y)
            0x60, 0x00,  // LD V0, 0 (x)
            0xD0, 0x15,  // DRW V0, V1, 5
            0x00, 0xEE   // RET
        };

        ASSERT_TRUE(chip.loadROM(testProgram));
    }

    void stepAndCheck(
        uint8_t stepNum,
        std::function<void(const BrilliantChip8::Chip8StateSnapshot &)>
            checker) {
        chip.emulateCycle();
        auto snapshot = chip.getStateSnapshot();
        checker(snapshot);
        EXPECT_EQ(snapshot.program_counter, 0x200 + (stepNum + 1) * 2);
    }
};

TEST_F(BrilliantChip8OpcodeStepTest, ExecutesWithSubroutineAndDrawsFont) {
    // Step 0: CALL 0x20C
    chip.emulateCycle();
    auto s0 = chip.getStateSnapshot();
    EXPECT_EQ(s0.stack_pointer, 1);  // (stack managed internally)
    EXPECT_EQ(s0.program_counter, 0x20C);

    // Step 1: LD V0, 5
    chip.emulateCycle();

    // Step 2: LD F, V0 => I = 0x50 + (5 * 5) = 0x6E
    chip.emulateCycle();
    auto s2 = chip.getStateSnapshot();
    EXPECT_EQ(s2.I, 0x50 + (5 * 5));  // 0x6E

    // Step 3: LD V1, 0
    chip.emulateCycle();

    // Step 4: LD V0, 0
    chip.emulateCycle();

    // Step 5: DRW V0, V1, 5 => draw font[5] at (0,0)
    chip.emulateCycle();
    auto drawState = chip.getStateSnapshot();
    EXPECT_TRUE(drawState.draw_flag);

    // Check first few pixels in top-left
    const auto &display = drawState.display;
    int pixelsOn = 0;
    for (int row = 0; row < 5; ++row)
        for (int col = 0; col < BrilliantChip8::CONST_DISPLAY_SIZE_X; ++col)
            pixelsOn += display[row][col];

    Renderer renderer = Renderer();
    BrilliantChip8::DisplayBuffer display2 = chip.getDisplay();
    renderer.render(display2);  // Render to console for visual check

    EXPECT_GT(pixelsOn, 0) << "Expected some pixels to be drawn";

    // Step 6: RET
    chip.emulateCycle();
    auto s6 = chip.getStateSnapshot();
    EXPECT_EQ(s6.program_counter,
              0x202);  // should return to instruction after CALL
}
