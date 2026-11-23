
#include "para.h"

const RobotPara_t wheel_para =
{
    .wheel_diameter = 70.0f,
    .wheel_circumference = 219.8f,
    .wheel_base = 218.0f,

    .encoder_ppr = 24.0f,
    .gear_ratio = 46.85227f,
    .distance_per_pulse = 0.19557f,
};


const MotorPara_t motor_para =
{
    .kp = 15.0f,
    .ki = 2.0f,
    .kd = 5.0f,

    .max_error = 15,
    .max_integral_error = 100,

    .initial_pwm = 900,
    .pid_period_ms = 20,
};


const MotionPara_t motion_para =
{
    .max_differ_speed = 100,

    .max_straight_differ_speed = 70,
    .straight_shift_angle = 2,
    .max_adjust_angle = 20,

    .astar_force_factor = 1.5f,
    .astar_normal_divisor = 15.0f,

    .avoid_speed_step_add = 20,
    .avoid_speed_step_sub = 5,
    .avoid_speed_max = 50,
    .avoid_distance_threshold = 4,
};


const TurnPara_t turn_para =
{
    .call_interval_ms = 5,
    .max_turn_time_ms = 3500,
    .suspend_time_ms = 50,

    .min_change_pulse = 100,
    .slow_speed = 60,

    .angle_tolerance = 40,
    .angle_slowdown_range = 450,
};
