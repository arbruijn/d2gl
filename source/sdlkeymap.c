//#include "sdl2/include/SDL.h"
#include "SDL2/SDL.h"
#define SDL2
#include "sdlkeymap.h"

enum PCKey {
	KEY_0 = 0x0B,
	KEY_1 = 0x02,
	KEY_2 = 0x03,
	KEY_3 = 0x04,
	KEY_4 = 0x05,
	KEY_5 = 0x06,
	KEY_6 = 0x07,
	KEY_7 = 0x08,
	KEY_8 = 0x09,
	KEY_9 = 0x0A,

	KEY_A = 0x1E,
	KEY_B = 0x30,
	KEY_C = 0x2E,
	KEY_D = 0x20,
	KEY_E = 0x12,
	KEY_F = 0x21,
	KEY_G = 0x22,
	KEY_H = 0x23,
	KEY_I = 0x17,
	KEY_J = 0x24,
	KEY_K = 0x25,
	KEY_L = 0x26,
	KEY_M = 0x32,
	KEY_N = 0x31,
	KEY_O = 0x18,
	KEY_P = 0x19,
	KEY_Q = 0x10,
	KEY_R = 0x13,
	KEY_S = 0x1F,
	KEY_T = 0x14,
	KEY_U = 0x16,
	KEY_V = 0x2F,
	KEY_W = 0x11,
	KEY_X = 0x2D,
	KEY_Y = 0x15,
	KEY_Z = 0x2C,

	KEY_MINUS = 0x0C,
	KEY_EQUAL = 0x0D,
	KEY_DIVIDE = 0x35,
	KEY_SLASH = 0x2B,
	KEY_COMMA = 0x33,
	KEY_PERIOD = 0x34,
	KEY_SEMICOL = 0x27,

	KEY_LBRACKET = 0x1A,
	KEY_RBRACKET = 0x1B,

	KEY_RAPOSTRO = 0x28,
	KEY_LAPOSTRO = 0x29,

	KEY_ESC = 0x01,
	KEY_ENTER = 0x1C,
	KEY_BACKSP = 0x0E,
	KEY_TAB = 0x0F,
	KEY_SPACEBAR = 0x39,

	KEY_NUMLOCK = 0x45,
	KEY_SCROLLOCK = 0x46,
	KEY_CAPSLOCK = 0x3A,

	KEY_LSHIFT = 0x2A,
	KEY_RSHIFT = 0x36,

	KEY_LALT = 0x38,
	KEY_RALT = 0xB8,

	KEY_LCTRL = 0x1D,
	KEY_RCTRL = 0x9D,

	KEY_F1 = 0x3B,
	KEY_F2 = 0x3C,
	KEY_F3 = 0x3D,
	KEY_F4 = 0x3E,
	KEY_F5 = 0x3F,
	KEY_F6 = 0x40,
	KEY_F7 = 0x41,
	KEY_F8 = 0x42,
	KEY_F9 = 0x43,
	KEY_F10 = 0x44,
	KEY_F11 = 0x57,
	KEY_F12 = 0x58,

	KEY_PAD0 = 0x52,
	KEY_PAD1 = 0x4F,
	KEY_PAD2 = 0x50,
	KEY_PAD3 = 0x51,
	KEY_PAD4 = 0x4B,
	KEY_PAD5 = 0x4C,
	KEY_PAD6 = 0x4D,
	KEY_PAD7 = 0x47,
	KEY_PAD8 = 0x48,
	KEY_PAD9 = 0x49,
	KEY_PADMINUS = 0x4A,
	KEY_PADPLUS = 0x4E,
	KEY_PADPERIOD = 0x53,
	KEY_PADDIVIDE = 0xB5,
	KEY_PADMULTIPLY = 0x37,
	KEY_PADENTER = 0x9C,

	KEY_INSERT = 0xD2,
	KEY_HOME = 0xC7,
	KEY_PAGEUP = 0xC9,
	KEY_DELETE = 0xD3,
	KEY_END = 0xCF,
	KEY_PAGEDOWN = 0xD1,
	KEY_UP = 0xC8,
	KEY_DOWN = 0xD0,
	KEY_LEFT = 0xCB,
	KEY_RIGHT = 0xCD,

