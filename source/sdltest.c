//#include "sdl2/include/SDL.h"
#define SDL2
#include "SDL2/SDL.h"
#include "sdlkeymap.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "fix.h"


//#define SDL_Window SDL_Surface

long lastTicks;
#ifdef SDL2
static SDL_Texture *texture;
static SDL_Renderer *render;
#endif
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

int last_time;
int sdl_time_start;
int time_speed = 1;
int play_speed = 8;
int play_show = 0;

FILE *log_file = NULL;
int is_record = 1;
int is_play = 0;

void draw() {
    if (is_play && !play_show)
        return;
    void *pixels;
    #ifdef SDL2
    int pitch;
    if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0) {
    #else
    if (SDL_LockSurface(window) != 0) {
    #endif
            error_set("SDL_LockSurface %s", SDL_GetError());
            abort();
            return;
    }
    #ifndef SDL2
    pixels = window->pixels;
    #endif
    copy_screen_to_texture(pixels);
    #ifdef SDL2
    SDL_UnlockTexture(texture);
    SDL_RenderCopy(render, texture, NULL, NULL);
    SDL_RenderPresent(render);
    #else
    SDL_UnlockSurface(window);
    SDL_UpdateRect(window, 0, 0, 0, 0);
    #endif
    EventLoop();
    if (0) {
            long now = SDL_GetTicks();
            if (lastTicks != 0 && now - lastTicks < 10) {
                    SDL_Delay(10 - (Uint32)(now - lastTicks));
                    now = SDL_GetTicks();
            }
            lastTicks = now;
    }
    int timeout = sdl_time_start + fixmuldiv(last_time, 1000, 65536 * time_speed);
    while (SDL_GetTicks() < timeout) {
        EventLoop();
        SDL_Delay(1);
    }
    return;
}

void sdl_time_sync() {
    sdl_time_start = SDL_GetTicks() - fixmuldiv(last_time, 1000, 65536 * time_speed);
}

#ifdef SDL2
static void *CreateTexture(int w, int h) {
	void *texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888,
									  SDL_TEXTUREACCESS_STREAMING, w, h);
	if (texture == NULL) {
		error_set("SDL_CreateTexture %s", SDL_GetError());
		return NULL;
	}
	return texture;
}
#endif	

int sdl_init() {
    if (is_play && !play_show)
        return;

    SDL_Init(SDL_INIT_EVERYTHING);
    #ifdef SDL2
    if (SDL_CreateWindowAndRenderer(640, 480,
                                                                    SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN,
                                                                    &window, &render) != 0) {
            error_set("SDL_CreateWindow %s", SDL_GetError());
            return 1;
    }
    SDL_SetWindowTitle(window, "Test");
    //SDL_RenderCopy(render, texture, NULL, NULL);
    //SDL_RenderPresent(render);
    #else
    if (!(window = SDL_SetVideoMode(640, 480, 32, 0))) {
            error_set("SDL_SetVideoMode %s", SDL_GetError());
            return 1;
    }
    SDL_WM_SetCaption("Test", NULL);
    #endif
    // SDL_UpdateWindowSurface(window);
    return 0;
}

extern int cur_w, cur_h;
void video_set_res(int w, int h) {
    if (!cur_w)
        sdl_init();
    cur_w = w;
    cur_h = h;
    if (is_play && !play_show)
        return;
#ifdef SDL2
    if (texture != NULL)
        SDL_DestroyTexture(texture);
    texture = CreateTexture(w, h);
#endif
}

static int EventLoop() {
	int keep_running = 1;
	// while (keep_running)
	//{
	/*
	if (SDL_WaitEvent(out SDL_Event wevt) != 1)
		throw new Exception("SDL_WaitEvent: " + SDL_GetError());
	SDL_PushEvent(ref wevt);
	*/
	SDL_Event evt;
	while (SDL_PollEvent(&evt) > 0) {
		switch (evt.type) {
		case SDL_QUIT:
			keep_running = 0;
			break;
		#ifdef SDL2
		case SDL_WINDOWEVENT:
			switch (evt.window.type) {
			case SDL_WINDOWEVENT_CLOSE:
				keep_running = 0;
				break;
			#ifdef SDL2
			case SDL_WINDOWEVENT_RESIZED:
				SDL_RenderCopy(render, texture, NULL, NULL);
				SDL_RenderPresent(render);
				break;
			#endif
			}
			break;
		#endif
		case SDL_KEYDOWN: {
			#ifdef SDL2
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
			#endif
			int pckey = sdlkeymap[(int)evt.key.keysym.scancode];
			on_key(pckey, 1);
			break;
		}
		case SDL_KEYUP: {
			int pckey2 = sdlkeymap[(int)evt.key.keysym.scancode];
			on_key(pckey2, 0);
			break;
    }
		}
	}
  return keep_running;
}

