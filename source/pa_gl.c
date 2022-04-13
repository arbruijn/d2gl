#include "config.h"
#ifdef GL
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <SDL2/SDL_opengl.h>
//#include <SDL2/SDL_opengl_glext.h>
//#define __MACOSX__
#include <SDL2/SDL_opengles2.h>
//#include <SDL2/SDL_opengl.h>
//#undef __MACOSX__
#define GLAPIENTRY
#define GL_DEBUG_TYPE_ERROR               0x824C
#define GL_DEBUG_OUTPUT                   0x92E0
#include "pa_gl.h"

const GLchar* sh_atest_vert =
    "precision mediump float;\n"
    "attribute vec3 pos;\n"
    "attribute vec2 uv;\n"
    "uniform mat4 mat;\n"
    "attribute float light;\n"
    "varying vec2 v_uv;\n"
    "varying float v_light;\n"
    "void main() {\n"
    //" gl_Position = vec4(pos.x / 320.0 - 1.0, 1.0 - pos.y / 240.0, 0.0, 1.0);\n" //pos.x / 320.0 - 320.0, pos.y / 240.0 - 240.0, 0.0, 1.0);\n"
    " gl_Position = mat * vec4(pos, 1.0);\n"
    " v_uv = uv;\n"
    " v_light = light;\n"
    "}";
const GLchar* sh_atest_frag =
    "precision mediump float;\n"
    "varying vec2 v_uv;\n"
    "varying float v_light;\n"
    "uniform sampler2D tex;\n"
    "uniform vec4 color;\n"
    "void main() {\n"
    "  vec4 s = texture2D(tex, v_uv);\n"
    "  if (s.a < 0.5) discard;\n"
    "  gl_FragColor = vec4(s.rgb * color.rgb * v_light, 1.0);\n"
    "}";

GLuint gl_create_shader_part(int tp, const char *src) {
    GLuint id = glCreateShader(tp);
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char msg[512];
        glGetShaderInfoLog(id, sizeof(msg), NULL, msg);
        fprintf(stderr, "%s: %s\n%s\n", tp == GL_VERTEX_SHADER ? "vertex" : "fragment", msg, src);
        abort();
    }
    return id;
}

GLuint gl_create_shader(const char *vert_src, const char *frag_src) {
    GLuint vertex = gl_create_shader_part(GL_VERTEX_SHADER, vert_src);

    GLuint fragment = gl_create_shader_part(GL_FRAGMENT_SHADER, frag_src);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    glLinkProgram(program);
    return program;
}

GLuint glGetAttribLocation_check(GLuint prog, const char *name) {
    int id = glGetAttribLocation(prog, name);
    if (id == -1) {
        fprintf(stderr, "missing attribute %s\n", name);
        //abort();
    }
    return id;
}

GLuint glGetUniformLocation_check(GLuint prog, const char *name) {
    int id = glGetUniformLocation(prog, name);
    if (id == -1) {
        fprintf(stderr, "missing uniform %s\n", name);
        //abort();
    }
    return id;
}

GLuint sh_atest;
GLint sh_atest_pos, sh_atest_uv, sh_atest_color, sh_atest_mat, sh_atest_light;
void gl_init_shaders() {
    sh_atest = gl_create_shader(sh_atest_vert, sh_atest_frag);
    sh_atest_pos = glGetAttribLocation_check(sh_atest, "pos");
    sh_atest_uv = glGetAttribLocation_check(sh_atest, "uv");
    sh_atest_light = glGetAttribLocation_check(sh_atest, "light");
    sh_atest_color = glGetUniformLocation_check(sh_atest, "color");
    sh_atest_mat = glGetUniformLocation_check(sh_atest, "mat");
}

GLuint gl_quad;
void gl_init_buffers() {
    glGenBuffers(1, &gl_quad);
    glBindBuffer(GL_ARRAY_BUFFER, gl_quad);
    float buf[16 * 6];
    memset(buf, 0, sizeof(buf));
    glBufferData(GL_ARRAY_BUFFER, sizeof(buf), buf, GL_DYNAMIC_DRAW);
}

