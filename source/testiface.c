#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <stdint.h>
//#include <emscripten/emscripten.h>
#include "types.h"
#include "fix.h"
#include "text.h"
#include "mono.h"
#include "inferno.h"
#include "titles.h"
#ifdef SHAREWARE
#include "unadpcm.h"
#endif
#include "gr.h"
#include "piggy.h"
#include "timer.h"

char *hog_data, *pig_data;
int hog_len, pig_len;

//#define SCR_W 320
//#define SCR_H 400
ubyte gr_screen_buffer[1024*768];

//ubyte *gr_video_memory = gr_screen_buffer;
int cur_w, cur_h;

//uint32_t texture[SCR_W*SCR_H];

// void play_sample(int gsndnum, int pan, int volume) {}

char *slurp(const char *path, int *plen) {
	FILE *f;
	long len;
	char *buf;
	if (plen)
		*plen = 0;
	if (!(f = fopen(path, "rb")))
		return NULL;
	fseek(f, 0, SEEK_END);
	len = (int)ftell(f);
	fseek(f, 0, SEEK_SET);
	if (len > INT_MAX) {
		fclose(f);
		return NULL;
	}
	if (!(buf = (char *)malloc(len))) {
		fclose(f);
		return NULL;
	}
	if (fread(buf, 1, len, f) != len) {
		fclose(f);
		return NULL;
	}
	fclose(f);
	if (plen)
		*plen = (int)len;
	return buf;
}

void init_data_files() {
	hog_data = slurp("descent2.hog", &hog_len);
	pig_data = slurp("descent2.pig", &pig_len);
	if (!hog_data || !pig_data)
		printf("hog %s pig %s\n", hog_data ? "ok" : "failed", pig_data ? "ok" : "failed");
}


void hmp_set_volume(int vol) {
}
void hmp_song_stop() {
}

void hmp_song_done() {
}

int hmp_song_play(char *data, int size, int loop) {
	return 0;
}

#include "dos.h"
int _dos_findfirst(const char *spec, int attr, find_t *data) { return 1; }
int _dos_findnext(find_t *data) { return 1; }

#ifdef EM_CC
#endif

/*
uint32_t *get_screen() {
	return texture;
}
*/

void copy_screen_to_texture(uint32_t *texture) {
	uint32_t pal[256];
	extern ubyte gr_visible_pal[256 * 3];
	#define UP(x) ((((x) & 0x3f) << 2) | (((x) & 0x3f) >> 4))
	for (int i = 0; i < 256; i++)
		pal[i] = UP(gr_visible_pal[i * 3 + 2]) +
			(UP(gr_visible_pal[i * 3 + 1]) << 8) +
			(UP(gr_visible_pal[i * 3 + 0]) << 16) +
			(0xff << 24);
	/*
	for (int j = 0; j < SCR_H; j++)
		for (int i = 0; i < SCR_W; i++)
			texture[j * SCR_W + i] = pal[gr_screen_buffer[(SCR_H - j - 1) * SCR_W + i]];
	*/
	uint32_t *dst = texture;
	ubyte *srcend = gr_screen_buffer + cur_w * cur_h;
	for (ubyte *src = gr_screen_buffer; src < srcend; src++, dst++)
		*dst = pal[*src];
}

#if EM_CC
EM_JS(void, draw, (), {
  mydraw();
});
#else
extern void draw();
extern int EventLoop(int sync);
#endif

double last_frame;

#if 0
void gr_sync_display() {
	//draw();
	EventLoop(1);
#if EM_CC
	if (!last_frame) {
		emscripten_sleep(16);
	} else {
		int t = last_frame + 1000.0/60.0 - emscripten_get_now();
		if (t > 17)
			t = 17;
		if (t > 0)
			emscripten_sleep(t);
	}
	last_frame = emscripten_get_now();
#endif
}
#endif

