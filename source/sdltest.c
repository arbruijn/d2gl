//#include "sdl2/include/SDL.h"
//#define MKPNGTIME 0
#include "glconfig.h"
#ifndef __WATCOMC__
#ifdef EMSCRIPTEN
#include "SDL/SDL.h"
#define GL_GLEXT_PROTOTYPES 1
//#include <SDL2/SDL_opengles2.h>
#include <SDL/SDL_opengl.h>
#else
#define SDL2
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#define GL_GLEXT_PROTOTYPES 1
//#include <SDL2/SDL_opengles2.h>
#include <SDL2/SDL_opengl.h>
#endif
#include "sdlkeymap.h"
#endif
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#ifdef __WATCOMC__
#undef stdout
#define stdout (__get_std_file(1))
#endif

#include "fix.h"
#include "effects.h"
#include "deslog.c"
#include "3d.h"
#include "gr.h"
#include "playsave.h"
#include "mission.h"
#include "cfile.h"
#ifdef GL
#include "pa_gl.h"
#endif
#include "gamepal.h"
#include "vga.h"
#include "weapon.h"
#include "newdemo.h"
#include "args.h"

//#define SDL_Window SDL_Surface

long lastTicks;
#ifndef __WATCOMC__
#ifdef SDL2
static SDL_Texture *texture;
static SDL_Renderer *render;
static SDL_Window *window;
#else
static SDL_Surface *window;
#endif
#endif

int EventLoop(int sync);
extern void on_key(int, int);
extern int test_main(int argc, char **argv);

void error_set(char *msg, ...) {
	va_list vp;
	va_start(vp, msg);
	vprintf(msg, vp);
	va_end(vp);
}

extern void copy_screen_to_texture(uint32_t *texture);

#ifndef __WATCOMC__
__attribute__((optimize("-O3")))
#endif
uint32_t fx_hasher(void *buf, int len) {
	uint32_t v = 0, *p = buf;
	while (len--)
		v = 0x9E3779B9U * (((v << 5) | (v >> 27)) ^ *p++);
	return v;
}

int last_time;
int sdl_time_start;
int time_speed = 1;
int play_speed = 32;
int play_show = 1;
int record_demo = 0;
int no_video = 0;
int img_time = -1;
int log_screen = 0;
int d1;

FILE *log_file = NULL;
int is_record = 1;
int is_play = 0;
int is_continue = 0;
int last_time_menu;
extern int g3_no_draw;

void frame_delay() {
    #ifndef __WATCOMC__
    EventLoop(0);
    #if 0
    SDL_Delay(1);
    #else
    int timeout = sdl_time_start + fixmuldiv(last_time + last_time_menu, 1000, 65536 * time_speed);
    while (SDL_GetTicks() < timeout) {
        EventLoop(0);
        SDL_Delay(1);
    }
    #endif
    #endif
}

#if 0
void maybe_swap_delay() {
    if (no_video)
        return;
	draw();
	#if 0
    #ifndef __WATCOMC__        
    #ifdef SDL2
    SDL_GL_SwapWindow(window);
    #else
    SDL_GL_SwapBuffers();
    #endif
    #endif
    frame_delay();
    #endif
}
#endif

void draw() {
    if (no_video)
        return;
    #ifndef __WATCOMC__
    #ifdef GL
    SDL_GL_SwapWindow(window);
    #else
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
    #endif
    frame_delay();
    if (0) {
            long now = SDL_GetTicks();
            if (lastTicks != 0 && now - lastTicks < 10) {
                    SDL_Delay(10 - (Uint32)(now - lastTicks));
                    now = SDL_GetTicks();
            }
            lastTicks = now;
    }
    #endif
    return;
}

void gr_sync_display() {
    last_time_menu += 65536 / 64;
    draw();
}