GLuint gl_tex;
void gl_init_textures() {
    glGenTextures(1, &gl_tex);
    int tex_buf = -1;
    glBindTexture(GL_TEXTURE_2D, gl_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &tex_buf);
}

#if 0
void gl_test() {
    // Create Vertex Array Object
    //GLuint vao;
    //glGenVertexArraysOES(1, &vao);
    //glBindVertexArrayOES(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat vertices[] = {0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f};
    //GLfloat vertices[] = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f};

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // Specify the layout of the vertex data
    glUseProgram(sh_atest);
    GLint posAttrib = glGetAttribLocation(sh_atest, "pos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

       // Clear the screen to black
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a triangle from the 3 vertices
        glDrawArrays(GL_TRIANGLES, 0, 3);

}
#endif

int pal[256];
int pal_ok;
#define UP(x) (((x)<<2)|((x)>>4))
void pal_conv(ubyte *pal18, int *pal32, int trans) {
    for (int i = 0; i < 256; i++)
        pal32[i] = UP(pal18[i*3]) | (UP(pal18[i*3+1]) << 8) | (UP(pal18[i*3+2]) << 16) |
            0xff000000;
    if (trans)
        pal32[255] = 0;
}

int *tex_buf;
ubyte *rle_buf;

#define MAX_PIX 640*480
void bm_bind_tex_pal32(grs_bitmap *bm, int *pal32) {
    ubyte *data, *dst, *end;
    int *dsti;
    if (!bm->bm_handle || bm->bm_flags & BM_FLAG_CHANGED) {
        if (!bm->bm_handle) {
            GLuint tex;
            glGenTextures(1, &tex);
            bm->bm_handle = tex;
        }
        glBindTexture(GL_TEXTURE_2D, bm->bm_handle);
        if (bm->bm_w * bm->bm_h > MAX_PIX) {
            printf("ignored texture %d %d\n", bm->bm_w, bm->bm_h);
            return;
        }
        if (bm->bm_flags & BM_FLAG_RLE) {
            data = bm->bm_data + 4 + bm->bm_h *
                (bm->bm_flags & BM_FLAG_RLE_BIG ? 2 : 1);
            dst = rle_buf;
            end = dst + bm->bm_w * bm->bm_h;
            while (dst < end) {
                ubyte c = *data++;
                if (c >= 0xe0) {
                    if (c == 0xe0)
                        continue;
                    int i = c - 0xe0;
                    c = *data++;
                    while (i--)
                        *dst++ = c;
                } else
                    *dst++ = c;
            }
            data = rle_buf;
        } else
            data = bm->bm_data;
        end = data + bm->bm_w * bm->bm_h;
        dsti = tex_buf;
        while (data < end)
            *dsti++ = pal32[*data++];
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bm->bm_w, bm->bm_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_buf);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        bm->bm_flags &= ~BM_FLAG_CHANGED;
    } else
        glBindTexture(GL_TEXTURE_2D, bm->bm_handle);
}

void bm_bind_tex_pal(grs_bitmap *bm, ubyte *pal, int trans) {
    int pal32[256];
    pal_conv(pal, pal32, trans);
    bm_bind_tex_pal32(bm, pal32);
}

void tex_buf_free() {
    free(rle_buf);
    free(tex_buf);
}

void tex_buf_init() {
    tex_buf = malloc(640*480*sizeof(*tex_buf));
    rle_buf = malloc(640*480*sizeof(*rle_buf));
    atexit(tex_buf_free);
}

void pal_init() {
    pal_conv(gr_palette, pal, 1);
    pal_ok = 1;
}

void bm_bind_tex(grs_bitmap *bm) {
    if (!pal_ok)
        pal_init();
    bm_bind_tex_pal32(bm, pal);
}

void gl_free_bitmap(grs_bitmap *bm) {
    if (bm->bm_handle) {
        GLuint tex = bm->bm_handle;
        glDeleteTextures(1, &tex);
    }
}

