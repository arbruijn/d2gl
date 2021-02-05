NASM=1
#
# $Source: f:/miner/source/2d/rcs/makefile $
# $Revision: 1.44 $
# $Author: matt $
# $Date: 1996/12/04 18:52:36 $
#
# Makefile for bios functions
#

# What to install, and where to install it
INSTALL_TARGS = effect2d.h gr.h gr.lib grdef.h gr.inc palette.h ibitblt.h rle.h pcx.h

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = gr

# What test programs to make
#TEST_PROGS = testg testp vesatest
#PROGS = testg

# What object files for the lib
GR_OBJS_ASM = linear.obj canvas.obj bitmap.obj gr.obj font.obj \
	bitblt.obj scanline.obj pixel.obj poly.obj disc.obj rect.obj box.obj  \
	gpixel.obj line.obj lbitblt.obj	   \
	scale.obj scalea.obj circle.obj ibitblt.obj rle.obj pcx.obj
GR_OBJS = canvas.obj bitmap.obj gr.obj font.obj \
	bitblt.obj scanline.obj pixel.obj poly.obj disc.obj rect.obj box.obj  \
	gpixel.obj line.obj  macstubs.obj \
	scalemac.obj circle.obj ibitblt.obj rle.obj pcx.obj
#blit.obj

#effects.obj roller.obj 

# What object files and libs for the test programs
#TESTG_SRCS = testg.c tmapbits.c
#TESTG_LIBS = io.lib gr.lib misc.lib mem.lib iff.lib  cfile.lib

#TESTP_OBJS = testp.obj
#TESTP_LIBS = io.lib gr.lib misc.lib mem.lib cflib.lib 

#VESATEST_OBJS = vesatest.obj
#VESATEST_LIBS = io.lib gr.lib misc.lib mem.lib cflib.lib 

#rules and targets common to all Miner makefiles
#include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

#
# $Source: f:/miner/source/3d/rcs/makefile $
# $Revision: 1.10 $
# $Author: matt $
# $Date: 1996/01/08 14:59:22 $
#
# Makefile for 3d library
#

# What to install
INSTALL_TARGS = 3d.lib 3d.h 3d.inc

# What libs to build
LIBS = 3d

# What objs in the lib
3D_OBJS_ASM = globvars.obj setup.obj matrix.obj points.obj draw.obj clipper.obj \
	horizon.obj instance.obj rod.obj interp.obj

3D_OBJS = globvars.obj setup.obj matrix.obj points.obj draw.obj clipper.obj \
	horizon.obj instance.obj rod.obj interp.obj

# What test programs to make
TEST_PROGS = 3dtest

# What object files and libs for the test programs
3DTEST_SRCS = 3dtest.c segment.c trakball.c
3DTEST_LIBS = 3d.lib vecmat.lib gr.lib io.lib fix.lib div0.lib texmap.lib misc.lib bm.lib

#rules and targets common to all Miner makefiles
#include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

#
# $Source: f:/miner/source/bios/rcs/makefile $
# $Revision: 1.20 $
# $Author: samir $
# $Date: 1995/06/12 18:37:24 $
#
# Makefile for bios functions
#

# What to install, and where to install it
INSTALL_TARGS = io.lib joy.h key.h mono.h mouse.h timer.h swift.h dpmi.h \
		ipx.h rbaudio.h	findfile.h error.h

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = io

# What test programs to make
TEST_PROGS = demo
# testj testk testm testms

# What object files for the lib
#IO_OBJS = key.obj joy.obj mouse.obj timer.obj mono.obj \
#	  joyc.obj dpmi.obj ipx.obj rbaudio.obj	findfile.obj error.obj
IO_OBJS = error.obj

# What object files and libs for the test programs
DEMO_SRCS = cdrom.c
DEMO_LIBS = io.lib

#rules and targets common to all Miner makefiles
#include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

#
# Master makefile for Miner source
#
# Makes all the executable by visiting subdirs and making there
#

CLEAN_TARGS = $(OBJDIR)\*.obj

INSTALL_TARGS = cfile.lib cfile.h nocfile.h

LIBS = cfile

CFILE_OBJS = cfile.obj

#all Miner makefiles should include this
#include $(INIT)\makefile.def
### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

#
# $Source: f:/miner/source/div/rcs/makefile $
# $Revision: 1.3 $
# $Author: matt $
# $Date: 1994/11/21 11:34:49 $
#
# Makefile for bios functions
#

# What to install, and where to install it
INSTALL_TARGS = div0.lib div0.h div0.inc

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = div0

