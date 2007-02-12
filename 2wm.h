/* (C)opyright MMVII Anselm R. Garbe <garbeam at gmail dot com>
 * See LICENSE file for license details.
 */

#include "config.h"
#include <X11/Xlib.h>

/* mask shorthands, used in event.c and client.c */
#define BUTTONMASK		(ButtonPressMask | ButtonReleaseMask)

enum { NetSupported, NetWMName, NetLast };		/* EWMH atoms */
enum { WMProtocols, WMDelete, WMState, WMLast };	/* default atoms */
enum { CurNormal, CurResize, CurMove, CurLast };	/* cursor */

typedef union {
	const char *cmd;
	int i;
} Arg; /* argument type */

typedef struct Client Client;
struct Client {
	char name[256];
	int x, y, w, h;
	int rx, ry, rw, rh; /* revert geometry */
	int basew, baseh, incw, inch, maxw, maxh, minw, minh;
	int minax, minay, maxax, maxay;
	long flags; 
	unsigned int border;
	Bool isfixed, isfloat, ismax, visible;
	Client *next;
	Client *prev;
	Client *snext;
	Window win;
};

extern int screen, sx, sy, sw, sh;		/* screen geometry */
extern unsigned int master, nmaster;		/* master percent, number of master clients */
extern unsigned int numlockmask;		/* dynamic key lock mask */
extern void (*handler[LASTEvent])(XEvent *);	/* event handler */
extern Atom wmatom[WMLast], netatom[NetLast];
extern Bool running, selscreen, visible;
extern Client *clients, *sel, *stack;		/* global client list and stack */
extern Cursor cursor[CurLast];
extern unsigned long normcol, selcol;		/* sel/normal color */
extern Display *dpy;
extern Window root;

/* client.c */
extern void configure(Client *c);		/* send synthetic configure event */
extern void focus(Client *c);			/* focus c, c may be NULL */
extern Client *getclient(Window w);		/* return client of w */
extern Bool isprotodel(Client *c);		/* returns True if c->win supports wmatom[WMDelete] */
extern void killclient(Arg *arg);		/* kill c nicely */
extern void manage(Window w, XWindowAttributes *wa);	/* manage new client */
extern void resize(Client *c, Bool sizehints);	/* resize c*/
extern void updatesizehints(Client *c);		/* update the size hint variables of c */
extern void updatetitle(Client *c);		/* update the name of c */
extern void unmanage(Client *c);		/* destroy c */

/* event.c */
extern void grabkeys(void);			/* grab all keys defined in config.h */

/* main.c */
extern void quit(Arg *arg);			/* quit 2wm nicely */
extern void sendevent(Window w, Atom a, long value);	/* send synthetic event to w */
extern int xerror(Display *dsply, XErrorEvent *ee);	/* 2wm's X error handler */

/* tag.c */
extern void initrregs(void);			/* initialize regexps of rules defined in config.h */
extern Client *getnext(Client *c);		/* returns next visible client */
extern Client *getprev(Client *c);		/* returns previous visible client */
extern void setvisible(Client *c, Client *trans);/* sets visibility of c */
extern void togglevisible(Arg *arg);		/* toggles c tags with arg's index */

/* util.c */
extern void *emallocz(unsigned int size);	/* allocates zero-initialized memory, exits on error */
extern void eprint(const char *errstr, ...);	/* prints errstr and exits with 1 */
extern void spawn(Arg *arg);			/* forks a new subprocess with to arg's cmd */

/* view.c */
extern void detach(Client *c);			/* detaches c from global client list */
extern void arrange(void);			/* arranges all windows tiled */
extern void focusnext(Arg *arg);		/* focuses next visible client, arg is ignored  */
extern void focusprev(Arg *arg);		/* focuses previous visible client, arg is ignored */
extern void incnmaster(Arg *arg);		/* increments nmaster with arg's index value */
extern void resizemaster(Arg *arg);		/* resizes the master percent with arg's index value */
extern void restack(void);			/* restores z layers of all clients */
extern void togglefloat(Arg *arg);		/* toggles focusesd client between floating/non-floating state */
extern void toggleview(Arg *arg);		/* toggles the tag with arg's index (in)visible */
extern void zoom(Arg *arg);			/* zooms the focused client to master area, arg is ignored */
