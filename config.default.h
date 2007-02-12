/* (C)opyright MMVI-MMVII Anselm R. Garbe <garbeam at gmail dot com>
 * See LICENSE file for license details.
 */

#define BORDERPX		1
#define NORMCOLOR		"#333"
#define SELCOLOR		"#69c"

#define SY			0
#define SH			DisplayHeight(0		/* sy = YOFFSET */
#define MASTER			600		/* per thousand */
#define MODKEY			Mod1Mask
#define NMASTER			1		/* clients in master area */
#define SNAP			40		/* pixel */

#define KEYS \
static Key key[] = { \
	/* modifier			key		function	argument */ \
	{ MODKEY|ShiftMask,		XK_Return,	spawn, \
		{ .cmd = "exec uxterm -bg '#222' -fg '#eee' -cr '#eee' +sb -fn '-*-terminus-medium-r-*-*-14-*-*-*-*-*-*-*'" } }, \
	{ MODKEY,			XK_p,		spawn, \
		{ .cmd = "exe=\"$(lsx `echo $PATH | sed 's/:/ /g'` | sort -u " \
			" | dmenu -fn '-*-terminus-medium-r-*-*-14-*-*-*-*-*-*-*' -nb '#222' -nf '#ccc' " \
			"-sb '#555' -sf '#fff')\" && exec $exe" } }, \
	{ MODKEY,			XK_a,		attach,		{ 0 } }, \
	{ MODKEY,			XK_d,		detach,		{ 0 } }, \
	{ MODKEY,			XK_j,		focusnext,	{ 0 } }, \
	{ MODKEY,			XK_k,		focusprev,	{ 0 } }, \
	{ MODKEY,			XK_Return,	zoom,		{ 0 } }, \
	{ MODKEY,			XK_g,		resizemaster,	{ .i = 15 } }, \
	{ MODKEY,			XK_s,		resizemaster,	{ .i = -15 } }, \
	{ MODKEY,			XK_i,		incnmaster,	{ .i = 1 } }, \
	{ MODKEY,			XK_r,		incnmaster,	{ .i = -1 } }, \
	{ MODKEY|ShiftMask,		XK_c,		killclient,	{ 0 } }, \
	{ MODKEY,			XK_space,	toggleview,	{ 0 } }, \
	{ MODKEY|ShiftMask,		XK_space,	togglefloat,	{ 0 } }, \
	{ MODKEY|ShiftMask,		XK_q,		quit,		{ 0 } }, \
};

#define FLOATS \
static char *floats[] = { "Gimp.*", "MPlayer.*", "Acroread.*", NULL };