extern ubyte domkeymap[];
extern void key_handle_key(int scancode);
#ifdef EM_CC
void on_key(int keycode, int down) {
	int c = domkeymap[keycode];
	if (!c)
		return;
	if (c & 0x80)
		key_handle_key(0xe0);
	key_handle_key((c & 0x7f) | (down ? 0 : 0x80));
}
#else
void on_key(int keycode, int down) {
	if (!keycode)
		return;
	if (keycode & 0x80)
		key_handle_key(0xe0);
	key_handle_key((keycode & 0x7f) | (down ? 0 : 0x80));
}
#endif

int kconfig_read_external_controls() { return -1; }


#ifdef EM_CC
static double start_time;
void timer_init() { start_time = emscripten_get_now(); }
fix timer_get_fixed_seconds() { return (fix)((emscripten_get_now() - start_time) * ((double)F1_0 / 1000.0)); }
#else
#ifdef SDL2
#include "sdl2/include/SDL.h"
#else
#include "SDL/SDL.h"
#endif
//Uint32 start_ticks;
//void timer_init() { start_ticks = SDL_GetTicks(); }
//fix timer_get_fixed_seconds() { return fixmuldiv(SDL_GetTicks() - start_ticks, F1_0, 1000); }
#endif
fix timer_get_approx_seconds() { return timer_get_fixed_seconds(); }

int filelength(int fd) {
	struct stat st;
	if (fstat(fd, &st))
		return -1;
	return st.st_size;
}

void _mprintf_at( int n, int row, int col, char * format, ... ) {
	va_list vp;
	va_start(vp, format);
	vfprintf(stderr, format, vp);
	va_end(vp);
}
void _mprintf( int n, char * format, ... ) {
	va_list vp;
	va_start(vp, format);
	vfprintf(stderr, format, vp);
	va_end(vp);
}

static digi_sound *get_sound(int num) {
	if (num < 0 || num >= sizeof(GameSounds) / sizeof(GameSounds[0]))
		return NULL;
	return &GameSounds[num];
}

int get_sample_size(int num) {
	digi_sound *snd = get_sound(num);
	return snd && snd->data ? snd->length : 0;
}

#ifdef SHAREWARE
static unsigned char sndbuf[69976]; // largest shareware sound (nuke01)
#endif

unsigned char *get_sample_data(int num) {
	digi_sound *snd = get_sound(num);
	if (!snd || !snd->data
#ifdef D1SW
		 || snd->length > sizeof(sndbuf)
#endif		 
		)
		return NULL;
#ifdef D1SW
	unadpcm(snd->data, sndbuf, snd->length);
	return sndbuf;
#else
	return snd->data;	
#endif	
}

#ifdef EM_CC
EM_JS(void, video_set_res, (int w, int h), {
	videoSetRes(w, h);
});

EM_JS(void, goto_steam, (void), {
	location = "https://store.steampowered.com/app/273570/Descent/";
});

EM_JS(void, goto_gog, (void), {
	location = "https://www.gog.com/game/descent";
});

EM_JS(void, play_ad, (void), {
	Asyncify.handleSleep(function(wakeUp) {
		playAd(wakeUp);
	});
});
EM_JS(void, goto_github, (void), {
	location = "https://github.com/arbruijn/d1wasm";
});
EM_JS(void, goto_opl3, (void), {
	location = "https://nukeykt.retrohost.net/";
});
EM_JS(int, load_filenames, (char *buf, int fnsize, int fnmax, const char *filespec, int stripext), {
	return loadFilenames(buf, fnsize, fnmax, filespec, stripext);
});
#endif

#ifndef EM_CC
#if 0
using namespace System;
namespace ClassLibrary1 {
	public ref class Renderer abstract {
	public:
		virtual void DrawFrame() = 0;
		virtual void ModeChange(int w, int h) = 0;
	};