# What test programs to make
TEST_PROGS = testd

# What object files for the lib
#DIV0_OBJS =  div0.obj
DIV0_OBJS=

# What object files and libs for the test programs
TESTD_SRCS = testd.c testda.asm
TESTD_LIBS = div0.lib

#rules and targets common to all Miner makefiles
#include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

#
# $Source: f:/miner/source/fix/rcs/makefile $
# $Revision: 1.6 $
# $Author: matt $
# $Date: 1995/12/21 14:00:19 $
#
# Makefile for fixed-point library
#

# What to install, and where to install it
INSTALL_TARGS = fix.lib fix.h fix.inc

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = fix

# What test programs to make
TEST_PROGS = fixtest

# What object files for the lib
FIX_OBJS = fix.obj tables.obj

# What object files and libs for the test programs
FIXTEST_SRCS = fixtest.c
FIXTEST_LIBS = fix.lib io.lib

#rules and targets common to all Miner makefiles
#include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

#
# $Source: f:/miner/source/iff/rcs/makefile $
# $Revision: 1.5 $
# $Author: john $
# $Date: 1994/02/15 12:51:20 $
#
# Makefile for IFF library
#

# What to install
INSTALL_TARGS = iff.lib iff.h

# What libs to build
LIBS = iff

# What objs in the iff lib
IFF_OBJS = iff.obj

# What test programs to make
TEST_PROGS = ifftest ifftestv

# What object files and libs for the test programs
IFFTEST_SRCS = ifftest.c
IFFTEST_LIBS = iff.lib vga.lib gr.lib mem.lib misc.lib cfile.lib io.lib

IFFTESTV_SRCS = ifftestv.c
IFFTESTV_LIBS = iff.lib vesa.lib cflib.lib cfile.lib

#rules and targets common to all Miner makefiles
#include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

#
# $Source: f:/miner/source/includes/rcs/makefile $
# $Revision: 1.2 $
# $Author: matt $
# $Date: 1995/12/21 13:59:42 $
#
# Makefile for macro and header files
#

# What to install
INSTALL_TARGS = pstypes.h pstypes.inc psmacros.inc

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What test programs to make
TEST_PROGS = test

# What object files and libs for the test programs
TEST_SRCS = test.c mactest.c
TEST_LIBS = io.lib

#rules and targets common to all Miner makefiles
#include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

#
# $Source: f:/miner/source/main/rcs/makefile $
# $Revision: 2.120 $
# $Author: matt $
# $Date: 1997/01/27 16:25:59 $
#
# Makefile for DESCENT execuable
#

INI_FILE = inferno.ini

# Load this person's personal settings
#include $(INI_FILE)

# Set target system if not in INI file, and make sure it's in uppercase
TARGSYS ?= DOS4G
TARGSYS := $(TARGSYS,UC)

# Where we're running the game.  Set here if no env var
MINERGAME ?= $(MINERLOCAL)\game

# Where to find libraries we didn't write
EXTLIB_DIR = $(MINERLOCAL)\source\extlib
EXTLIBS = $(EXTLIB_DIR)\greenlf $(EXTLIB_DIR)\glide	\
	  $(EXTLIB_DIR)\mvelib  $(EXTLIB_DIR)\iforce	\
	  $(EXTLIB_DIR)\s3dtk   $(EXTLIB_DIR)\loadpats	\
	  $(EXTLIB_DIR)\eregwin $(EXTLIB_DIR)\dd	\
	  $(EXTLIB_DIR)\soswin  $(EXTLIB_DIR)\mvewin    \
	  $(EXTLIB_DIR)\rend    

# What programs to make
ifeq ($(TARGSYS),WIN32)
PROGS = $(MINERGAME)\descentw 
else
ifdef SHAREWARE
PROGS = $(MINERGAME)\d2share
else
PROGS = $(MINERGAME)\descent2
endif
endif

# Files to link for Descent
# cdmix32.c
DESCENT2_SRCS =  game.c gamerend.c gamecntl.c gauges.c mglobal.c \
	scores.c object.c laser.c physics.c bm.c \
	menu.c gamesave.c switch.c effects.c texmerge.c polyobj.c \
	gamemine.c fireball.c ai.c ai2.c aipath.c aid1.c hostage.c \
	powerup.c fuelcen.c render.c piggy.c digiobj.c \
	gameseg.c wall.c collide.c lighting.c \
	robot.c morph.c vclip.c weapon.c fvi.c newdemo.c titles.c \
	vfx.c vfxread.c gameseq.c controls.c automap.c text.c \
	network.c newmenu.c netmisc.c \
	gamefont.c hud.c playsave.c \
	endlevel.c kconfig.c crypt.c\
	multi.c cntrlcen.c credits.c config.c kmatrix.c	\
	paging.c mission.c songs.c bmread.c \
	escort.c gamepal.c multibot.c state.c \
	3dmax.c 
	#demo.c garage.c radar.c coindev.c serial.c hash.c args.c 


