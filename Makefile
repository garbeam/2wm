# 2wm - stereo window manager
#   (C)opyright MMVII Anselm R. Garbe

include config.mk

SRC = client.c draw.c event.c main.c tag.c util.c view.c
OBJ = ${SRC:.c=.o}

all: options 2wm

options:
	@echo 2wm build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: 2wm.h config.h config.mk

config.h:
	@echo creating $@ from config.default.h
	@cp config.default.h $@

2wm: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}
	@strip $@

clean:
	@echo cleaning
	@rm -f 2wm ${OBJ} 2wm-${VERSION}.tar.gz

dist: clean
	@echo creating dist tarball
	@mkdir -p 2wm-${VERSION}
	@cp -R LICENSE Makefile README config.*.h config.mk \
		2wm.1 2wm.h ${SRC} 2wm-${VERSION}
	@tar -cf 2wm-${VERSION}.tar 2wm-${VERSION}
	@gzip 2wm-${VERSION}.tar
	@rm -rf 2wm-${VERSION}

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f 2wm ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/2wm
	@echo installing manual page to ${DESTDIR}${MANPREFIX}/man1
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@sed 's/VERSION/${VERSION}/g' < 2wm.1 > ${DESTDIR}${MANPREFIX}/man1/2wm.1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/2wm.1

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/2wm
	@echo removing manual page from ${DESTDIR}${MANPREFIX}/man1
	@rm -f ${DESTDIR}${MANPREFIX}/man1/2wm.1

.PHONY: all options clean dist install uninstall