void sdl_time_sync() {
    last_time_menu = 0;
    #ifndef __WATCOMC__
    sdl_time_start = SDL_GetTicks() - fixmuldiv(last_time, 1000, 65536 * time_speed);
    #endif
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
    if (no_video) {
        //gl_init();
        return 0;
    }

    #ifndef __WATCOMC__
    SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
    SDL_Init(SDL_INIT_EVERYTHING);

    #ifdef GL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetSwapInterval(0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    #endif

    #ifdef SDL2
    if (!(window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN
         #ifdef GL
         | SDL_WINDOW_OPENGL
         #endif
         ))) {
            error_set("SDL_CreateWindow %s", SDL_GetError());
            return 1;
    }
    SDL_SetWindowTitle(window, "Test");
    #ifdef GL
    SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(0); // disable vsync
    #else
    if (!(render = SDL_CreateRenderer(window, -1, 0))) {
        error_set("SDL_CreateRenderer %s", SDL_GetError());
        return 1;
    }
    #endif
    #else
    SDL_EnableKeyRepeat(0, 0);
    if (!(window = SDL_SetVideoMode( 640, 480, 32, SDL_OPENGL )))
        return 1;
    #endif
    #endif

    #ifdef GL
    gl_init();
    #endif

    return 0;
}

extern int cur_w, cur_h;
void video_set_res(int w, int h) {
    if (!cur_w)
        sdl_init();
    cur_w = w;
    cur_h = h;
    if (no_video)
        return;
#if defined(SDL2) && !defined(GL)
    if (texture != NULL)
        SDL_DestroyTexture(texture);
    if (!(texture = CreateTexture(w, h))) {
        fprintf(stderr, "cannot create texture %dx%d\n", w, h);
        exit(1);
    }
#endif
}

int EventLoop(int sync) {
    int keep_running = 1;
    #ifndef __WATCOMC__
        if (sync) {
            #ifdef GL
            #ifdef SDL2
            SDL_GL_SwapWindow(window);
            #else
            SDL_GL_SwapBuffers();
            #endif
            glClear(GL_COLOR_BUFFER_BIT);
            #endif
            SDL_Delay(10);
        }

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
			int pckey = sdlkeymap[(int)evt.key.keysym.scancode];
			#else
			int pckey = sdlkeymap[(int)evt.key.keysym.sym];
			#endif
			//printf("key %d pc %d\n", evt.key.keysym.scancode, pckey);
			on_key(pckey, 1);
			break;
		}
		case SDL_KEYUP: {
		        #ifdef SDL2
			int pckey2 = sdlkeymap[(int)evt.key.keysym.scancode];
		        #else
			int pckey2 = sdlkeymap[(int)evt.key.keysym.sym];
			#endif
			on_key(pckey2, 0);
			break;
    }
		}
	}
    #endif
    return keep_running;
}

//#include "types.h"
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
#ifdef ENDLEVEL
extern int explosion_wait1, explosion_wait2;
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
extern int Countdown_seconds_left;
int *pCountdown_seconds_left = &Countdown_seconds_left;
extern unsigned int d_rand_seed;
unsigned int *pRandNext = &d_rand_seed;
extern int Num_awareness_events;
int *pNum_awareness_events = &Num_awareness_events;
extern int Laser_offset;
extern int Missile_gun;
extern fix Time_flash_last_played;
extern fix Afterburner_charge;
extern ubyte PrimaryOrder[];
extern ubyte SecondaryOrder[];
extern int Spreadfire_toggle;
extern int Helix_orientation;
extern byte Lighting_objects[MAX_OBJECTS];
extern fix Dynamic_light[MAX_VERTICES];
extern fix object_light[MAX_OBJECTS];
extern ubyte object_id[MAX_OBJECTS];
extern object *old_viewer;