#ifeq ($(TARGSYS),WIN32)
DESCENT2WIN_SRCS += winferno.c descentw.c modem.c joydefsw.c windigi.c movie.c
#else
#DESCENT2DOS_SRCS += inferno.c modem.c joydefs.c digi.c #victor.c iglasses.c movie.c digi.c[B
DESCENT2DOS_SRCS += joydefs.c
#endif

ifdef ARCADE
  DESCENT2_SRCS += arcade.c 
endif

ifndef NDEBUG
  DESCENT2_SRCS += dumpmine.c
endif

ifndef RELEASE_VERSION
  DESCENT2_SRCS += slew.c
endif

ifdef SHAREWARE
  DESCENT2_SRCS += terrain.c 
endif

# These sources are always built optimized
DESCENT2_SRCS_OPT = ## gameseg.c wall.c

DESCENT2_LIBS =	3d.lib gr.lib fix.lib iff.lib vecmat.lib \
		div0.lib misc.lib \
          	loadpats.lib cfile.lib \
		vidlib.lib texmap.lib unarj.lib
		#iglasses.lib
		#lzw.lib sosdwxcr.lib sosmwxcr.lib 


ifndef NDEBUG
DESCENT2_LIBS += mem.lib
endif

ifeq ($(TARGSYS),WIN32)
  DESCENT2_LIBS += win95.lib \
		   ddraw.lib dsound.lib mvelibw.lib \
		   novga.lib tapi32.lib

ifdef TACTILE
  DESCENT2_LIBS += iforce.lib
endif

ifdef RELEASE_VERSION
  DESCENT2_LIBS += eregreg2.lib
endif

else
  DESCENT2_LIBS += io.lib vga.lib gclfr32.lib sosdigi.lib sosmidi.lib mvelb32s.lib \
		poly_acc.lib s3dtkwdr.lib vlib.lib spd3d.lib

endif

DESCENT2_DIRS = editor

# Link flags for Descent
DESCENT2_LFLAGS = option map,verbose,stack=50k

# Files in the editor subdirectory
ifdef BUILD_EDITOR

EDITOR_SRCS = med.c einit.c eglobal.c mine.c curves.c kcurve.c \
	khelp.c ksegsel.c kbuild.c kview.c kmine.c seguvs.c ksegsize.c \
	ksegmove.c ktmap.c kfuncs.c autosave.c group.c texture.c \
	fixseg.c eobject.c elight.c texpage.c objpage.c medmisc.c \
	medsel.c kgame.c info.c medwall.c eswitch.c medrobot.c meddraw.c \
	segment.c centers.c	#ehostage.c 

# These sources are always built optimized
EDITOR_SRCS_OPT =  

DESCENT2_LIBS += ui.lib #medlisp.lib 

# Editor data files to check out with source
EDITOR_DATA = segmove.pad segsize.pad texture.pad object.pad curve.pad \
	      group.pad objmov.pad lighting.pad test.pad med.mnu global.key

EDITOR_DATA_FILES = $(EDITOR_DATA,<$(MINERGAME)\)

$(EDITOR_DATA_FILES) .NODEFAULT: $(@,F,<editor\)
	%if %exists($@)
	  @attrib -r $@	#allow overwriting
	%endif
	@echo Copying $< to $(@,D)
	@copy $< $@ >nul

DESCENT2_DATA = $(EDITOR_DATA_FILES)

else
EDITOR_SRCS = 
endif

# Force include of settings for all files
GLOBAL_INCFILES += settings.h

# DescentW is really Descent 2
DESCENTW_SRCS   = $(DESCENT2_SRCS)
DESCENTW_LIBS   = $(DESCENT2_LIBS)
DESCENTW_DIRS   = $(DESCENT2_DIRS)
DESCENTW_LFLAGS = $(DESCENT2_LFLAGS)
DESCENTW_DATA   = $(DESCENT2_DATA)

