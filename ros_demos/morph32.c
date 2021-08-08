/*-
 * morph3d.c - Shows 3D morphing objects
 *
 * Converted to GLUT by brianp on 1/1/98
 *
 * Converted to ROSMesa by David Boddie on 16th June 1998
 *
 * This program was inspired on a WindowsNT(R)'s screen saver. It was written
 * from scratch and it was not based on any other source code.
 *
 * Porting it to xlock (the final objective of this code since the moment I
 * decided to create it) was possible by comparing the original Mesa's gear
 * demo with it's ported version, so thanks for Danny Sung for his indirect
 * help (look at gear.c in xlock source tree). NOTE: At the moment this code
 * was sent to Brian Paul for package inclusion, the XLock Version was not
 * available. In fact, I'll wait it to appear on the next Mesa release (If you
 * are reading this, it means THIS release) to send it for xlock package
 * inclusion). It will probably there be a GLUT version too.
 *
 * Thanks goes also to Brian Paul for making it possible and inexpensive
 * to use OpenGL at home.
 *
 * Since I'm not a native english speaker, my apologies for any gramatical
 * mistake.
 *
 * My e-mail addresses are
 *
 * vianna@cat.cbpf.br
 *         and
 * marcelo@venus.rdc.puc-rio.br
 *
 * Marcelo F. Vianna (Feb-13-1997)
 */

/*
This document is VERY incomplete, but tries to describe the mathematics used
in the program. At this moment it just describes how the polyhedra are
generated. On futhurer versions, this document will be probabbly improved.

Since I'm not a native english speaker, my apologies for any gramatical
mistake.

Marcelo Fernandes Vianna
- Undergraduate in Computer Engeneering at Catholic Pontifical University
- of Rio de Janeiro (PUC-Rio) Brasil.
- e-mail: vianna@cat.cbpf.br or marcelo@venus.rdc.puc-rio.br
- Feb-13-1997

POLYHEDRA GENERATION

For the purpose of this program it's not sufficient to know the polyhedra
vertexes coordinates. Since the morphing algorithm applies a nonlinear
transformation over the surfaces (faces) of the polyhedron, each face has
to be divided into smaller ones. The morphing algorithm needs to transform
each vertex of these smaller faces individually. It's a very time consoming
task.

In order to reduce calculation overload, and since all the macro faces of
the polyhedron are transformed by the same way, the generation is made by
creating only one face of the polyhedron, morphing it and then rotating it
around the polyhedron center.

What we need to know is the face radius of the polyhedron (the radius of
the inscribed sphere) and the angle between the center of two adjacent
faces using the center of the sphere as the angle's vertex.

The face radius of the regular polyhedra are known values which I decided
to not waste my time calculating. Following is a table of face radius for
the regular polyhedra with edge length = 1:

    TETRAHEDRON  : 1/(2*sqrt(2))/sqrt(3)
    CUBE	 : 1/2
    OCTAHEDRON   : 1/sqrt(6)
    DODECAHEDRON : T^2 * sqrt((T+2)/5) / 2     -> where T=(sqrt(5)+1)/2
    ICOSAHEDRON  : (3*sqrt(3)+sqrt(15))/12

I've not found any reference about the mentioned angles, so I needed to
calculate them, not a trivial task until I figured out how :)
Curiously these angles are the same for the tetrahedron and octahedron.
A way to obtain this value is inscribing the tetrahedron inside the cube
by matching their vertexes. So you'll notice that the remaining unmatched
vertexes are in the same straight line starting in the cube/tetrahedron
center and crossing the center of each tetrahedron's face. At this point
it's easy to obtain the bigger angle of the isosceles triangle formed by
the center of the cube and two opposite vertexes on the same cube face.
The edges of this triangle have the following lenghts: sqrt(2) for the base
and sqrt(3)/2 for the other two other edges. So the angle we want is:
     +-----------------------------------------------------------+
     | 2*ARCSIN(sqrt(2)/sqrt(3)) = 109.47122063449069174 degrees |
     +-----------------------------------------------------------+
For the cube this angle is obvious, but just for formality it can be
easily obtained because we also know it's isosceles edge lenghts:
sqrt(2)/2 for the base and 1/2 for the other two edges. So the angle we
want is:
     +-----------------------------------------------------------+
     | 2*ARCSIN((sqrt(2)/2)/1)   = 90.000000000000000000 degrees |
     +-----------------------------------------------------------+
For the octahedron we use the same idea used for the tetrahedron, but now
we inscribe the cube inside the octahedron so that all cubes's vertexes
matches excatly the center of each octahedron's face. It's now clear that
this angle is the same of the thetrahedron one:
     +-----------------------------------------------------------+
     | 2*ARCSIN(sqrt(2)/sqrt(3)) = 109.47122063449069174 degrees |
     +-----------------------------------------------------------+
For the dodecahedron it's a little bit harder because it's only relationship
with the cube is useless to us. So we need to solve the problem by another
way. The concept of Face radius also exists on 2D polygons with the name
Edge radius:
  Edge Radius For Pentagon (ERp)
  ERp = (1/2)/TAN(36 degrees) * VRp = 0.6881909602355867905
  (VRp is the pentagon's vertex radio).
  Face Radius For Dodecahedron
  FRd = T^2 * sqrt((T+2)/5) / 2 = 1.1135163644116068404
Why we need ERp? Well, ERp and FRd segments forms a 90 degrees angle,
completing this triangle, the lesser angle is a half of the angle we are
looking for, so this angle is:
     +-----------------------------------------------------------+
     | 2*ARCTAN(ERp/FRd)	 = 63.434948822922009981 degrees |
     +-----------------------------------------------------------+
For the icosahedron we can use the same method used for dodecahedron (well
the method used for dodecahedron may be used for all regular polyhedra)
  Edge Radius For Triangle (this one is well known: 1/3 of the triangle height)
  ERt = sin(60)/3 = sqrt(3)/6 = 0.2886751345948128655
  Face Radius For Icosahedron
  FRi= (3*sqrt(3)+sqrt(15))/12 = 0.7557613140761707538
So the angle is:
     +-----------------------------------------------------------+
     | 2*ARCTAN(ERt/FRi)	 = 41.810314895778596167 degrees |
     +-----------------------------------------------------------+

*/

