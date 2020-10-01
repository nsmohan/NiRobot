/** 
 *  @file      unittest_PCA9685.cc
 *  @brief     Unittests for PCA9685 Driver
 *  @details   Unittests for PWM Driver
 *  @author    Nitin Mohan
 *  @date      March 22, 2019
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/ 
#include <gtest/gtest.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"
#include "NMT_log.h"
#include "LD27MG_stub.h"
#include "PCA9685_stub.h"
#include "RMCT_lib.hpp"

/* @class MyEnvironment
 *  Environment Setup for Test */
class MyEnvironment: public ::testing::Environment
{
public:
  virtual ~MyEnvironment() = default;

  virtual void SetUp() {NMT_log_init((char *)"/tmp/", false);}

  virtual void TearDown() {NMT_log_finish();}
};

/* @class PCA9685_Test_Fixture
 * Test Fixture for unittests */
class RMCT_lib_Test_Fixture : public ::testing::Test
{
    public:
        RMCT_hw_settings rmct_hw_settings = {0};

    RMCT_lib_Test_Fixture()
    {
        rmct_hw_settings.cam_mtr_step_size = 10;
        rmct_hw_settings.default_drive_speed = 50;

        /* PCA9685 Driver Config */
        strcpy(rmct_hw_settings.pca9685_hw_config.hw_name, "PCA9685_PWM_DRIVER");
        rmct_hw_settings.pca9685_hw_config.hw_sim_mode = true;

        /* Settings for Left Drive Motor */
        rmct_hw_settings.left_motor_hw_config = {0};
        strcpy(rmct_hw_settings.left_motor_hw_config.hw_name, "LEFT_DRV_MOTOR");
        rmct_hw_settings.left_motor_hw_config.hw_sim_mode = true;
        rmct_hw_settings.left_motor_hw_config.hw_interface = (RSXA_pins *)malloc(sizeof(RSXA_pins) * 2);
        strcpy(rmct_hw_settings.left_motor_hw_config.hw_interface[0].pin_name, "forward");
        strcpy(rmct_hw_settings.left_motor_hw_config.hw_interface[1].pin_name, "reverse");
        rmct_hw_settings.left_motor_hw_config.hw_interface[0].pin_no = 1;
        rmct_hw_settings.left_motor_hw_config.hw_interface[1].pin_no = 2;

        /* Settings for Right Drive Motor */
        rmct_hw_settings.right_motor_hw_config = {0};
        strcpy(rmct_hw_settings.left_motor_hw_config.hw_name, "RIGHT_DRV_MTR");
        rmct_hw_settings.right_motor_hw_config.hw_sim_mode = true;
        rmct_hw_settings.right_motor_hw_config.hw_interface = (RSXA_pins *)malloc(sizeof(RSXA_pins) * 2);
        strcpy(rmct_hw_settings.right_motor_hw_config.hw_interface[0].pin_name, "forward");
        strcpy(rmct_hw_settings.right_motor_hw_config.hw_interface[1].pin_name, "reverse");
        rmct_hw_settings.right_motor_hw_config.hw_interface[0].pin_no = 1;
        rmct_hw_settings.right_motor_hw_config.hw_interface[1].pin_no = 2;

       rmct_hw_settings.cam_motor_hw_config = {0};
       strcpy(rmct_hw_settings.cam_motor_hw_config.hw_name, "CAMERA_MOTORS");
       rmct_hw_settings.cam_motor_hw_config.hw_sim_mode = false;
       rmct_hw_settings.cam_motor_hw_config.hw_interface = (RSXA_pins *)malloc(sizeof(RSXA_pins) * 2);
       strcpy(rmct_hw_settings.cam_motor_hw_config.hw_interface[0].pin_name, "CAM_HRZN_MTR");
       strcpy(rmct_hw_settings.cam_motor_hw_config.hw_interface[1].pin_name, "CAM_VERT_MTR");
       rmct_hw_settings.cam_motor_hw_config.hw_interface[0].pin_no = 1;
       rmct_hw_settings.cam_motor_hw_config.hw_interface[1].pin_no = 2;
    }
};

