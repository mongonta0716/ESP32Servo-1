/*
MIT License

Copyright (c) 2018 ShellAddicted <github.com/ShellAddicted>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "servoControl.h"

static const char *TAG = "[servoControl]";

double servoControl::getDutyByPercentage(double percentage){
	if (percentage <= 0){
		return 0;
	}
	if (percentage > 100){
		percentage = 100;
	}
	return (percentage / 100.0) * ((2<<(_timerResolution-1))-1);
}

double servoControl::getDutyByuS(double uS){
	return getDutyByPercentage(((uS * 100.0)/(1000000/_freqHz)));
}

void servoControl::attach(gpio_num_t pin, unsigned int minuS, unsigned int maxuS, ledc_channel_t ledcChannel, ledc_timer_t ledcTimer){
	_min = minuS;
	_max = maxuS;
	_ledcChannel = ledcChannel;
	_timerResolution = LEDC_TIMER_16_BIT;

	ledc_timer_config_t timer_conf;
	timer_conf.duty_resolution 	= _timerResolution;
	timer_conf.freq_hz    		= _freqHz;
	timer_conf.speed_mode 		= LEDC_HIGH_SPEED_MODE;
	timer_conf.timer_num  		= ledcTimer;
	timer_conf.clk_cfg        = LEDC_AUTO_CLK;
	ledc_timer_config(&timer_conf);

	ledc_channel_config_t ledc_conf;
	ledc_conf.channel		= _ledcChannel;
	ledc_conf.duty			= 0;
	ledc_conf.gpio_num		= (int)pin;
	ledc_conf.intr_type		= LEDC_INTR_DISABLE;
	ledc_conf.speed_mode	= LEDC_HIGH_SPEED_MODE;
	ledc_conf.timer_sel		= ledcTimer;
	ledc_conf.hpoint        = 0;
	ledc_channel_config(&ledc_conf);
}

void servoControl::detach(){
	ledc_stop(LEDC_HIGH_SPEED_MODE, _ledcChannel, 0);
}

void servoControl::writeMicroSeconds(unsigned int uS){
	ledc_set_duty(LEDC_HIGH_SPEED_MODE, _ledcChannel, getDutyByuS(uS));
	ledc_update_duty(LEDC_HIGH_SPEED_MODE, _ledcChannel);
}

void servoControl::write(unsigned int value) {
	// 0 = MinServoAngle ; 180 = Max ServoAngle;
	int scale = (value - 0) * (_max - _min) / (180 - 0) + _min;
	writeMicroSeconds(scale);
}

void servoControl::smoothMove(uint16_t start_degree, uint16_t stop_degree, uint16_t millis_for_move, uint16_t min_degree) {
	if (start_degree == stop_degree) return;
	setMinRotate(min_degree);
	int16_t degree_for_move = start_degree - stop_degree;
	uint16_t move_count = 0;
	bool clockwise = true;
    if (degree_for_move > _minRotate) {
		// 反時計回り
		move_count = degree_for_move / _minRotate;
		clockwise = false;
	} else if (degree_for_move < (_minRotate * -1)) {
		// 時計回り
		move_count = abs(degree_for_move) / _minRotate;
		clockwise = true;
	} else {
		// 最小回転角度以下なら動かない
		return;
	}

	uint16_t move_interval = millis_for_move / move_count;
	uint16_t next_degree = 0;
	for (int i=0; i<move_count; i++) {
		if (clockwise) {
			next_degree = start_degree + (i * min_degree);	
		} else {
			next_degree = start_degree - (i * min_degree);
		}

		write(next_degree);
		vTaskDelay(move_interval / portTICK_PERIOD_MS);
	}
}

