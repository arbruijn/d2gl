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

#if !defined(PA_ENABL_H)
#define PA_ENABL_H

#ifdef NASM
extern int PAEnabled;
#endif
//#define POLY_ACC                        // define this if poly support wanted.

#if defined(POLY_ACC)

// uncomment only ONE of the following.
//#define PA_S3_VIRGE
//#define PA_DIAMOND_S3_VIRGE

#endif

#ifdef PA_3DFX_VOODOO
 #define PA_DFX(x) x
 #define NO_DFX(x)
#else
 #define PA_DFX(x)
 #define NO_DFX(x) x
#endif

#endif
