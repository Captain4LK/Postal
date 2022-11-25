
# !!! FIXME: Make this more robust. MUCH more robust.
# !!! FIXME: ...or at least comment the rest of these options...

#ifeq ($(PANDORA),1)
  #macosx := false
  #CPUARCH := arm
  #CC := g++ 
  #LINKER := g++
  #steamworks := false
  #CFLAGS += -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp -ftree-vectorize -ffast-math -DPANDORA
#else ifeq ($(ODROID),1)
  #macosx := false
  #CPUARCH := arm
  #CC := g++
  #LINKER := g++
  #steamworks := false
  #CFLAGS += -mcpu=cortex-a9 -mfpu=neon -mfloat-abi=hard -ftree-vectorize -ffast-math -DODROID
#else ifeq ($(linux_x86),1)
  target := linux_x86
#else
  #target := macosx_x86
  #steamworks := true
#endif

BINDIR := ./bin
SRCDIR := .
#debug := true
debug := false

# ----------------------------------------------------- ... bleh.

#ifeq ($(strip $(target)),linux_x86)
  macosx := false
  CPUARCH := x86
  CC := g++ -Wall -Wextra -Wconversion
  LINKER := g++ -Wall -Wextra -Wconversion
#endif
#ifeq ($(strip $(target)),macosx_x86)
#  macosx := true
#  CPUARCH := x86
#  CC := g++
#  LINKER := g++
#endif

