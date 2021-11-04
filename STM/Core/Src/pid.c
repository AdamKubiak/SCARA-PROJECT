/*
 * pid.c
 *
 *  Created on: Oct 18, 2021
 *      Author: john
 */

#include "pid.h"

#define ERR_SUM_MAX		255

struct pid_params
{
	float kp;
	float ki;
	float kd;
	float err;
	float err_sum;
	float err_last;
	int sampleTime;
};

static struct pid_params pid_params;
static struct pid_params pid_params1;

void pid_init(float kp, float ki, float kd)
{
	pid_params.kp = kp;
	pid_params.ki = ki;
	pid_params.kd = kd;
	pid_params.err = 0;
	pid_params.err_sum = 0;
	pid_params.err_last = 0;
	pid_params.sampleTime = 1;
}

float pid_calculate(float set_val, float read_val)
{
	float err_d, u;

	pid_params.err = set_val - read_val;
	pid_params.err_sum += pid_params.err;

	if (pid_params.err_sum > ERR_SUM_MAX) {
		pid_params.err_sum = ERR_SUM_MAX;
	} else if (pid_params.err_sum < -ERR_SUM_MAX) {
		pid_params.err_sum = -ERR_SUM_MAX;
	}

	err_d = (pid_params.err_last - pid_params.err)/pid_params.sampleTime;
	u = pid_params.kp * pid_params.err + pid_params.ki * pid_params.err_sum*pid_params.sampleTime
			+ pid_params.kd * err_d;

	//if(u<60 && u>-60) u = 0;

	return u;
}

void pid_init1(float kp, float ki, float kd)
{
	pid_params1.kp = kp;
	pid_params1.ki = ki;
	pid_params1.kd = kd;
	pid_params1.err = 0;
	pid_params1.err_sum = 0;
	pid_params1.err_last = 0;
	pid_params1.sampleTime = 1;
}

float pid_calculate1(float set_val, float read_val)
{
	float err_d, u;

	pid_params1.err = set_val - read_val;
	pid_params1.err_sum += pid_params1.err;

	if (pid_params1.err_sum > ERR_SUM_MAX) {
		pid_params1.err_sum = ERR_SUM_MAX;
	} else if (pid_params1.err_sum < -ERR_SUM_MAX) {
		pid_params1.err_sum = -ERR_SUM_MAX;
	}

	err_d = (pid_params1.err_last - pid_params1.err)/pid_params1.sampleTime;
	u = pid_params1.kp * pid_params1.err + pid_params1.ki * pid_params1.err_sum*pid_params1.sampleTime
			+ pid_params1.kd * err_d;

	//if(u<60 && u>-60) u = 0;

	return u;
}