	KEY_PRINT_SCREEN = 0xB7,
	KEY_PAUSE = 0x61
};
#ifdef SDL2
unsigned char sdlkeymap[SDL_NUM_SCANCODES];
#else
unsigned char sdlkeymap[SDLK_LAST];
#endif
void InitKeyMap() {

#ifdef SDL2
	sdlkeymap[SDL_SCANCODE_0] = KEY_0;
	sdlkeymap[SDL_SCANCODE_1] = KEY_1;
	sdlkeymap[SDL_SCANCODE_2] = KEY_2;
	sdlkeymap[SDL_SCANCODE_3] = KEY_3;
	sdlkeymap[SDL_SCANCODE_4] = KEY_4;
	sdlkeymap[SDL_SCANCODE_5] = KEY_5;
	sdlkeymap[SDL_SCANCODE_6] = KEY_6;
	sdlkeymap[SDL_SCANCODE_7] = KEY_7;
	sdlkeymap[SDL_SCANCODE_8] = KEY_8;
	sdlkeymap[SDL_SCANCODE_9] = KEY_9;

	sdlkeymap[SDL_SCANCODE_A] = KEY_A;
	sdlkeymap[SDL_SCANCODE_B] = KEY_B;
	sdlkeymap[SDL_SCANCODE_C] = KEY_C;
	sdlkeymap[SDL_SCANCODE_D] = KEY_D;
	sdlkeymap[SDL_SCANCODE_E] = KEY_E;
	sdlkeymap[SDL_SCANCODE_F] = KEY_F;
	sdlkeymap[SDL_SCANCODE_G] = KEY_G;
	sdlkeymap[SDL_SCANCODE_H] = KEY_H;
	sdlkeymap[SDL_SCANCODE_I] = KEY_I;
	sdlkeymap[SDL_SCANCODE_J] = KEY_J;
	sdlkeymap[SDL_SCANCODE_K] = KEY_K;
	sdlkeymap[SDL_SCANCODE_L] = KEY_L;
	sdlkeymap[SDL_SCANCODE_M] = KEY_M;
	sdlkeymap[SDL_SCANCODE_N] = KEY_N;
	sdlkeymap[SDL_SCANCODE_O] = KEY_O;
	sdlkeymap[SDL_SCANCODE_P] = KEY_P;
	sdlkeymap[SDL_SCANCODE_Q] = KEY_Q;
	sdlkeymap[SDL_SCANCODE_R] = KEY_R;
	sdlkeymap[SDL_SCANCODE_S] = KEY_S;
	sdlkeymap[SDL_SCANCODE_T] = KEY_T;
	sdlkeymap[SDL_SCANCODE_U] = KEY_U;
	sdlkeymap[SDL_SCANCODE_V] = KEY_V;
	sdlkeymap[SDL_SCANCODE_W] = KEY_W;
	sdlkeymap[SDL_SCANCODE_X] = KEY_X;
	sdlkeymap[SDL_SCANCODE_Y] = KEY_Y;
	sdlkeymap[SDL_SCANCODE_Z] = KEY_Z;

	sdlkeymap[SDL_SCANCODE_MINUS] = KEY_MINUS;
	sdlkeymap[SDL_SCANCODE_EQUALS] = KEY_EQUAL;
	sdlkeymap[SDL_SCANCODE_SLASH] = KEY_DIVIDE;
	sdlkeymap[SDL_SCANCODE_BACKSLASH] = KEY_SLASH;
	sdlkeymap[SDL_SCANCODE_COMMA] = KEY_COMMA;
	sdlkeymap[SDL_SCANCODE_PERIOD] = KEY_PERIOD;
	sdlkeymap[SDL_SCANCODE_SEMICOLON] = KEY_SEMICOL;

	sdlkeymap[SDL_SCANCODE_LEFTBRACKET] = KEY_LBRACKET;
	sdlkeymap[SDL_SCANCODE_RIGHTBRACKET] = KEY_RBRACKET;

	sdlkeymap[SDL_SCANCODE_GRAVE] = KEY_RAPOSTRO;
	sdlkeymap[SDL_SCANCODE_APOSTROPHE] = KEY_LAPOSTRO;

	sdlkeymap[SDL_SCANCODE_ESCAPE] = KEY_ESC;
	sdlkeymap[SDL_SCANCODE_RETURN] = KEY_ENTER;
	sdlkeymap[SDL_SCANCODE_BACKSPACE] = KEY_BACKSP;
	sdlkeymap[SDL_SCANCODE_TAB] = KEY_TAB;
	sdlkeymap[SDL_SCANCODE_SPACE] = KEY_SPACEBAR;

	sdlkeymap[SDL_SCANCODE_NUMLOCKCLEAR] = KEY_NUMLOCK;
	sdlkeymap[SDL_SCANCODE_SCROLLLOCK] = KEY_SCROLLOCK;
	sdlkeymap[SDL_SCANCODE_CAPSLOCK] = KEY_CAPSLOCK;

	sdlkeymap[SDL_SCANCODE_LSHIFT] = KEY_LSHIFT;
	sdlkeymap[SDL_SCANCODE_RSHIFT] = KEY_RSHIFT;

	sdlkeymap[SDL_SCANCODE_LALT] = KEY_LALT;
	sdlkeymap[SDL_SCANCODE_RALT] = KEY_RALT;

	sdlkeymap[SDL_SCANCODE_LCTRL] = KEY_LCTRL;
	sdlkeymap[SDL_SCANCODE_RCTRL] = KEY_RCTRL;

	sdlkeymap[SDL_SCANCODE_F1] = KEY_F1;
	sdlkeymap[SDL_SCANCODE_F2] = KEY_F2;
	sdlkeymap[SDL_SCANCODE_F3] = KEY_F3;
	sdlkeymap[SDL_SCANCODE_F4] = KEY_F4;
	sdlkeymap[SDL_SCANCODE_F5] = KEY_F5;
	sdlkeymap[SDL_SCANCODE_F6] = KEY_F6;
	sdlkeymap[SDL_SCANCODE_F7] = KEY_F7;
	sdlkeymap[SDL_SCANCODE_F8] = KEY_F8;
	sdlkeymap[SDL_SCANCODE_F9] = KEY_F9;
	sdlkeymap[SDL_SCANCODE_F10] = KEY_F10;
	sdlkeymap[SDL_SCANCODE_F11] = KEY_F11;
	sdlkeymap[SDL_SCANCODE_F12] = KEY_F12;

	sdlkeymap[SDL_SCANCODE_KP_0] = KEY_PAD0;
	sdlkeymap[SDL_SCANCODE_KP_1] = KEY_PAD1;
	sdlkeymap[SDL_SCANCODE_KP_2] = KEY_PAD2;
	sdlkeymap[SDL_SCANCODE_KP_3] = KEY_PAD3;
	sdlkeymap[SDL_SCANCODE_KP_4] = KEY_PAD4;
	sdlkeymap[SDL_SCANCODE_KP_5] = KEY_PAD5;
	sdlkeymap[SDL_SCANCODE_KP_6] = KEY_PAD6;
	sdlkeymap[SDL_SCANCODE_KP_7] = KEY_PAD7;
	sdlkeymap[SDL_SCANCODE_KP_8] = KEY_PAD8;
	sdlkeymap[SDL_SCANCODE_KP_9] = KEY_PAD9;
	sdlkeymap[SDL_SCANCODE_KP_MINUS] = KEY_PADMINUS;
	sdlkeymap[SDL_SCANCODE_KP_PLUS] = KEY_PADPLUS;
	sdlkeymap[SDL_SCANCODE_KP_PERIOD] = KEY_PADPERIOD;
	sdlkeymap[SDL_SCANCODE_KP_DIVIDE] = KEY_PADDIVIDE;
	sdlkeymap[SDL_SCANCODE_KP_MULTIPLY] = KEY_PADMULTIPLY;
	sdlkeymap[SDL_SCANCODE_KP_ENTER] = KEY_PADENTER;

	sdlkeymap[SDL_SCANCODE_INSERT] = KEY_INSERT;
	sdlkeymap[SDL_SCANCODE_HOME] = KEY_HOME;
	sdlkeymap[SDL_SCANCODE_PAGEUP] = KEY_PAGEUP;
	sdlkeymap[SDL_SCANCODE_DELETE] = KEY_DELETE;
	sdlkeymap[SDL_SCANCODE_END] = KEY_END;
	sdlkeymap[SDL_SCANCODE_PAGEDOWN] = KEY_PAGEDOWN;
	sdlkeymap[SDL_SCANCODE_UP] = KEY_UP;
	sdlkeymap[SDL_SCANCODE_DOWN] = KEY_DOWN;
	sdlkeymap[SDL_SCANCODE_LEFT] = KEY_LEFT;
	sdlkeymap[SDL_SCANCODE_RIGHT] = KEY_RIGHT;

	sdlkeymap[SDL_SCANCODE_PRINTSCREEN] = KEY_PRINT_SCREEN;
	sdlkeymap[SDL_SCANCODE_PAUSE] = KEY_PAUSE;
#else
	sdlkeymap[SDLK_0] = KEY_0;
	sdlkeymap[SDLK_1] = KEY_1;
	sdlkeymap[SDLK_2] = KEY_2;
	sdlkeymap[SDLK_3] = KEY_3;
	sdlkeymap[SDLK_4] = KEY_4;
	sdlkeymap[SDLK_5] = KEY_5;
	sdlkeymap[SDLK_6] = KEY_6;
	sdlkeymap[SDLK_7] = KEY_7;
	sdlkeymap[SDLK_8] = KEY_8;
	sdlkeymap[SDLK_9] = KEY_9;

	sdlkeymap[SDLK_a] = KEY_A;
	sdlkeymap[SDLK_b] = KEY_B;
	sdlkeymap[SDLK_c] = KEY_C;
	sdlkeymap[SDLK_d] = KEY_D;
	sdlkeymap[SDLK_e] = KEY_E;
	sdlkeymap[SDLK_f] = KEY_F;
	sdlkeymap[SDLK_g] = KEY_G;
	sdlkeymap[SDLK_h] = KEY_H;
	sdlkeymap[SDLK_i] = KEY_I;
	sdlkeymap[SDLK_j] = KEY_J;
	sdlkeymap[SDLK_k] = KEY_K;
	sdlkeymap[SDLK_l] = KEY_L;
	sdlkeymap[SDLK_m] = KEY_M;
	sdlkeymap[SDLK_n] = KEY_N;
	sdlkeymap[SDLK_o] = KEY_O;
	sdlkeymap[SDLK_p] = KEY_P;
	sdlkeymap[SDLK_q] = KEY_Q;
	sdlkeymap[SDLK_r] = KEY_R;
	sdlkeymap[SDLK_s] = KEY_S;
	sdlkeymap[SDLK_t] = KEY_T;
	sdlkeymap[SDLK_u] = KEY_U;
	sdlkeymap[SDLK_v] = KEY_V;
	sdlkeymap[SDLK_w] = KEY_W;
	sdlkeymap[SDLK_x] = KEY_X;
	sdlkeymap[SDLK_y] = KEY_Y;
	sdlkeymap[SDLK_z] = KEY_Z;

	sdlkeymap[SDLK_MINUS] = KEY_MINUS;
	sdlkeymap[SDLK_EQUALS] = KEY_EQUAL;
	sdlkeymap[SDLK_SLASH] = KEY_DIVIDE;
	sdlkeymap[SDLK_BACKSLASH] = KEY_SLASH;
	sdlkeymap[SDLK_COMMA] = KEY_COMMA;
	sdlkeymap[SDLK_PERIOD] = KEY_PERIOD;
	sdlkeymap[SDLK_SEMICOLON] = KEY_SEMICOL;

	sdlkeymap[SDLK_LEFTBRACKET] = KEY_LBRACKET;
	sdlkeymap[SDLK_RIGHTBRACKET] = KEY_RBRACKET;

	sdlkeymap[SDLK_BACKQUOTE] = KEY_RAPOSTRO;
	sdlkeymap[SDLK_QUOTE] = KEY_LAPOSTRO;

	sdlkeymap[SDLK_ESCAPE] = KEY_ESC;
	sdlkeymap[SDLK_RETURN] = KEY_ENTER;
	sdlkeymap[SDLK_BACKSPACE] = KEY_BACKSP;
	sdlkeymap[SDLK_TAB] = KEY_TAB;
	sdlkeymap[SDLK_SPACE] = KEY_SPACEBAR;

	sdlkeymap[SDLK_NUMLOCK] = KEY_NUMLOCK;
	sdlkeymap[SDLK_SCROLLOCK] = KEY_SCROLLOCK;
	sdlkeymap[SDLK_CAPSLOCK] = KEY_CAPSLOCK;

	sdlkeymap[SDLK_LSHIFT] = KEY_LSHIFT;
	sdlkeymap[SDLK_RSHIFT] = KEY_RSHIFT;

	sdlkeymap[SDLK_LALT] = KEY_LALT;
	sdlkeymap[SDLK_RALT] = KEY_RALT;

	sdlkeymap[SDLK_LCTRL] = KEY_LCTRL;
	sdlkeymap[SDLK_RCTRL] = KEY_RCTRL;

	sdlkeymap[SDLK_F1] = KEY_F1;
	sdlkeymap[SDLK_F2] = KEY_F2;
	sdlkeymap[SDLK_F3] = KEY_F3;
	sdlkeymap[SDLK_F4] = KEY_F4;
	sdlkeymap[SDLK_F5] = KEY_F5;
	sdlkeymap[SDLK_F6] = KEY_F6;
	sdlkeymap[SDLK_F7] = KEY_F7;
	sdlkeymap[SDLK_F8] = KEY_F8;
	sdlkeymap[SDLK_F9] = KEY_F9;
	sdlkeymap[SDLK_F10] = KEY_F10;
	sdlkeymap[SDLK_F11] = KEY_F11;
	sdlkeymap[SDLK_F12] = KEY_F12;

	sdlkeymap[SDLK_KP0] = KEY_PAD0;
	sdlkeymap[SDLK_KP1] = KEY_PAD1;
	sdlkeymap[SDLK_KP2] = KEY_PAD2;
	sdlkeymap[SDLK_KP3] = KEY_PAD3;
	sdlkeymap[SDLK_KP4] = KEY_PAD4;
	sdlkeymap[SDLK_KP5] = KEY_PAD5;
	sdlkeymap[SDLK_KP6] = KEY_PAD6;
	sdlkeymap[SDLK_KP7] = KEY_PAD7;
	sdlkeymap[SDLK_KP8] = KEY_PAD8;
	sdlkeymap[SDLK_KP9] = KEY_PAD9;
	sdlkeymap[SDLK_KP_MINUS] = KEY_PADMINUS;
	sdlkeymap[SDLK_KP_PLUS] = KEY_PADPLUS;
	sdlkeymap[SDLK_KP_PERIOD] = KEY_PADPERIOD;
	sdlkeymap[SDLK_KP_DIVIDE] = KEY_PADDIVIDE;
	sdlkeymap[SDLK_KP_MULTIPLY] = KEY_PADMULTIPLY;
	sdlkeymap[SDLK_KP_ENTER] = KEY_PADENTER;

	sdlkeymap[SDLK_INSERT] = KEY_INSERT;
	sdlkeymap[SDLK_HOME] = KEY_HOME;
	sdlkeymap[SDLK_PAGEUP] = KEY_PAGEUP;
	sdlkeymap[SDLK_DELETE] = KEY_DELETE;
	sdlkeymap[SDLK_END] = KEY_END;
	sdlkeymap[SDLK_PAGEDOWN] = KEY_PAGEDOWN;
	sdlkeymap[SDLK_UP] = KEY_UP;
	sdlkeymap[SDLK_DOWN] = KEY_DOWN;
	sdlkeymap[SDLK_LEFT] = KEY_LEFT;
	sdlkeymap[SDLK_RIGHT] = KEY_RIGHT;

	sdlkeymap[SDLK_PRINT] = KEY_PRINT_SCREEN;
	sdlkeymap[SDLK_PAUSE] = KEY_PAUSE;
#endif
}
