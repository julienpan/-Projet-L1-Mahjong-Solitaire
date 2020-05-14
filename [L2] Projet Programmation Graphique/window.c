/*!\file window.c
 */
#include <stdlib.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4dh.h>
#include <GL4D/gl4duw_SDL2.h>
#include "animations.h"
#include "audioHelper.h"

static void init(void);
static void quit(void);
static void resize(int w, int h);
static void keydown(int keycode);

static GL4DHanime _animations[] = {

 { 4400,  animation1, NULL, NULL},
 { 4000, animation2, NULL, NULL},
 { 2000, animation3, NULL, NULL},
 { 2400, animation4, NULL, NULL},
 { 1000, animation5, NULL, NULL},
 { 500, animation5bis, NULL, NULL},
 { 5000, animation6, animation7, reflet},
 { 4000, animation7, animation8, reflet},
 { 2800, animation9, animation10, reflet},
 { 3000, animation10, NULL, transition1},
 { 2000, animation11, animation12, reflet},
 { 2000, animation12, animation12bis2, transition2},
 { 2000, animation12bis, NULL, NULL},
 { 1500, animation13, NULL, NULL},
 { 2500, animation14, animation15, reflet},
 { 1500, animation15, animation16, reflet},
 { 1500, animation16, animation17, transition2},
 { 3500, animation18, NULL, NULL},
 { 1700,  animation19, NULL, NULL },
 { 1700, animation20, NULL, NULL },
 { 1700, animation19, NULL, NULL },
 { 300, animation21, NULL, NULL },
 { 300, animation22, NULL, NULL },
 { 300, animation21, NULL, NULL },
 { 300, animation22, NULL, NULL },
 { 300, animation21, NULL, NULL },
 { 300, animation22, NULL, NULL },
 { 1000, animation23, NULL, NULL },
 { 2000, animation24, NULL, NULL },
 { 2000, animation25, NULL, NULL},
 { 17480, animation26, animation27, transition2},
 {0, NULL, NULL, NULL }
};


static GLfloat _dim[] = {1000, 800};


int main(int argc, char ** argv) {

  if(!gl4duwCreateWindow(argc, argv, "Un projet", 
			 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			 _dim[0], _dim[1],
			 SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN))
    return 1;
  init();
  atexit(quit);
  gl4duwResizeFunc(resize);
  gl4duwKeyDownFunc(keydown);
  gl4duwDisplayFunc(gl4dhDraw);

  ahInitAudio("DeathAcc.mp3");
  gl4duwMainLoop();
  return 0;
}

static void init(void) {
  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
  gl4dhInit(_animations, _dim[0], _dim[1], animationsInit);
  resize(_dim[0], _dim[1]);
}


static void resize(int w, int h) {
  _dim[0] = w; _dim[1] = h;
  glViewport(0, 0, _dim[0], _dim[1]);
}


static void keydown(int keycode) {
  switch(keycode) {
  case SDLK_ESCAPE:
  case 'q':
    exit(0);
  default: break;
  }
}


static void quit(void) {
  ahClean();
  gl4duClean(GL4DU_ALL);
}
