CC      = g++
C	= cpp
H	= h

CFLAGS 	= -g `Magick++-config --cppflags`
LFLAGS  = -g `Magick++-config --ldflags`

ifeq ("$(shell uname)", "Darwin")
    LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lm -lMagick++
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -L /usr/lib64/ -lglut -lGL -lm -lGLU -lMagick++
  endif
endif

HFILES 	= Utility.${H} geometricTool.${H} morph.${H}
OFILES 	= Utility.o geometricTool.o

PROJECT = morph

${PROJECT}:	${PROJECT}.o $(OFILES)
	${CC} $(CFLAGS) ${LFLAGS} -o ${PROJECT} ${PROJECT}.o $(OFILES) $(LDFLAGS)

${PROJECT}.o: ${PROJECT}.${C} $(HFILES)
	${CC} $(CFLAGS) -c ${PROJECT}.${C} $(HFILES)

geometricTool.o: geometricTool.${C} geometricTool.${H} Utility.${H}
	${CC} $(CFLAGS) -c geometricTool.${C}

Utility.o: Utility.${C} Utility.${H}
	${CC} $(CFLAGS) -c Utility.${C}

debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm -f *.o *~ ${PROJECT} core.*

