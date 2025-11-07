#include "./board.h"
#include "./timer.h"
#include "./window.h"
#include "./primitives.h"
#include "./tetrimino.h"


static tetromino* g_current_tetromino   = NULL;
static bool       g_game_over           = false;
static bool       g_tetromino_collision = false;
static bool       g_pause_timer         = false;

static void handle_key_event (key_type key, action_type action);

void update_speed(void)
{
    // increase the speed of the timer
    if (count_for_speed % 10 == 0)
        set_interval(*get_interval() - 0.1);
}

void create_tetromino(void) 
{
	// check if the current tetromino is in collision with the board
	if (!g_tetromino_collision) return;

	// free the current tetromino and set it to the board
    if (g_current_tetromino != NULL)
    {
        set_block_to_board(*g_current_tetromino);
        free(g_current_tetromino);
    }

	// create a new tetromino
    g_current_tetromino = new_rand_tetromino();
    g_tetromino_collision = false;
    update_speed();
}

static void handle_key_event (key_type const key, action_type const action) 
{
    if (g_current_tetromino == NULL) return;

    printf("Handle Key Event %s \n", string_from_key(key, action));
    
	if (action == action_press) return;
    
    switch (key)
    {
    case key_left:
        set_horizontal(&g_current_tetromino, -1);
        break;
    case key_right:
        set_horizontal(&g_current_tetromino, 1);
        break;
    case key_down:
        g_tetromino_collision = !set_vertical(&g_current_tetromino, -1);
        break;
    case key_up:
        set_vertical(&g_current_tetromino, 1);
        break;
    case key_esc:
        g_game_over = true;
        break;
    case key_control:
        rotate_block(&g_current_tetromino, clockwise);
        break;
    case key_alt:
		rotate_block(&g_current_tetromino, counterclockwise);
        break;
#if DEBUG
	case key_space:
        g_pause_timer = !g_pause_timer;
#endif
    default:
        return;
    }
}

void on_key(key_type const key, action_type const action) 
{
    if (key == key_up) return;

    handle_key_event(key, action);
}

void on_paint(void)
{
	// render board and tetromino and check for board full
	g_game_over = !render_tetromino_block(g_current_tetromino);
}

void on_timer(double const elapsed) 
{
	// only in debug mode the timer can be paused
    if (g_pause_timer) return;
    printf("Handle Time %s \n", string_from_duration(elapsed));
    handle_key_event(key_down, action_release);
}

// -------------------------------------------------------------------------------------------------

void dispose(void)
{
	// free the current tetromino
	if (g_current_tetromino != NULL)
		free(g_current_tetromino);
	// free the board
	dispose_board();
}

int main (void) 
{
	initialize_renderer (N_ROWS, N_COLS, on_key);
	initialize_timer (on_timer);

	// create new tetromino
    g_tetromino_collision = true;
	create_tetromino();

	do {        
		drive_timer ();
		begin_frame ();

		// create new tetromino if the current one is in collision
		create_tetromino();

		// render the board and the tetromino
		on_paint ();
		end_frame ();

		if (!renderer_is_open ())
            g_game_over = true;
	} while (!g_game_over);

	// free the current tetromino and dispose the board
	dispose();
	shut_down_renderer();
    printf("game over\n");
}