/* ---- Start of Tests -------------*/
using namespace testing;
TEST_F(RMCT_lib_Test_Fixture, VerifyConstructorGW)
{
   /*!
    *  @test Verify RMCT Constructor
    *  Verify PCA9685 and LD27MG Initialize
    */
    LD27MGMocker ld27mgmock;
    PCA9685Mocker pca9685mock;
    
    /* Set Expectations */
    EXPECT_CALL(ld27mgmock, LD27MG_init(_)).Times(1);
    EXPECT_CALL(pca9685mock, PCA9685_init(_)).Times(1);

    /* Perform Action */
    RobotMotorController  obj(rmct_hw_settings);   
}

TEST_F(RMCT_lib_Test_Fixture, VerifyConstructorBW1)
{
   /*!
    *  @test Verify RMCT Constructor
    *  Verify Result is NOK if PCA9685 Init Fails
    */
    LD27MGMocker ld27mgmock;
    PCA9685Mocker pca9685mock;
    
    /* Set Expectations */
    EXPECT_CALL(pca9685mock, PCA9685_init(_)).Times(1)
        .WillOnce(Return(NOK));
    EXPECT_CALL(ld27mgmock, LD27MG_init(_)).Times(0);

    /* Perform Action */
    try
    {
        RobotMotorController  obj(rmct_hw_settings);   
    }
    catch (std::exception &e)
    {
        SUCCEED();
    }
}

TEST_F(RMCT_lib_Test_Fixture, VerifyConstructorBW2)
{
   /*!
    *  @test Verify RMCT Constructor
    *  Verify Result is NOK if LD27MG_init fails
    */
    LD27MGMocker ld27mgmock;
    PCA9685Mocker pca9685mock;
    
    /* Set Expectations */
    EXPECT_CALL(pca9685mock, PCA9685_init(_)).Times(1);
    EXPECT_CALL(ld27mgmock, LD27MG_init(_)).Times(1)
        .WillOnce(Return(NOK));

    /* Perform Action */
    try
    {
        RobotMotorController  obj(rmct_hw_settings);   
    }
    catch (std::exception &e)
    {
        SUCCEED();
    }
}

TEST_F(RMCT_lib_Test_Fixture, VerifyMoveCameraGW1)
{
   /*!
    *  @test Verify Camera Motor Actions
    *  motor=CAMERA
    *  direction=UP
    *  angle=0.00
    *  speed=0.00
    */
    LD27MGMocker ld27mgmock;
    double ret_angle = 10.00;

    double angle_to_move = ret_angle + rmct_hw_settings.cam_mtr_step_size;
    
    /* Set Expectations */
    EXPECT_CALL(ld27mgmock, LD27MG_init(_)).Times(1);
    EXPECT_CALL(ld27mgmock, LD27MG_get_current_position(CAM_VERT_MTR, _)).Times(1)
        .WillOnce(DoAll(SetArgPointee<1>(ret_angle), Return(OK)));
    EXPECT_CALL(ld27mgmock, LD27MG_move_motor(CAM_VERT_MTR, angle_to_move)).Times(1);

    /* Initialize */
    RobotMotorController  obj(rmct_hw_settings);   

    /* Perform Action */
    obj.process_motor_action("CAMERA", "UP", 0, 0);
}

TEST_F(RMCT_lib_Test_Fixture, VerifyMoveCameraGW2)
{
   /*!
    *  @test Verify Camera Motor Actions
    *  motor=CAMERA
    *  direction=DOWN
    *  angle=0.00
    *  speed=0.00
    */
    LD27MGMocker ld27mgmock;
    double ret_angle = 10.00;

    double angle_to_move = ret_angle - rmct_hw_settings.cam_mtr_step_size;
    
    /* Set Expectations */
    EXPECT_CALL(ld27mgmock, LD27MG_init(_)).Times(1);
    EXPECT_CALL(ld27mgmock, LD27MG_get_current_position(CAM_VERT_MTR, _)).Times(1)
        .WillOnce(DoAll(SetArgPointee<1>(ret_angle), Return(OK)));
    EXPECT_CALL(ld27mgmock, LD27MG_move_motor(CAM_VERT_MTR, angle_to_move)).Times(1);

    /* Initialize */
    RobotMotorController  obj(rmct_hw_settings);   

    /* Perform Action */
    obj.process_motor_action("CAMERA", "DOWN", 0, 0);
}

