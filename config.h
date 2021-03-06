/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "DejaVu Sans:size=10", "Font Awesome 5 Free:size=9:antialias=true" };
static const char dmenufont[]       = "DejaVu Sans:size=10";
static const char bg[]              = "#282936";
static const char fg[]              = "#e9e9f4";
static const char color[]           = "#ea51b2";
static const char alt_color[]       = "#b45bcf";
static const char *colors[][3]      = {
    /*               fg         bg         border   */
	[SchemeNorm] = { fg,        bg,        bg },
	[SchemeSel]  = { alt_color, bg,        alt_color },
	[SchemeStatus]  = { fg,     bg,        "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]  = { color, bg,        "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeTagsNorm]  = { fg,   bg,        "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]  = { alt_color, bg,    "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm]  = { fg,   bg,        "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* gaps config */
static const int gappih = 10;
static const int gappiv = 10;
static const int gappov = 10;
static const int gappoh = 10;
static const int smartgaps = 0;
static const int smartborders = 1;

/* bar padding */
static const int vertpad = 5;
static const int sidepad = 8;

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb",  bg, "-nf", fg, "-sb", bg, "-sf", color, "-l", "10", NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,             		    XK_Return, spawn,          {.v = termcmd } },
    { MODKEY,                       XK_f,      spawn,          SHCMD("firefox") },
    { MODKEY,                       XK_r,      spawn,          SHCMD("st -e ranger") },
    { MODKEY,                       XK_m,      spawn,          SHCMD("st -e neomutt") },
    { MODKEY,                       XK_i,      spawn,          SHCMD("/home/lrossi/.local/bin/img_menu") },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,		                XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    /* System controls                                                          */
    { 0,                     XF86XK_AudioMute, spawn,          SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
    { MODKEY,                       XK_F1,     spawn,          SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
    { 0,              XF86XK_AudioRaiseVolume, spawn,          SHCMD("pamixer --allow-boost -i 3; kill -44 $(pidof dwmblocks)") },
    { MODKEY,                       XK_F3,     spawn,          SHCMD("pamixer --allow-boost -i 3; kill -44 $(pidof dwmblocks)") },
    { 0,              XF86XK_AudioLowerVolume, spawn,          SHCMD("pamixer --allow-boost -d 3; kill -44 $(pidof dwmblocks)") },
    { MODKEY,                       XK_F2,     spawn,          SHCMD("pamixer --allow-boost -d 3; kill -44 $(pidof dwmblocks)") },
    { 0,                            XK_Print,  spawn,          SHCMD("scrot ~/pix/$(date '+%Y-%m-%d_%H:%M:%S').png") },
    { ShiftMask,                    XK_Print,  spawn,          SHCMD("scrot -s ~/pix/$(date '+%Y-%m-%d_%H:%M:%S').png") },
    /* Tag controls                                                              */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_e,      quit,           {0} },
    { MODKEY,                       XK_x,      spawn,          SHCMD("slock") },
    { MODKEY|ShiftMask,             XK_x,      spawn,          SHCMD("shutdown -h now") },
    { MODKEY|ShiftMask,             XK_r,      spawn,          SHCMD("reboot") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