int level = 1;
int skipped_levels = 0;
int end_level = 0;
int reset_effects = 0;
#define ELMS(x) (sizeof(x)/sizeof((x)[0]))
void load_stat(FILE *f, int have_level) {
    char line[256], *p, *arg;
    unsigned short matcen_creator_obj[64];
    unsigned char matcen_creator_num[64];

    if (!have_level) {
        if (!fgets(line, sizeof(line), f))
            exit(0);
        if ((p = strchr(line, '\n')))
            *p = 0;
        if (strcmp(line, "!newlevel") != 0) {
            fprintf(stderr, "missing !newlevel on new level: %s\n", line);
            abort();
        }
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
        if (strcmp(line, "difficulty") == 0)
            *pDifficulty_level = (int)strtol(arg, NULL, 10);
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
        if (strcmp(line, "laser_level") == 0)
            Player->laser_level = (int)strtol(arg, NULL, 10);
        if (strcmp(line, "primary_flags") == 0)
            Player->primary_weapon_flags = (int)strtoul(arg, NULL, 16);
        if (strcmp(line, "secondary_flags") == 0)
            Player->secondary_weapon_flags = (int)strtoul(arg, NULL, 16);
        if (strcmp(line, "primary_ammo") == 0)
            parse_ushort_array(arg, Player->primary_ammo, 10);
        if (strcmp(line, "secondary_ammo") == 0)
            parse_ushort_array(arg, Player->secondary_ammo, 10);
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
        if (strcmp(line, "laser_offset") == 0)
            Laser_offset = (int)strtoul(arg, NULL, 16);
        #ifdef ENDLEVEL
        if (strcmp(line, "explosion_wait1") == 0)
            explosion_wait1 = (int)strtoul(arg, NULL, 16);
        if (strcmp(line, "explosion_wait2") == 0)
            explosion_wait2 = (int)strtoul(arg, NULL, 16);
        #endif
        if (strcmp(line, "missile_gun") == 0)
            Missile_gun = (int)strtol(arg, NULL, 10);
        if (strcmp(line, "reset_effects") == 0)
            reset_effects = (int)strtol(arg, NULL, 10);
        if (strcmp(line, "primary_last_was_super") == 0)
            parse_ubyte_array(arg, Primary_last_was_super, SUPER_WEAPON);
        if (strcmp(line, "secondary_last_was_super") == 0)
            parse_ubyte_array(arg, Secondary_last_was_super, SUPER_WEAPON);
        if (strcmp(line, "time_flash_last_played") == 0)
            Time_flash_last_played = (int)strtol(arg, NULL, 16);
        if (strcmp(line, "afterburner_charge") == 0)
            Afterburner_charge = (int)strtol(arg, NULL, 16);
        if (strcmp(line, "matcen_creator_obj") == 0)
            parse_ushort_array(arg, matcen_creator_obj, ELMS(matcen_creator_obj));
        if (strcmp(line, "matcen_creator_num") == 0) {
            int i, n = parse_ubyte_array(arg, matcen_creator_num, ELMS(matcen_creator_num));
            for (i = 0; i < n; i++)
                Objects[matcen_creator_obj[i]].matcen_creator = matcen_creator_num[i] | 0x80;
        }
        if (strcmp(line, "primary_order") == 0)
            parse_ubyte_array(arg, PrimaryOrder, MAX_PRIMARY_WEAPONS + 1);
        if (strcmp(line, "secondary_order") == 0)
            parse_ubyte_array(arg, SecondaryOrder, MAX_SECONDARY_WEAPONS + 1);
    }
    if ((p = strchr(line, '\n')))
        *p = 0;
    if (strcmp(line, "!game") != 0) {
        fprintf(stderr, "missing !game on new level: %s\n", line);
        abort();
    }
}