TEST_F(RMCT_lib_Test_Fixture, VerifyMoveCameraGW3)
{
   /*!
    *  @test Verify Camera Motor Actions
    *  motor=CAMERA
    *  direction=LEFT
    *  angle=0.00
    *  speed=0.00
    */
    LD27MGMocker ld27mgmock;
    double ret_angle = 10.00;

    double angle_to_move = ret_angle + rmct_hw_settings.cam_mtr_step_size;
    
    /* Set Expectations */
    EXPECT_CALL(ld27mgmock, LD27MG_init(_)).Times(1);
    EXPECT_CALL(ld27mgmock, LD27MG_get_current_position(CAM_HRZN_MTR, _)).Times(1)
        .WillOnce(DoAll(SetArgPointee<1>(ret_angle), Return(OK)));
    EXPECT_CALL(ld27mgmock, LD27MG_move_motor(CAM_HRZN_MTR, angle_to_move)).Times(1);

    /* Initialize */
    RobotMotorController  obj(rmct_hw_settings);   

    /* Perform Action */
    obj.process_motor_action("CAMERA", "LEFT", 0, 0);
}

TEST_F(RMCT_lib_Test_Fixture, VerifyMoveCameraGW4)
{
   /*!
    *  @test Verify Camera Motor Actions
    *  motor=CAMERA
    *  direction=RIGHT
    *  angle=0.00
    *  speed=0.00
    */
    LD27MGMocker ld27mgmock;
    double ret_angle = 10.00;

    double angle_to_move = ret_angle - rmct_hw_settings.cam_mtr_step_size;
    
    /* Set Expectations */
    EXPECT_CALL(ld27mgmock, LD27MG_init(_)).Times(1);
    EXPECT_CALL(ld27mgmock, LD27MG_get_current_position(CAM_HRZN_MTR, _)).Times(1)
        .WillOnce(DoAll(SetArgPointee<1>(ret_angle), Return(OK)));
    EXPECT_CALL(ld27mgmock, LD27MG_move_motor(CAM_HRZN_MTR, angle_to_move)).Times(1);

    /* Initialize */
    RobotMotorController  obj(rmct_hw_settings);   

    /* Perform Action */
    obj.process_motor_action("CAMERA", "RIGHT", 0, 0);
}

TEST_F(RMCT_lib_Test_Fixture, VerifyMoveCameraGW5)
{
   /*!
    *  @test Verify Camera Motor Actions
    *  motor=CAM_HRZN_MTR
    *  direction=""
    *  angle=20;
    *  speed=0.00
    */
    LD27MGMocker ld27mgmock;
    double angle_to_move = 20.00;

    /* Set Expectations */
    EXPECT_CALL(ld27mgmock, LD27MG_init(_)).Times(1);
    EXPECT_CALL(ld27mgmock, LD27MG_get_current_position(_, _)).Times(0);
    EXPECT_CALL(ld27mgmock, LD27MG_move_motor(CAM_HRZN_MTR, angle_to_move)).Times(1);

    /* Initialize */
    RobotMotorController  obj(rmct_hw_settings);   

    /* Perform Action */
    obj.process_motor_action("CAM_HRZN_MTR", "", angle_to_move, 0);
}

TEST_F(RMCT_lib_Test_Fixture, VerifyMoveCameraGW6)
{
   /*!
    *  @test Verify Camera Motor Actions
    *  motor=CAM_VERT_MTR
    *  direction=""
    *  angle=20;
    *  speed=0.00
    */
    LD27MGMocker ld27mgmock;
    double angle_to_move = 20.00;

    /* Set Expectations */
    EXPECT_CALL(ld27mgmock, LD27MG_init(_)).Times(1);
    EXPECT_CALL(ld27mgmock, LD27MG_get_current_position(_, _)).Times(0);
    EXPECT_CALL(ld27mgmock, LD27MG_move_motor(CAM_VERT_MTR, angle_to_move)).Times(1);

    /* Initialize */
    RobotMotorController  obj(rmct_hw_settings);   

    /* Perform Action */
    obj.process_motor_action("CAM_VERT_MTR", "", angle_to_move, 0);
}