void gl_draw_tmap(grs_bitmap *bm,int nv,g3s_point **vertlist) {
    bm_bind_tex(bm);
    //glEnable(GL_TEXTURE_2D);
    if (nv > 16) {
        printf("too many verts%d\n", nv);
        abort();
    }
    //if (nv > 4)
    //    nv = 4;

    float buf[16 * 6];
    for (int i = 0; i < nv; i++) {
        g3s_point *p = vertlist[i];
        float x = f2fl(p->p3_x), y = f2fl(p->p3_y), z = -f2fl(p->p3_z);
        buf[i * 6 + 0] = x; buf[i * 6 + 1] = y; buf[i * 6 + 2] = z;
        buf[i * 6 + 3] = f2fl(p->p3_u); buf[i * 6 + 4] = f2fl(p->p3_v);
        float l = f2fl(p->p3_l);
        if (l > 1.0f) {
            //printf("light > 1: %f\n", l);
            l = 1.0f;
        }
        buf[i * 6 + 5] = l;
    }

    #if 1
    
    glUseProgram(sh_atest);
    glBindBuffer(GL_ARRAY_BUFFER, gl_quad);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 6 * nv, buf);
    glVertexAttribPointer(sh_atest_pos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glVertexAttribPointer(sh_atest_uv, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(sh_atest_light, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(5 * sizeof(float)));
    glEnableVertexAttribArray(sh_atest_pos);
    glEnableVertexAttribArray(sh_atest_uv);
    glEnableVertexAttribArray(sh_atest_light);
    glDrawArrays(GL_TRIANGLE_FAN, 0, nv);
    //glUseProgram(0);
   
    
    #else
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glBegin(GL_TRIANGLE_FAN);
    extern int Canv_w2, Canv_h2;
    extern double fCanv_w2, fCanv_h2;
    for (int i = 0; i < nv; i++) {
        g3s_point *p = vertlist[i];
        glTexCoord2f(f2fl(p->p3_u), f2fl(p->p3_v));
        float c = f2fl(p->p3_l);
        glColor3f(c, c, c);
        //glVertex3f(f2fl(p->p3_x), f2fl(p->p3_y), f2fl(p->p3_z));
        //glVertex2f(f2fl(p->p3_sx), f2fl(p->p3_sy)); //, f2fl(p->p3_z));
        float x = f2fl(p->p3_x), y = f2fl(p->p3_y), z = f2fl(p->p3_z);
        //float x = f2fl(p->p3_x)/f2fl(p->p3_z), y = f2fl(p->p3_y)/f2fl(p->p3_z);
        //float x = fCanv_w2+f2fl(p->p3_x)*fCanv_w2/f2fl(p->p3_z), y = fCanv_h2-f2fl(p->p3_y)*fCanv_h2/f2fl(p->p3_z);
        //glVertex2f(fCanv_w2+f2fl(p->p3_x)*fCanv_w2/f2fl(p->p3_z), fCanv_h2+f2fl(p->p3_y)*fCanv_h2/f2fl(p->p3_z)[A); //, f2fl(p->p3_z));
        //glVertex3f(x, y, 0); //kxmakf2fl(p->p3_z));
        //glVertex4f(x, y, 0, z);
        glVertex3f(x, y, -z);
        //printf("%f %f %f s %f %f m %f %f\n", f2fl(p->p3_x), f2fl(p->p3_y), f2fl(p->p3_z), f2fl(p->p3_sx), f2fl(p->p3_sy), x, y);
        
    }
    glEnd();
    #endif
}

void gl_draw_rod(g3s_point *pnt, fix width, fix height, grs_bitmap *bm, int o) {
    bm_bind_tex(bm);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    float x = f2fl(pnt->p3_x), y = f2fl(pnt->p3_y), z = -f2fl(pnt->p3_z), w = f2fl(width), h = f2fl(height);
    float buf[] = {
        x - w, y - h, z, 0, 1, 1,
        x + w, y - h, z, 1, 1, 1,
        x + w, y + h, z, 1, 0, 1,
        x - w, y + h, z, 0, 0, 1 };

    glUseProgram(sh_atest);
    glBindBuffer(GL_ARRAY_BUFFER, gl_quad);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(buf), buf);
    glVertexAttribPointer(sh_atest_pos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glVertexAttribPointer(sh_atest_uv, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(sh_atest_light, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(5 * sizeof(float)));
    glEnableVertexAttribArray(sh_atest_pos);
    glEnableVertexAttribArray(sh_atest_uv);
    glEnableVertexAttribArray(sh_atest_light);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glUseProgram(0);

    #if 0
    glEnable(GL_TEXTURE_2D);
    glAlphaFunc(GL_GREATER, 0.5);
    glEnable(GL_ALPHA_TEST);
    glColor3f(1.0f, 1.0f, 1.0f);
    //glBegin(GL_POLYGON);
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x - w, y - h, z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x + w, y - h, z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x + w, y + h, z);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x - w, y + h, z);
    glEnd();
    #endif
    //printf("bitmap %f %f %f - %f %f\n", x, y, z, w, h);
}