extern int Buddy_last_seen_player, Buddy_last_player_path_created;
extern fix Escort_last_path_created;
void my_StartNewLevelSub(int arg_level, int flag, int secret) {
    int i;

    level = arg_level;
    if (is_play) {
        if (level == end_level) {
		    printf("got end_level\n");
            exit(0);
		}
        load_stat(log_file, skipped_levels);
    }
    skipped_levels = 0;

    // clear AI globals
    for (i = 0; i < MAX_OBJECTS; i++)
	    Objects[i].control_type = 0; // prevent init_ai_objects
    memset(pAi_local_info, 0, sizeof(Ai_local_info)); //184 * 350);
    memset(pAi_cloak_info, 0, 8 * 16);
    *pLast_gate_time = *pLast_teleport_time = *pBoss_cloak_end_time = 0;
    *pNum_awareness_events = 0;
    
    *pCountdown_seconds_left = 0;


    Buddy_last_seen_player = 0;
    Buddy_last_player_path_created = 0;
    Escort_last_path_created = 0;

    Last_fired_upon_player_pos.x = Last_fired_upon_player_pos.y = Last_fired_upon_player_pos.z = 0;

    Spreadfire_toggle = 0;
    Helix_orientation = 0;

    #if 1
    init_special_effects();
    for (i = 0; i < Num_effects; i++)
        Effects[i].frame_count = 0;
    #endif

    memset(Lighting_objects, 0, sizeof(Lighting_objects));
    memset(Dynamic_light, 0, sizeof(Dynamic_light));
    memset(object_light, 0, sizeof(object_light));
    memset(object_id, 0, sizeof(object_id));
    old_viewer = NULL;

    call_reg3(StartNewLevelSub, level, flag, secret);
}


void init_data_files();
extern int grd_fades_disabled;
extern int skip_endlevel;
extern int no_ambient_sounds;
extern int key_inkey_time_sync;
extern int VR_screen_mode;
extern int Missile_view_enabled;
extern int Guided_in_big_window;
extern int no_render;
extern int no_read_player_file;

#ifdef __WATCOMC__
int mprotect(void *, int, int);
#define PROT_READ  1
#define PROT_WRITE 2
#define PROT_EXEC  4
void scale_do_cc_scanline();
#endif

int test_main2(int argc, char **argv) {
	int display_mode, ret;
	d1 = FindArg("-d1");
	//init_data_files();
	
	#ifdef SHAREWARE
	cfile_init("d2demo.hog");
	#else
	cfile_init("descent2.hog");
	#endif
	load_text();

	if (d1) {
	 	strcat(TXT_CANT_OPEN_DOOR, ".");
	 	TXT_W_SPREADFIRE_S = "Spreadfire";
		TXT_SMART = "Smart";
	}

	Lighting_on = 1;
	g3_no_draw = (no_video && !log_screen); // || (slow_time != -1); //is_play && !play_show;

	grd_fades_disabled = 1;
	skip_endlevel = 1;
	no_ambient_sounds = 1; // depends on loaded bitmaps for previous level
	key_inkey_time_sync = 0;

	#ifdef __WATCOMC__
	if (mprotect((void *)((int)&scale_do_cc_scanline & ~4095), 8192, PROT_READ | PROT_WRITE | PROT_EXEC))
		abort();
	#endif

	#define SM_320x200C 0
	#define SM_640x480V 14
	#define VR_NONE 0
	#if 0
	int screen_mode = !no_video ? SM_640x480V : SM_320x200C;
	int screen_width = modes[screen_mode][0];
	int screen_height = modes[screen_mode][1];
	int vr_mode = VR_NONE;
	int screen_compatible = 0;
	int use_double_buffer = 0;
	call_reg5(game_init_render_buffers, screen_mode, screen_width, screen_height, 0, 0); //use_double_buffer, vr_mode, screen_compatible );
	#endif
	display_mode = 1; // no_video ? 0 : 1
	set_display_mode(display_mode);

	no_render = is_play && !play_show;
	no_read_player_file = 1;

	call_reg0(gr_init);
	//printf("cur canv %p data=%08x\n", *cur_canvas, (*cur_canvas)[3]);
	call_reg1(vga_set_mode, VR_screen_mode);
	//grd_curcanv->cv_bitmap.bm_type = BM_GL;
	//printf("gr_set_mode=%d\n", ret);
	//printf("cur canv %p data=%08x\n", *cur_canvas, (*cur_canvas)[3]);
	//(*cur_canvas)[3] = (int)vga_screen;
	//goto err;
	call_reg1(gr_use_palette_table, "default.256");
	gamefont_init();
	bm_init(d1);

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
	//build_mission_list(0);
	//read_mission_file("d2.mn2", 0, ML_CURDIR);
	#ifndef SHAREWARE
	if (d1) {
		int read_mission_file(const char*,int,int);
		read_mission_file("descent.mn2", 1, ML_CURDIR);
	}
	#endif
	call_reg1(load_mission, d1 ? 1 : 0);
	Function_mode = 2; // FMODE_GAME
	#ifndef Skip_briefing_screens
	Skip_briefing_screens = 1;
	#endif

	read_player_file();
	Cockpit_mode = CM_FULL_SCREEN;
	Missile_view_enabled = 0;
	Difficulty_level = 0;
	Guided_in_big_window = 1;
	Auto_leveling_on = 0;
	write_player_file();
	//extern int Current_display_mode;
	//Current_display_mode = display_mode;

	init_player_stats_game();
	gr_palette_set_gamma(0);

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

	#if 0
	if (skipped_levels) {
	    char *level_name = Level_names[level-1];
	    CFILE *f = cfopen(level_name, "rb");
	    char buf[64], *pal, *p;
	    if (!f) {
	        fprintf(stderr, "cannot open %s for palette peek\n", level_name);
	        return 1;
            }
            cfread(buf, sizeof(buf), 1, f);
            cfclose(f);
            if (memcmp(buf, "LVLP", 4) != 0 || buf[4] < 5) {
                fprintf(stderr, "invalid level for palette peek %s\n", level_name);
                return 1;
            }
            pal = buf + 16 + (buf[5] >= 8 ? 7 : 0);
            p = memchr(pal, '\n', buf + 64 - pal);
            if (!p) {
                fprintf(stderr, "invalid level for palette peek %s\n", level_name);
                return 1;
            }
            *p = 0;
            load_palette(pal,1,1); 
	}
	#endif

	//call_reg2(StartNewLevelSub, start_level, 1);
	my_StartNewLevelSub(level, 1, 0);

	
	//game();

	//void (*tmap_drawer_ptr)(grs_bitmap *bm,int nv,g3s_point **vertlist) = gl_draw_tmap;
	//void (*flat_drawer_ptr)(int nv,int *vertlist) = gl_draw_flat;
	//int (*line_drawer_ptr)(fix x0,fix y0,fix x1,fix y1) = gl_draw_line;

        //g3_set_special_render(tmap_drawer_ptr, NULL, line_drawer_ptr);

	//game_render_frame();
	//SDL_GL_SwapWindow(window);
	//gl_test();
	//getchar();
	game();
    
	return 0;
}

