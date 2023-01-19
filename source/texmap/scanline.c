/*
THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.  
COPYRIGHT 1993-1999 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/


#pragma off (unreferenced)
static char rcsid[] = "$Id: scanline.c 1.3 1996/01/24 16:38:16 champaign Exp $";
#pragma on (unreferenced)

#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include "fix.h"
#include "mono.h"
#include "gr.h"
#include "grdef.h"
#include "texmap.h"
#include "texmapl.h"
#include "scanline.h"

extern ubyte * dest_row_data;
extern int loop_count;

void c_tmap_scanline_flat()
{
	ubyte *dest;
	int x;

	dest = dest_row_data;

	for (x=loop_count; x >= 0; x-- ) {
		*dest++ = tmap_flat_color;
	}
}

void c_tmap_scanline_shaded()
{
	int fade;
	ubyte *dest;
	int x;

	dest = dest_row_data;

	fade = (tmap_flat_shade_value & 31)<<8; // & 31 is asm code bug compat :(
	for (x=loop_count; x >= 0; x-- ) {
		*dest++ = gr_fade_table[ fade |(*dest)];
	}
}

void c_tmap_scanline_lin_nolight()
{
	ubyte *dest;
	uint c;
	int x;
	fix u,v,dudx, dvdx;

	u = fx_u;
	v = fx_v*64;
	dudx = fx_du_dx; 
	dvdx = fx_dv_dx*64; 

	dest = dest_row_data;

	if (!Transparency_on)	{
		for (x=loop_count; x >= 0; x-- ) {
			*dest++ = (uint)pixptr[ (f2i(v)&(64*63)) + (f2i(u)&63) ];
			u += dudx;
			v += dvdx;
		}
	} else {
		for (x=loop_count; x >= 0; x-- ) {
			c = (uint)pixptr[ (f2i(v)&(64*63)) + (f2i(u)&63) ];
			if ( c!=255)
				*dest = c;
			dest++;
			u += dudx;
			v += dvdx;
		}
	}
}


__attribute__((optimize("-O3"))) void c_tmap_scanline_lin()
{
	ubyte *dest = dest_row_data, *src = pixptr;
	uint c;
	int x;
	fix l, dldx;
	uint vu, dvudx;

	vu = ((fx_u<<10)&~0xffff)|((fx_v>>6)&0xffff);
	dvudx = ((fx_du_dx<<10)&~0xffff)|((fx_dv_dx>>6)&0xffff);

	l = fx_l;
	dldx = fx_dl_dx;

	if (!Transparency_on)	{
		for (x=loop_count; x >= 0; x-- ) {
			*dest++ = gr_fade_table[ (l&(0xff00)) + (uint)src[ ((vu>>4)&(64*63)) + (vu>>26) ] ];
			l += dldx;
			vu += dvudx;
		}
	} else {
		for (x=loop_count; x >= 0; x-- ) {
			c = (uint)src[ ((vu>>4)&(64*63)) + (vu>>26) ];
			if ( c!=255)
				*dest = gr_fade_table[ (l&(0xff00)) + c ];
			dest++;
			l += dldx;
			vu += dvudx;
		}
	}
}


void c_tmap_scanline_per_nolight()
{
	ubyte *dest;
	uint c;
	int x;
	fix u,v,z,dudx, dvdx, dzdx;

	u = fx_u;
	v = fx_v*64;
	z = fx_z;
	dudx = fx_du_dx; 
	dvdx = fx_dv_dx*64; 
	dzdx = fx_dz_dx;

	dest = dest_row_data;

	if (!Transparency_on)	{
		for (x=loop_count; x >= 0; x-- ) {
			*dest++ = (uint)pixptr[ ( (v/z)&(64*63) ) + ((u/z)&63) ];
			u += dudx;
			v += dvdx;
			z += dzdx;
		}
	} else {
		for (x=loop_count; x >= 0; x-- ) {
			c = (uint)pixptr[ ( (v/z)&(64*63) ) + ((u/z)&63) ];
			if ( c!=255)
				*dest = c;
			dest++;
			u += dudx;
			v += dvdx;
			z += dzdx;
		}
	}
}

#if 1
__attribute__((optimize("-O3")))
void c_tmap_scanline_per()
{
	ubyte *dest = dest_row_data, *src = pixptr;
	uint c;
	int i, j;
	fix u, v, z, zi, u0, v0, u1, v1, dvudx, l, dldx;
	unsigned int vu;
	int nbits = 5, n = 1 << nbits;
	int num_outer, num_left_over;

	u = fx_u;
	v = fx_v;
	z = fx_z;
	zi = divide_table[(z >> Z_SHIFTER) & (DIVIDE_TABLE_SIZE - 1)];
	u0 = fixmul(u, zi);
	v0 = fixmul(v, zi);

	num_left_over = (loop_count + 1) & ((1 << nbits) - 1);
	num_outer = (loop_count + 1) >> nbits;

	l = fx_l;
	dldx = fx_dl_dx;

	for (j = 0; j < num_outer; j++) {
		u += fx_du_dx << nbits;
		v += fx_dv_dx << nbits;
		z += fx_dz_dx << nbits;

		zi = divide_table[(z >> Z_SHIFTER) & (DIVIDE_TABLE_SIZE - 1)];

		u1 = fixmul(u, zi);
		v1 = fixmul(v, zi);

		vu = ((u0 << 10) & 0xffff0000) | ((v0 >> 6) & 0xffff);
		dvudx = (((u1 - u0) << (10 - nbits)) & 0xffff0000) | (((v1 - v0) >> (6 + nbits)) & 0xffff);


		if (!Transparency_on)	{
			//#pragma GCC unroll 32
			for (i = 0; i < n; i++) {
				*dest++ = gr_fade_table[ (l&(0xff00)) + (uint)src[ ((vu & 0xfc00) >> 4) | (vu >> 26) ] ];
				l += dldx;
				vu += dvudx;
			}
		} else {
			//#pragma GCC unroll 32
			for (i = 0; i < n; i++) {
				c = (uint)src[ ((vu & 0xfc00) >> 4) | (vu >> 26) ];
				if (c != 255)
					*dest = gr_fade_table[ (l&(0xff00)) + c ];
				dest++;
				l += dldx;
				vu += dvudx;
			}
		}
		u0 = u1;
		v0 = v1;
	}

	if (!num_outer || num_left_over > 4) { // If less than 4, keep interpolating without a new DU:DV
		z = fx_z_right;
		zi = divide_table[(z >> Z_SHIFTER) & (DIVIDE_TABLE_SIZE - 1)];
		u1 = fixmul(fx_u_right, zi);
		v1 = fixmul(fx_v_right, zi);
		dvudx = ((((u1 - u0) / num_left_over) << 10) & 0xffff0000) | ((((v1 - v0) / num_left_over) >> 6) & 0xffff);
		vu = ((u0 << 10) & 0xffff0000) | ((v0 >> 6) & 0xffff);
	}

	if (!Transparency_on)	{
		for (i = 0; i < num_left_over; i++) {
			*dest++ = gr_fade_table[ (l&(0xff00)) + (uint)src[ ((vu & 0xfc00) >> 4) | (vu >> 26) ] ];
			l += dldx;
			vu += dvudx;
		}
	} else {
		for (i = 0; i < num_left_over; i++) {
			c = (uint)src[ ((vu & 0xfc00) >> 4) | (vu >> 26) ];
			if (c != 255)
				*dest = gr_fade_table[ (l&(0xff00)) + c ];
			dest++;
			l += dldx;
			vu += dvudx;
		}
	}
}
#else
void c_tmap_scanline_per()
{
	ubyte *dest;
	uint c;
	int x;
	fix u,v,z,l,dudx, dvdx, dzdx, dldx;

	u = fx_u;
	v = fx_v*64;
	z = fx_z;
	dudx = fx_du_dx; 
	dvdx = fx_dv_dx*64; 
	dzdx = fx_dz_dx;

	l = fx_l;
	dldx = fx_dl_dx;
	dest = dest_row_data;

	if (!Transparency_on)	{
		for (x=loop_count; x >= 0; x-- ) {
			*dest++ = gr_fade_table[ (l&(0xff00)) + (uint)pixptr[ ( (v/z)&(64*63) ) + ((u/z)&63) ] ];
			l += dldx;
			u += dudx;
			v += dvdx;
			z += dzdx;
		}
	} else {
		for (x=loop_count; x >= 0; x-- ) {
			c = (uint)pixptr[ ( (v/z)&(64*63) ) + ((u/z)&63) ];
			if ( c!=255)
				*dest = gr_fade_table[ (l&(0xff00)) + c ];
			dest++;
			l += dldx;
			u += dudx;
			v += dvdx;
			z += dzdx;
		}
	}
}
#endif

#define zonk 1

void c_tmap_scanline_editor()
{
	ubyte *dest;
	uint c;
	int x;
	fix u,v,z,dudx, dvdx, dzdx;

	u = fx_u;
	v = fx_v*64;
	z = fx_z;
	dudx = fx_du_dx; 
	dvdx = fx_dv_dx*64; 
	dzdx = fx_dz_dx;

	dest = dest_row_data;

	if (!Transparency_on)	{
		for (x=loop_count; x >= 0; x-- ) {
			*dest++ = zonk;
			//(uint)pixptr[ ( (v/z)&(64*63) ) + ((u/z)&63) ];
			u += dudx;
			v += dvdx;
			z += dzdx;
		}
	} else {
		for (x=loop_count; x >= 0; x-- ) {
			c = (uint)pixptr[ ( (v/z)&(64*63) ) + ((u/z)&63) ];
			if ( c!=255)
				*dest = zonk;
			dest++;
			u += dudx;
			v += dvdx;
			z += dzdx;
		}
	}
}

void no_light() {
	fx_l = 33 * 256;
	fx_dl_dx = 0;
}

void asm_tmap_scanline_per() { c_tmap_scanline_per(); }
void asm_tmap_scanline_llt() { c_tmap_scanline_lin(); }
void asm_tmap_scanline_lln() { c_tmap_scanline_lin(); }
void asm_tmap_scanline_plt() { c_tmap_scanline_per(); }
void asm_tmap_scanline_pln() { c_tmap_scanline_per(); }
void asm_tmap_scanline_lnt() { no_light(); c_tmap_scanline_lin(); }
void asm_tmap_scanline_lnn() { no_light(); c_tmap_scanline_lin(); }
void asm_tmap_scanline_pnt() { no_light(); c_tmap_scanline_per(); }
void asm_tmap_scanline_pnn() { no_light(); c_tmap_scanline_per(); }
void asm_tmap_scanline_editor() { c_tmap_scanline_editor(); }
void asm_tmap_scanline_flat() { c_tmap_scanline_flat(); }
void asm_tmap_scanline_shaded() { c_tmap_scanline_shaded(); }
