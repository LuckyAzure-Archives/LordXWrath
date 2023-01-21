TARGET = funkin
TYPE = ps-exe

SRCS = src/main.c \
	   \
       src/characters/bf/bf.c \
	   \
       src/characters/dad/dad.c \
	   src/characters/lordxc/lordxc.c \
	   src/characters/lordx/lordx.c \
	   src/characters/slaves/slaves.c \
	   \
       src/characters/gf/gf.c \
       src/characters/gf/speaker.c \
	   \
       src/stages/default/default.c \
       src/stages/blank/blank.c \
	   \
       src/scenes/menu/menu.c \
	   \
       src/scenes/stage/stage.c \
       src/scenes/stage/pause.c \
	   src/scenes/stage/animation.c \
       src/scenes/stage/character.c \
       src/scenes/stage/object.c \
       src/scenes/stage/object/combo.c \
       src/scenes/stage/object/splash.c \
	   \
       src/events/event.c \
	   \
       src/fonts/font.c \
	   \
       src/psx/mutil.c \
       src/psx/random.c \
       src/psx/archive.c \
       src/psx/trans.c \
       src/psx/loadscr.c \
       src/psx/psx.c \
       src/psx/io.c \
       src/psx/gfx.c \
       src/psx/audio.c \
       src/psx/pad.c \
       src/psx/timer.c \
       src/psx/movie.c \
       mips/common/crt0/crt0.s

CPPFLAGS += -Wall -Wextra -pedantic -mno-check-zero-division
LDFLAGS += -Wl,--start-group
# TODO: remove unused libraries
LDFLAGS += -lapi
#LDFLAGS += -lc
LDFLAGS += -lc2
#LDFLAGS += -lcard
LDFLAGS += -lcd
#LDFLAGS += -lcomb
LDFLAGS += -lds
LDFLAGS += -letc
LDFLAGS += -lgpu
#LDFLAGS += -lgs
#LDFLAGS += -lgte
#LDFLAGS += -lgun
#LDFLAGS += -lhmd
#LDFLAGS += -lmath
#LDFLAGS += -lmcrd
#LDFLAGS += -lmcx
LDFLAGS += -lpad
LDFLAGS += -lpress
#LDFLAGS += -lsio
LDFLAGS += -lsnd
LDFLAGS += -lspu
#LDFLAGS += -ltap
LDFLAGS += -flto -Wl,--end-group

include mips/common.mk
