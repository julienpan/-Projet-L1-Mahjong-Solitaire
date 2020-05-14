/*!\file animations.c
 */
#include <GL4D/gl4dh.h>
#include "audioHelper.h"
#include "animations.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <GL4D/gl4dg.h>
#include <SDL_image.h>
#include <GL4D/gl4duw_SDL2.h>
#include <GL4D/gl4du.h>

/*!\brief identifiant de la géométrie QUAD GL4Dummies */
static GLuint _quadId = 0;
//static void LoadTexture(GLuint id, const char * filename);


typedef struct site_t site_t;

struct site_t {
  int x, y;
  Uint32 color;
  int alive;
};

static site_t * diag = NULL;
static int nbCells, radius;

int circleNOM(int x0, int y0, int r) {
  int x, y, del, incH, incO, t = M_SQRT1_2 * r + 1;
  del = 3 - (r << 1);
  incH = 6;
  incO = 10 - (r << 2);
  for(x = 0, y = r; x <= t; x++, incH += 4, incO += 4) {
    if(IN_SCREEN(x0 + x, y0 + y)) gl4dpPutPixel(x0 + x, y0 + y);
    if(IN_SCREEN(x0 + x, y0 - y)) gl4dpPutPixel(x0 + x, y0 - y);
    if(IN_SCREEN(x0 - x, y0 + y)) gl4dpPutPixel(x0 - x, y0 + y);
    if(IN_SCREEN(x0 - x, y0 - y)) gl4dpPutPixel(x0 - x, y0 - y);
    if(IN_SCREEN(x0 + y, y0 + x)) gl4dpPutPixel(x0 + y, y0 + x);
    if(IN_SCREEN(x0 + y, y0 - x)) gl4dpPutPixel(x0 + y, y0 - x);
    if(IN_SCREEN(x0 - y, y0 + x)) gl4dpPutPixel(x0 - y, y0 + x);
    if(IN_SCREEN(x0 - y, y0 - x)) gl4dpPutPixel(x0 - y, y0 - x);
    if(del < 0) del += incH;
    else {
      y--;
      if(IN_SCREEN(x0 + x, y0 + y)) gl4dpPutPixel(x0 + x, y0 + y);
      if(IN_SCREEN(x0 + x, y0 - y)) gl4dpPutPixel(x0 + x, y0 - y);
      if(IN_SCREEN(x0 - x, y0 + y)) gl4dpPutPixel(x0 - x, y0 + y);
      if(IN_SCREEN(x0 - x, y0 - y)) gl4dpPutPixel(x0 - x, y0 - y);
      if(IN_SCREEN(x0 + y, y0 + x)) gl4dpPutPixel(x0 + y, y0 + x);
      if(IN_SCREEN(x0 + y, y0 - x)) gl4dpPutPixel(x0 + y, y0 - x);
      if(IN_SCREEN(x0 - y, y0 + x)) gl4dpPutPixel(x0 - y, y0 + x);
      if(IN_SCREEN(x0 - y, y0 - x)) gl4dpPutPixel(x0 - y, y0 - x);
      incO += 4;
      del += incO;
    }
  }
  return 1;
}

static void diagFree(void) {
  if(diag) {
    free(diag);
    diag = NULL;
  }  
}

static void diagInit(int n) {
  int x, y, i;
  Uint8 r, g, b;
  nbCells = n;
  if(diag)
    diagFree();
  else
    atexit(diagFree);
  radius = 1;
  diag = malloc(nbCells * sizeof *diag);
  assert(diag);
  gl4dpClearScreen();
  for(i = 0; i < nbCells; i++) {
    do {
      x = (int)(gl4dmURand() * gl4dpGetWidth());
      y = (int)(gl4dmURand() * gl4dpGetHeight());
    } while(gl4dpGetPixel(x, y));
    r = gl4dmURand() * 256;
    g = gl4dmURand() * 0;
    b = gl4dmURand() * 0;
    diag[i].x = x;
    diag[i].y = y;
    diag[i].color = RGB(r,g,b);
    diag[i].alive = 1;
  }
}

