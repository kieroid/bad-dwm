/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* theme */
static const char *fonts[]		= { "monospace:size=10" };	/* dwm font */
static const char dmenufont[]		= "monospace:size=10";		/* dmenu font */
static const char col_b[]		= "#000000";			/* black color */
static const char col_g[]		= "#555555";			/* grey color */
static const char col_w[]		= "#ffffff";			/* white color */
static const char col_re[]		= "#aa0000";			/* red color */
static const char col_gr[]		= "#00aa00";			/* green color */
static const char col_bl[]		= "#0000ff";			/* blue color */
static const char col_ye[]		= "#ffff00";			/* yellow color */
static const unsigned int baralpha	= 0xd0;				/* transparency */
static const unsigned int borderalpha	= OPAQUE;			/* border transparency */

/* options */
static const char *tags[]		= { "1", "2", "3", "4", "5", "6", "7" };
static const unsigned int snap		= 5;		/* snap pixel */
static const unsigned int gappih	= 12;		/* horiz inner gap between windows */
static const unsigned int gappiv	= 12;		/* vert inner gap between windows */
static const unsigned int gappoh	= 12;		/* horiz outer gap between windows and screen edge */
static const unsigned int gappov	= 12;		/* vert outer gap between windows and screen edge */
static       int smartgaps		= 1;		/* 1 means no outer gap when there is only one window */
static const int showbar		= 1;		/* 0 means no bar */
static const int topbar			= 1;		/* 0 means bottom bar */
static const float mfact		= 0.5;		/* factor of master area size [0.05..0.95] */
static const int nmaster		= 1;		/* number of clients in master area */
static const int resizehints		= 1;		/* 1 means respect size hints in tiled resizals */
static const int lockfullscreen		= 0;		/* 1 will force focus on the fullscreen window */
static const unsigned int borderpx = 1;

/* rules */
static const char *colors[][3]      = { 
	[SchemeNorm] = { col_w, col_b, col_g }, 
	[SchemeSel]  = { col_b, col_w,  col_w }, 
	[SchemeStatus]={ col_w, col_b,  NULL  } 
}; 

static const unsigned int alphas[][3]      = { 
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha }, 
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha }, 
	[SchemeStatus]={ OPAQUE, baralpha, borderalpha } 
};

static const char *tagsel[][2] = {
	/*   fg         bg    */
	{ col_w, col_b }, /* norm */
	{ col_b, col_w }, /* sel */
	{ col_b, col_g }, /* occ but not sel */
	{ col_b, col_re }, /* has pinned tag */
};


static const Rule rules[] = { 
	{ "krita", NULL, NULL, 0, 1, -1 }
};
#define FORCE_VSPLIT 1 /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[@]",      spiral},
	{ "[M]",      monocle},
	{ "[]=",      tile },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },
	{ NULL,       NULL },
};

/* status bar */
static const Block blocks[] = {
	/* fg     	command                         interval        signal */
	{ col_ye,	"special_disk",			9000,		1},
	{ col_bl,	"special_vol",			0,		2},
	{ col_gr,	"special_mem",			0,		3},
	{ col_re,	"special_battery",		10,		4},
	{ col_w,	"special_date",			20,		5}
};

#define INVERSED       0
static char delimiter[] = " ";
#define CMDLENGTH      50

