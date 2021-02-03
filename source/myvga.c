#include "gr.h"
#include "modes.h"
extern ubyte gr_screen_buffer[];

int gr_set_mode(int mode) {
	unsigned int w,h,t,r;
	if (mode < 0 || mode > sizeof(modes) / sizeof(modes[0]))
		return 1;

	w = modes[mode][0]; r = w; h = modes[mode][1]; t=BM_LINEAR;
	memset(gr_screen_buffer, 0, w * h);
	extern void video_set_res(int w, int h);
	video_set_res(w, h);

	gr_palette_clear();

	return gr_init_screen( t,w,h,0,0,r,gr_screen_buffer);
}