# D2Share is really Descent 2
D2SHARE_SRCS   = $(DESCENT2_SRCS)
D2SHARE_LIBS   = $(DESCENT2_LIBS)
D2SHARE_DIRS   = $(DESCENT2_DIRS)
D2SHARE_LFLAGS = $(DESCENT2_LFLAGS)
D@SHARE_DATA   = $(DESCENT2_DATA)

	 
#rules and targets common to all Miner makefiles
#include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

#
# $Source: f:/miner/source/mem/rcs/makefile $
# $Revision: 1.1 $
# $Author: john $
# $Date: 1993/11/02 17:45:38 $
#
# Makefile for memory debugging
#

# What to install, and where to install it
INSTALL_TARGS = mem.lib mem.h

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = mem

MEM_OBJS = mem.obj

#rules and targets common to all Miner makefiles
#include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

#
# $Source: f:/miner/source/misc/rcs/makefile $
# $Revision: 1.7 $
# $Author: matt $
# $Date: 1996/01/23 22:32:42 $
#
# Makefile for misc libaray
#

# What to install
INSTALL_TARGS = misc.lib parsarg.h debug.h hash.h args.h byteswap.h

# What libs to build
LIBS = misc

# What objs in the lib
#MISC_OBJS = parsarg.obj hash.obj args.obj
MISC_OBJS = hash.obj args.obj strutil.obj

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What test programs to make
TEST_PROGS = errtest parstest

# What object files and libs for the test programs
PARSTEST_SRCS = parstest.c
PARSTEST_LIBS = misc.lib

ERRTEST_SRCS = errtest.c
ERRTEST_LIBS = misc.lib

#rules and targets common to all Miner makefiles
#include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

#
# $Source: f:/miner/source/novga/rcs/makefile $
# $Revision: 1.2 $
# $Author: samir $
# $Date: 1996/03/15 19:05:21 $
#
# Makefile for nonvga functions
#

# What to install, and where to install it
INSTALL_TARGS = novga.lib

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = novga


# What object files for the lib
NOVGA_OBJS = novesa.obj 

#rules and targets common to all Miner makefiles
#include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

#
# Makefile for dummy poly_acc library
#

# What to install, and where to install it
INSTALL_TARGS = poly_acc.lib pa_enabl.h poly_acc.h

# What libs to build
LIBS = poly_acc

# What objs in the poly_acc lib
POLY_ACC_OBJS = poly_acc.obj

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

#rules and targets common to all Miner makefiles
#include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.


#
# $Source: f:/miner/source/texmap/rcs/makefile $
# $Revision: 1.29 $
# $Author: matt $
# $Date: 1996/12/04 19:27:43 $
#
# Makefile for hack3df testbed
#

#Uncomment the following 3 lines if you want to build with only C code.
#NASM = 1
#GLOBAL_CFLAGS+=/dNASM
#GLOBAL_AFLAGS+=/DNASM

# What to install
INSTALL_TARGS = texmap.lib texmap.h

# What libs to build
LIBS = texmap

TEXMAP_OBJS = 	ntmap.obj tmapflat.obj  

ifndef NASM
TEXMAP_OBJS +=	tmap_per.obj tmap_flt.obj tmapfade.obj tmap_ed.obj 
TEXMAP_OBJS +=  tmap_pnn.obj tmap_pln.obj tmap_plt.obj tmap_pnt.obj
TEXMAP_OBJS +=  tmap_qnn.obj tmap_qln.obj tmap_qlt.obj tmap_qnt.obj
TEXMAP_OBJS +=  tmap_lnn.obj tmap_lln.obj tmap_llt.obj tmap_lnt.obj
else
TEXMAP_OBJS += scanline.obj
endif

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

#rules and targets common to all Miner makefiles
#include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

#
# $Source: f:/miner/source/ui/rcs/makefile $
# $Revision: 1.9 $
# $Author: matt $
# $Date: 1995/05/30 22:55:30 $
#
# Makefile for user-interface functions
#

# What to install, and where to install it
INSTALL_TARGS = ui.lib ui.h func.h

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = ui

# What programs to make
TEST_PROGS = testg

# What object files for the lib
UI_OBJS = ui.obj mouse.obj window.obj uidraw.obj button.obj \
		  gadget.obj listbox.obj message.obj popup.obj scroll.obj \
		  radio.obj checkbox.obj inputbox.obj userbox.obj menu.obj \
		  harderr.obj file.obj keytrap.obj icon.obj keypress.obj \
		  menubar.obj keypad.obj func.obj