/* key definitions */
#define MODKEY Mod4Mask
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview, 		{.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,		{.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,		{.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,		{.ui = 1 << TAG} },

/* multimedia commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run_history", "-fn", dmenufont, "-nb", col_b, "-nf", col_w, "-sb", col_w, "-sf", col_b, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *dmenunm[] = { "dmenu_nm", NULL };
static const char *brupcmd[] = { "brightnessctl", "set", "10%+", NULL };
static const char *brdowncmd[] = { "brightnessctl", "set", "10%-", NULL };
static const char *upvol[]      = { "/usr/bin/amixer",  "set", "Master", "5%+", NULL };
static const char *downvol[]    = { "/usr/bin/amixer",  "set", "Master", "5%-", NULL };
static const char *mutevol[]    = { "/usr/bin/amixer", "set", "Master", "toggle", NULL };
static const char *micmute[]	= { "/usr/bin/amixer", "set", "Capture", "toggle", NULL };
static const char *medplaypausecmd[] = { "playerctl", "play-pause", NULL };
static const char *mednextcmd[] = { "playerctl", "next", NULL };
static const char *medprevcmd[] = { "playerctl", "previous", NULL };
static const char *rotatescreen[] = { "special_rotate", NULL };
static const char *togglekeys[] = { "special_keyboard", NULL };
static const char *cmdprintscreen[]  = { "flameshot", "gui", NULL };

/* launcher commands */
static const Launcher launchers[] = {
	/* command	name to display */
	{ dmenucmd,	"l" },
	{ termcmd,	"t" },
};

/* keybinds */
#include "exitdwm.c"
static const Key keys[] = {
	/* modifier                     key				function        argument */
	{ MODKEY,                       XK_r,      			spawn,          {.v = dmenucmd } },
	{ MODKEY,	                XK_Return, 			spawn,          {.v = termcmd } },
	{ MODKEY, 	                XK_Right,  			focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Left,   			focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Right,  			incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Left,   			incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_Tab,    			view,           {0} },
	{ MODKEY,	                XK_q,      			killclient,     {0} },
	{ MODKEY,	                XK_space,  			togglefloating, {0} },
	{ MODKEY,                       XK_w,      			view,           {.ui = ~0 } },
	{ MODKEY,	                XK_e,      			tag,            {.ui = ~0 } },
	TAGKEYS(                        XK_1,           		           	0)
	TAGKEYS(                        XK_2,           		           	1)
	TAGKEYS(                        XK_3,           		           	2)
	TAGKEYS(                        XK_4,           		           	3)
	TAGKEYS(                        XK_5,           		           	4)
	TAGKEYS(                        XK_6,           		           	5)
	TAGKEYS(                        XK_7,           		           	6)
	{ MODKEY|ShiftMask,             XK_q,      			exitdwm,        {0} },
	{ 0,				XK_Print,			spawn,		{.v = cmdprintscreen } },
	{ 0, 				XF86XK_MonBrightnessUp,  	spawn,          {.v = brupcmd} },
	{ 0, 				XF86XK_MonBrightnessDown,	spawn,          {.v = brdowncmd} },
	{ 0,				XF86XK_AudioLowerVolume, 	spawn, 		{.v = downvol } },
	{ 0,				XF86XK_AudioMicMute, 		spawn, 		{.v = micmute } },
	{ 0,				XF86XK_AudioMute, 		spawn, 		{.v = mutevol } },
	{ 0,				XF86XK_AudioRaiseVolume, 	spawn, 		{.v = upvol } },
	{ 0, 				XF86XK_AudioPlay, 		spawn, 		{.v = medplaypausecmd } },
	{ 0, 				XF86XK_AudioNext, 		spawn, 		{.v = mednextcmd } },
	{ 0, 				XF86XK_AudioPrev, 		spawn, 		{.v = medprevcmd } },
	{ 0,				XF86XK_HomePage, 		spawn, 		{.v = rotatescreen } },
	{ 0,				XF86XK_TaskPane, 		spawn, 		{.v = togglekeys } },
};

/* button definitions */
static const Button buttons[] = {
	/* click                	event mask      button          function        argument */
	{ ClkLtSymbol,			0,		Button1,	cyclelayout,	{.i = -1 } },
	{ ClkLtSymbol,			0,		Button3,	cyclelayout,	{.i = +1 } },
	{ ClkTagBar,            	MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            	MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          	0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        	0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkStatusText,        0,              Button1,        sendstatusbar,   {.i = 1 } },
	{ ClkStatusText,        0,              Button2,        sendstatusbar,   {.i = 2 } },
	{ ClkStatusText,        0,              Button3,        sendstatusbar,   {.i = 3 } },
	{ ClkStatusText,        0,              Button4,        sendstatusbar,   {.i = 4 } },
	{ ClkStatusText,        0,              Button5,        sendstatusbar,   {.i = 5 } },
	{ ClkStatusText,        ShiftMask,      Button1,        sendstatusbar,   {.i = 6 } },
	{ ClkClientWin,         	MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         	MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         	MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            	0,              Button1,        view,           {0} },
	{ ClkTagBar,            	0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            	MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            	MODKEY,         Button3,        toggletag,      {0} },
};

void
setlayoutex(const Arg *arg)
{
	setlayout(&((Arg) { .v = &layouts[arg->i] }));
}

void
viewex(const Arg *arg)
{
	view(&((Arg) { .ui = 1 << arg->ui }));
}

void
viewall(const Arg *arg)
{
	view(&((Arg){.ui = ~0}));
}

void
toggleviewex(const Arg *arg)
{
	toggleview(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagex(const Arg *arg)
{
	tag(&((Arg) { .ui = 1 << arg->ui }));
}

void
toggletagex(const Arg *arg)
{
	toggletag(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagall(const Arg *arg)
{
	tag(&((Arg){.ui = ~0}));
}

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signame> [<type> <value>]"` */
static Signal signals[] = {
	/* signum           function */
	{ "focusstack",     focusstack },
	{ "setmfact",       setmfact },
	{ "togglebar",      togglebar },
	{ "incnmaster",     incnmaster },
	{ "togglefloating", togglefloating },
	{ "focusmon",       focusmon },
	{ "tagmon",         tagmon },
	{ "zoom",           zoom },
	{ "view",           view },
	{ "viewall",        viewall },
	{ "viewex",         viewex },
	{ "toggleview",     view },
	{ "toggleviewex",   toggleviewex },
	{ "tag",            tag },
	{ "tagall",         tagall },
	{ "tagex",          tagex },
	{ "toggletag",      tag },
	{ "toggletagex",    toggletagex },
	{ "killclient",     killclient },
	{ "quit",           quit },
	{ "setlayout",      setlayout },
	{ "setlayoutex",    setlayoutex },
};



