#include "gr.h"
#include "3d.h"

void gl_free_bitmap(grs_bitmap *bm);
void gl_draw_tmap(grs_bitmap *bm,int nv,g3s_point **vertlist);
void bm_bind_tex(grs_bitmap *bm);
void bm_bind_tex_pal(grs_bitmap *bm, ubyte *pal, int trans);
void gl_draw_rod(g3s_point *pnt, fix width, fix height, grs_bitmap *bm, int o);
void gl_get_rgb(ubyte color, float *r, float *g, float *b);
void gl_draw_flat(ubyte color, int nv, g3s_point**pnts);
void gl_start_frame();
void gl_end_frame();
void gl_draw_bitmap_2d(int sx, int sy, grs_bitmap *bm);
void gl_ubitblt(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap *bm);
void gl_font_start(grs_font *font, ubyte color);
void gl_draw_char(int sx, int sy, grs_font *font, int c);
void gl_font_end();
void gl_init_font(grs_font *font);
void gl_done_font(grs_font *font);
void gl_init_canvas(grs_canvas *canv);
void gl_init();
void gl_pal_changed();
void gl_set_screen_size(int, int);
void gl_clear();
void gl_line_2d(ubyte color, int, int, int, int);
void gl_line(ubyte color, g3s_point *p0, g3s_point *p1);