#define WIDTH 128
#define HEIGHT 128
#define HEADER 60
#define BPP 32
#define LOG2BPP 5
#define UNITS 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/rosmesa.h>
#include <math.h>
#include <swis.h>
#include <kernel.h>

_kernel_swi_regs	regs;

#define Scale                      0.3

#define VectMul(X1,Y1,Z1,X2,Y2,Z2) (Y1)*(Z2)-(Z1)*(Y2),(Z1)*(X2)-(X1)*(Z2),(X1)*(Y2)-(Y1)*(X2)
#define sqr(A)                     ((A)*(A))

/* Increasing this values produces better image quality, the price is speed. */
/* Very low values produces erroneous/incorrect plotting */
#define tetradivisions             23
#define cubedivisions              20
#define octadivisions              21
#define dodecadivisions            10
#define icodivisions               15

#define tetraangle                 109.47122063449069174
#define cubeangle                  90.000000000000000000
#define octaangle                  109.47122063449069174
#define dodecaangle                63.434948822922009981
#define icoangle                   41.810314895778596167

#ifndef Pi
#define Pi                         3.1415926535897932385
#endif
#define SQRT2                      1.4142135623730951455
#define SQRT3                      1.7320508075688771932
#define SQRT5                      2.2360679774997898051
#define SQRT6                      2.4494897427831778813
#define SQRT15                     3.8729833462074170214
#define cossec36_2                 0.8506508083520399322
#define cos72                      0.3090169943749474241
#define sin72                      0.9510565162951535721
#define cos36                      0.8090169943749474241
#define sin36                      0.5877852522924731292

/*************************************************************************/

static int       mono=0;
static int       smooth=0; /* was 1 - David Boddie (Wed 24th June 1998) */
static GLint     WindH, WindW;
static GLfloat   step=0;
static GLfloat   seno;
static int       object;
static int       edgedivisions;
static void      (*draw_object)( void );
static float     Magnitude;
static float     *MaterialColor[20];

static float front_shininess[] =   {60.0};
static float front_specular[]  =   { 0.7, 0.7, 0.7, 1.0 };
static float ambient[]         =   { 0.0, 0.0, 0.0, 1.0 };
static float diffuse[]         =   { 1.0, 1.0, 1.0, 1.0 };
static float position0[]       =   { 1.0, 1.0, 1.0, 0.0 };
static float position1[]       =   {-1.0,-1.0, 1.0, 0.0 };
static float lmodel_ambient[]  =   { 0.5, 0.5, 0.5, 1.0 };
static float lmodel_twoside[]  =   {GL_TRUE};

static float MaterialRed[]     =   { 0.7, 0.0, 0.0, 1.0 };
static float MaterialGreen[]   =   { 0.1, 0.5, 0.2, 1.0 };
static float MaterialBlue[]    =   { 0.0, 0.0, 0.7, 1.0 };
static float MaterialCyan[]    =   { 0.2, 0.5, 0.7, 1.0 };
static float MaterialYellow[]  =   { 0.7, 0.7, 0.0, 1.0 };
static float MaterialMagenta[] =   { 0.6, 0.2, 0.5, 1.0 };
static float MaterialWhite[]   =   { 0.7, 0.7, 0.7, 1.0 };
static float MaterialGray[]    =   { 0.2, 0.2, 0.2, 1.0 };

#define TRIANGLE(Edge, Amp, Divisions, Z)                                                                        \
{                                                                                                                \
  GLfloat   Xf,Yf,Xa,Yb,Xf2,Yf2;                                                                                 \
  GLfloat   Factor,Factor1,Factor2;                                                                              \
  GLfloat   VertX,VertY,VertZ,NeiAX,NeiAY,NeiAZ,NeiBX,NeiBY,NeiBZ;                                               \
  GLfloat   Ax,Ay,Bx;                                                                                            \
  int       Ri,Ti;                                                                                               \
  GLfloat   Vr=(Edge)*SQRT3/3;                                                                                   \
  GLfloat   AmpVr2=(Amp)/sqr(Vr);                                                                                \
  GLfloat   Zf=(Edge)*(Z);                                                                                       \
                                                                                                                 \
  Ax=(Edge)*(+0.5/(Divisions)), Ay=(Edge)*(-SQRT3/(2*Divisions));                                                \
  Bx=(Edge)*(-0.5/(Divisions));                                                                                  \
                                                                                                                 \
  for (Ri=1; Ri<=(Divisions); Ri++) {                                                                            \
    glBegin(GL_TRIANGLE_STRIP);                                                                                  \
    for (Ti=0; Ti<Ri; Ti++) {                                                                                    \
      Xf=(float)(Ri-Ti)*Ax + (float)Ti*Bx;                                                                       \
      Yf=Vr+(float)(Ri-Ti)*Ay + (float)Ti*Ay;                                                                    \
      Xa=Xf+0.001; Yb=Yf+0.001;                                                                                  \
      Factor=1-(((Xf2=sqr(Xf))+(Yf2=sqr(Yf)))*AmpVr2);                                                           \
      Factor1=1-((sqr(Xa)+Yf2)*AmpVr2);                                                                          \
      Factor2=1-((Xf2+sqr(Yb))*AmpVr2);                                                                          \
      VertX=Factor*Xf;        VertY=Factor*Yf;        VertZ=Factor*Zf;                                           \
      NeiAX=Factor1*Xa-VertX; NeiAY=Factor1*Yf-VertY; NeiAZ=Factor1*Zf-VertZ;                                    \
      NeiBX=Factor2*Xf-VertX; NeiBY=Factor2*Yb-VertY; NeiBZ=Factor2*Zf-VertZ;                                    \
      glNormal3f(VectMul(NeiAX, NeiAY, NeiAZ, NeiBX, NeiBY, NeiBZ));                                             \
      glVertex3f(VertX, VertY, VertZ);                                                                           \
                                                                                                                 \
      Xf=(float)(Ri-Ti-1)*Ax + (float)Ti*Bx;                                                                     \
      Yf=Vr+(float)(Ri-Ti-1)*Ay + (float)Ti*Ay;                                                                  \
      Xa=Xf+0.001; Yb=Yf+0.001;                                                                                  \
      Factor=1-(((Xf2=sqr(Xf))+(Yf2=sqr(Yf)))*AmpVr2);                                                           \
      Factor1=1-((sqr(Xa)+Yf2)*AmpVr2);                                                                          \
      Factor2=1-((Xf2+sqr(Yb))*AmpVr2);                                                                          \
      VertX=Factor*Xf;        VertY=Factor*Yf;        VertZ=Factor*Zf;                                           \
      NeiAX=Factor1*Xa-VertX; NeiAY=Factor1*Yf-VertY; NeiAZ=Factor1*Zf-VertZ;                                    \
      NeiBX=Factor2*Xf-VertX; NeiBY=Factor2*Yb-VertY; NeiBZ=Factor2*Zf-VertZ;                                    \
      glNormal3f(VectMul(NeiAX, NeiAY, NeiAZ, NeiBX, NeiBY, NeiBZ));                                             \
      glVertex3f(VertX, VertY, VertZ);                                                                           \
                                                                                                                 \
    }                                                                                                            \
    Xf=(float)Ri*Bx;                                                                                             \
    Yf=Vr+(float)Ri*Ay;                                                                                          \
    Xa=Xf+0.001; Yb=Yf+0.001;                                                                                    \
    Factor=1-(((Xf2=sqr(Xf))+(Yf2=sqr(Yf)))*AmpVr2);                                                             \
    Factor1=1-((sqr(Xa)+Yf2)*AmpVr2);                                                                            \
    Factor2=1-((Xf2+sqr(Yb))*AmpVr2);                                                                            \
    VertX=Factor*Xf;        VertY=Factor*Yf;        VertZ=Factor*Zf;                                             \
    NeiAX=Factor1*Xa-VertX; NeiAY=Factor1*Yf-VertY; NeiAZ=Factor1*Zf-VertZ;                                      \
    NeiBX=Factor2*Xf-VertX; NeiBY=Factor2*Yb-VertY; NeiBZ=Factor2*Zf-VertZ;                                      \
    glNormal3f(VectMul(NeiAX, NeiAY, NeiAZ, NeiBX, NeiBY, NeiBZ));                                               \
    glVertex3f(VertX, VertY, VertZ);                                                                             \
    glEnd();                                                                                                     \
  }                                                                                                              \
}