static int diagGrow(void) {
  int i, fini = 1;
  for(i = 0; i < nbCells; i++) {
    if(!diag[i].alive) continue;
    gl4dpSetColor(diag[i].color);
    if(circleNOM(diag[i].x, diag[i].y, radius))
      fini = 0;
    else
      diag[i].alive = 0;
  }
  radius++;
  return !fini;
}
 
static void texture(GLuint id, const char * filename) {
  SDL_Surface * t;
  glBindTexture(GL_TEXTURE_2D, id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if( (t = IMG_Load(filename)) != NULL ) {
#ifdef __APPLE__
    int mode = t->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR;
#else
    int mode = t->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
#endif       
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->w, t->h, 0, mode, GL_UNSIGNED_BYTE, t->pixels);
    SDL_FreeSurface(t);
  } else {
    fprintf(stderr, "can't open file %s : %s\n", filename, SDL_GetError());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  }
}


void transition1(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state) {
  /* INITIALISEZ VOS VARIABLES */
  int vp[4], i;
  GLint tId;
  static GLuint tex[3], pId;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE TRANSITION (SES VARIABLES <STATIC>s) */
    glGetIntegerv(GL_VIEWPORT, vp);
    glGenTextures(3, tex);
    for(i = 0; i < 3; i++) {
      glBindTexture(GL_TEXTURE_2D, tex[i]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vp[2], vp[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
    
    texture(tex[2], "picture/titre.jpg");
    pId = gl4duCreateProgram("<vs>shaders/basici.vs", "<fs>shaders/mixi.fs", NULL);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    if(tex[0]) {
      glDeleteTextures(3, tex);
      tex[0] = tex[1] = 0;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR LES DEUX ANIMATIONS EN FONCTION DU SON */
    if(a0) a0(state);
    if(a1) a1(state);
    return;
  default: /* GL4DH_DRAW */
    /* RECUPERER L'ID DE LA DERNIERE TEXTURE ATTACHEE AU FRAMEBUFFER */
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &tId);
    /* JOUER LES DEUX ANIMATIONS */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[0],  0);
    if(a0) a0(state);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[1],  0);
    if(a1) a1(state);
    /* MIXER LES DEUX ANIMATIONS */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tId,  0);
    glDisable(GL_DEPTH);
    glUseProgram(pId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex[1]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, tex[2]);
    if(et / (GLfloat)t > 1) {
      fprintf(stderr, "%d-%d -- %f\n", et, t, et / (GLfloat)t);
      exit(0);
    }
    glUniform1f(glGetUniformLocation(pId, "dt"), et / (GLfloat)t);
    glUniform1i(glGetUniformLocation(pId, "tex0"), 0);
    glUniform1i(glGetUniformLocation(pId, "tex1"), 1);
    glUniform1i(glGetUniformLocation(pId, "tex2"), 2);
    gl4dgDraw(_quadId);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    return;
  }
}


void transition2(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state) {
  /* INITIALISEZ VOS VARIABLES */
  int vp[4], i;
  GLint tId;
  static GLuint tex[3], pId;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE TRANSITION (SES VARIABLES <STATIC>s) */
    glGetIntegerv(GL_VIEWPORT, vp);
    glGenTextures(3, tex);
    for(i = 0; i < 3; i++) {
      glBindTexture(GL_TEXTURE_2D, tex[i]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vp[2], vp[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
    
    texture(tex[2], "picture/fond.jpg");
    pId = gl4duCreateProgram("<vs>shaders/basici.vs", "<fs>shaders/mixi.fs", NULL);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    if(tex[0]) {
      glDeleteTextures(3, tex);
      tex[0] = tex[1] = 0;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR LES DEUX ANIMATIONS EN FONCTION DU SON */
    if(a0) a0(state);
    if(a1) a1(state);
    return;
  default: /* GL4DH_DRAW */
    /* RECUPERER L'ID DE LA DERNIERE TEXTURE ATTACHEE AU FRAMEBUFFER */
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &tId);
    /* JOUER LES DEUX ANIMATIONS */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[0],  0);
    if(a0) a0(state);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[1],  0);
    if(a1) a1(state);
    /* MIXER LES DEUX ANIMATIONS */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tId,  0);
    glDisable(GL_DEPTH);
    glUseProgram(pId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex[1]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, tex[2]);
    if(et / (GLfloat)t > 1) {
      fprintf(stderr, "%d-%d -- %f\n", et, t, et / (GLfloat)t);
      exit(0);
    }
    glUniform1f(glGetUniformLocation(pId, "dt"), et / (GLfloat)t);
    glUniform1i(glGetUniformLocation(pId, "tex0"), 0);
    glUniform1i(glGetUniformLocation(pId, "tex1"), 1);
    glUniform1i(glGetUniformLocation(pId, "tex2"), 2);
    gl4dgDraw(_quadId);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    return;
  }
}