#include "types.h"
#include "text.h"
#include "mono.h"
#include "inferno.h"
#include "titles.h"
#include "gr.h"
#include "piggy.h"
#include "game.h"
#include "gameseq.h"
#include "3d.h"
#include "menu.h"
#include "bm.h"
#include "gamefont.h"
#include "texmap.h"
#include "texmerge.h"
#include "screens.h"
#include "palette.h"

extern int modes[][2];
//#include "modes.h"

#define call_reg0(x) x()
#define call_reg1(x, a) x(a);
#define call_reg2(x, a, b) x(a, b);
#define call_reg3(x, a, b, c) x(a, b, c);
#define call_reg4(x, a, b, c, d) x(a, b, c, d);
#define call_reg5(x, a, b, c, d, e) x(a, b, c, d, e);
#define call_reg6(x, a, b, c, d, e, f) x(a, b, c, d, e, f);

#include "player.h"
#include "ai.h"
#include "fuelcen.h"
#if 0
extern char Ai_cloak_info[8 * 16];
extern int Last_gate_time;
#endif
player *Player = &Players[0];
int *pDifficulty_level = &Difficulty_level;
int *pGameTime = &GameTime;
int *pCurrent_level_num = &Current_level_num;
void *pAi_local_info = &Ai_local_info;
void *pAi_cloak_info = &Ai_cloak_info;
int *pLast_gate_time = &Last_gate_time;
int *pLast_teleport_time = &Last_teleport_time;
int *pBoss_cloak_end_time = &Boss_cloak_end_time;
int Fuelcen_seconds_left;
int *pFuelcen_seconds_left = &Fuelcen_seconds_left;
extern unsigned int d_rand_seed;
unsigned int *pRandNext = &d_rand_seed;
int Num_awareness_events;
int *pNum_awareness_events = &Num_awareness_events;

int level = 1;
void load_stat(FILE *f) {
    char line[256], *p, *arg;
    if (!fgets(line, sizeof(line), f))
        line[0] = 0;
    if ((p = strchr(line, '\n')))
        *p = 0;
    if (strcmp(line, "!newlevel") != 0) {
        fprintf(stderr, "missing !newlevel on new level: %s\n", line);
        abort();
    }
    //*pLastGateTime = *pLastTeleportTime = *pBossCloakEndTime = 0;

    while (fgets(line, sizeof(line), f) && line[0] != '!') {
        if ((p = strchr(line, '\n')))
            *p = 0;
        if (!(arg = strchr(line, '=')))
            continue;
        *arg++ = 0;
        if (strcmp(line, "level") == 0)
            level = (int)strtol(arg, NULL, 10);
        if (strcmp(line, "seed") == 0)
            *pRandNext = (int)strtoul(arg, NULL, 16);
        if (strcmp(line, "time") == 0)
            *pGameTime = (int)strtoul(arg, NULL, 16);
        if (strcmp(line, "framecount") == 0)
            FrameCount = (int)strtol(arg, NULL, 10);
        if (strcmp(line, "flags") == 0)
            Player->flags = (int)strtoul(arg, NULL, 16);
        if (strcmp(line, "energy") == 0)
            Player->energy = (int)strtoul(arg, NULL, 16);
        if (strcmp(line, "shields") == 0)
            Player->shields = (int)strtoul(arg, NULL, 16);
        if (strcmp(line, "lives") == 0)
            Player->lives = (int)strtol(arg, NULL, 10);
        if (strcmp(line, "primary_flags") == 0)
            Player->primary_weapon_flags = (int)strtoul(arg, NULL, 16);
        if (strcmp(line, "secondary_flags") == 0)
            Player->secondary_weapon_flags = (int)strtoul(arg, NULL, 16);
        if (strcmp(line, "score") == 0)
            Player->score = (int)strtol(arg, NULL, 10);
        if (strcmp(line, "time_total") == 0)
            Player->time_total = (int)strtoul(arg, NULL, 16);
        if (strcmp(line, "num_kills_total") == 0)
            Player->num_kills_total = (int)strtol(arg, NULL, 10);
        if (strcmp(line, "hostages_rescued_total") == 0)
            Player->hostages_rescued_total = (int)strtol(arg, NULL, 10);
        if (strcmp(line, "hours_total") == 0)
            Player->hours_total = (int)strtol(arg, NULL, 10);
        if (strcmp(line, "primary_weapon") == 0)
            Primary_weapon = (char)strtol(arg, NULL, 10);
        if (strcmp(line, "secondary_weapon") == 0)
            Secondary_weapon = (char)strtol(arg, NULL, 10);
    }
    if ((p = strchr(line, '\n')))
        *p = 0;
    if (strcmp(line, "!game") != 0) {
        fprintf(stderr, "missing !game on new level: %s\n", line);
        abort();
    }
}