#define SQUARE(Edge, Amp, Divisions, Z)                                                                          \
{                                                                                                                \
  int       Xi,Yi;                                                                                               \
  GLfloat   Xf,Yf,Y,Xf2,Yf2,Y2,Xa,Yb;                                                                            \
  GLfloat   Factor,Factor1,Factor2;                                                                              \
  GLfloat   VertX,VertY,VertZ,NeiAX,NeiAY,NeiAZ,NeiBX,NeiBY,NeiBZ;                                               \
  GLfloat   Zf=(Edge)*(Z);                                                                                       \
  GLfloat   AmpVr2=(Amp)/sqr((Edge)*SQRT2/2);                                                                    \
                                                                                                                 \
  for (Yi=0; Yi<(Divisions); Yi++) {                                                                             \
    Yf=-((Edge)/2.0) + ((float)Yi)/(Divisions)*(Edge);                                                           \
    Yf2=sqr(Yf);                                                                                                 \
    Y=Yf+1.0/(Divisions)*(Edge);                                                                                 \
    Y2=sqr(Y);                                                                                                   \
    glBegin(GL_QUAD_STRIP);                                                                                      \
    for (Xi=0; Xi<=(Divisions); Xi++) {                                                                          \
      Xf=-((Edge)/2.0) + ((float)Xi)/(Divisions)*(Edge);                                                         \
      Xf2=sqr(Xf);                                                                                               \
                                                                                                                 \
      Xa=Xf+0.001; Yb=Y+0.001;                                                                                   \
      Factor=1-((Xf2+Y2)*AmpVr2);                                                                                \
      Factor1=1-((sqr(Xa)+Y2)*AmpVr2);                                                                           \
      Factor2=1-((Xf2+sqr(Yb))*AmpVr2);                                                                          \
      VertX=Factor*Xf;        VertY=Factor*Y;         VertZ=Factor*Zf;                                           \
      NeiAX=Factor1*Xa-VertX; NeiAY=Factor1*Y-VertY;  NeiAZ=Factor1*Zf-VertZ;                                    \
      NeiBX=Factor2*Xf-VertX; NeiBY=Factor2*Yb-VertY; NeiBZ=Factor2*Zf-VertZ;                                    \
      glNormal3f(VectMul(NeiAX, NeiAY, NeiAZ, NeiBX, NeiBY, NeiBZ));                                             \
      glVertex3f(VertX, VertY, VertZ);                                                                           \
                                                                                                                 \
      Xa=Xf+0.001; Yb=Yf+0.001;                                                                                  \
      Factor=1-((Xf2+Yf2)*AmpVr2);                                                                               \
      Factor1=1-((sqr(Xa)+Yf2)*AmpVr2);                                                                          \
      Factor2=1-((Xf2+sqr(Yb))*AmpVr2);                                                                          \
      VertX=Factor*Xf;        VertY=Factor*Yf;        VertZ=Factor*Zf;                                           \
      NeiAX=Factor1*Xa-VertX; NeiAY=Factor1*Yf-VertY; NeiAZ=Factor1*Zf-VertZ;                                    \
      NeiBX=Factor2*Xf-VertX; NeiBY=Factor2*Yb-VertY; NeiBZ=Factor2*Zf-VertZ;                                    \
      glNormal3f(VectMul(NeiAX, NeiAY, NeiAZ, NeiBX, NeiBY, NeiBZ));                                             \
      glVertex3f(VertX, VertY, VertZ);                                                                           \
    }                                                                                                            \
    glEnd();                                                                                                     \
  }                                                                                                              \
}