void gl_get_rgb(ubyte color, float *r, float *g, float *b) {
    if (!pal_ok)
        pal_init();
    int cc = pal[color];
    *r = (cc & 255) / 255.0f;
    *g = ((cc >> 8) & 255) / 255.0f;
    *b = ((cc >> 16) & 255) / 255.0f;
}

void gl_draw_flat(ubyte color, int nv, g3s_point**vertlist) {
    float r, g, b;
    gl_get_rgb(color, &r, &g, &b);
    //glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gl_tex);
    
    if (nv > 16)
        abort();

    float buf[16 * 6];
    for (int i = 0; i < nv; i++) {
        g3s_point *p = vertlist[i];
        float x = f2fl(p->p3_x), y = f2fl(p->p3_y), z = -f2fl(p->p3_z);
        buf[i * 6 + 0] = x; buf[i * 6 + 1] = y; buf[i * 6 + 2] = z;
        buf[i * 6 + 3] = 0; buf[i * 6 + 4] = 0; buf[i * 6 + 5] = 1;
    }

    glUseProgram(sh_atest);
    if (Gr_scanline_darkening_level >= GR_FADE_LEVELS) {
        glUniform4f(sh_atest_color, r, g, b, 1);
    } else {
        float c = (float)(GR_FADE_LEVELS - Gr_scanline_darkening_level)/(float)GR_FADE_LEVELS;
        glUniform4f(sh_atest_color, 0, 0, 0, c);
    }
    glBindBuffer(GL_ARRAY_BUFFER, gl_quad);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(buf), buf);
    glVertexAttribPointer(sh_atest_pos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glVertexAttribPointer(sh_atest_uv, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(sh_atest_light, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(5 * sizeof(float)));
    glEnableVertexAttribArray(sh_atest_pos);
    glEnableVertexAttribArray(sh_atest_uv);
    glEnableVertexAttribArray(sh_atest_light);
    glDrawArrays(GL_TRIANGLE_FAN, 0, nv);
    glUniform4f(sh_atest_color, 1, 1, 1, 1);
    glUseProgram(0);

    #if 0
    glDisable(GL_BLEND);
    glColor3f(c * r, c * g, c * b);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < nv; i++) {
        g3s_point *p = pnts[i];
        glVertex3f(f2fl(p->p3_x), f2fl(p->p3_y), -f2fl(p->p3_z));
    }
    glEnd();
    #endif
}
void gl_draw_line(fix x0,fix y0,fix x1,fix y1) {
}

#include "grdef.h"
float proj[16] = { 1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, -1, -1,
    0, 0, -0.02, 0 };

extern int cur_w, cur_h;
int gl_w, gl_h;

