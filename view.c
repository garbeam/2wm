/* (C)opyright MMVI-MMVII Anselm R. Garbe <garbeam at gmail dot com>
 * See LICENSE file for license details.
 */
#include "2wm.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <X11/Xutil.h>


typedef struct { regex_t *regex; } RReg;

/* static */

FLOATS

static RReg *rreg = NULL;
static unsigned int len = 0;


static Client *
nexttiled(Client *c) {
	for(c = getnext(c); c && c->isfloat; c = getnext(c->next));
	return c;
}

static void
togglemax(Client *c) {
	XEvent ev;
		
	if(c->isfixed)
		return;

	if((c->ismax = !c->ismax)) {
		c->rx = c->x; c->x = sx;
		c->ry = c->y; c->y = sy;
		c->rw = c->w; c->w = sw - 2 * BORDERPX;
		c->rh = c->h; c->h = sh - 2 * BORDERPX;
	}
	else {
		c->x = c->rx;
		c->y = c->ry;
		c->w = c->rw;
		c->h = c->rh;
	}
	resize(c, True);
	while(XCheckMaskEvent(dpy, EnterWindowMask, &ev));
}

/* extern */

Client *
getnext(Client *c) {
	for(; c && c->visible != visible; c = c->next);
	return c;
}

Client *
getprev(Client *c) {
	for(; c && c->visible != visible; c = c->prev);
	return c;
}

void
initrregs(void) {
	unsigned int i;
	regex_t *reg;

	if(rreg)
		return;
	for(len = 0; floats[len]; len++);
	rreg = emallocz(len * sizeof(RReg));
	for(i = 0; i < len; i++) {
		if(floats[i]) {
			reg = emallocz(sizeof(regex_t));
			if(regcomp(reg, floats[i], REG_EXTENDED))
				free(reg);
			else
				rreg[i].regex = reg;
		}
	}
}

Bool
isfloat(Client *c) {
	char prop[512];
	unsigned int i;
	regmatch_t tmp;
	Bool ret = False;
	XClassHint ch = { 0 };

	XGetClassHint(dpy, c->win, &ch);
	snprintf(prop, sizeof prop, "%s:%s:%s",
			ch.res_class ? ch.res_class : "",
			ch.res_name ? ch.res_name : "", c->name);
	for(i = 0; i < len; i++)
		if(rreg[i].regex && !regexec(rreg[i].regex, prop, 1, &tmp, 0)) {
			ret = True;
			break;
		}
	if(ch.res_class)
		XFree(ch.res_class);
	if(ch.res_name)
		XFree(ch.res_name);
	return ret;
}

void
togglevisible(Arg *arg) {
	if(!sel)
		return;
	sel->visible = !sel->visible;
	arrange();
}

void
detach(Client *c) {
	if(c->prev)
		c->prev->next = c->next;
	if(c->next)
		c->next->prev = c->prev;
	if(c == clients)
		clients = c->next;
	c->next = c->prev = NULL;
}

void
arrange(void) {
	unsigned int i, n, mw, mh, tw, th;
	Client *c;

	for(n = 0, c = nexttiled(clients); c; c = nexttiled(c->next))
		n++;
	/* window geoms */
	mh = (n > nmaster) ? sh / nmaster : sh / (n > 0 ? n : 1);
	mw = (n > nmaster) ? (sw * master) / 1000 : sw;
	th = (n > nmaster) ? sh / (n - nmaster) : 0;
	tw = sw - mw;

	for(i = 0, c = clients; c; c = c->next)
		if(c->visible == visible) {
			if(c->isfloat) {
				resize(c, True);
				continue;
			}
			c->ismax = False;
			c->x = sx;
			c->y = sy;
			if(i < nmaster) {
				c->y += i * mh;
				c->w = mw - 2 * BORDERPX;
				c->h = mh - 2 * BORDERPX;
			}
			else {  /* tile window */
				c->x += mw;
				c->w = tw - 2 * BORDERPX;
				if(th > 2 * BORDERPX) {
					c->y += (i - nmaster) * th;
					c->h = th - 2 * BORDERPX;
				}
				else /* fallback if th <= 2 * BORDERPX */
					c->h = sh - 2 * BORDERPX;
			}
			resize(c, False);
			i++;
		}
		else
			XMoveWindow(dpy, c->win, c->x + 2 * sw, c->y);
	if(!sel || sel->visible != visible) {
		for(c = stack; c && c->visible != visible; c = c->snext);
		focus(c);
	}
	restack();
}

void
focusnext(Arg *arg) {
	Client *c;
   
	if(!sel)
		return;
	if(!(c = getnext(sel->next)))
		c = getnext(clients);
	if(c) {
		focus(c);
		restack();
	}
}

void
focusprev(Arg *arg) {
	Client *c;

	if(!sel)
		return;
	if(!(c = getprev(sel->prev))) {
		for(c = clients; c && c->next; c = c->next);
		c = getprev(c);
	}
	if(c) {
		focus(c);
		restack();
	}
}

void
incnmaster(Arg *arg) {
	if((nmaster + arg->i < 1) || (sh / (nmaster + arg->i) <= 2 * BORDERPX))
		return;
	nmaster += arg->i;
	if(sel)
		arrange();
}

void
resizemaster(Arg *arg) {
	if(arg->i == 0)
		master = MASTER;
	else {
		if(sw * (master + arg->i) / 1000 >= sw - 2 * BORDERPX
			|| sw * (master + arg->i) / 1000 <= 2 * BORDERPX)
			return;
		master += arg->i;
	}
	arrange();
}

void
restack(void) {
	Client *c;
	XEvent ev;

	if(!sel)
		return;
	if(sel->isfloat)
		XRaiseWindow(dpy, sel->win);
	else
		XLowerWindow(dpy, sel->win);
	for(c = nexttiled(clients); c; c = nexttiled(c->next)) {
		if(c == sel)
			continue;
		XLowerWindow(dpy, c->win);
	}
	XSync(dpy, False);
	while(XCheckMaskEvent(dpy, EnterWindowMask, &ev));
}

void
togglefloat(Arg *arg) {
	if(!sel)
		return;
	sel->isfloat = !sel->isfloat;
	arrange();
}

void
toggleview(Arg *arg) {
	visible = !visible;
	arrange();
}

void
zoom(Arg *arg) {
	unsigned int n;
	Client *c;

	if(!sel)
		return;
	if(sel->isfloat) {
		togglemax(sel);
		return;
	}
	for(n = 0, c = nexttiled(clients); c; c = nexttiled(c->next))
		n++;

	if((c = sel) == nexttiled(clients))
		if(!(c = nexttiled(c->next)))
			return;
	detach(c);
	if(clients)
		clients->prev = c;
	c->next = clients;
	clients = c;
	focus(c);
	arrange();
}