#define PENTAGON(Edge, Amp, Divisions, Z)                                                                        \
{                                                                                                                \
  int       Ri,Ti,Fi;                                                                                            \
  GLfloat   Xf,Yf,Xa,Yb,Xf2,Yf2;                                                                                 \
  GLfloat   x[6],y[6];                                                                                           \
  GLfloat   Factor,Factor1,Factor2;                                                                              \
  GLfloat   VertX,VertY,VertZ,NeiAX,NeiAY,NeiAZ,NeiBX,NeiBY,NeiBZ;                                               \
  GLfloat   Zf=(Edge)*(Z);                                                                                       \
  GLfloat   AmpVr2=(Amp)/sqr((Edge)*cossec36_2);                                                                 \
                                                                                                                 \
  for(Fi=0;Fi<6;Fi++) {                                                                                          \
    x[Fi]=-cos( Fi*2*Pi/5 + Pi/10 )/(Divisions)*cossec36_2*(Edge);                                                \
    y[Fi]=sin( Fi*2*Pi/5 + Pi/10 )/(Divisions)*cossec36_2*(Edge);                                                \
  }                                                                                                              \
                                                                                                                 \
  for (Ri=1; Ri<=(Divisions); Ri++) {                                                                            \
    for (Fi=0; Fi<5; Fi++) {                                                                                     \
      glBegin(GL_TRIANGLE_STRIP);                                                                                \
      for (Ti=0; Ti<Ri; Ti++) {                                                                                  \
        Xf=(float)(Ri-Ti)*x[Fi] + (float)Ti*x[Fi+1];                                                             \
        Yf=(float)(Ri-Ti)*y[Fi] + (float)Ti*y[Fi+1];                                                             \
        Xa=Xf+0.001; Yb=Yf+0.001;                                                                                \
	Factor=1-(((Xf2=sqr(Xf))+(Yf2=sqr(Yf)))*AmpVr2);                                                         \
	Factor1=1-((sqr(Xa)+Yf2)*AmpVr2);                                                                        \
	Factor2=1-((Xf2+sqr(Yb))*AmpVr2);                                                                        \
        VertX=Factor*Xf;        VertY=Factor*Yf;        VertZ=Factor*Zf;                                         \
        NeiAX=Factor1*Xa-VertX; NeiAY=Factor1*Yf-VertY; NeiAZ=Factor1*Zf-VertZ;                                  \
        NeiBX=Factor2*Xf-VertX; NeiBY=Factor2*Yb-VertY; NeiBZ=Factor2*Zf-VertZ;                                  \
        glNormal3f(VectMul(NeiAX, NeiAY, NeiAZ, NeiBX, NeiBY, NeiBZ));                                           \
	glVertex3f(VertX, VertY, VertZ);                                                                         \
                                                                                                                 \
        Xf=(float)(Ri-Ti-1)*x[Fi] + (float)Ti*x[Fi+1];                                                           \
        Yf=(float)(Ri-Ti-1)*y[Fi] + (float)Ti*y[Fi+1];                                                           \
        Xa=Xf+0.001; Yb=Yf+0.001;                                                                                \
	Factor=1-(((Xf2=sqr(Xf))+(Yf2=sqr(Yf)))*AmpVr2);                                                         \
	Factor1=1-((sqr(Xa)+Yf2)*AmpVr2);                                                                        \
	Factor2=1-((Xf2+sqr(Yb))*AmpVr2);                                                                        \
        VertX=Factor*Xf;        VertY=Factor*Yf;        VertZ=Factor*Zf;                                         \
        NeiAX=Factor1*Xa-VertX; NeiAY=Factor1*Yf-VertY; NeiAZ=Factor1*Zf-VertZ;                                  \
        NeiBX=Factor2*Xf-VertX; NeiBY=Factor2*Yb-VertY; NeiBZ=Factor2*Zf-VertZ;                                  \
        glNormal3f(VectMul(NeiAX, NeiAY, NeiAZ, NeiBX, NeiBY, NeiBZ));                                           \
	glVertex3f(VertX, VertY, VertZ);                                                                         \
                                                                                                                 \
      }                                                                                                          \
      Xf=(float)Ri*x[Fi+1];                                                                                      \
      Yf=(float)Ri*y[Fi+1];                                                                                      \
      Xa=Xf+0.001; Yb=Yf+0.001;                                                                                  \
      Factor=1-(((Xf2=sqr(Xf))+(Yf2=sqr(Yf)))*AmpVr2);                                                           \
      Factor1=1-((sqr(Xa)+Yf2)*AmpVr2);                                                                          \
      Factor2=1-((Xf2+sqr(Yb))*AmpVr2);                                                                          \
      VertX=Factor*Xf;        VertY=Factor*Yf;        VertZ=Factor*Zf;                                           \
      NeiAX=Factor1*Xa-VertX; NeiAY=Factor1*Yf-VertY; NeiAZ=Factor1*Zf-VertZ;                                    \
      NeiBX=Factor2*Xf-VertX; NeiBY=Factor2*Yb-VertY; NeiBZ=Factor2*Zf-VertZ;                                    \
      glNormal3f(VectMul(NeiAX, NeiAY, NeiAZ, NeiBX, NeiBY, NeiBZ));                                             \
      glVertex3f(VertX, VertY, VertZ);                                                                           \
      glEnd();                                                                                                   \
    }                                                                                                            \
  }                                                                                                              \
}

static void draw_tetra( void )
{
  GLuint list;

  list = glGenLists( 1 );
  glNewList( list, GL_COMPILE );
  TRIANGLE(2,seno,edgedivisions,0.5/SQRT6);
  glEndList();

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[0]);
  glCallList(list);
  glPushMatrix();
  glRotatef(180,0,0,1);
  glRotatef(-tetraangle,1,0,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[1]);
  glCallList(list);
  glPopMatrix();
  glPushMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-180+tetraangle,0.5,SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[2]);
  glCallList(list);
  glPopMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-180+tetraangle,0.5,-SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[3]);
  glCallList(list);

  glDeleteLists(list,1);
}