void my_StartNewLevelSub(int arg_level, int flag, int secret) {
    level = arg_level;
    if (is_play)
        load_stat(log_file);

    // clear AI globals
    memset(pAi_local_info, 0, 184 * 350);
    memset(pAi_cloak_info, 0, 8 * 16);
    *pLast_gate_time = *pLast_teleport_time = *pBoss_cloak_end_time = 0;
    *pNum_awareness_events = 0;
    
    *pFuelcen_seconds_left = 0;

    call_reg3(StartNewLevelSub, level, flag, secret);
}


void init_data_files();
int test_main2(int argc, char **argv) {
	init_data_files();
	
	cfile_init("descent2.hog");
	load_text();
	Lighting_on = 1;

	#define SM_320x200C 0
	#define SM_640x480V 14
	#define VR_NONE 0
	Cockpit_mode = CM_FULL_SCREEN;
	int screen_mode = !is_play || play_show ? SM_640x480V : SM_320x200C;
	int screen_width = modes[screen_mode][0];
	int screen_height = modes[screen_mode][1];
	int vr_mode = VR_NONE;
	int screen_compatible = 0;
	int use_double_buffer = 0;
	call_reg5(game_init_render_buffers, screen_mode, screen_width, screen_height, 0, 0); //use_double_buffer, vr_mode, screen_compatible );

	int ret = call_reg0(gr_init);
	(void)ret;
	//printf("gr_init=%d\n", ret);
	//printf("cur canv %p data=%08x\n", *cur_canvas, (*cur_canvas)[3]);
	ret = call_reg1(gr_set_mode, screen_mode);
	//printf("gr_set_mode=%d\n", ret);
	//printf("cur canv %p data=%08x\n", *cur_canvas, (*cur_canvas)[3]);
	//(*cur_canvas)[3] = (int)vga_screen;
	//goto err;
	call_reg1(gr_use_palette_table, "default.256");
	gamefont_init();
	bm_init();

	//call_reg2(show_title_screen, (int)"iplogo1.pcx", 1);
	//call_reg2(show_title_screen, (int)"logo.pcx", 1);
	#if 0
	char title_pal[768];
	call_reg4(pcx_read_bitmap, (int)"descent.pcx",
	    (int)*cur_canvas, ((char *)*cur_canvas)[5], (int)&title_pal);
	call_reg3(gr_palette_fade_in, (int)&title_pal, 32, 0);
	#endif

	g3_init();
	call_reg1(texmerge_init, 10);
	call_reg1(set_screen_mode, 0);
	//call_reg1(gr_palette_load, (int)gr_palette);
	init_game();
	call_reg1(set_detail_level_parameters, 4);
	//set_detail_level_parameters(4);
	//RegisterPlayer();
	call_reg1(load_mission, 0);
	Function_mode = 2; // FMODE_GAME
	#ifndef Skip_briefing_screens
	Skip_briefing_screens = 1;
	#endif
	init_player_stats_game();


	#if 0
	if (argc <= 1) {
	    is_record = 1;
	    char fn[64];
	    snprintf(fn, sizeof(fn), "input_%ld", time(NULL));
	    log_file = fopen(fn, "wb");
	} else {
	    is_record = 0;
	    is_play = 1;
	    time_factor = play_speed;
	    log_file = fopen(argv[1], "rb");
	    printf("pos %ld %d\n", ftell(log_file), fileno(log_file));
	}

	cur_time = 0;
	#endif

	//call_reg2(StartNewLevelSub, start_level, 1);
	my_StartNewLevelSub(1, 1, 0);

	
	game();
    
	return 0;
}