# What object files and libs for the test programs
TESTG_SRCS = testg.c
TESTG_LIBS = io.lib gr.lib ui.lib misc.lib mem.lib readfile.lib iff.lib func.lib cflib.lib cfile.lib
TESTG_LFLAGS = option stack=50k

#rules and targets common to all Miner makefiles
#include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

#
# $Source: f:/miner/source/vecmat/rcs/makefile $
# $Revision: 1.3 $
# $Author: matt $
# $Date: 1996/02/07 18:15:43 $
#
# Makefile for vector/matrix library
#

# What to install
INSTALL_TARGS = vecmat.lib vecmat.h vecmat.inc

# What libs to build
LIBS = vecmat

# What objs in the lib
VECMAT_OBJS = vecmat.obj

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What test programs to make
TEST_PROGS = vectest 

# What object files and libs for the test programs
VECTEST_SRCS = vectest.c
VECTEST_LIBS = vecmat.lib io.lib fix.lib

#rules and targets common to all Miner makefiles
#include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

#
# $Source: f:/miner/source/vga/rcs/makefile $
# $Revision: 1.7 $
# $Author: matt $
# $Date: 1996/03/05 13:35:39 $
#
# Makefile for bios functions
#

# What to install, and where to install it
INSTALL_TARGS = vga.h vga.lib

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = vga

# What object files for the lib
#VGA_OBJS = vga.obj palette.obj vesa.obj modex.obj framebuf.obj
VGA_OBJS= palette.obj

#rules and targets common to all Miner makefiles
#include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

#
# $Source: f:/miner/source/win95/rcs/makefile $
# $Revision: 1.19 $
# $Author: matt $
# $Date: 1996/12/04 19:21:02 $
#
# Makefile for Win32 'PseudoBIOS' Extensions

# Build for Windows
TARGSYS = WIN32

# What to install
INSTALL_TARGS = winapp.h comm.h win95.lib dd.h ds.h winregs.h winmidi.h xtapi.h \
		tactile.h

# Where to install
LIB_SUBDIR = win

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = win95

# What object files for the lib
WIN95_OBJS = palette.obj key.obj findfile.obj timer.obj rbaudio.obj error.obj \
	     ipx.obj joyc.obj swift.obj mouse.obj winapp.obj mono.obj \
	     comm.obj winregs.obj ds.obj midifile.obj winmidi.obj winckpit.obj \
	     gfx.obj dd.obj ddgr.obj direct3d.obj xtapi.obj iforce.obj tactile.obj

# Where to find the DirectX stuff
EXTLIBS = $(MINERLOCAL)\source\extlib\dd


#rules and targets common to all Miner makefiles
#include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

WATCOM = /c/watcomo10
#WATCOM = /c/watcom
JWASM = /c/temp/jwasm
#SOS = /c/temp/kitchensink/sdks/dos/sosaudio
SOS = /c/temp/Actua-Soccer-master
SOSLIB = $(SOS)/LIBRARY/x
PATH += :$(WATCOM)\binnt:$(JWASM)
INC =  -I=lib -I=includes -I=pa_null -I=2d -I=3d -I=texmap \
	-I=fix -I=vecmat -I=bios -I=mem -I=cfile -I=misc -I=iff \
	-I=vga -I=div \
	-I=$(subst /,\,$(patsubst /c/%,c:/%,$(WATCOM)/h/sys)) \
	-I=$(subst /,\,$(patsubst /c/%,c:/%,$(SOS)))
SOSLIBS = $(addprefix $(SOSLIB)/,sosdw1pr.lib sosmw1pr.lib)
EXT_LIBS = $(patsubst /c/%,c:/%,$(SOSLIBS))
#WCFLAGS = $(INC) -dNETWORK=1 -u_WIN32 -u__NT__ -d2 -hw -bt=dos
WCFLAGS = $(INC) -dNETWORK=1 -d2 -hw -bt=dos -zq -zld -w3 -on -oa -s -zp=1
#WCFLAGS = $(INC) -I=win95 -dNETWORK=1 -d2 -hw -dWINDOWS=1 -zq -zld -w3 -on -oa -s -zp=1
DESCENT2_LFLAGS += op stub=$(WATCOM)/binw/wstub.exe
# -u__NT__
# -u_WIN32
WAFLAGS = $(INC)
WLFLAGS = $(DESCENT2_LFLAGS) debug watcom

MAIN_OBJS = $(patsubst %.c,%.obj,\
	$(patsubst movie.c,,$(patsubst iglasses.c,,$(patsubst victor.c,,$(patsubst 3dmax.c,,$(patsubst vfx%.c,,$(DESCENT2_SRCS)))))))