static void draw_cube( void )
{
  GLuint list;

  list = glGenLists( 1 );
  glNewList( list, GL_COMPILE );
  SQUARE(2, seno, edgedivisions, 0.5)
  glEndList();

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[0]);
  glCallList(list);
  glRotatef(cubeangle,1,0,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[1]);
  glCallList(list);
  glRotatef(cubeangle,1,0,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[2]);
  glCallList(list);
  glRotatef(cubeangle,1,0,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[3]);
  glCallList(list);
  glRotatef(cubeangle,0,1,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[4]);
  glCallList(list);
  glRotatef(2*cubeangle,0,1,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[5]);
  glCallList(list);

  glDeleteLists(list,1);
}

static void draw_octa( void )
{
  GLuint list;

  list = glGenLists( 1 );
  glNewList( list, GL_COMPILE );
  TRIANGLE(2,seno,edgedivisions,1/SQRT6);
  glEndList();

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[0]);
  glCallList(list);
  glPushMatrix();
  glRotatef(180,0,0,1);
  glRotatef(-180+octaangle,1,0,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[1]);
  glCallList(list);
  glPopMatrix();
  glPushMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-octaangle,0.5,SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[2]);
  glCallList(list);
  glPopMatrix();
  glPushMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-octaangle,0.5,-SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[3]);
  glCallList(list);
  glPopMatrix();
  glRotatef(180,1,0,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[4]);
  glCallList(list);
  glPushMatrix();
  glRotatef(180,0,0,1);
  glRotatef(-180+octaangle,1,0,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[5]);
  glCallList(list);
  glPopMatrix();
  glPushMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-octaangle,0.5,SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[6]);
  glCallList(list);
  glPopMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-octaangle,0.5,-SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[7]);
  glCallList(list);

  glDeleteLists(list,1);
}

static void draw_dodeca( void )
{
  GLuint list;

  #define TAU ((SQRT5+1)/2)

  list = glGenLists( 1 );
  glNewList( list, GL_COMPILE );
  PENTAGON(1,seno,edgedivisions,sqr(TAU) * sqrt((TAU+2)/5) / 2);
  glEndList();

  glPushMatrix();
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[0]);
  glCallList(list);
  glRotatef(180,0,0,1);
  glPushMatrix();
  glRotatef(-dodecaangle,1,0,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[1]);
  glCallList(list);
  glPopMatrix();
  glPushMatrix();
  glRotatef(-dodecaangle,cos72,sin72,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[2]);
  glCallList(list);
  glPopMatrix();
  glPushMatrix();
  glRotatef(-dodecaangle,cos72,-sin72,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[3]);
  glCallList(list);
  glPopMatrix();
  glPushMatrix();
  glRotatef(dodecaangle,cos36,-sin36,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[4]);
  glCallList(list);
  glPopMatrix();
  glRotatef(dodecaangle,cos36,sin36,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[5]);
  glCallList(list);
  glPopMatrix();
  glRotatef(180,1,0,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[6]);
  glCallList(list);
  glRotatef(180,0,0,1);
  glPushMatrix();
  glRotatef(-dodecaangle,1,0,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[7]);
  glCallList(list);
  glPopMatrix();
  glPushMatrix();
  glRotatef(-dodecaangle,cos72,sin72,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[8]);
  glCallList(list);
  glPopMatrix();
  glPushMatrix();
  glRotatef(-dodecaangle,cos72,-sin72,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[9]);
  glCallList(list);
  glPopMatrix();
  glPushMatrix();
  glRotatef(dodecaangle,cos36,-sin36,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[10]);
  glCallList(list);
  glPopMatrix();
  glRotatef(dodecaangle,cos36,sin36,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[11]);
  glCallList(list);

  glDeleteLists(list,1);
}

static void draw_ico( void )
{
  GLuint list;

  list = glGenLists( 1 );
  glNewList( list, GL_COMPILE );
  TRIANGLE(1.5,seno,edgedivisions,(3*SQRT3+SQRT15)/12);
  glEndList();

  glPushMatrix();

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[0]);
  glCallList(list);
  glPushMatrix();
  glRotatef(180,0,0,1);
  glRotatef(-icoangle,1,0,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[1]);
  glCallList(list);
  glPushMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-180+icoangle,0.5,SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[2]);
  glCallList(list);
  glPopMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-180+icoangle,0.5,-SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[3]);
  glCallList(list);
  glPopMatrix();
  glPushMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-180+icoangle,0.5,SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[4]);
  glCallList(list);
  glPushMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-180+icoangle,0.5,SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[5]);
  glCallList(list);
  glPopMatrix();
  glRotatef(180,0,0,1);
  glRotatef(-icoangle,1,0,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[6]);
  glCallList(list);
  glPopMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-180+icoangle,0.5,-SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[7]);
  glCallList(list);
  glPushMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-180+icoangle,0.5,-SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[8]);
  glCallList(list);
  glPopMatrix();
  glRotatef(180,0,0,1);
  glRotatef(-icoangle,1,0,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[9]);
  glCallList(list);
  glPopMatrix();
  glRotatef(180,1,0,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[10]);
  glCallList(list);
  glPushMatrix();
  glRotatef(180,0,0,1);
  glRotatef(-icoangle,1,0,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[11]);
  glCallList(list);
  glPushMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-180+icoangle,0.5,SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[12]);
  glCallList(list);
  glPopMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-180+icoangle,0.5,-SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[13]);
  glCallList(list);
  glPopMatrix();
  glPushMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-180+icoangle,0.5,SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[14]);
  glCallList(list);
  glPushMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-180+icoangle,0.5,SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[15]);
  glCallList(list);
  glPopMatrix();
  glRotatef(180,0,0,1);
  glRotatef(-icoangle,1,0,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[16]);
  glCallList(list);
  glPopMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-180+icoangle,0.5,-SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[17]);
  glCallList(list);
  glPushMatrix();
  glRotatef(180,0,1,0);
  glRotatef(-180+icoangle,0.5,-SQRT3/2,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[18]);
  glCallList(list);
  glPopMatrix();
  glRotatef(180,0,0,1);
  glRotatef(-icoangle,1,0,0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialColor[19]);
  glCallList(list);

  glDeleteLists(list,1);
}