	public ref class ClassicGame abstract sealed {
	public:
		static Renderer ^globalRender;
		static void GameMain(Renderer ^render) {
			globalRender = render;
			char *argv[] = { "", ""}; //"-1024x768" };
			main(2, argv);
		}
		static void OnKey(int pckey, int down) {
			on_key(pckey, down);
		}
		static void CopyScreenToTexture(System::IntPtr texture) {
			copy_screen_to_texture((uint32_t *)(int)texture);
		}
	};
}
void draw() {
	ClassLibrary1::ClassicGame::globalRender->DrawFrame();
}
void video_set_res(int w, int h) {
	cur_w = w;
	cur_h = h;
	ClassLibrary1::ClassicGame::globalRender->ModeChange(w, h);
}
#endif
void goto_steam() {}
void goto_gog() {}
void play_ad() {}
void goto_github() {}
void goto_opl3() {}
int load_filenames(char *buf, int fnsize, int fnmax, const char *filespec, int stripext, int host) { return 0; }
void *load_file(const char *filename, int *psize) {
	return slurp(filename, psize);
}
void save_file(const char *filename, void *buf, int size) {
	FILE *f;
	if (!(f = fopen(filename, "wb")))
		return;
	fwrite(buf, size, 1, f);
	fclose(f);
}

void music_volume_set(int mvolume) {}
void play_stop(){}
void play_start(){}
int play_sample(int num, int pan, int vol, int repeat) { return 0; }
void update_sample(int idx, int pan, int vol){}
void stop_sample(int idx){}
const char *playerweb_play_data(void *song_data, int song_len,
	void *melobnk_data, int melobnk_len,
	void *drumbnk_data, int drumbnk_len, int loop){ return NULL; }

#endif

#include "rbaudio.h"
void RBAInit(ubyte drv) {}
void RBARegisterCD(void) {}
long RBAGetDeviceStatus() { return 0; }
int RBAPlayTrack(int track) { return 0; }
int RBAPlayTracks(int start, int end) { return 0; }
void RBAStop() {}
void RBASetStereoAudio(RBACHANNELCTL *channels) {}
void RBASetQuadAudio(RBACHANNELCTL *channels) {}
void RBAGetAudioInfo(RBACHANNELCTL *channels) {}
void RBAPause() {}
int RBAResume() { return 0; }
void RBASetChannelVolume(int channel, int volume) {}
void RBASetVolume(int volume) {}
long RBAGetHeadLoc(int *min, int *sec, int *frame) { return 0; }
int RBAGetTrackNum() { return 0; }
int RBAPeekPlayStatus() { return 0; }
int RBAEnabled() { return 0; }
void RBAEnable() {}
void RBADisable() {}
int RBAGetNumberOfTracks(void) { return 0; }

int descent_critical_error = 0;

void outp(int port, int val) {}

void timer_delay(fix time) { SDL_Delay(fixmul(time, 1000)); }
void delay(int ms) { SDL_Delay(ms); }

#include "findfile.h"
#include <dirent.h>
#include <fnmatch.h>
static char ff_search_str[13];
static DIR *ff_dir;
int FileFindFirst(char *search_str, FILEFINDSTRUCT *ffstruct) {
	char path[PATH_MAX], *p;
	if (ff_dir)
		return -1;
	if ((p = strchr(search_str, '/'))) {
		memcpy(path, search_str, p - search_str);
		path[p - search_str] = 0;
		p++;
	} else {
		strcpy(path, ".");
		p = search_str;
	}
	strcpy(ff_search_str, p);
	if (!(ff_dir = opendir(path)))
		return -1;
	return FileFindNext(ffstruct);
}
int FileFindNext(FILEFINDSTRUCT *ffstruct) {
	struct dirent *d;
	do {
		if (!(d = readdir(ff_dir)))
			return -1;
	} while (fnmatch(ff_search_str, d->d_name, FNM_PATHNAME | FNM_NOESCAPE) != 0);
	strcpy(ffstruct->name, d->d_name);
	return 0;
}
int FileFindClose(void) {
	closedir(ff_dir);
	ff_dir = NULL;
	return 0;
}
int GetFileDateTime(int filehandle, FILETIMESTRUCT *ftstruct) { return -1; }
int SetFileDateTime(int filehandle, FILETIMESTRUCT *ftstruct) { return -1; }
int GetDiskFree () { return -1; }