int init_play_record(int argc, char **argv) {
    if (argc <= 1) {
        is_record = 1;
        char fn[64];
        snprintf(fn, sizeof(fn), "input_%ld", time(NULL));
        log_file = fopen(fn, "wb");
    } else {
        is_record = 0;
        is_play = 1;
        time_speed = play_speed;
        //time_factor = play_speed;
        log_file = fopen(argv[1], "rb");
        if (!log_file) {
            perror("opening file");
            return 1;
        }
        //start_level = strstr(argv[1], "level3") ? 3 : 1;
    }
    return 0;
}


int main(int argc, char **argv) {
    if (init_play_record(argc, argv))
        return 1;
    InitKeyMap();
    #if 0
    if (!is_play || play_show)
        if (sdl_init())
            return 1;
    #endif
    test_main2(argc, argv);
}


int key_last, game_key;
#include "key.h"
#include "kconfig.h"

#define BTN_COUNT 14
typedef struct my_control_info {
union {
struct {
	fix	pitch_time;						
	fix	vertical_thrust_time;
	fix	heading_time;
	fix	sideways_thrust_time;
	fix	bank_time;
	fix	forward_thrust_time;
	
	ubyte	rear_view_down_count;	
	ubyte	rear_view_down_state;	
	
	ubyte	fire_primary_down_count;
	ubyte	fire_primary_state;
	ubyte	fire_secondary_state;
	ubyte	fire_secondary_down_count;
	ubyte	fire_flare_down_count;

	ubyte	drop_bomb_down_count;	

	ubyte	automap_down_count;
	ubyte	automap_state;
};
struct {
	fix thrust[6];
	ubyte buttons[BTN_COUNT];
};
};
} my_control_info;
my_control_info *controls = &Controls;

#include "itoa.h"

void timer_init() {}
fix timer_get_fixed_seconds() { return last_time; } //fixmuldiv(SDL_GetTicks(), F1_0, 1000); }