void reflet(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state) {
  /* INITIALISEZ VOS VARIABLES */
  int vp[4], i;
  GLint tId;
  static GLuint tex[2], pId;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE TRANSITION (SES VARIABLES <STATIC>s) */
    glGetIntegerv(GL_VIEWPORT, vp);
    glGenTextures(2, tex);
    for(i = 0; i < 2; i++) {
      glBindTexture(GL_TEXTURE_2D, tex[i]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vp[2], vp[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
    pId = gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/mix.fs", NULL);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    if(tex[0]) {
      glDeleteTextures(2, tex);
      tex[0] = tex[1] = 0;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR LES DEUX ANIMATIONS EN FONCTION DU SON */
    if(a0) a0(state);
    if(a1) a1(state);
    return;
  default: /* GL4DH_DRAW */
    /* RECUPERER L'ID DE LA DERNIERE TEXTURE ATTACHEE AU FRAMEBUFFER */
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &tId);
    /* JOUER LES DEUX ANIMATIONS */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[0],  0);
    if(a0) a0(state);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[1],  0);
    if(a1) a1(state);
    /* MIXER LES DEUX ANIMATIONS */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tId,  0);
    glDisable(GL_DEPTH);
    glUseProgram(pId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex[1]);
    if(et / (GLfloat)t > 1) {
      fprintf(stderr, "%d-%d -- %f\n", et, t, et / (GLfloat)t);
      exit(0);
    }
    glUniform1f(glGetUniformLocation(pId, "dt"), et / (GLfloat)t);
    glUniform1i(glGetUniformLocation(pId, "tex0"), 0);
    glUniform1i(glGetUniformLocation(pId, "tex1"), 1);
    gl4dgDraw(_quadId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    return;
  }
}