#include "movie.h"
int MovieHires;
int PlayMovie(const char *filename, int allow_abort) { return -1; }
int PlayMovies(int num_files, const char *filename[], int graphmode, int allow_abort) { return -1; }
int InitRobotMovie (char *filename) { return -1; }
void DeInitRobotMovie(void) {}
void init_movies() { }
int init_subtitles(char *filename) { return -1; }
void close_subtitles() {}


#include "vga.h"
#include "game.h"
#include "config.h"
#include "joy.h"
//ConfigInfo gConfigInfo;
ubyte Config_master_volume;
ubyte CybermouseActive;
int descent_critical_deverror;
int digi_is_channel_playing(int ch) { return -1; }
void digi_stop_all_channels() { }
void gr_bm_ubitblt_double(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap *src, grs_bitmap *dest) { }
int hide_cursor() { return -1; }
int init_extra_robot_movie(char *n) { return -1; }
int InitMovieBriefing() { return -1; }
int inp(int port) { return -1; }
int joy_btn_name() { return -1; }
//int joydefs_calibrate() { return -1; }
//int joydefs_calibrating() { return -1; }
//int joydefs_set_type() { return -1; }
int joy_do_buttons() { return -1; }
int joy_get_any_button_down_cnt() { return -1; }
int macintosh_quit() { return -1; }
int ms_warning() { return -1; }
int MVEPaletteCalls;
int network_dump_appletalk_player() { return -1; }
int p2cstr() { return -1; }
int pa_draw_line(int a, int b, int c, int d) { return -1; }
int pa_render_start() { return -1; }
int read_int_swap() { return -1; }
int receive_netgame_packet() { return -1; }
int receive_sequence_packet() { return -1; }
int robot_movies;
void RotateRobot() { }
int send_sequence_packet() { return -1; }
int show_cursor() { return -1; }
int stackavail() { return 1048576; }
int StackSpace() { return -1; }
int swapint() { return -1; }
int swap_object() { return -1; }
int unarj_specific_file(char *a,char*b,char*c) { return -1; }
short vga_check_mode(short mode) { return 0; }
int virtual_memory_on() { return -1; }

int c2pstr() { return -1; }
int descent_critical_errcode;
void digi_end_sound(int ch) { }
int digi_set_master_volume() { return -1; }
void gr_ibitblt_create_mask_pa( grs_bitmap * mask_bmp, int sx, int sy, int sw, int sh, int srowsize ) { }
//int joydefs_config() { return -1; }
int joy_have_firebird() { return -1; }
int pa_render_end() { return -1; }
int RBAMountDisk() { return -1; }
int receive_netplayers_packet() { return -1; }
int SavePictScreen() { return -1; }
int send_netgame_packet() { return -1; }
int swapshort() { return -1; }
int tm_warning() { return -1; }
int ch_warning() { return -1; }
void digi_set_channel_pan(int ch, int pan) { }
int joy_have_mousestick() { return -1; }
int RBAEjectDisk() { return -1; }
int send_netplayers_packet() { return -1; }
int gr_set_mode(int mode);
short vga_set_mode(short mode) { return gr_set_mode(mode); }
int game_3dmax_on() { return -1; }
int game_3dmax_off() { return -1; }

int vfx_enable_stereo() { return -1; }
int vfx_disable_stereo() { return -1; }
int vfx1_installed;
int request_cd(){ return -1; }
int vfx_get_data(int*a,int*b,int*c,int*d){return-1;}
int vfx_center_headset(){return -1;}
int CD_blast_mixer(){ return -1; }

#undef mprintf
extern void mprintf( int n, char * format, ... ) { va_list vp; va_start(vp, format); vfprintf(stderr, format, vp); va_end(vp); }
void WinInt3() {
#ifndef EMSCRIPTEN
asm volatile("int $3");
#endif
}

void digi_debug(){}
#ifndef NMONO
void mopen(int n, int row, int col, int width, int height, char * title ){}
void mclose(int n){}
#undef mprintf_at
void mprintf_at(int n, int row, int col, char * format, ...){va_list vp; va_start(vp, format); vfprintf(stderr, format, vp); va_end(vp);}
#endif
int PAEnabled;