MAINDOS_OBJS = $(patsubst %.c,%.obj,$(DESCENT2DOS_SRCS))
MAINWIN_OBJS = $(patsubst %.c,%.obj,$(DESCENT2WIN_SRCS))


ALL_DOSOBJS = $(addprefix 2d/,$(GR_OBJS)) $(addprefix 3d/,$(3D_OBJS)) \
	$(addprefix texmap/,$(TEXMAP_OBJS)) \
	$(addprefix fix/,$(FIX_OBJS)) \
	$(addprefix vecmat/,$(VECMAT_OBJS)) \
	$(addprefix bios/,$(IO_OBJS)) \
	$(addprefix cfile/,$(CFILE_OBJS)) \
	$(addprefix misc/,$(MISC_OBJS)) \
	$(addprefix mem/,$(MEM_OBJS)) \
	$(addprefix iff/,$(IFF_OBJS)) \
	$(addprefix vga/,$(VGA_OBJS)) \
	$(addprefix div/,$(DIV0_OBJS)) \
	$(addprefix main/,$(MAIN_OBJS)) \
	$(addprefix main/,$(MAINDOS_OBJS))
#	fake/fake.obj

ALL_WINOBJS = $(addprefix 2d/,$(GR_OBJS)) $(addprefix 3d/,$(3D_OBJS)) \
	$(addprefix texmap/,$(TEXMAP_OBJS)) \
	$(addprefix fix/,$(FIX_OBJS)) \
	$(addprefix vecmat/,$(VECMAT_OBJS)) \
	$(addprefix cfile/,$(CFILE_OBJS)) \
	$(addprefix misc/,$(MISC_OBJS)) \
	$(addprefix mem/,$(MEM_OBJS)) \
	$(addprefix iff/,$(IFF_OBJS)) \
	$(addprefix win95/,$(WIN95_OBJS)) \
	$(addprefix main/,$(MAIN_OBJS)) \
	$(addprefix main/,$(MAINWIN_OBJS)) \
	fake/fake.obj \
	novga/novesa.obj win95/smfread.obj

CFLAGS += -DNDOS -DNASM -w -Dfar="" -D_far="" -DKEY_COMMAND=512 -Dnear="" -Dcdecl=""
#CFLAGS += -DNMONO=1 -DNDEBUG=1 -DRELEASE
CFLAGS += -DNETWORK -DNPACK
CFLAGS += -Ilib -Iincludes -Ipa_null -I2d -I3d -Itexmap \
	-Ifix -Ivecmat -Ibios -Imem -Icfile -Imisc -Iiff \
	-Ivga -Idiv -Iarch -Imain
CFLAGS += -Og -g -m32 -fno-pie

all: descent2

descent2: $(patsubst %.obj,%.o,$(ALL_DOSOBJS)) \
	testiface.o sdltest.o sdlkeymap.o psrand.o stubs.o mydigi.o key.o \
	myipx.o myvga.o cfile_ext.o
	$(CC) -m32 -o $@ $^ -lSDL2 -lm

#all: $(ALL_DOSOBJS)
#	wlink system dos4g $(WLFLAGS) $(addprefix file ,$^) $(addprefix file ,$(subst /,\,$(EXT_LIBS))) name descent2.exe

owin: $(patsubst bios/timer.obj,,$(patsubst div/div0.obj,,$(ALL_OBJS))) fake/fakewin.obj
	wlink $(WLFLAGS) $(addprefix file ,$^) $(addprefix file ,$(subst /,\,$(EXT_LIBS))) name descentw.exe

win: $(ALL_WINOBJS) fake/fakewin.obj
	wlink $(WLFLAGS) $(addprefix file ,$^) library dsound.lib library directx\dxguid.lib library directx\dsound.lib library ddraw.lib library wsock32.lib library winmm.lib name descentw.exe
	
%.obj: %.c
	wcc386 $(WCFLAGS) -fo=$(subst /,\,$@) $(subst /,\,$<)
%.obj: %.cpp
	wpp386 $(WCFLAGS) -fo=$(subst /,\,$@) $(subst /,\,$<)
%.obj: %.asm
	jwasm $(WAFLAGS) -Fo$@ $<
	#/c/masm615/bin/ml -c -Fo$@ $(patsubst -I=%,-I%,$(INC)) $<

clean:
	rm -f $(ALL_DOSOBJS) $(ALL_WINOBJS) $(patsubst %.obj,%.o,$(ALL_DOSOBJS)) *.o