int main(int argc, char **argv) {
	int i;
    init_play_args(argc, argv);
    InitArgs(argc, argv);
    no_video = (is_play && !play_show) || FindArg("-novideo");
    img_time = (i = FindArg("-imgtime")) ? strtol(Args[i + 1],NULL,16) : -1;
    log_screen = FindArg("-logscreen");
    if (start_play_record())
        return 1;
    #ifndef __WATCOMC__
    InitKeyMap();
    #endif
    #if 0
    if (!no_video)
        if (sdl_init())
            return 1;
    #endif
    return test_main2(argc, argv);
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
my_control_info *controls = (void *)&Controls;

#include "itoa.h"

void timer_init() {}
fix timer_get_fixed_seconds() { return last_time + last_time_menu; } //fixmuldiv(SDL_GetTicks(), F1_0, 1000); }
//fix timer_get_fixed_seconds() { return fixmuldiv(SDL_GetTicks(), F1_0, 1000); }

void my_controls_read_all() {
    //printf("controls_read_all FrameTime=%x\n", FrameTime);
    char line[256];
    if (is_record) {
        int key, li, i;
        char *p;

        controls_read_all();
        if (!log_file)
            return;
        key = key_last & 0xff;
        //if (key == KEY_ESC)
        //    printf("key=esc, elseq=%d\n", *pEndlevel_sequence);
        if ((key >= KEY_1 && key <= KEY_0)) {
            //(key == KEY_ESC && *pEndlevel_sequence)) { // not run in exit seq
            game_key = key;
            key_last = 0;
        } else {
            game_key = 0;
        }
        li = 0;
        p = line;
        for (i = 0; i < 6; i++)
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
        for (i = 0; i < BTN_COUNT; i++)
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
        char *ret, *p;
        int i;

        memset(controls, 0, sizeof(*controls));
        game_key = 0;
        while ((ret = fgets(line, sizeof(line), log_file)) &&
            line[0] && line[0] != '\n') {
            if (line[0] == '!') {
                printf("unexpected state change in recording: %s\n", line);
                abort();
            }
            p = strchr(line, '=');
            if (!p)
                continue;
            p++;
            switch (line[0]) {
                case 't':
                    if (line[1] != 'h')
                        break;
                    for (i = 0; i < 6; i++) {
                        controls->thrust[i] = (int)strtol(p, &p, 16);
                        if (*p++ != ',')
                            break;
                    }
                    break;
                case 'b':
                    for (i = 0; i < BTN_COUNT; i++) {
                        controls->buttons[i] = (int)strtol(p, &p, 10);
                        if (*p++ != ',')
                            break;
                    }
                    controls->buttons[8] = controls->buttons[9] = 0; // disable automap
                    break;
                case 'k':
                    game_key = strtol(p, NULL, 10);
                    on_key(game_key, 1);
                    on_key(game_key, 0);
                    break;
                case 's': {
                    int seed = strtoul(p, NULL, 16);
                    if (seed != *pRandNext) {
                        printf("seed mismatch read %x is %x at %x\n", seed, *pRandNext, *pGameTime);
                        //*pRandNext = seed;
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
        if (!ret) {
            printf("play: eof\n");
            exit(1);
        }
    }
}

int my_key_inkey_time(fix *time) {
    return key_inkey_time(time);
}

void game_render_frame();
void my_game_render_frame() {
    //glClear(GL_DEPTH_BUFFER_BIT);
    extern int ntmap_dbg;
    extern char gr_screen_buffer[];
    //FILE *f;

    ntmap_dbg = GameTime == img_time;
    if (ntmap_dbg)
    	g3_no_draw = 0;

    memset(gr_screen_buffer, 0, cur_w * cur_h);
    game_render_frame();
    last_time += 65536/32;
    draw();
    #ifndef GL
    if (log_screen || GameTime == img_time)
	    printf("screen=%x %x\n", GameTime, fx_hasher(gr_screen_buffer, cur_w * cur_h >> 2));
    #endif

    if (GameTime != img_time)
        return;
	//f = fopen("out.bin", "wb"); fwrite(gr_screen_buffer, cur_w, cur_h, f); fclose(f);
	#ifdef __WATCOMC__
	pcx_write_bitmap("out.pcx", &grd_curcanv->cv_bitmap, gr_palette);
	#else
    static int count = 0;
    char filename[32];
    SDL_Surface *img = SDL_CreateRGBSurface(0, cur_w, cur_h, 32, 0, 0, 0, 0);
    SDL_LockSurface(img);
    copy_screen_to_texture(img->pixels);
    SDL_UnlockSurface(img);
    snprintf(filename, sizeof(filename), "out%04d.png", count++);
    IMG_SavePNG(img, filename);
    SDL_FreeSurface(img);
    #endif
    exit(1);

    //printf("%d\n", Players[0].shields / 65536);
    //printf("energy=%x\n", Players[0].energy);
    //printf("rendered time=%x count=%d\n", last_time, FrameCount);
}

#if 0
int psrand_pal() {
    return 12345;
}
#endif

extern int Current_level_D1;

void print_stat(FILE *f) {
    ushort *ps;
    ubyte *pb;
    char line1[256], *p1;
    char line2[256], *p2;
    char buf[256];
    int i;

    fprintf(f, "!newlevel\n");
    fprintf(f, "level=%d\n", *pCurrent_level_num);
    fprintf(f, "difficulty=%d\n", *pDifficulty_level);
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
    ps = Player->primary_ammo;
    if (Current_level_D1)
	    fprintf(f, "primary_ammo=%d,%d,%d,%d,%d\n", ps[0], ps[1], ps[2], ps[3], ps[4]);
    else
	    fprintf(f, "primary_ammo=%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", ps[0], ps[1], ps[2], ps[3], ps[4], ps[5], ps[6], ps[7], ps[8], ps[9]);
    ps = Player->secondary_ammo;
    if (Current_level_D1)
	    fprintf(f, "secondary_ammo=%d,%d,%d,%d,%d\n", ps[0], ps[1], ps[2], ps[3], ps[4]);
	else
	    fprintf(f, "secondary_ammo=%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", ps[0], ps[1], ps[2], ps[3], ps[4], ps[5], ps[6], ps[7], ps[8], ps[9]);
    fprintf(f, "score=%d\n", Player->score);
    fprintf(f, "time_total=%x\n", Player->time_total);
    fprintf(f, "num_kills_total=%d\n", Player->num_kills_total);
    fprintf(f, "hostages_rescued_total=%d\n", Player->hostages_rescued_total);
    fprintf(f, "hours_total=%d\n", Player->hours_total);
    fprintf(f, "primary_weapon=%d\n", Primary_weapon);
    fprintf(f, "secondary_weapon=%d\n", Secondary_weapon);
    fprintf(f, "laser_offset=%x\n", Laser_offset);
    #ifdef ENDLEVEL
    if (d1) {
	    fprintf(f, "explosion_wait1=%x\n", explosion_wait1);
	    fprintf(f, "explosion_wait2=%x\n", explosion_wait2);
	}
    #endif
    fprintf(f, "missile_gun=%d\n", Missile_gun);
    if (!Current_level_D1) {
	    pb = Primary_last_was_super;
	    fprintf(f, "primary_last_was_super=%d,%d,%d,%d,%d\n", pb[0], pb[1], pb[2], pb[3], pb[4]);
	    pb = Secondary_last_was_super;
	    fprintf(f, "secondary_last_was_super=%d,%d,%d,%d,%d\n", pb[0], pb[1], pb[2], pb[3], pb[4]);
	    fprintf(f, "time_flash_last_played=%x\n", Time_flash_last_played);
	    fprintf(f, "afterburner_charge=%x\n", Afterburner_charge);
	}

    p1 = line1; p2 = line2;
    for (i = 0; i < Highest_object_index; i++)
        if (Objects[i].type == OBJ_ROBOT && Objects[i].matcen_creator & 0x80) {
            if (p1 + 4 >= line1 + sizeof(line1) || p2 + 4 >= line2 + sizeof(line2))
                break;
            p1 += sprintf(p1, "%d,", i);
            p2 += sprintf(p2, "%d,", (byte)(Objects[i].matcen_creator-0x80));
        }
    if (p1 > line1) {
        p1[-1] = p2[-1] = 0;
        fprintf(f, "matcen_creator_obj=%s\n", line1);
        fprintf(f, "matcen_creator_num=%s\n", line2);
    }
    if (!Current_level_D1) {
	    fprintf(f, "primary_order=%s\n", fmt_ubyte_array(buf, sizeof(buf), PrimaryOrder, MAX_PRIMARY_WEAPONS + 1));
	    fprintf(f, "secondary_order=%s\n", fmt_ubyte_array(buf, sizeof(buf), SecondaryOrder, MAX_SECONDARY_WEAPONS + 1));
	}
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

    if (record_demo && Newdemo_state == ND_STATE_NORMAL) {
        extern int Newdemo_no_rename;
        Newdemo_no_rename = 1;
	    newdemo_start_recording();
	    atexit(newdemo_stop_recording);
	}
}

void inferno_init() {}
void inferno_done() {}
void inferno_loop() {}

void my_start_player_death_sequence(object *obj) {
    printf("player died\n");
    exit(1);
}

void my_DoEndGame() {
    printf("game finished\n");
    exit(1);
}

void mouse_show_cursor() {}
void mouse_hide_cursor() {}
void mouse_get_pos(int *x, int *y) {}

int Function_mode;
int Screen_mode;
ubyte Version_major, Version_minor, Version_fix;
void show_order_form() {}
