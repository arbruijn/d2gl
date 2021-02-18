#define SDL2
#include "SDL2/SDL.h"
#include "sdlkeymap.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "fix.h"
#include "effects.h"
#include "deslog.h"


long lastTicks;
static SDL_Texture *texture;
static SDL_Renderer *render;
static SDL_Window *window;

static int EventLoop();
extern void on_key(int, int);
extern int test_main(int argc, char **argv);

void error_set(char *msg, ...) {
	va_list vp;
	va_start(vp, msg);
	vprintf(msg, vp);
	va_end(vp);
}

extern void copy_screen_to_texture(uint32_t *texture);

void draw() {
    void *pixels;
    int pitch;
    if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0) {
            error_set("SDL_LockTexture %s", SDL_GetError());
            abort();
            return;
    }
    copy_screen_to_texture(pixels);
    SDL_UnlockTexture(texture);
    SDL_RenderCopy(render, texture, NULL, NULL);
    SDL_RenderPresent(render);
    EventLoop();
    long now;
    long timeout = lastTicks + 1000/60;
    while ((now = SDL_GetTicks()) < timeout) {
        EventLoop();
        SDL_Delay(1);
    }
    lastTicks = now;
}

static void *CreateTexture(int w, int h) {
	void *texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888,
									  SDL_TEXTUREACCESS_STREAMING, w, h);
	if (texture == NULL) {
		error_set("SDL_CreateTexture %s", SDL_GetError());
		return NULL;
	}
	return texture;
}

int sdl_init() {
    SDL_Init(SDL_INIT_EVERYTHING);
    if (SDL_CreateWindowAndRenderer(640, 480,
                                                                    SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN,
                                                                    &window, &render) != 0) {
            error_set("SDL_CreateWindow %s", SDL_GetError());
            return 1;
    }
    SDL_SetWindowTitle(window, "Test");
    return 0;
}

extern int cur_w, cur_h;
void video_set_res(int w, int h) {
    if (!cur_w)
        sdl_init();
    cur_w = w;
    cur_h = h;
    if (texture != NULL)
        SDL_DestroyTexture(texture);
    texture = CreateTexture(w, h);
}

static int EventLoop() {
        int keep_running = 1;
	SDL_Event evt;
	while (SDL_PollEvent(&evt) > 0) {
		switch (evt.type) {
		case SDL_QUIT:
			keep_running = 0;
			break;
		case SDL_WINDOWEVENT:
			switch (evt.window.type) {
			case SDL_WINDOWEVENT_CLOSE:
				keep_running = 0;
				break;
			case SDL_WINDOWEVENT_RESIZED:
				SDL_RenderCopy(render, texture, NULL, NULL);
				SDL_RenderPresent(render);
				break;
			}
			break;
		case SDL_KEYDOWN: {
			if (evt.key.keysym.sym == SDLK_RETURN &&
				(int)(evt.key.keysym.mod & KMOD_ALT) != 0) {
				if ((SDL_GetWindowFlags(window) &
					 (int)SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
					SDL_SetWindowFullscreen(window, 0);
				else
					SDL_SetWindowFullscreen(window,
											(int)SDL_WINDOW_FULLSCREEN_DESKTOP);
				break;
			}
			int pckey = sdlkeymap[(int)evt.key.keysym.scancode];
			on_key(pckey, 1);
			break;
		}
		case SDL_KEYUP: {
			int pckey2 = sdlkeymap[(int)evt.key.keysym.scancode];
			on_key(pckey2, 0);
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP: {
			void on_mousebutton(int btn, int down);
			on_mousebutton(evt.button.button - 1, evt.button.state == SDL_PRESSED);
			break;
		}
		case SDL_MOUSEMOTION: {
			void on_mousemove(int dx, int dy);
			on_mousemove(evt.motion.xrel, evt.motion.yrel);
			break;
		}
		}
	}
	return keep_running;
}

#include "timer.h"
#include "game.h"
#include "gameseq.h"
#include "kconfig.h"
int timer_get_fixed_seconds() { return fixmuldiv(SDL_GetTicks(), 65536, 1000); } 
void my_reset_time() { reset_time(); }
void my_start_player_death_sequence(object *obj) { start_player_death_sequence(obj); }
void game_render_frame();
void my_game_render_frame() { game_render_frame(); }
void my_controls_read_all() { controls_read_all(); }
void my_StartNewLevelSub(int level, int page_in, int secret) { StartNewLevelSub(level, page_in, secret); }

#include "timer.h"
#include "mouse.h"
#include "digi.h"
#include "mono.h"
#include "vga.h"
#include "joy.h"
#include "sdlkeymap.h"
int grd_fades_disabled;
void timer_init() { InitKeyMap(); grd_fades_disabled = 1; }
int mouse_init(int cyberman) { return 1; }
int digi_init() { return 0; }
int minit() { return -1; }
short vga_init() { return 0; }
int joy_init() { return 1; }
void mouse_show_cursor() { SDL_ShowCursor(SDL_ENABLE); printf("show_cursor\n"); }
void mouse_hide_cursor() { SDL_ShowCursor(SDL_DISABLE); printf("hide_cursor\n"); }

void mouse_get_pos(int *x, int *y) { SDL_GetMouseState(x, y); printf("pos %d,%d\n", *x, *y); }
