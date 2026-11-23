
#ifndef __PARA_H
#define __PARA_H

#include <stdint.h>

typedef struct
{
    float wheel_diameter;       // 轮子直径 mm
    float wheel_circumference;  // 轮子周长 mm
    float wheel_base;           // 左右轮中心距 mm

    float encoder_ppr;          // 编码器线数
    float gear_ratio;           // 减速比
    float distance_per_pulse;   // 单脉冲距离 mm

} RobotPara_t;

typedef struct
{
    float kp;
    float ki;
    float kd;

    int16_t max_error;
    int32_t max_integral_error;

    uint16_t initial_pwm;
    uint16_t pid_period_ms;

} MotorPara_t;

typedef struct
{
    int16_t max_differ_speed;

    int16_t max_straight_differ_speed;
    int16_t straight_shift_angle;
    int16_t max_adjust_angle;

    float astar_force_factor;
    float astar_normal_divisor;

    int16_t avoid_speed_step_add;
    int16_t avoid_speed_step_sub;
    int16_t avoid_speed_max;
    int16_t avoid_distance_threshold;

} MotionPara_t;

typedef struct
{
    uint16_t call_interval_ms;
    uint16_t max_turn_time_ms;
    uint16_t suspend_time_ms;

    uint16_t min_change_pulse;
    uint16_t slow_speed;

    uint16_t angle_tolerance;
    uint16_t angle_slowdown_range;

} TurnPara_t;


extern const RobotPara_t wheel_para;
extern const MotorPara_t motor_para;
extern const MotionPara_t motion_para;
extern const TurnPara_t turn_para;

#endif
