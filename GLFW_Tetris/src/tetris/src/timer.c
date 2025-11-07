#include "./timer.h"

#pragma region Macros

#define SLOW_SPEED 5.0
#define HIGH_SPEED 0.05

#pragma endregion

// -------------------------------------------------------------------------------------------------

static on_timer_type * g_on_timer = NULL;   // the user's event handler for events from the timer

// -------------------------------------------------------------------------------------------------

void drive_timer (void) {
	if ((glfwGetTime () >= *get_interval ()) && g_on_timer) {
		g_on_timer (glfwGetTime ()); reset_timer ();
	}	
}

double * get_interval (void) {
	static double interval = 1; return &interval;
}

void initialize_timer (on_timer_type * const on_timer) {
	g_on_timer = on_timer; reset_timer ();
}

void reset_timer (void) {
	glfwSetTime (0);
}

void set_interval (double const interval) {
	// set speed of the timer
	*get_interval () = interval >= HIGH_SPEED ? (interval <= SLOW_SPEED ? interval : SLOW_SPEED) : HIGH_SPEED;
}