static void draw ( void ) {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glPushMatrix();

    glTranslatef( 0.0, 0.0, -10.0 );
    glScalef( Scale*WindH/WindW, Scale, Scale );
    glTranslatef(2.5*WindW/WindH*sin(step*1.11),2.5*cos(step*1.25*1.11),0);
    glRotatef(step*100,1,0,0);
    glRotatef(step*95,0,1,0);
    glRotatef(step*90,0,0,1);

  seno=(sin(step)+1.0/3.0)*(4.0/5.0)*Magnitude;

  draw_object();

  glPopMatrix();

  glFlush();

  /* glutSwapBuffers(); */

  step+=0.05;
}

/*
static void idle_( void )
{
   glutPostRedisplay();
}
*/

static void reshape( int width, int height )
{
  glViewport(0, 0, WindW=(GLint)width, WindH=(GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 15.0 );
  glMatrixMode(GL_MODELVIEW);
}

/*
static void pinit(void);
*/

/*
static void key( unsigned char k, int x, int y )
{
  switch (k) {
    case '1': object=1; break;
    case '2': object=2; break;
    case '3': object=3; break;
    case '4': object=4; break;
    case '5': object=5; break;
    case ' ': mono^=1; break;
    case 13: smooth^=1; break;
    case 27:
       exit(0);
  }
  pinit();
}
*/

static void pinit(void)
{
  switch(object) {
    case 1:
      draw_object=draw_tetra;
      MaterialColor[0]=MaterialRed;
      MaterialColor[1]=MaterialGreen;
      MaterialColor[2]=MaterialBlue;
      MaterialColor[3]=MaterialWhite;
      edgedivisions=tetradivisions;
      Magnitude=2.5;
      break;
    case 2:
      draw_object=draw_cube;
      MaterialColor[0]=MaterialRed;
      MaterialColor[1]=MaterialGreen;
      MaterialColor[2]=MaterialCyan;
      MaterialColor[3]=MaterialMagenta;
      MaterialColor[4]=MaterialYellow;
      MaterialColor[5]=MaterialBlue;
      edgedivisions=cubedivisions;
      Magnitude=2.0;
      break;
    case 3:
      draw_object=draw_octa;
      MaterialColor[0]=MaterialRed;
      MaterialColor[1]=MaterialGreen;
      MaterialColor[2]=MaterialBlue;
      MaterialColor[3]=MaterialWhite;
      MaterialColor[4]=MaterialCyan;
      MaterialColor[5]=MaterialMagenta;
      MaterialColor[6]=MaterialGray;
      MaterialColor[7]=MaterialYellow;
      edgedivisions=octadivisions;
      Magnitude=2.5;
      break;
    case 4:
      draw_object=draw_dodeca;
      MaterialColor[ 0]=MaterialRed;
      MaterialColor[ 1]=MaterialGreen;
      MaterialColor[ 2]=MaterialCyan;
      MaterialColor[ 3]=MaterialBlue;
      MaterialColor[ 4]=MaterialMagenta;
      MaterialColor[ 5]=MaterialYellow;
      MaterialColor[ 6]=MaterialGreen;
      MaterialColor[ 7]=MaterialCyan;
      MaterialColor[ 8]=MaterialRed;
      MaterialColor[ 9]=MaterialMagenta;
      MaterialColor[10]=MaterialBlue;
      MaterialColor[11]=MaterialYellow;
      edgedivisions=dodecadivisions;
      Magnitude=2.0;
      break;
    case 5:
      draw_object=draw_ico;
      MaterialColor[ 0]=MaterialRed;
      MaterialColor[ 1]=MaterialGreen;
      MaterialColor[ 2]=MaterialBlue;
      MaterialColor[ 3]=MaterialCyan;
      MaterialColor[ 4]=MaterialYellow;
      MaterialColor[ 5]=MaterialMagenta;
      MaterialColor[ 6]=MaterialRed;
      MaterialColor[ 7]=MaterialGreen;
      MaterialColor[ 8]=MaterialBlue;
      MaterialColor[ 9]=MaterialWhite;
      MaterialColor[10]=MaterialCyan;
      MaterialColor[11]=MaterialYellow;
      MaterialColor[12]=MaterialMagenta;
      MaterialColor[13]=MaterialRed;
      MaterialColor[14]=MaterialGreen;
      MaterialColor[15]=MaterialBlue;
      MaterialColor[16]=MaterialCyan;
      MaterialColor[17]=MaterialYellow;
      MaterialColor[18]=MaterialMagenta;
      MaterialColor[19]=MaterialGray;
      edgedivisions=icodivisions;
      Magnitude=2.5;
      break;
  }
  if (mono) {
    int loop;
    for (loop=0; loop<20; loop++) MaterialColor[loop]=MaterialGray;
  }
  if (smooth) {
    glShadeModel( GL_SMOOTH );
  } else {
    glShadeModel( GL_FLAT );
  }

}

void clear_screen(void *buffer)
{
	int	i;
	GLuint *ptr4;

	ptr4 = (GLuint *) buffer;

	for (i=0;i < (WIDTH*HEIGHT*BPP/32); i++)
	{
		ptr4[i] = 0;
	}
}

void INIT(void)
{
   ROSMesaContext ctx;
   void *buffer;
   int temp[3];
   int	k;
   int	b;
   _kernel_swi_regs regs;
   int	window_block[23] = {0, 256, 256, 256+(WIDTH*UNITS), 256+(HEIGHT*UNITS), 0, 0, -1, 0x87040012, 0xff070207, 0x000c0103, 0, -HEIGHT*UNITS, WIDTH*UNITS, 0, 0x2700003d, 0x00002000, 1, 0, 000, 0, 0, 0};
	char		task_name[16] = "Morph 3D";
	unsigned int	task_handle;
   int	icon_block[9] = {0, 0, -HEIGHT*UNITS, WIDTH*UNITS, 0, 0x2102, 0, 0, 0};
   int	message[64];
   int	quit = 0;
   char	sprite_name[12] = "display";
   int	menu_block[70];
   FILE	*output;
   char	filename[256] = "ADFS::HardDisc4.$.Personal.Graphics.Mesa.Widget.Picture";

/*
	output = fopen("ADFS::HardDisc4.$.Personal.Graphics.Mesa.Widget.debugging", "w");
*/

	/* Write the window title */
	strcpy((char *)&window_block[19], "Morph 3D");

	strcpy((char *)menu_block, "Morph 3D");
	menu_block[3] = 0x00070207;
	menu_block[4] = (17+1)*16;
	menu_block[5] = 44;
	menu_block[6] = 0;

/*
	fprintf(output, "Menu header written.\n");
	fprintf(output, "Menu at %x\n", (unsigned int)&menu_block);
*/

	menu_block[7] = 0;
	menu_block[8] = -1;
	menu_block[9] = 0x07000101;
	menu_block[10] = (unsigned int)(&menu_block[37]);	/* text buffer */
	menu_block[11] = (unsigned int)(&menu_block[42]);	/* validation string */
	menu_block[12] = 20;		/* buffer length */
	strcpy((char *)(unsigned int)&(menu_block[37]), "Tetrahedron");
	menu_block[42] = 0;

/*
	fprintf(output, "Menu item 1 written.\n");
	fprintf(output, "Text at %x\n", (unsigned int)&menu_block[37]);
	fprintf(output, "Text is \"%s\"\n", (char *)menu_block[10]);
	fprintf(output, "\n");
*/

	menu_block[13] = 0;
	menu_block[14] = -1;
	menu_block[15] = 0x07000101;
	menu_block[16] = (unsigned int)(&menu_block[43]);	/* text buffer */
	menu_block[17] = (unsigned int)(&menu_block[48]);	/* validation string */
	menu_block[18] = 20;		/* buffer length */
	strcpy((char *)(unsigned int)&(menu_block[43]), "Hexahedron (Cube)");
	menu_block[48] = 0;

/*
	fprintf(output, "Menu item 2 written.\n");
	fprintf(output, "Text at %x\n", (unsigned int)&menu_block[43]);
	fprintf(output, "Text is \"%s\"\n", (char *)menu_block[16]);
	fprintf(output, "\n");
*/

	menu_block[19] = 0;
	menu_block[20] = -1;
	menu_block[21] = 0x07000101;
	menu_block[22] = (unsigned int)(&menu_block[49]);	/* text buffer */
	menu_block[23] = (unsigned int)(&menu_block[54]);	/* validation string */
	menu_block[24] = 20;		/* buffer length */
	strcpy((char *)(unsigned int)&(menu_block[49]), "Octahedron");
	menu_block[54] = 0;

/*
	fprintf(output, "Menu item 3 written.\n");
	fprintf(output, "Text at %x\n", (unsigned int)&menu_block[49]);
	fprintf(output, "Text is \"%s\"\n", (char *)menu_block[22]);
	fprintf(output, "\n");
*/

	menu_block[25] = 0;
	menu_block[26] = -1;
	menu_block[27] = 0x07000101;
	menu_block[28] = (unsigned int)(&menu_block[55]);	/* text buffer */
	menu_block[29] = (unsigned int)(&menu_block[60]);	/* validation string */
	menu_block[30] = 20;		/* buffer length */
	strcpy((char *)(unsigned int)&(menu_block[55]), "Dodecahedron");
	menu_block[60] = 0;

/*
	fprintf(output, "Menu item 4 written.\n");
	fprintf(output, "Text at %x\n", (unsigned int)&menu_block[55]);
	fprintf(output, "Text is \"%s\"\n", (char *)menu_block[28]);
	fprintf(output, "\n");
*/

	menu_block[31] = 128;
	menu_block[32] = -1;
	menu_block[33] = 0x07000101;
	menu_block[34] = (unsigned int)(&menu_block[61]);	/* text buffer */
	menu_block[35] = (unsigned int)(&menu_block[66]);	/* validation string */
	menu_block[36] = 20;		/* buffer length */
	strcpy((char *)(unsigned int)&(menu_block[61]), "Icosahedron");
	menu_block[66] = 0;

/*
	fprintf(output, "Menu item 5 written.\n");
	fprintf(output, "Text at %x\n", (unsigned int)&menu_block[61]);
	fprintf(output, "Text is \"%s\"\n", (char *)menu_block[34]);
	fprintf(output, "\n");
/*

/*
  printf("Morph 3D - Shows morphing platonic polyhedra\n");
  printf("Author: Marcelo Fernandes Vianna (vianna@cat.cbpf.br)\n\n");
  printf("  [1]    - Tetrahedron\n");
  printf("  [2]    - Hexahedron (Cube)\n");
  printf("  [3]    - Octahedron\n");
  printf("  [4]    - Dodecahedron\n");
  printf("  [5]    - Icosahedron\n");
  printf(" [ESC]   - Quit\n");

  fscanf(stdin, "%i", &object);
*/
   object = 1;
/*
  if ((object<1) || (object>5)) object=1;
/*

   /* Create a single buffered RGBA-mode context */
   ctx = ROSMesaCreateContext( ROSMESA_RGBA, 8, GL_FALSE );
   if (ctx==NULL)
   {
/*
	output = fopen("ADFS::HardDisc4.$.Personal.Graphics.Mesa.Widget.debugging", "w");
	fprintf(output, "Can't create context\n");
	fclose(output);
*/
   	exit(0);
   }

   /* Allocate the image buffer */
	buffer = malloc( (size_t) WIDTH * HEIGHT * 4 + HEADER );

   /* Initialise a sprite area */

	*(unsigned int *)buffer		= WIDTH * HEIGHT * 4 + HEADER;
	*(unsigned int *)(buffer + 8)	= 0;
	*(unsigned int *)(buffer + 8)	= 16;
	*(unsigned int *)(buffer + 8)	= 16;

	regs.r[0] = 256+9;
	regs.r[1] = (unsigned int)buffer;
   _kernel_swi(OS_SpriteOp, &regs, &regs);

	regs.r[0] = 256+15;
	regs.r[1] = (unsigned int)buffer;
	regs.r[2] = (unsigned int)&sprite_name;
	regs.r[3] = 0;
	regs.r[4] = WIDTH;
	regs.r[5] = HEIGHT;
	regs.r[6] = (6 << 27) | ((180/UNITS) << 14) | ((180/UNITS) << 1) | 1;
   _kernel_swi(OS_SpriteOp, &regs, &regs);

  clear_screen(buffer+HEADER);

  /* Bind the buffer to the context and make it current */
  if (ROSMesaMakeCurrent( ctx, buffer+HEADER, NULL, GL_UNSIGNED_BYTE, WIDTH, HEIGHT )==GL_FALSE)
  {
/*
	output = fopen("ADFS::HardDisc4.$.Personal.Graphics.Mesa.Widget.debugging", "w");
	fprintf(output, "Can't make current\n");
	fclose(output);
*/
	exit(0);
  }

/* Initialise task */
	regs.r[0] = (unsigned int)310;
	regs.r[1] = (unsigned int)0x4b534154;
	regs.r[2] = (unsigned int)&task_name;
	regs.r[3] = 0;
   _kernel_swi(Wimp_Initialise, &regs, &regs);
	task_handle = regs.r[1];

/* Create window */
	regs.r[1] = (unsigned int)(&window_block) + 4;
   _kernel_swi(Wimp_CreateWindow, &regs, &regs);
   	window_block[0] = regs.r[0];

/* Create icon */
        icon_block[0] = window_block[0];
        icon_block[6] = (unsigned int)(buffer) + 20;
	icon_block[7] = (unsigned int)buffer;
	icon_block[8] = 7;
        regs.r[0] = 0;
        regs.r[1] = (unsigned int)&icon_block;
   _kernel_swi(Wimp_CreateIcon, &regs, &regs);

/* Open window */
        regs.r[1] = (unsigned int)&window_block;
   _kernel_swi(Wimp_OpenWindow, &regs, &regs);


  glClearDepth(1.0);
  glClearColor( 0.0, 0.0, 0.0, 1.0 );
  glColor3f( 1.0, 1.0, 1.0 );

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glFlush();
  /* glutSwapBuffers(); */

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, position0);
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, position1);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);

  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, front_shininess);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, front_specular);

  glHint(GL_FOG_HINT, GL_FASTEST);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