void my_controls_read_all() {
    char line[256];
    if (is_record) {
        controls_read_all();
        if (!log_file)
            return;
        int key = key_last & 0xff;
        //if (key == KEY_ESC)
        //    printf("key=esc, elseq=%d\n", *pEndlevel_sequence);
        if ((key >= KEY_1 && key <= KEY_0)) {
            //(key == KEY_ESC && *pEndlevel_sequence)) { // not run in exit seq
            game_key = key;
            key_last = 0;
        } else {
            game_key = 0;
        }
        int li = 0;
        char *p = line;
        for (int i = 0; i < 6; i++)
            if (controls->thrust[i]) {
                while (li < i) {
                    *p++ = ',';
                    li++;
                }
                itoa(controls->thrust[i], p, 16);
                p += strlen(p);
            }
        if (p > line)
            fprintf(log_file, "th=%s\n", line);
        p = line;
        li = 0;
        for (int i = 0; i < BTN_COUNT; i++)
            if (controls->buttons[i]) {
                while (li < i) {
                    *p++ = ',';
                    li++;
                }
                itoa(controls->buttons[i], p, 10);
                p += strlen(p);
            }
        if (p > line)
            fprintf(log_file, "bt=%s\n", line);
        if (game_key != 0) {
            fprintf(log_file, "key=%d\n", game_key);
        }
        fprintf(log_file, "seed=%x\n", *pRandNext);
        //fprintf(log_file, "hashscr=%x\n", XXH32(vga_screen, vga_w * vga_h, 0));
        fprintf(log_file, "\n");
    } else {
        memset(controls, 0, sizeof(*controls));
        game_key = 0;
        char *ret;
        while ((ret = fgets(line, sizeof(line), log_file)) &&
            line[0] && line[0] != '\n') {
            if (line[0] == '!') {
                printf("unexpected state change in recording: %s\n", line);
                abort();
            }
            char *p = strchr(line, '=');
            if (!p)
                continue;
            p++;
            switch (line[0]) {
                case 't':
                    for (int i = 0; i < 6; i++) {
                        controls->thrust[i] = (int)strtol(p, &p, 16);
                        if (*p++ != ',')
                            break;
                    }
                    break;
                case 'b':
                    for (int i = 0; i < BTN_COUNT; i++) {
                        controls->buttons[i] = (int)strtol(p, &p, 10);
                        if (*p++ != ',')
                            break;
                    }
                    break;
                case 'k':
                    game_key = strtol(p, NULL, 10);
                    on_key(game_key, 1);
                    on_key(game_key, 0);
                    break;
                case 's': {
                    int seed = strtoul(p, NULL, 16);
                    if (seed != *pRandNext) {
                        printf("seed mismatch read %x is %x\n", seed, *pRandNext);
                        *pRandNext = seed;
                        exit(1);
                    }
                    break;
                }
                #if 0
                case 'h': {
                    uint32_t hashscr = strtoul(p, NULL, 16);
                    uint32_t cur = XXH32(vga_screen, vga_w * vga_h, 0);
                    if (hashscr != cur) {
                        printf("screen mismatch read %x is %x\n", hashscr, cur);
                        exit(1);
                    }
                    break;
                }
                #endif
            }
        }
        if (!ret)
            exit(1);
    }
}

int my_key_inkey_time(fix *time) {
    return key_inkey_time(time);
}

void game_render_frame();
void my_game_render_frame() {
    game_render_frame();
    last_time += 65536/32;
    draw();
}

int psrand_pal() {
    return 12345;
}


void print_stat(FILE *f) {
    fprintf(f, "!newlevel\n");
    fprintf(f, "difficulty=%d\n", *pDifficulty_level);
    fprintf(f, "level=%d\n", *pCurrent_level_num);
    fprintf(f, "seed=%x\n", *pRandNext);
    fprintf(f, "time=%x\n", *pGameTime);
    fprintf(f, "framecount=%d\n", FrameCount);

    fprintf(f, "flags=%x\n", Player->flags);
    fprintf(f, "energy=%x\n", Player->energy);
    fprintf(f, "shields=%x\n", Player->shields);
    fprintf(f, "lives=%d\n", Player->lives);
    fprintf(f, "laser_level=%d\n", Player->laser_level);
    fprintf(f, "primary_flags=%x\n", Player->primary_weapon_flags);
    fprintf(f, "secondary_flags=%x\n", Player->secondary_weapon_flags);
    ushort *p = Player->primary_ammo;
    fprintf(f, "primary_ammo=%d,%d,%d,%d,%d\n", p[0], p[1], p[2], p[3], p[4]);
    p = Player->secondary_ammo;
    fprintf(f, "secondary_ammo=%d,%d,%d,%d,%d\n", p[0], p[1], p[2], p[3], p[4]);
    fprintf(f, "score=%d\n", Player->score);
    fprintf(f, "time_total=%x\n", Player->time_total);
    fprintf(f, "num_kills_total=%d\n", Player->num_kills_total);
    fprintf(f, "hostages_rescued_total=%d\n", Player->hostages_rescued_total);
    fprintf(f, "hours_total=%d\n", Player->hours_total);
    fprintf(f, "primary_weapon=%d\n", Primary_weapon);
    fprintf(f, "secondary_weapon=%d\n", Secondary_weapon);
}


void reset_time();
void my_reset_time() {
    if (is_record) {
        print_stat(log_file);
        fprintf(log_file, "!game\n");
    } else if (is_play) {
        print_stat(stdout);
        //load_stat(log_file);
    }
    set_screen_mode(SCREEN_GAME);
    gr_palette_load(gr_palette);
    sdl_time_sync();
}

void inferno_init() {}
void inferno_done() {}
void inferno_loop() {}