void gl_start_frame() {
    float ws = (float)gl_w / (float)cur_w;
    float hs = (float)gl_h / (float)cur_h;
    glViewport(XOFFSET * ws, gl_h - (HEIGHT + YOFFSET) * hs, WIDTH * ws, HEIGHT * hs);
    //glViewport(XOFFSET * ws, gl_h - YOFFSET * hs, WIDTH * ws, HEIGHT * hs);
    #if 0
    glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //printf("start frame %d %d\n", WIDTH, HEIGHT);
    //double s = (WIDTH * 3.0) / (HEIGHT * 4.0);
    //double r = 0.01, t = 0.01 * 1.0;
    //glFrustum(-r, r, -t, t, 0.01, 5000.0);
    //printf("gl 3d\n");
    glLoadMatrixf(proj);
    #endif
    glUseProgram(sh_atest);
    glUniformMatrix4fv(sh_atest_mat, 1, GL_FALSE, proj);
    glUniform4f(sh_atest_color, 1, 1, 1, 1);
    glUseProgram(0);
}

float ortho[16] = {
    2.0f/640, 0, 0, 0,
    0, 2.0f/-480, 0, 0,
    0, 0, -1, 0,
    -1, 1, 0, 1};
void gl_end_frame() {
    //printf("gl_w %d gl_h %d curw %d curh %d\n", gl_w, gl_h, cur_w, cur_h);
    ortho[0] = 2.0f/cur_w;
    ortho[4+1] = 2.0f/-cur_h;
    glViewport(0, 0, gl_w, gl_h);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glOrtho(0, 640, 480, 0, -1, 1);
    //printf("gl 2d\n");
    //glLoadMatrixf(ortho);
    glUseProgram(sh_atest);
    glUniformMatrix4fv(sh_atest_mat, 1, GL_FALSE, ortho);
}