/* Added by David Boddie (Wed 24th June 1998) */
  glDisable(GL_DITHER);

  pinit();

  reshape(WIDTH, HEIGHT);

  b = 0;
  k = 0;


do
{
	unsigned int	code;

	/* Poll */
		regs.r[0] = (unsigned int)0;
		regs.r[1] = (unsigned int)&message;
		regs.r[2] = 10;
	   _kernel_swi(Wimp_PollIdle, &regs, &regs);

	code = (unsigned int)regs.r[0];

/*
	fprintf(output, "code = %i\n", code);
*/

		switch (code)
		{
			case 0:		/* Null */
					draw();
					regs.r[0] = window_block[0];
					regs.r[1] = 0;
					regs.r[2] = -HEIGHT*UNITS;
					regs.r[3] = WIDTH*UNITS;
					regs.r[4] = 0;
					_kernel_swi(Wimp_ForceRedraw, &regs, &regs);
					break;
			case 2:		/* Open window */
					_kernel_swi(Wimp_OpenWindow, &regs, &regs);
					break;
                	case 3:		/* Close window */
                			_kernel_swi(Wimp_CloseWindow, &regs, &regs);
                			quit = 1;
                			break;
                	case 6:		/* Button */
                			if (message[2] == 2)
                			{
                				/* Open menu */

						/* Fill in tick for relevant entry */
						menu_block[1+(object*6)] = 1 | ((object==5)*128);

                                        	/* Get pointer position */
                                        	regs.r[1] = (unsigned int)&message;
						_kernel_swi(Wimp_GetPointerInfo, &regs, &regs);
                                        	regs.r[1] = (unsigned int)&menu_block;
                                        	regs.r[2] = message[0] - 64;
                                        	regs.r[3] = message[1];
                                        	_kernel_swi(Wimp_CreateMenu, &regs, &regs);
                                        }
                                        break;
			case 9:		/* Menu selection */

					/* Untick object entry */
					menu_block[1+(object*6)] = menu_block[1+(object*6)] ^ 1;

					/* Change object number */
					object = message[0] + 1;
					pinit();
                                       	/* Get pointer position */
                                       	regs.r[1] = (unsigned int)&message;
					_kernel_swi(Wimp_GetPointerInfo, &regs, &regs);
					if (message[2] == 1)
					{
						/* Fill in tick for relevant entry */
						menu_block[1+(object*6)] = menu_block[1+(object*6)] ^ 1;

                                        	regs.r[1] = (unsigned int)&menu_block;
                                        	regs.r[2] = message[0] - 64;
                                        	regs.r[3] = message[1];
                                        	_kernel_swi(Wimp_CreateMenu, &regs, &regs);
					}
					break;
                	case 17:
                	case 18:
                			if (message[4] == 0)
                			{
                				quit = 1;
                                        }
                                        break;
                        default:
                        break;
		}
} while (quit == 0);


/*
	fprintf(output, "After Poll\n");
*/


/* Close window */
        regs.r[1] = (unsigned int)&window_block;
   _kernel_swi(Wimp_CloseWindow, &regs, &regs);

/* Delete window */
        regs.r[1] = (unsigned int)&window_block;
   _kernel_swi(Wimp_DeleteWindow, &regs, &regs);

/* Closedown */
	regs.r[0] = task_handle;
	regs.r[1] = (unsigned int)0x4b534154;
   _kernel_swi(Wimp_CloseDown, &regs, &regs);

/* Save the image */
/*
   regs.r[0] = 10;
   regs.r[1] = (int) filename;
   regs.r[2] = 0xff9;
   regs.r[4] = (int) buffer + 4;
   regs.r[5] = (int) buffer + HEADER + (WIDTH * HEIGHT * 4);
   _kernel_swi(OS_File, &regs, &regs);
*/

   free(buffer);

   /* destroy the context */
   ROSMesaDestroyContext( ctx );

}

int main(int argc, char **argv)
{
  INIT();
  return(0);
}