void animation1(int state) {
  // INITIALISEZ VOS VARIABLES
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {0.0, 0.0, 1, 1}, r2[] = {0, 0, 1, 1}, r = 0.0;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    // INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) 
    surf = IMG_Load("picture/batiment.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    // LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s 
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    // METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON 
    return;
  default: // GL4DH_DRAW 
    // JOUER L'ANIMATION 
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    r += 0.005;
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}



void animation2(int state) {
  // INITIALISEZ VOS VARIABLES
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {33.0, 33.0, 0, 0}, r2[] = {36, 36, -0.01, -0.06}, r = 0.0;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    // INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) 
    surf = IMG_Load("picture/esca3-ConvertImage.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    // LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s 
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    // METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON 
    return;
  default: // GL4DH_DRAW 
    // JOUER L'ANIMATION 
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    r2[3] += 0.0006;
    r2[2] += 0.0006;
    r2[0] -= 0.045;
    r2[1] -= 0.045;

    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}

void animation3(int state) {
  // INITIALISEZ VOS VARIABLES
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {0.0, 0.0, 1, 1}, r2[] = {0, 0, 1, 1}, r = 7;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    // INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) 
    surf = IMG_Load("picture/esca2-ConvertImage.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    // LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s 
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    // METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON 
    return;
  default: // GL4DH_DRAW 
    // JOUER L'ANIMATION 
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    r += 0.001;
    r2[0] += 0.0001;
    r2[3] -= 0.0005;
    r2[2] -= 0.00001;
    
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}

void animation4(int state) {
  // INITIALISEZ VOS VARIABLES
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {40.0, 40.0, 0, 0}, r2[] = {28, 28, 0, 0}, r = 0;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    // INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) 
    surf = IMG_Load("picture/ryukkira-ConvertImage.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    // LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s 
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    // METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON 
    return;
  default: // GL4DH_DRAW 
    // JOUER L'ANIMATION 
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    r2[3] += 0.0002;
    r2[2] += 0.0003;
    r2[0] -= 0.001;
    r2[1] -= 0.001;
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}



void animation5(int state) {
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {40.0, 40.0, 0, 0}, r2[] = {40, 40, 0, 0}, r = 0;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    surf = IMG_Load("picture/kira1-ConvertImage.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
	r2[2] += 0.0001;
	r2[3] += 0.0001;
	r2[0] -= 0.01;
	r2[1] -= 0.01;
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}

void animation5bis(int state) {
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {40.0, 40.0, 0, 0}, r2[] = {30, 30, 0, 0}, r = 0;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    surf = IMG_Load("picture/kira2.jpeg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    r2[2] += 0.0001;
    r2[3] += 0.0001;
    r2[0] -= 0.01;
    r2[1] -= 0.01;
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}


void animation6(int state) {
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {80.0, 80.0, 0, 0}, r2[] = {60, 60, 0, 0}, r = 0;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    surf = IMG_Load("picture/esca.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    /*r1[0] += 0.08;
    r2[1] -= 0.08;*/
    r2[3] += 0.00004;
    r2[2] += 0.00004;
    r2[0] -= 0.01;
    r2[1] -= 0.01;
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}


void animation7(int state) {
	
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {40.0, 40.0, 0, 0}, r2[] = {40, 30, 0, 0},r = 0 ;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    surf = IMG_Load("picture/lumiere-ConvertImage.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);

    r2[3] += 0.00001;
    r2[2] += 0.00001;
    r2[0] -= 0.001;
    r2[1] -= 0.001;
	
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}

void animation8(int state) {
	
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {40.0, 40.0, 0, 0}, r2[] = {40, 30, 0, 0},r = 0 ;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    surf = IMG_Load("picture/light-ConvertImage.png");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);

    r2[3] += 0.00001;
    r2[2] += 0.00001;
    r2[0] -= 0.001;
    r2[1] -= 0.001;
	
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}

void animation9(int state) {
	
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {30.0, 40.0, 0, 0}, r2[] = {29.9, 29.68, 0, 0},r = 0 ;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    surf = IMG_Load("picture/light-ConvertImage.png");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}



void animation10(int state) {
	
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {30.0, 40.0, 0, 0}, r2[] = {29.9, 29.68, 0, 0},r = 0 ;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    surf = IMG_Load("picture/wall-ConvertImage.png");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);

    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}

void animation11(int state) {
	
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {40.0, 40.0, 0, 0}, r2[] = {40.0, 40.0, 0, 0},r = 0 ;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    surf = IMG_Load("picture/ryuklune-ConvertImage.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}


void animation12(int state) {
	
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {40.0, 40.0, 0, 0}, r2[] = {40.0, 40.0, 0, 0},r = 0 ;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    surf = IMG_Load("picture/lune2.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}

void animation12bis2(int state) {
	
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {40.0, 40.0, 0, 0}, r2[] = {40.0, 40.0, 0, 0},r = 0 ;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    surf = IMG_Load("picture/lune2bis.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}


void animation12bis(int state) {
  /* INITIALISEZ VOS VARIABLES */
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    gl4dpClearScreenWith(0x0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpDeleteScreen();
    gl4dpDeleteScreen();
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    if(!diagGrow()) {
    gl4dpUpdateScreen(NULL);
    diagInit(100);
    }
    gl4dpUpdateScreen(NULL);
    return;
  }
}


void animation13(int state) {
	
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {40.0, 40.0, 0, 0}, r2[] = {40.0, 40.0, 0, 0},r = 0 ;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    surf = IMG_Load("picture/po2.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    r2[3] += 0.005;
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}

void animation14(int state) {
	
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {40.0, 40.0, 0, 0}, r2[] = {40.0, 40.0, 0, 0},r = 0 ;
  static SDL_Surface * surf = NULL;
  static SDL_Surface* surf2 = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    surf2 = IMG_Load("picture/pomme3.jpg");
    surf = IMG_Load("picture/catch-ConvertImage.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpCopyFromSDLSurfaceWithTransforms(surf2, s, t);
    
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf){
      SDL_FreeSurface(surf2);
      SDL_FreeSurface(surf);
      
      surf = NULL;
      surf2 = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}

void animation15(int state) {
	
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {40.0, 40.0, 0, 0}, r2[] = {40.0, 40.0, 0, 0},r = 0 ;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    surf = IMG_Load("picture/catch.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}

void animation16(int state) {
	
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {40.0, 40.0, 0, 0}, r2[] = {40.0, 40.0, 0, 0},r = 0 ;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    surf = IMG_Load("picture/catch.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}


void animation17(int state) {
	
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {40.0, 40.0, 0, 0}, r2[] = {40.0, 40.0, 0, 0},r = 0 ;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    surf = IMG_Load("picture/ryuk3.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}

void animation18(int state) {
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {40.0, 40.0, 0, 0}, r2[] = {30, 30, 0, 0}, r = 0;
  static SDL_Surface * surf = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    surf = IMG_Load("picture/paysage.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(surf, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(surf) {
      SDL_FreeSurface(surf);
      surf = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    gl4dpUpdateScreen(NULL);
    /*r2[0] -= 0.05;
    r2[1] -= 0.05;
    r1[2] -= 0.0001;*/
 
    r2[0] -= 0.05;
    r2[1] -= 0.05;
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}



void animation19(int state) {
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {30.0, 30.0, 0, 0}, r2[] = {30.0, 30.0, 0.0, 0.0}, r = 0;

  static SDL_Surface * sprite = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    sprite = IMG_Load("picture/archi.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(sprite, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    r2[2] += 0.001;
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}


void animation20(int state) {
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {30.0, 30.0, 0, 0}, r2[] = {30.0, 30.0, 0.0, 0.0}, r = 0;

  static SDL_Surface * sprite = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    sprite = IMG_Load("picture/archi.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(sprite, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    r2[3] += 0.001;
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}


void animation21(int state) {
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {45.0, 45.0, 0, 0}, r2[] = {45, 45, 0, 0}, r = 0;
  static SDL_Surface * sprite = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    sprite = IMG_Load("picture/bat4.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(sprite, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    r2[3] += 0.0005;
    r2[2] += 0.0005;
    r2[0] -= 0.05;
    r2[1] -= 0.05;
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}


void animation22(int state) {
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {45.0, 45.0, 0, 0}, r2[] = {45, 45, 0, 0}, r = 0;
  static SDL_Surface * sprite = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    sprite = IMG_Load("picture/bat.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(sprite, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
	    r2[3] += 0.0008;
	    r2[2] += 0.0008;
	    r2[0] -= 0.08;
	    r2[1] -= 0.08;
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}


void animation23(int state) {
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {40.0, 40.0, 0, 0}, r2[] = {32.0, 32.0, 0.12, 0.12}, r = 0;
  static SDL_Surface * sprite = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    sprite = IMG_Load("picture/bat2.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(sprite, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
   r1[0] -= 0.04;
   r2[2] -= 0.0004;
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}


void animation24(int state) {
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {27.8, 40.0, 0, 0}, r2[] = {32.0, 32.0, 0.12, 0.12}, r = 0;
  static SDL_Surface * sprite = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    sprite = IMG_Load("picture/bat2.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(sprite, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    r1[0] += 0.04;
   r2[2] += 0.0004;
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}


void animation25(int state) {
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {78.299492, 45.0, 0, 0.05}, r2[] = {45, 45, 0, 0}, r = 0;
  static SDL_Surface * sprite = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    sprite = IMG_Load("picture/architecture.jpg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(sprite, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    r1[1] += 0.01;
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}


void animation26(int state) {
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {40.0, 40.0, 0, 0}, r2[] = {38.0, 38.0, 0, 0}, r = 0;

  static SDL_Surface * sprite = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    sprite = IMG_Load("picture/titre.jpeg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(sprite, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    r2[0]+= 0.0015;
    r2[1]+= 0.0015;
    gl4dpUpdateScreen(NULL);
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}

void animation27(int state) {
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {40.0, 40.0, 0, 0}, r2[] = {40, 40, 0, 0}, r = 0;

  static SDL_Surface * sprite = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    sprite = IMG_Load("picture/titre2.jpeg");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0x0);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(sprite, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}





		
	



void animationsInit(void) {
  if(!_quadId)
    _quadId = gl4dgGenQuadf();
}
