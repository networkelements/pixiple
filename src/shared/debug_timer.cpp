#include "debug_timer.h"

#include "assert.h"

#include <Windows.h>

const int n_debug_timers = 8;
static long long debug_timer_value[n_debug_timers];

void debug_timer_reset(const int index) {
	assert(index >= 0 && index < n_debug_timers);

	QueryPerformanceCounter((LARGE_INTEGER*)&debug_timer_value[index]);
}

float debug_timer(const int index, const int n_samples) {
	const int max_samples = 100;

	assert(index >= 0 && index < n_debug_timers);
	assert(n_samples > 0 && n_samples <= max_samples);

	long long counts_per_second;
	QueryPerformanceFrequency((LARGE_INTEGER*)&counts_per_second);
	long long count;
	QueryPerformanceCounter((LARGE_INTEGER*)&count);

	float time = float(count - debug_timer_value[index]) / (float)counts_per_second;

	static float times[n_debug_timers][max_samples];
	static int sample_index[n_debug_timers];
	times[index][sample_index[index]++] = time;

	static float average[n_debug_timers];

	if (sample_index[index] == n_samples) {
		average[index] = 0;
		for (int i = 0; i < n_samples; i++)
			average[index] += times[index][i];
		average[index] /= n_samples;

		sample_index[index] = 0;
	}

	return average[index];
}
