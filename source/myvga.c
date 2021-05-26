#include <string.h>
#include "gr.h"
#include "palette.h"
#include "modes.h"
#include "pa_gl.h"

extern ubyte gr_screen_buffer[];
int VGA_current_mode;

int gr_set_mode(int mode) {
	int w,h,t,r;
	int ret;
	if (mode < 0 || mode > sizeof(modes) / sizeof(modes[0]))
		return 1;

	w = modes[mode][0]; r = w; h = modes[mode][1]; t=BM_LINEAR;
	memset(gr_screen_buffer, 0, w * h);
	extern void video_set_res(int w, int h);
	video_set_res(w, h);

	VGA_current_mode = mode;

	gr_palette_clear();

	if ((ret = gr_init_screen( t,w,h,0,0,r,gr_screen_buffer)))
		return ret;
	gl_init_canvas(grd_curcanv);
	return 0;
}