CLEANUP := $(wildcard *.exe) $(wildcard *.obj) \
          $(wildcard $(BINDIR)/*.exe) $(wildcard $(BINDIR)/*.obj) \
          $(wildcard *~) $(wildcard *.err) \
          $(wildcard .\#*) core

SRCS := \
	src/AnimThing.cpp \
	src/ball.cpp \
	src/barrel.cpp \
	src/bouy.cpp \
	src/character.cpp \
	src/chunk.cpp \
	src/deathWad.cpp \
	src/demon.cpp \
	src/dispenser.cpp \
	src/doofus.cpp \
	src/dude.cpp \
	src/explode.cpp \
	src/fire.cpp \
	src/fireball.cpp \
	src/firebomb.cpp \
	src/flag.cpp \
	src/flagbase.cpp \
	src/grenade.cpp \
	src/heatseeker.cpp \
	src/hood.cpp \
	src/item3d.cpp \
	src/mine.cpp \
	src/napalm.cpp \
	src/navnet.cpp \
	src/ostrich.cpp \
	src/person.cpp \
	src/PowerUp.cpp \
	src/pylon.cpp \
	src/rocket.cpp \
	src/sentry.cpp \
	src/SndRelay.cpp \
	src/SoundThing.cpp \
	src/thing.cpp \
	src/Thing3d.cpp \
	src/trigger.cpp \
	src/warp.cpp \
	src/weapon.cpp \
	src/alphablitforpostal.cpp \
	src/Anim3D.cpp \
	src/BufQ.cpp \
	src/bulletFest.cpp \
	src/camera.cpp \
	src/crawler.cpp \
	src/cutscene.cpp \
	src/encrypt.cpp \
	src/gameedit.cpp \
	src/GameSettings.cpp \
	src/grip.cpp \
	src/IdBank.cpp \
	src/InputSettings.cpp \
	src/InputSettingsDlg.cpp \
	src/keys.cpp \
	src/Log.cpp \
	src/logtab.cpp \
	src/MemFileFest.cpp \
	src/MenuSettings.cpp \
	src/MenuTrans.cpp \
	src/net.cpp \
	src/NetBrowse.cpp \
	src/NetClient.cpp \
	src/NetDlg.cpp \
	src/netmsgr.cpp \
	src/NetServer.cpp \
	src/organ.cpp \
	src/Personatorium.cpp \
	src/ProtoBSDIP.cpp \
	src/realm.cpp \
	src/scene.cpp \
	src/score.cpp \
	src/settings.cpp \
	src/smash.cpp \
	src/socket.cpp \
	src/StockPile.cpp \
	src/TexEdit.cpp \
	src/toolbar.cpp \
	src/TriggerRegions.cpp \
	src/update.cpp \
	src/yatime.cpp \
	src/aivars.cpp \
	src/band.cpp \
	src/credits.cpp \
	src/game.cpp \
	src/input.cpp \
	src/localize.cpp \
	src/main.cpp \
	src/menus.cpp \
	src/play.cpp \
	src/SampleMaster.cpp \
	src/title.cpp \
	RSPiX/BLUE/unix/Bdebug.cpp \
	RSPiX/BLUE/unix/Bjoy.cpp \
	RSPiX/BLUE/unix/Bkey.cpp \
	RSPiX/BLUE/unix/Bmain.cpp \
	RSPiX/BLUE/unix/Bmouse.cpp \
	RSPiX/BLUE/unix/Btime.cpp \
	RSPiX/BLUE/unix/Bdisp.cpp \
	RSPiX/BLUE/unix/Bsound.cpp \
	RSPiX/GREEN/Hot/hot.cpp \
	RSPiX/GREEN/Image/Image.cpp \
	RSPiX/GREEN/Image/Imagecon.cpp \
	RSPiX/GREEN/Image/ImageFile.cpp \
	RSPiX/GREEN/InputEvent/InputEvent.cpp \
	RSPiX/GREEN/Mix/mix.cpp \
	RSPiX/GREEN/Mix/MixBuf.cpp \
	RSPiX/GREEN/Image/pal.cpp \
	RSPiX/GREEN/Image/PalFile.cpp \
	RSPiX/GREEN/Sample/sample.cpp \
	RSPiX/GREEN/Snd/snd.cpp \
	RSPiX/GREEN/SndFx/SndFx.cpp \
	RSPiX/GREEN/Task/task.cpp \
	RSPiX/GREEN/3D/pipeline.cpp \
	RSPiX/GREEN/3D/render.cpp \
	RSPiX/GREEN/3D/types3d.cpp \
	RSPiX/GREEN/3D/zbuffer.cpp \
	RSPiX/GREEN/BLiT/alphablit.cpp \
	RSPiX/GREEN/BLiT/BLIT.cpp \
	RSPiX/GREEN/BLiT/BLITINIT.cpp \
	RSPiX/GREEN/BLiT/BLiTT.cpp \
	RSPiX/GREEN/BLiT/CFNT.cpp \
	RSPiX/GREEN/BLiT/Fspr1.cpp \
	RSPiX/GREEN/BLiT/FSPR8.cpp \
	RSPiX/GREEN/BLiT/line.cpp \
	RSPiX/GREEN/BLiT/mono.cpp \
	RSPiX/GREEN/BLiT/Rotate96.cpp \
	RSPiX/GREEN/BLiT/RPrint.cpp \
	RSPiX/GREEN/BLiT/ScaleFlat.cpp \
	RSPiX/ORANGE/GameLib/ANIMSPRT.cpp \
	RSPiX/ORANGE/Attribute/attribute.cpp \
	RSPiX/ORANGE/GUI/btn.cpp \
	RSPiX/ORANGE/Channel/channel.cpp \
	RSPiX/ORANGE/color/colormatch.cpp \
	RSPiX/ORANGE/DirtRect/DirtRect.cpp \
	RSPiX/ORANGE/color/dithermatch.cpp \
	RSPiX/ORANGE/GUI/dlg.cpp \
	RSPiX/ORANGE/GUI/edit.cpp \
	RSPiX/ORANGE/File/file.cpp \
	RSPiX/ORANGE/QuickMath/FixedPoint.cpp \
	RSPiX/ORANGE/GUI/guiItem.cpp \
	RSPiX/ORANGE/IFF/iff.cpp \
	RSPiX/ORANGE/ImageTools/lasso.cpp \
	RSPiX/ORANGE/Laymage/laymage.cpp \
	RSPiX/ORANGE/GUI/ListBox.cpp \
	RSPiX/ORANGE/GUI/ListContents.cpp \
	RSPiX/ORANGE/Meter/meter.cpp \
	RSPiX/ORANGE/MsgBox/MsgBox.cpp \
	RSPiX/ORANGE/GUI/MultiBtn.cpp \
	RSPiX/ORANGE/MultiGrid/MultiGrid.cpp \
	RSPiX/ORANGE/MultiGrid/MultiGridIndirect.cpp \
	RSPiX/ORANGE/GUI/ProcessGui.cpp \
	RSPiX/ORANGE/Debug/profile.cpp \
	RSPiX/ORANGE/GUI/PushBtn.cpp \
	RSPiX/ORANGE/QuickMath/QuickMath.cpp \
	RSPiX/ORANGE/GameLib/Region.cpp \
	RSPiX/ORANGE/RString/rstring.cpp \
	RSPiX/ORANGE/GUI/scrollbar.cpp \
	RSPiX/ORANGE/GameLib/SHAPES.cpp \
	RSPiX/ORANGE/Parse/SimpleBatch.cpp \
	RSPiX/ORANGE/GameLib/SPRITE.cpp \
	RSPiX/ORANGE/str/str.cpp \
	RSPiX/ORANGE/GUI/txt.cpp \
	RSPiX/CYAN/Unix/uDialog.cpp \
	RSPiX/CYAN/Unix/uColors.cpp \
	RSPiX/CYAN/Unix/uPath.cpp \
	WishPiX/Menu/menu.cpp \
	WishPiX/Prefs/prefline.cpp \
	WishPiX/Prefs/prefs.cpp \
	WishPiX/ResourceManager/resmgr.cpp \
	WishPiX/Spry/spry.cpp

    # wtf is THIS?!
	#RSPiX/Src/ORANGE/MTask/mtask.cpp \

CLIENTEXE := $(BINDIR)/postal1-bin
OBJS0 := $(SRCS:.s=.o)
OBJS1 := $(OBJS0:.c=.o)
OBJS2 := $(OBJS1:.cpp=.o)
OBJS3 := $(OBJS2:.nasm=.o)
OBJS4 := $(OBJS3:.s=.o)
OBJS := $(foreach f,$(OBJS4),$(BINDIR)/$(f))
SRCS := $(foreach f,$(SRCS),$(SRCDIR)/$(f))

# !!! FIXME: Get -Wall in here, some day.
CFLAGS += -fsigned-char -g -DPLATFORM_UNIX -w -Wall

ifeq ($(strip $(macosx)),true)
  CFLAGS += -DPLATFORM_MACOSX
endif

# defines the game needs...
CFLAGS += -DLOCALE=US -DTARGET=POSTAL_2015

# includes ...
CFLAGS += -I$(SRCDIR)
CFLAGS += -I$(SRCDIR)/src
CFLAGS += -I/usr/include/SDL2
CFLAGS += -I$(SRCDIR)/RSPiX
CFLAGS += -I$(SRCDIR)/RSPiX/BLUE
CFLAGS += -I$(SRCDIR)/RSPiX/BLUE/unix
CFLAGS += -I$(SRCDIR)/RSPiX/CYAN
CFLAGS += -I$(SRCDIR)/RSPiX/CYAN/Unix
CFLAGS += -I$(SRCDIR)/RSPiX/ORANGE
CFLAGS += -I$(SRCDIR)/RSPiX/ORANGE/CDT
CFLAGS += -I$(SRCDIR)/RSPiX/ORANGE/GameLib
CFLAGS += -I$(SRCDIR)/RSPiX/ORANGE/File
CFLAGS += -I$(SRCDIR)/RSPiX/ORANGE/Multigrid
CFLAGS += -I$(SRCDIR)/RSPiX/GREEN/Image
CFLAGS += -I$(SRCDIR)/WishPiX
CFLAGS += -I$(SRCDIR)/WishPiX/Spry

ifeq ($(strip $(expiring_beta)),true)
  CFLAGS += -DBETAEXPIRE=$(shell date +%s)
endif

#ifeq ($(strip $(debug)),true)
  CFLAGS += -DDEBUG -D_DEBUG -Og
#else
  #OPTFLAG := -O3
  #CFLAGS += -DNDEBUG -D_NDEBUG $(OPTFLAG)
#endif

#ifeq ($(strip $(macosx)),true)
  #CFLAGS += -arch i386 -mmacosx-version-min=10.5
  #LDFLAGS += -arch i386 -mmacosx-version-min=10.5
  #LDFLAGS += -framework CoreFoundation -framework Cocoa
  #LIBS += SDL2/libs/macosx/libSDL2-2.0.0.dylib
  #STEAMLDFLAGS += steamworks/sdk/redistributable_bin/osx32/libsteam_api.dylib
#else
  ifeq ($(CPUARCH),arm)
    LIBS += -lSDL2
  else
    LIBS += -lSDL2
    #LIBS += SDL2/libs/linux-x86/libSDL2-2.0.so.0
    LDFLAGS += -Wl,-rpath,\$$ORIGIN
    STEAMLDFLAGS += steamworks/sdk/redistributable_bin/linux32/libsteam_api.so
 endif
#endif

ifeq ($(strip $(steamworks)),true)
  CFLAGS += -DWITH_STEAMWORKS=1 -Isteamworks/sdk/public
  LDFLAGS += $(STEAMLDFLAGS)
endif

CFLAGS += -DALLOW_TWINSTICK

.PHONY: all bindir

all: $(CLIENTEXE)


$(BINDIR)/%.o: $(SRCDIR)/%.s
	$(CC) $(CFLAGS) -DELF -x assembler-with-cpp -o $@ -c $<

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINDIR)/%.a: $(SRCDIR)/%.a
	cp $< $@
	ranlib $@

$(CLIENTEXE): $(BINDIR) $(OBJS) $(LIBS)
	$(LINKER) -o $(CLIENTEXE) $(OBJS) $(LDFLAGS) $(LIBS)

$(BINDIR) :
	$(MAKE) bindir

bindir :
	mkdir -p $(BINDIR)
	mkdir -p $(BINDIR)/src
	mkdir -p $(BINDIR)/RSPiX/BLUE/unix
	mkdir -p $(BINDIR)/RSPiX/GREEN/Hot
	mkdir -p $(BINDIR)/RSPiX/GREEN/Image
	mkdir -p $(BINDIR)/RSPiX/GREEN/InputEvent
	mkdir -p $(BINDIR)/RSPiX/GREEN/Mix
	mkdir -p $(BINDIR)/RSPiX/GREEN/Sample
	mkdir -p $(BINDIR)/RSPiX/GREEN/Snd
	mkdir -p $(BINDIR)/RSPiX/GREEN/SndFx
	mkdir -p $(BINDIR)/RSPiX/GREEN/Task
	mkdir -p $(BINDIR)/RSPiX/GREEN/3D
	mkdir -p $(BINDIR)/RSPiX/GREEN/BLiT
	mkdir -p $(BINDIR)/RSPiX/ORANGE/GameLib
	mkdir -p $(BINDIR)/RSPiX/ORANGE/Attribute
	mkdir -p $(BINDIR)/RSPiX/ORANGE/GUI
	mkdir -p $(BINDIR)/RSPiX/ORANGE/Channel
	mkdir -p $(BINDIR)/RSPiX/ORANGE/color
	mkdir -p $(BINDIR)/RSPiX/ORANGE/DirtRect
	mkdir -p $(BINDIR)/RSPiX/ORANGE/File
	mkdir -p $(BINDIR)/RSPiX/ORANGE/QuickMath
	mkdir -p $(BINDIR)/RSPiX/ORANGE/IFF
	mkdir -p $(BINDIR)/RSPiX/ORANGE/ImageTools
	mkdir -p $(BINDIR)/RSPiX/ORANGE/Laymage
	mkdir -p $(BINDIR)/RSPiX/ORANGE/Meter
	mkdir -p $(BINDIR)/RSPiX/ORANGE/MsgBox
	mkdir -p $(BINDIR)/RSPiX/ORANGE/MTask
	mkdir -p $(BINDIR)/RSPiX/ORANGE/MultiGrid
	mkdir -p $(BINDIR)/RSPiX/ORANGE/Debug
	mkdir -p $(BINDIR)/RSPiX/ORANGE/RString
	mkdir -p $(BINDIR)/RSPiX/ORANGE/Parse
	mkdir -p $(BINDIR)/RSPiX/ORANGE/str
	mkdir -p $(BINDIR)/RSPiX/CYAN/Unix
	mkdir -p $(BINDIR)/WishPiX/Menu
	mkdir -p $(BINDIR)/WishPiX/Prefs
	mkdir -p $(BINDIR)/WishPiX/ResourceManager
	mkdir -p $(BINDIR)/WishPiX/Spry
	mkdir -p $(BINDIR)/libs

distclean: clean

clean:
	rm -f $(CLEANUP)
	rm -rf $(BINDIR)
	#rm -f $(SRCDIR)/parser/y.tab.c
	#rm -f $(SRCDIR)/parser/lex.yy.c

# end of Makefile ...
