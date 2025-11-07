#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#pragma region Macros

#undef  UNUSED
#define UNUSED(v) ((void) (v))

#define DEBUG false

#pragma endregion

#pragma region Types

typedef struct position_type {
   int x;
   int y;
} position_type;

typedef enum {
   action_unknown,
   action_press,
   action_release,
   action_repeat,
} action_type;

typedef enum {
   key_unknown,
   key_down,
   key_left,
   key_right,
   key_up,
   key_esc,
   key_control,
   key_alt,
   key_space, // Only for debug
} key_type;

typedef void on_key_type   (key_type, action_type);   // the user's event handler for events from the keyboard
typedef void on_timer_type (double elapsed);          // the user's event handler for events from the timer

#pragma endregion

position_type make_position (int x, int y);
bool set_position(position_type* position, int const dx, int const dy);

action_type action_type_from_glfw_action (int glfw_action);
key_type    key_type_from_glfw_key       (int glfw_key);

char const * string_from_duration (double duration);
char const * string_from_key      (key_type key, action_type action);

void signal_last_gl_error   (bool force_abort);
void signal_last_glad_error (bool force_abort);
void signal_last_glfw_error (bool force_abort);
void signal_last_ogl_error  (GLuint shader, bool force_abort);
