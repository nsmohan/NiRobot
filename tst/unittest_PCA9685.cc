#include <gtest/gtest.h>
#include "PCA9685.h"
#include "wiringPi_stub.h"
#include "NMT_stdlib.h"
#include "NMT_log.h"

TEST(InitPCA9685, TestResultGW)
{
   PCA9685_settings hw_settings = {0};
   NMT_result result = OK;
   bool sim_mode = true;
   hw_settings.freq = 10; 
   NMT_log_init((char *)"/var/log/NiRobot", false);

   std::cout << "I am here" << std::endl;
   ASSERT_EQ(result, PCA9685_init(&hw_settings, sim_mode));

}
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