TEST_F(RMCT_lib_Test_Fixture, VerifyMoveCameraGW7)
{
   /*!
    *  @test Verify Camera Motor Actions
    *  motor=CAM_VERT_MTR
    *  direction=UP
    *  angle=20.00
    *  speed=0.00
    *  **** Direction Should be ignord ****
    */

    
    LD27MGMocker ld27mgmock;
    double angle_to_move = 20.00;
    
    /* Set Expectations */
    EXPECT_CALL(ld27mgmock, LD27MG_init(_)).Times(1);
    EXPECT_CALL(ld27mgmock, LD27MG_get_current_position(_, _)).Times(0);
    EXPECT_CALL(ld27mgmock, LD27MG_move_motor(CAM_VERT_MTR, angle_to_move)).Times(1);

    /* Initialize */
    RobotMotorController  obj(rmct_hw_settings);   

    /* Perform Action */
    obj.process_motor_action("CAM_VERT_MTR", "UP", 20.00, 0);
}

TEST_F(RMCT_lib_Test_Fixture, VerifyMoveCameraGW8)
{
   /*!
    *  @test Verify Camera Motor Actions
    *  motor=LEFT_DRV_MTR
    *  direction=FORWARD
    *  angle=0.00
    *  speed=0.00
    */

    LD27MGMocker ld27mgmock;
    PCA9685Mocker pwmstub;

    rmct_hw_settings.left_motor_hw_config.hw_sim_mode = false;
    rmct_hw_settings.right_motor_hw_config.hw_sim_mode = false;
    
    /* Set Expectations */
    EXPECT_CALL(ld27mgmock, LD27MG_init(_)).Times(1);
    EXPECT_CALL(ld27mgmock, LD27MG_get_current_position(_, _)).Times(0);
    EXPECT_CALL(ld27mgmock, LD27MG_move_motor(_, _)).Times(0);
    EXPECT_CALL(pwmstub, PCA9685_init(_)).Times(AtLeast(1));
    EXPECT_CALL(pwmstub, PCA9685_setPWM(_, _, _)).Times(AtLeast(1));

    /* Initialize */
    RobotMotorController obj(rmct_hw_settings);   
    
    EXPECT_CALL(pwmstub, PCA9685_setPWM(_, _, _)).Times(2);
    obj.process_motor_action("LEFT_DRV_MTR", "FORWARD", 0, 0);
}

TEST_F(RMCT_lib_Test_Fixture, VerifyMoveCameraGW9)
{
   /*!
    *  @test Verify Camera Motor Actions
    *  motor=RIGHT_DRV_MTR
    *  direction=REVERSE
    *  angle=0.00
    *  speed=80
    */

    LD27MGMocker ld27mgmock;
    PCA9685Mocker pwmstub;

    rmct_hw_settings.left_motor_hw_config.hw_sim_mode = false;
    rmct_hw_settings.right_motor_hw_config.hw_sim_mode = false;
    
    /* Set Expectations */
    EXPECT_CALL(ld27mgmock, LD27MG_init(_)).Times(1);
    EXPECT_CALL(ld27mgmock, LD27MG_get_current_position(_, _)).Times(0);
    EXPECT_CALL(ld27mgmock, LD27MG_move_motor(_, _)).Times(0);
    EXPECT_CALL(pwmstub, PCA9685_init(_)).Times(AtLeast(1));
    EXPECT_CALL(pwmstub, PCA9685_setPWM(_, _, _)).Times(AtLeast(1));

    /* Initialize */
    RobotMotorController obj(rmct_hw_settings);   
    
    EXPECT_CALL(pwmstub, PCA9685_setPWM(_, _, _)).Times(2);
    obj.process_motor_action("RIGHT_DRV_MTR", "REVERSE", 0, 80);
}
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    MyEnvironment* env = new MyEnvironment(); 
    ::testing::AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}