#if 0
void gl_draw_bitmap_2d(int sx, int sy, grs_bitmap *bm) {
    bm_bind_tex(bm);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glEnable(GL_TEXTURE_2D);
    glAlphaFunc(GL_GREATER, 0.5);
    glEnable(GL_ALPHA_TEST);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    float x = sx, y = sy, w = bm->bm_w, h = bm->bm_h;
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(x + w, y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(x + w, y + h);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(x, y + h);
    glEnd();
    //printf("bm_gl %f %f %f %f\n", x, y, w, h);
}
#endif

void gl_ubitblt(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap *bm) {
    float bw = bm->bm_w, bh = bm->bm_h;
    float x = dx, y = dy;
    float u0 = (float)sx / bw, u1 = (float)(sx + w) / bw;
    float v0 = (float)sy / bh, v1 = (float)(sy + h) / bh;

    float buf[] = {x, y, 0, u0, v0, 1,
        x + w, y, 0, u1, v0, 1,
        x + w, y + h, 0, u1, v1, 1,
        x, y + h, 0, u0, v1, 1 };
    bm_bind_tex(bm);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glUseProgram(sh_atest);
    glUniform4f(sh_atest_color, 1.0f, 1.0f, 1.0f, 1.0f);
    glBindBuffer(GL_ARRAY_BUFFER, gl_quad);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(buf), buf);
    glVertexAttribPointer(sh_atest_pos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glVertexAttribPointer(sh_atest_uv, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(sh_atest_light, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(5 * sizeof(float)));
    glEnableVertexAttribArray(sh_atest_pos);
    glEnableVertexAttribArray(sh_atest_uv);
    glEnableVertexAttribArray(sh_atest_light);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glUseProgram(0);
    #if 0
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glEnable(GL_TEXTURE_2D);
    glAlphaFunc(GL_GREATER, 0.5);
    glEnable(GL_ALPHA_TEST);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
 
    glTexCoord2f(u0, v0);
    glVertex2f(x, y);
    glTexCoord2f(u1, v0);
    glVertex2f(x + w, y);
    glTexCoord2f(u1, v1);
    glVertex2f(x + w, y + h);
    glTexCoord2f(u0, v1);
    glVertex2f(x, y + h);
    glEnd();
    #endif
}

void gl_draw_bitmap_2d(int sx, int sy, grs_bitmap *bm) {
    gl_ubitblt(bm->bm_w, bm->bm_h, sx, sy, 0, 0, bm);
}

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam ) {
    //if (severity == 0x826b) return;
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

void gl_init_canvas(grs_canvas *canv) {
    canv->cv_bitmap.bm_type = BM_GL;
}

void bit_split(ubyte *src, ubyte *dst, ubyte c, int w) {
    ubyte v = *src++;
    for (int i = 128; w--; dst++) {
        if (v & i)
            *dst = c;
        if (!(i >>= 1)) {
            i = 128;
            v = *src++;
        }
    }
}

void gl_init_font(grs_font *font) {
    int color = font->ft_flags & FT_COLOR;
    int tex_w = 512;
    int nchars = (font->ft_maxchar - font->ft_minchar) + 1;
    int ncol = tex_w / font->ft_w;
    int nrow = (nchars + ncol - 1) / ncol;
    int tex_h = nrow * (font->ft_h + 1);
    font->ft_bitmap.bm_w = font->ft_bitmap.bm_rowsize = tex_w;
    font->ft_bitmap.bm_h = tex_h;
    font->ft_bitmap.bm_flags = 0;
    if (!(font->ft_bitmap.bm_data = malloc(tex_w * tex_h)))
        abort();
    ubyte *dst = font->ft_bitmap.bm_data;
    memset(dst, color ? 255 : 0, tex_w * tex_h);
    for (int i = 0, ci = 0; i < nchars; i++) {
        ubyte *src = font->ft_chars[i];
        //if (!color)
        //    printf("%2d %02x  ", i, *src);
        //printf("@%8x ", dst - font->ft_bitmap.bm_data);
        //printf("%c: ", i + font->ft_minchar);
        int w = font->ft_widths[i];
        for (int j = 0; j < font->ft_h; j++) {
            if (color) {
                memcpy(dst + tex_w * j, src, w);
                src += w;
            } else {
                //printf("%02x  ", *src);
                bit_split(src, dst + tex_w * j, 0xff, w);
                src += (w + 7) / 8;
            }
        }
        //printf("\n");
        dst += font->ft_w;
        if (++ci == ncol) {
            ci = 0;
            dst -= font->ft_w * ncol;
            dst += (font->ft_h + 1) * tex_w;
        }
    }
    font->ft_bm_row_chars = ncol;
    //if (!(font->ft_flags & FT_COLOR))
    //    pcx_write_bitmap("monofnt.pcx", &font->ft_bitmap, gr_palette);
}

void gl_done_font(grs_font *font) {
    free(font->ft_bitmap.bm_data);
}

void gl_font_start(grs_font *font, ubyte color) {
    if (!font->ft_bitmap.bm_handle) {
        int fontpal[256];
        if (!pal_ok)
            pal_init();
        memcpy(fontpal, pal, sizeof(fontpal));
        if (!(font->ft_flags & FT_COLOR)) {
            fontpal[0] = 0;
            fontpal[255] = 0xffffffff;
        }
        bm_bind_tex_pal32(&font->ft_bitmap, fontpal);
    } else
        glBindTexture(GL_TEXTURE_2D, font->ft_bitmap.bm_handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glEnable(GL_TEXTURE_2D);
    #if 0
    glAlphaFunc(GL_GREATER, 0.5);
    glEnable(GL_ALPHA_TEST);
    #endif
    float r, g, b;
    if (!(font->ft_flags & FT_COLOR)) {
        gl_get_rgb(color, &r, &g, &b);
    } else
        r = g = b = 1.0f;
    //glColor3f(r, g, b);
    glUseProgram(sh_atest);
    glUniform4f(sh_atest_color, r, g, b, 1.0f);
    glBindBuffer(GL_ARRAY_BUFFER, gl_quad);
    glDisableVertexAttribArray(sh_atest_light);
    glVertexAttrib1f(sh_atest_light, 1);
}

void gl_draw_char(int sx, int sy, grs_font *font, int c) {
    c -= font->ft_minchar;
    if (c < 0 || c > font->ft_maxchar)
        return;
    float x = sx, y = sy, w = font->ft_widths[c], h = font->ft_h;
    int bx = (c % font->ft_bm_row_chars) * font->ft_w;
    int by = (c / font->ft_bm_row_chars) * (font->ft_h + 1);
    float u0 = (float)bx / font->ft_bitmap.bm_w;
    float u1 = (float)(bx + w) / font->ft_bitmap.bm_w;
    float v0 = (float)by / font->ft_bitmap.bm_h;
    float v1 = (float)(by + h) / font->ft_bitmap.bm_h;
    float buf[] = {x, y, 0, u0, v0, 1,
        x + w, y, 0, u1, v0, 1,
        x + w, y + h, 0, u1, v1, 1,
        x, y + h, 0, u0, v1, 1 };
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(buf), buf);
    glVertexAttribPointer(sh_atest_pos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0); //sizeof(float) * 2);
    glVertexAttribPointer(sh_atest_uv, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    //glVertexAttribPointer(sh_atest_light, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(5 * sizeof(float)));
    glEnableVertexAttribArray(sh_atest_pos);
    glEnableVertexAttribArray(sh_atest_uv);
    //glEnableVertexAttribArray(sh_atest_light);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    #if 0
    glUseProgram(sh_atest);
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(u0, v0);
    glVertex2f(x, y);
    glTexCoord2f(u1, v0);
    glVertex2f(x + w, y);
    glTexCoord2f(u1, v1);
    glVertex2f(x + w, y + h);
    glTexCoord2f(u0, v1);
    glVertex2f(x, y + h);
    glEnd();
    #endif
}

void gl_font_end() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

void gl_init_viewport() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    gl_w = viewport[2];
    gl_h = viewport[3];
    printf("gl screen size %d %d\n", gl_w, gl_h);
    gl_end_frame(); // set ortho
}

void gl_init() {
    #ifndef EMSCRIPTEN
    // During init, enable debug output
    glEnable              ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );
    #endif
    gl_init_shaders();
    gl_init_buffers();
    gl_init_textures();
    tex_buf_init();
    gl_init_viewport();
}

void gl_set_screen_size(int w, int h) {
    gl_w = w;
    gl_h = h;
    gl_end_frame();
}

void gl_pal_changed() {
    pal_ok = 0;
}

void gl_clear() {
    if (!pal_ok)
        pal_init();
    unsigned int c = (unsigned int)pal[COLOR];
    glClearColor(((c >> 16) & 255) / 255.0f,
        ((c >> 8) & 255) / 255.0f, (c & 255) / 255.0f,
        (c >> 24) / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
#else
#include "gr.h"
#include "3d.h"
void gl_free_bitmap(grs_bitmap *bm) {}
void gl_draw_tmap(grs_bitmap *bm,int nv,g3s_point **vertlist) {}
void bm_bind_tex(grs_bitmap *bm) {}
void bm_bind_tex_pal(grs_bitmap *bm, ubyte *pal, int trans) {}
void gl_draw_rod(g3s_point *pnt, fix width, fix height, grs_bitmap *bm, int o) {}
void gl_get_rgb(ubyte color, float *r, float *g, float *b) {}
void gl_draw_flat(ubyte color, int nv, g3s_point**pnts) {}
void gl_start_frame() {}
void gl_end_frame() {}
void gl_draw_bitmap_2d(int sx, int sy, grs_bitmap *bm) {}
void gl_ubitblt(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap *bm) {}
void gl_font_start(grs_font *font, ubyte color) {}
void gl_draw_char(int sx, int sy, grs_font *font, int c) {}
void gl_init_font(grs_font *font) {}
void gl_done_font() {}
void gl_init_canvas(grs_canvas *canv) {}
void gl_init() {}
void gl_font_end() {}
void gl_pal_changed() {}
void gl_set_screen_size(int x, int y) {}
void gl_clear() {}
#endif
