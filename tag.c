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


typedef struct {
	const char *clpattern;
	const char *tpattern;
	Bool isfloat;
} Rule;

typedef struct {
	regex_t *clregex;
	regex_t *tregex;
} RReg;

/* static */

RULES

static RReg *rreg = NULL;
static unsigned int len = 0;

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
	len = sizeof rule / sizeof rule[0];
	rreg = emallocz(len * sizeof(RReg));
	for(i = 0; i < len; i++) {
		if(rule[i].clpattern) {
			reg = emallocz(sizeof(regex_t));
			if(regcomp(reg, rule[i].clpattern, REG_EXTENDED))
				free(reg);
			else
				rreg[i].clregex = reg;
		}
		if(rule[i].tpattern) {
			reg = emallocz(sizeof(regex_t));
			if(regcomp(reg, rule[i].tpattern, REG_EXTENDED))
				free(reg);
			else
				rreg[i].tregex = reg;
		}
	}
}

void
setvisible(Client *c, Client *trans) {
	char prop[512];
	unsigned int i;
	regmatch_t tmp;
	XClassHint ch = { 0 };

	if(trans)
		c->visible = trans->visible;
	else {
		XGetClassHint(dpy, c->win, &ch);
		snprintf(prop, sizeof prop, "%s:%s:%s",
				ch.res_class ? ch.res_class : "",
				ch.res_name ? ch.res_name : "", c->name);
		for(i = 0; i < len; i++)
			if(rreg[i].clregex && !regexec(rreg[i].clregex, prop, 1, &tmp, 0))
				c->isfloat = rule[i].isfloat;
		if(ch.res_class)
			XFree(ch.res_class);
		if(ch.res_name)
			XFree(ch.res_name);
	}
}

void
togglevisible(Arg *arg) {
	if(!sel)
		return;
	sel->visible = !sel->visible;
	arrange();
}
