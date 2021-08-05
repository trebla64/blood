// Base services interface declaration
// for the Build Engine
// by Jonathon Fowler (jonof@edgenetwk.com)

#ifndef __baselayer_h__
#define __baselayer_h__

#ifdef __cplusplus
extern "C" {
#endif

extern int _buildargc;
extern char **_buildargv;

extern char quitevent, appactive;

extern char *startwin_labeltext;

// video
extern int32_t xres, yres, bpp, fullscreen, bytesperline, imageSize;
extern intptr_t frameplace;
extern char offscreenrendering;

extern void (*baselayer_onvideomodechange)(int);

#ifdef USE_OPENGL
struct glinfo {
	const char *vendor;
	const char *renderer;
	const char *version;
	const char *extensions;

	float maxanisotropy;
	char bgra, clamptoedge, texcompr, texnpot;
};
extern struct glinfo glinfo;
#endif

extern char inputdevices;

// keys
#define KEYFIFOSIZ 64
extern char keystatus[256], keyfifo[KEYFIFOSIZ];
extern uint8_t keyfifoplc, keyfifoend;
extern unsigned char keyasciififo[KEYFIFOSIZ], keyasciififoplc, keyasciififoend;

// mouse
extern int32_t mousex, mousey, mouseb;

// joystick
extern int32_t *joyaxis, *joyhat, joyb;
extern char joyisgamepad, joynumaxes, joynumbuttons, joynumhats;
extern int32_t joyaxespresent;



int initsystem(void);
void uninitsystem(void);

void initprintf(const char *, ...);
void debugprintf(const char *,...);

int handleevents(void);

typedef void (*KeyPressCallback)(int32_t,int32_t);
typedef void (*MousePressCallback)(int32_t,int32_t);
typedef void (*JoyPressCallback)(int32_t,int32_t);
int initinput(void);
void uninitinput(void);
void releaseallbuttons(void);
void setkeypresscallback(void (*callback)(int32_t,int32_t));
void setmousepresscallback(void (*callback)(int32_t,int32_t));
void setjoypresscallback(void (*callback)(int32_t,int32_t));
const unsigned char *getkeyname(int num);
const unsigned char *getjoyname(int what, int num);	// what: 0=axis, 1=button, 2=hat

unsigned char bgetchar(void);
int bkbhit(void);
void bflushchars(void);

int initmouse(void);
void uninitmouse(void);
void grabmouse(char a);
void readmousexy(int32_t *x, int32_t *y);
void readmousebstatus(int32_t *b);
void setjoydeadzone(int axis, unsigned short dead, unsigned short satur);
void getjoydeadzone(int axis, unsigned short *dead, unsigned short *satur);

int inittimer(int);
void uninittimer(void);
void sampletimer(void);
uint32_t getticks(void);
int gettimerfreq(void);
void (*installusertimercallback(void (*callback)(void)))(void);

int checkvideomode(int *x, int *y, int c, int fs);
int setvideomode(int x, int y, int c, int fs);
void getvalidmodes(void);
void resetvideomode(void);

void begindrawing(void);
void enddrawing(void);
void showframe(int);

int setpalette(int start, int num, char *dapal);
//int getpalette(int start, int num, char *dapal);
int setgamma(float ro, float go, float bo);

int switchrendermethod(int,int);	// 0 = software, 1 = opengl | bool = reinit

int wm_msgbox(char *name, char *fmt, ...);
int wm_ynbox(char *name, char *fmt, ...);
void wm_setapptitle(char *name);

// baselayer.c
int baselayer_init();

#ifdef __cplusplus
}
#endif

#endif // __baselayer_h__

