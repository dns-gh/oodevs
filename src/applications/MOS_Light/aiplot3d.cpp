/*
	Written 1998-1999 by Uwe Langenkamp

	This software may be used and distributed according to the terms
	of the GNU Public License, incorporated herein by reference.

	The author may be reached as ul@it-guru.de
*/

/* History:
 *          1999-10-28 Last Change
 *          1999-10-28 BG shading effect
 *          1998-06-05 .....
 *          1998-12-04 .....
 *
 */

#include "MOS_Light_Pch.h"

#include <qpainter.h>
#include <qbitmap.h> 
#include <qpntarry.h>
#include <qapp.h>
#include <qsocknot.h>
#include <qpopmenu.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qkeycode.h>
#include <qtimer.h>
#include <qbttngrp.h>
#include <qfiledlg.h>
#include <qstrlist.h>
#include <qmsgbox.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef SGI
#ifdef GLP
#include <glp.h>
#endif
#endif

#include <sys/types.h>
#include <time.h>

#include "aiplot3d.h"

struct my_color {
  double r;
  double g;
  double b;
};

struct my_color colfield[9]={
  0.5, 0.0, 1.0,			/* violet */
  0.0, 0.0, 1.0,			/* blue */
  0.0, 1.0, 1.0,			/* cyan */
  0.0, 1.0, 0.0,			/* green */
  1.0, 1.0, 0.0,			/* yellow */
  1.0, 0.5, 0.0,			/* orange */
  1.0, 0.0, 0.0,			/* red */
  1.0, 1.0, 1.0,			/* white */
  0.0, 0.0, 0.0,			/* black */
};

#define RED(val) (colfield[(int)(val*9)].r)
#define GREEN(val) (colfield[(int)(val*9)].g)
#define BLUE(val) (colfield[(int)(val*9)].b)

#ifdef SGI
#ifdef GLP
static GLPenum GLPerr;
#define GLPERR if((GLPerr = glpGetError()) != GLP_NONE) {fprintf(stderr,">>>>\tGLP error %d (%s) at line %d in %s\n",GLPerr,glpGetErrorMsg(GLPerr),__LINE__,__FILE__);}
#endif
#endif

#define AI_PLOT3D_VALCOLOR(val) glColor3f(RED(((val)-zmin)/(zmax-zmin)),GREEN(((val)-zmin)/(zmax-zmin)),BLUE(((val)-zmin)/(zmax-zmin)))


void nop(void)
{
}

//#define AI_PLOT3D_VALCOLOR(val) nop()

AIPlot3D::AIPlot3D(QWidget *parent,const char *name,int b,int d)
  : QGLWidget( parent, name )
{
  int i;

  bufsize=b,bufdepth=d;

  graphtype=AI_PLOT3D_LINES;
  recalc=FALSE;

  // Get inputs
  xtrace= new double[bufsize];
  memset(xtrace,0,bufsize*sizeof(double));

  ytrace= new double[bufdepth];
  memset(ytrace,0,bufdepth*sizeof(double));

  zmatrix = new double * [bufdepth];
  for (i=0;i<bufdepth;i++) {
    zmatrix[i] = new double [bufsize];
    memset(zmatrix[i],0,bufsize*sizeof(double));
  }

  nmatrix = new GLfloat * [bufdepth];
  for (i=0;i<bufdepth;i++) {
    nmatrix[i] = new GLfloat [bufsize*3];
  }

  actdepth=0;
  autoscale=TRUE;

  makeCurrent();
  grid = glGenLists( 1 );
  curves = glGenLists( 1 );
  squares = glGenLists( 1 );
  polygons = glGenLists( 1 );
  scales = glGenLists( 1 );

  // Standard Color Set, MMS Style
  setPlotColor(0,255,0);
  setBackgroundColor(0,0,128);
  setCommonColor(255,255,0);
  setGridColor(255,255,255);

  //glEnable( GL_CULL_FACE );

  //glShadeModel( GL_FLAT );
  //glDisable(GL_DITHER);
  //glPolygonOffset( 1.0, 0.5 );   
  
  percent=100.0;
  scale=1.0;zRot=0;yRot=0;xRot=0;

  x_axis_display_type=AXIS_NUMBER;
  y_axis_display_type=AXIS_NUMBER;
  z_axis_display_type=AXIS_NUMBER;

  zmin=-1.0,zmax=1.0;
  xmin=-1.0,xmax=1.0;
  ymin=-1.0,ymax=1.0;
}

AIPlot3D::~AIPlot3D()
{
  int i;

  for (i=0;i<bufdepth;i++) {
    delete zmatrix[i];
  }

  delete zmatrix;
  delete xtrace;
  delete ytrace;
}

/*!
  Paint the box. The actual openGL commands for drawing the box are
  performed here.
*/

void AIPlot3D::paintGL()
{

  if (!isVisible())
    return;

  if (autoscale) {
	 if (width()>height()) {
		 resize(height(),height());
	 } else if (width()<height()){
		 resize(width(),width());
	 }
  }
  
  if(recalc) {
    updateObjects();
    recalc=FALSE;
  }

  /* ######### new BG shading######### */
  glEnable(GL_SCISSOR_TEST);
  int i;
  for (i=0;i<height();i++) {
	glScissor(0,i,width(),1);
	glClearColor((double)0.25,
	       (double)0.25,
	       (double)1.0-(double)i/(2*height()), 
	       0.0 ); // Let OpenGL clear to bg
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  }
  /* ######### new BG shading######### */
  
  //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glLoadIdentity();
  glTranslatef( 0.0, 0.0, -10.0 );
  glScalef( scale, scale, scale );
  
  glRotatef( xRot, 1.0, 0.0, 0.0 ); 
  glRotatef( yRot, 0.0, 1.0, 0.0 ); 
  glRotatef( zRot, 0.0, 0.0, 1.0 );

  glPushMatrix();
  glScissor(0,0,width(),height());
  glDisable( GL_LIGHTING );

  glCallList(grid);
  glCallList( scales );

  switch(graphtype) {
  case AI_PLOT3D_COLQUAD:
  case AI_PLOT3D_FLATWALL:
    glEnable( GL_LIGHTING );
    glCallList( squares );
    glDisable( GL_LIGHTING );
    break;
  case AI_PLOT3D_HLINES:
  case AI_PLOT3D_HCLINES:
    glCallList( polygons );
  case AI_PLOT3D_LINES:
  case AI_PLOT3D_CLINES:
  default:
    glCallList( curves );
    break;
  }

  glPopMatrix();
  glFlush(); // for printing reasons ???
}


/*!
  Set up the OpenGL view port, matrix mode, etc.
*/

void AIPlot3D::resizeGL( int w, int h )
{
  GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 }; 

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glEnable( GL_LIGHT0 );
  glEnable( GL_COLOR_MATERIAL);
  glEnable( GL_DEPTH_TEST );

  glEnable( GL_NORMALIZE );

  //glViewport( 0, 0, (GLint)w*percent/100.0, (GLint)h*percent/100.0 );

  int l;

  if (width()<height())
    l=width();
  else
    l=height();

  glViewport( width()-(width()*percent/100.0)/2-width()/2, 
	      height()-(height()*percent/100.0)/2-height()/2, 
	      (GLint)l*percent/100.0, 
	      (GLint)l*percent/100.0);

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 15.0 );
  glMatrixMode( GL_MODELVIEW );
  glTranslatef(0.0, 2.0, 5.0);
}


/*!
  Generate an OpenGL display list for the object to be shown, i.e. the box
*/

GLuint AIPlot3D::makeGrid()
{	
    glNewList( grid, GL_COMPILE );
    glPushMatrix();

    //draw the grid
    glColor3f( (double)c_grid.red()/256,
	       (double)c_grid.green()/256,
	       (double)c_grid.blue()/256);
    
    // Draw 3D Grid
    int i;
    glBegin( GL_LINES );
    /* z=-1.0 */
    /* xlines */
    for (i=0;i<=10;i++) {
      glVertex3f(-1.0+i*2/10.0,-1.0,-1.0);   
      glVertex3f(-1.0+i*2/10.0,1.0,-1.0);
    }
    /* ylines */
    for (i=0;i<=10;i++) {
      glVertex3f(-1.0,-1.0+i*2/10.0,-1.0);   
      glVertex3f(1.0,-1.0+i*2/10.0,-1.0);
    }
    glEnd();

    glBegin( GL_LINES );
    /* x=-1.0 */
    /* xlines */
    for (i=0;i<=10;i++) {
      glVertex3f(-1.0,-1.0+i*2/10.0,-1.0);   
      glVertex3f(-1.0,-1.0+i*2/10.0,1.0);
    }
    /* ylines */
    for (i=0;i<=10;i++) {
      glVertex3f(-1.0,-1.0,-1.0+i*2/10.0);   
      glVertex3f(-1.0,1.0,-1.0+i*2/10.0);
    }
    glEnd();

    glBegin( GL_LINES );
    /* y=-1.0 */
    /* xlines */
    for (i=0;i<=10;i++) {
      glVertex3f(-1.0+i*2/10.0,-1.0,-1.0);   
      glVertex3f(-1.0+i*2/10.0,-1.0,1.0);
    }
    /* ylines */
    for (i=0;i<=10;i++) {
      glVertex3f(-1.0,-1.0,-1.0+i*2/10.0);   
      glVertex3f(1.0,-1.0,-1.0+i*2/10.0);
    }
    glEnd();

    /* Draw sample data */

    glPopMatrix();
    glEndList();

    return 0;
}


// Create the curves from data
GLuint AIPlot3D::makeCurves()
{	
    int x,y;

    if (!isVisible())
      return 0;

    glNewList( curves, GL_COMPILE );
    glPushMatrix();

    for (y=0;y<actdepth;y++) {
      if (ytrace[y]>=ymin && ytrace[y]<=ymax) {
	glBegin( GL_LINE_STRIP );
	for (x=0;x<bufsize;x++) {
	  if (xtrace[x]>=xmin && xtrace[x]<=xmax) {
	    if (graphtype==AI_PLOT3D_HCLINES ||
		graphtype==AI_PLOT3D_CLINES ){
	      AI_PLOT3D_VALCOLOR(zmatrix[y][x]);
	    } else {
	      // Set the Plot Color
	      glColor3f(
			c_plot.red()/256.0,
			c_plot.green()/256.0,
			c_plot.blue()/256.0);

	    }
	    
	    glVertex3f(
		       2.0*(xtrace[x]-xmin)/(xmax-xmin)-1.0,
		       2.0*(zmatrix[y][x]-zmin)/(zmax-zmin)-1.0,
		       2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0
		       );
	  }
	}
	glEnd();     
      }
    }

    glPopMatrix();
    glEndList();
    
    return 0;
}

void
normalizedcross(GLfloat* u, GLfloat* v, GLfloat* n)
{
  GLfloat l;

  /* compute the cross product (u x v for right-handed [ccw]) */
  n[0] = u[1] * v[2] - u[2] * v[1];
  n[1] = u[2] * v[0] - u[0] * v[2];
  n[2] = u[0] * v[1] - u[1] * v[0];

  /* normalize */
  l = (GLfloat)sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
  n[0] /= l;
  n[1] /= l;
  n[2] /= l;
}

GLuint AIPlot3D::makeSquares()
{	
    int x,y;
    GLfloat u[3],v[3],*n;

    glNewList( squares, GL_COMPILE );
    glPushMatrix();

#define NORMALS
#ifdef NORMALS
    // Normal calculation, ### WARNING ### Calculation is reversed, 
    // cause the surface's front shows towards the grid -> -1.0 line

    for (y=0;y<actdepth-1;y++) {
      for (x=0;x<bufsize-1;x++) {
	/* get two vectors to cross */
	u[0] = 
	  (2.0*(xtrace[x]-xmin)/(xmax-xmin)-1.0)
	  -(2.0*(xtrace[x+1]-xmin)/(xmax-xmin)-1.0);
	u[1] = 
	  (2.0*(zmatrix[y][x]-zmin)/(zmax-zmin)-1.0)
	  -(2.0*(zmatrix[y][x+1]-zmin)/(zmax-zmin)-1.0);
	u[2] = 0; /*
		    (2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0)
		    -(2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0); */
	
	v[0] = 0; /*
		    (2.0*(xtrace[x+1]-xmin)/(xmax-xmin)-1.0)
		    -(2.0*(xtrace[x+1]-xmin)/(xmax-xmin)-1.0); */
	v[1] = 
	  (2.0*(zmatrix[y+1][x+1]-zmin)/(zmax-zmin)-1.0)
	  -(2.0*(zmatrix[y][x+1]-zmin)/(zmax-zmin)-1.0);
	v[2] = 
	  (2.0*(ytrace[y+1]-ymin)/(ymax-ymin)-1.0)
	  -(2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0);
	

	n=(GLfloat *)&nmatrix[y][x*3];

	/* get the normalized cross product */
	normalizedcross(u, v, n);
      }
    }			
#endif //NORMALS


    // Plot normals
    //#define SHOW_NORMAL 
#ifdef SHOW_NORMAL // Only for debugging
    for (y=0;y<actdepth-1;y++) {
      if (ytrace[y]>=ymin && ytrace[y+1]<=ymax) {
	for (x=0;x<bufsize-1;x++) {
	  if (xtrace[x]>=xmin && xtrace[x+1]<=xmax) {
	    glBegin( GL_LINE_STRIP );	
	    glColor3f(1.0,1.0,1.0);
	    n=(GLfloat *)&nmatrix[y][x*3];
	    glVertex3f(	
		       2.0*(xtrace[x]-xmin)/(xmax-xmin)-1.0,
		       2.0*(zmatrix[y][x]-zmin)/(zmax-zmin)-1.0,
		       2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0
		       );
	    glVertex3f(	
		       2.0*(xtrace[x]-xmin)/(xmax-xmin)-1.0+n[0]/5,
		       2.0*(zmatrix[y][x]-zmin)/(zmax-zmin)-1.0+n[1]/5,
		       2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0+n[2]/5
		       );
	    glEnd();
	  }
	}
      }
    }
#endif //SHOW_NORMAL, Only for debugging

    for (y=0;y<actdepth-1;y++) {
      if (ytrace[y]>=ymin && ytrace[y+1]<=ymax) {

	// ### WARNING ### Front and Back are changed, 
	// cause the surface's front shows towards the grid -> -1.0 line
	//glPolygonMode(GL_FRONT, GL_LINE);

	glBegin( GL_QUADS );
	for (x=0;x<bufsize-1;x++) {
	  if (xtrace[x]>=xmin && xtrace[x+1]<=xmax) {
	    if (graphtype==AI_PLOT3D_FLATWALL) {
	      // X
	      AI_PLOT3D_VALCOLOR(zmatrix[y][x]);
	      glNormal3f(1,0,0);
	      glVertex3f(
			 -1.0,
			 2.0*(zmatrix[y][x]-zmin)/(zmax-zmin)-1.0,
			 2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0
			 );
	      AI_PLOT3D_VALCOLOR(zmatrix[y][x+1]);
	      glNormal3f(1,0,0);
	      glVertex3f(
		     -1.0,
		     2.0*(zmatrix[y][x+1]-zmin)/(zmax-zmin)-1.0,
		     2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0
		     );
	      AI_PLOT3D_VALCOLOR(zmatrix[y+1][x+1]);
	      glNormal3f(1,0,0);
	      glVertex3f(
			 -1.0,
			 2.0*(zmatrix[y+1][x+1]-zmin)/(zmax-zmin)-1.0,
			 2.0*(ytrace[y+1]-ymin)/(ymax-ymin)-1.0
			 );
	      AI_PLOT3D_VALCOLOR(zmatrix[y+1][x]);
	      glNormal3f(1,0,0);
	      glVertex3f(
			 -1.0,
			 2.0*(zmatrix[y+1][x]-zmin)/(zmax-zmin)-1.0,
			 2.0*(ytrace[y+1]-ymin)/(ymax-ymin)-1.0
			 );
	      //Y
	      AI_PLOT3D_VALCOLOR(zmatrix[y][x]);
	      glNormal3f(0,1,0);
	      glVertex3f(
			 2.0*(xtrace[x]-xmin)/(xmax-xmin)-1.0,
			 2.0*(zmatrix[y][x]-zmin)/(zmax-zmin)-1.0,
			 -1.0
			 );
	      AI_PLOT3D_VALCOLOR(zmatrix[y][x+1]);
	      glNormal3f(0,1,0);
	      glVertex3f(
			 2.0*(xtrace[x+1]-xmin)/(xmax-xmin)-1.0,
			 2.0*(zmatrix[y][x+1]-zmin)/(zmax-zmin)-1.0,
			 -1.0
			 );
	      AI_PLOT3D_VALCOLOR(zmatrix[y+1][x+1]);
	      glNormal3f(0,1,0);
	      glVertex3f(
			 2.0*(xtrace[x+1]-xmin)/(xmax-xmin)-1.0,
			 2.0*(zmatrix[y+1][x+1]-zmin)/(zmax-zmin)-1.0,
			 -1.0
			 );
	      AI_PLOT3D_VALCOLOR(zmatrix[y+1][x]);
	      glNormal3f(0,1,0);
	      glVertex3f(
			 2.0*(xtrace[x]-xmin)/(xmax-xmin)-1.0,
			 2.0*(zmatrix[y+1][x]-zmin)/(zmax-zmin)-1.0,
			 -1.0
			 );
	      //Z
	      AI_PLOT3D_VALCOLOR(zmatrix[y][x]);
	      glNormal3f(0,0,1);
	      glVertex3f(
			 2.0*(xtrace[x]-xmin)/(xmax-xmin)-1.0,
			 -1.0,
			 2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0
			 );
	      AI_PLOT3D_VALCOLOR(zmatrix[y][x+1]);
	      glNormal3f(0,0,1);
	      glVertex3f(
			 2.0*(xtrace[x+1]-xmin)/(xmax-xmin)-1.0,
			 -1.0,
			 2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0
			 );
	      AI_PLOT3D_VALCOLOR(zmatrix[y+1][x+1]);
	      glNormal3f(0,0,1);
	      glVertex3f(
			 2.0*(xtrace[x+1]-xmin)/(xmax-xmin)-1.0,
			 -1.0,
			 2.0*(ytrace[y+1]-ymin)/(ymax-ymin)-1.0
			 );
	      AI_PLOT3D_VALCOLOR(zmatrix[y+1][x]);
	      glNormal3f(0,0,1);
	      glVertex3f(
			 2.0*(xtrace[x]-xmin)/(xmax-xmin)-1.0,
			 -1.0,
			 2.0*(ytrace[y+1]-ymin)/(ymax-ymin)-1.0
			 );
	    } else {
			int i;
			double step=(zmax-zmin)/20.0;
			int count=abs((zmatrix[y][x]-zmatrix[y][x+1])/step);
			int count2=abs((zmatrix[y+1][x]-zmatrix[y+1][x+1])/step);
			if (count2>count)
				count=count2;
			/*
			  if (count>15)
			  count=20;
			  else if (count>10)
			  count=15;
			  else if (count>5)
			  count=10;
			  else if (count>0)
			  count=5;*/			

			double xstep=(xtrace[x+1]-xtrace[x])/count;
			double step1=(zmatrix[y][x+1]-zmatrix[y][x])/count;
			double step2=(zmatrix[y+1][x+1]-zmatrix[y+1][x])/count;
			count=0;
			if (count) {
				for (i=0;i<count;i++) {
#ifdef NORMALS
				  n=(GLfloat *)&nmatrix[y][x*3];
				  glNormal3f(n[0],n[1],n[2]);
#endif			
				  AI_PLOT3D_VALCOLOR(zmatrix[y][x]+i*step1);
				  glVertex3f(
					     2.0*(xtrace[x]+i*xstep-xmin)/(xmax-xmin)-1.0,
					     2.0*(zmatrix[y][x]+i*step1-zmin)/(zmax-zmin)-1.0,
					     2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0
					     );
#ifdef NORMALS
				  n=(GLfloat *)&nmatrix[y][(x+1)*3];
				  glNormal3f(n[0],n[1],n[2]);
#endif			
				  AI_PLOT3D_VALCOLOR(zmatrix[y][x]+(i+1)*step1);
				  glVertex3f(
					     2.0*(xtrace[x]+(i+1)*xstep-xmin)/(xmax-xmin)-1.0,
					     2.0*(zmatrix[y][x]+(i+1)*step1-zmin)/(zmax-zmin)-1.0,
					     2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0
					     );
#ifdef NORMALS
				  n=(GLfloat *)&nmatrix[y+1][(x+1)*3];
				  glNormal3f(n[0],n[1],n[2]);
#endif			
				  AI_PLOT3D_VALCOLOR(zmatrix[y+1][x]+(i+1)*step2);
				  glVertex3f(
					     2.0*(xtrace[x]+(i+1)*xstep-xmin)/(xmax-xmin)-1.0,
					     2.0*(zmatrix[y+1][x]+(i+1)*step2-zmin)/(zmax-zmin)-1.0,
					     2.0*(ytrace[y+1]-ymin)/(ymax-ymin)-1.0
					     );

#ifdef NORMALS
				  n=(GLfloat *)&nmatrix[y+1][x*3];
				  glNormal3f(n[0],n[1],n[2]);
#endif			
				  AI_PLOT3D_VALCOLOR(zmatrix[y+1][x]+i*step2);
				  glVertex3f(
					     2.0*(xtrace[x]+i*xstep-xmin)/(xmax-xmin)-1.0,
					     2.0*(zmatrix[y+1][x]+i*step2-zmin)/(zmax-zmin)-1.0,
					     2.0*(ytrace[y+1]-ymin)/(ymax-ymin)-1.0
					     );
				}
			} else {
#ifdef NORMALS
			  n=(GLfloat *)&nmatrix[y][x*3];
			  glNormal3f(n[0],n[1],n[2]);
#endif			
				AI_PLOT3D_VALCOLOR(zmatrix[y][x]);
				glVertex3f(
					2.0*(xtrace[x]-xmin)/(xmax-xmin)-1.0,
					2.0*(zmatrix[y][x]-zmin)/(zmax-zmin)-1.0,
					2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0
				);
#ifdef NORMALS
			  n=(GLfloat *)&nmatrix[y][(x+1)*3];
			  glNormal3f(n[0],n[1],n[2]);
#endif			
				AI_PLOT3D_VALCOLOR(zmatrix[y][x+1]);
				glVertex3f(
					2.0*(xtrace[x+1]-xmin)/(xmax-xmin)-1.0,
					2.0*(zmatrix[y][x+1]-zmin)/(zmax-zmin)-1.0,
					2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0
				);
#ifdef NORMALS
			  n=(GLfloat *)&nmatrix[y+1][(x+1)*3];
			  glNormal3f(n[0],n[1],n[2]);
#endif			
				AI_PLOT3D_VALCOLOR(zmatrix[y+1][x+1]);
				glVertex3f(
					2.0*(xtrace[x+1]-xmin)/(xmax-xmin)-1.0,
					2.0*(zmatrix[y+1][x+1]-zmin)/(zmax-zmin)-1.0,
					2.0*(ytrace[y+1]-ymin)/(ymax-ymin)-1.0
				);
#ifdef NORMALS
			  n=(GLfloat *)&nmatrix[y+1][x*3];
			  glNormal3f(n[0],n[1],n[2]);
#endif			
				AI_PLOT3D_VALCOLOR(zmatrix[y+1][x]);
				glVertex3f(
					2.0*(xtrace[x]-xmin)/(xmax-xmin)-1.0,
					2.0*(zmatrix[y+1][x]-zmin)/(zmax-zmin)-1.0,
					2.0*(ytrace[y+1]-ymin)/(ymax-ymin)-1.0
				);
			}
	    }
	  }
	}
	glEnd();     
      }
    }

    glPopMatrix();
    glEndList();
    
    return 0;
}

GLuint AIPlot3D::makeScales()
{
  QString str="";
  int  len;
  int  i,j;
  double sfact=1.2;

  void *glut_font = GLUT_STROKE_ROMAN;

  glNewList( scales, GL_COMPILE );
  glPushMatrix();

  glColor3f( (double)c_grid.red()/256,
	     (double)c_grid.green()/256,
	     (double)c_grid.blue()/256);

  /* X-Axis -y,-z */
  glPushMatrix();
  glScalef( 0.001/sfact, 0.001/sfact, 0.001/sfact );
  glTranslatef(-1000.0*sfact, -1000.0*sfact, 1400.0*sfact /*1250*/);
  //glTranslatef(200.0, 0, 0);
  glTranslatef(50.0*sfact, 0, 0);
  for (j=0;j<=10;j++) {
    if ((x_axis_display_type & AXIS_NUMBER)==AXIS_NUMBER) {
      str.sprintf("%.2f",(double)(xmin+(xmax-xmin)*(double)j/(double)10.0));
    } else if ((x_axis_display_type & AXIS_DATE)==AXIS_DATE) {
      if ((x_axis_display_type & AXIS_UK)==AXIS_UK) {
      } else {
	time_t tt=(time_t)(double)(xmin+(xmax-xmin)*(double)j/(double)10.0);
	struct tm *ts = localtime(&tt);
	str.sprintf("%02d:%02d:%02d",
		ts->tm_hour,ts->tm_min,ts->tm_sec);
      }
    }

    len = (int) str.length();
    glPushMatrix();
    glRotatef( -90.0, 1.0, 0.0, 0.0 ); //rx
    glRotatef( 90.0, 0.0, 0.0, 1.0);   //ry
    for (i = 0; i < len; i++) {
      glutStrokeCharacter(glut_font, str.latin1()[i]);
    }
    glPopMatrix();
    glTranslatef(200.0*sfact, 0, 0);
  }
  glPopMatrix();

  /* X-Axis +y,+z */
  glPushMatrix();
  glScalef( 0.001/sfact, 0.001/sfact, 0.001/sfact );
  glTranslatef(-1000.0*sfact, 1050.0*sfact, -1000.0*sfact);
  //glTranslatef(200.0, 0, 0);
  glTranslatef(50.0*sfact, 0, 0);
  for (j=0;j<=10;j++) {
    if ((x_axis_display_type & AXIS_NUMBER)==AXIS_NUMBER) {
      str.sprintf("%.2f",(double)(xmin+(xmax-xmin)*(double)j/(double)10.0));
    } else if ((x_axis_display_type & AXIS_DATE)==AXIS_DATE) {
      if ((x_axis_display_type & AXIS_UK)==AXIS_UK) {
      } else {
	time_t tt=(time_t)(double)(xmin+(xmax-xmin)*(double)j/(double)10.0);
	struct tm *ts = localtime(&tt);
	str.sprintf("%02d:%02d:%02d",
		ts->tm_hour,ts->tm_min,ts->tm_sec);
      }
    }

    len = (int) str.length();
    glPushMatrix();
    glRotatef( 90.0, 0.0, 0.0, 1.0 ); //ry
    for (i = 0; i < len; i++) {
      glutStrokeCharacter(glut_font, str.latin1()[i]);
    }
    glPopMatrix();
    glTranslatef(200.0*sfact, 0, 0);
  }
  glPopMatrix();

  /* Y-Axis +x,-z */
  glPushMatrix();
  glScalef( 0.001/sfact, 0.001/sfact, 0.001/sfact );
  glTranslatef(1050.0*sfact, -1000.0*sfact, -1000.0*sfact);
  //glTranslatef(0.0, 0.0, 200.0);
  glTranslatef(0.0, 0.0, 50.0*sfact);
  for (j=0;j<=10;j++) {
    if ((y_axis_display_type & AXIS_NUMBER)==AXIS_NUMBER) {
      str.sprintf("%.2f",(double)(ymin+(ymax-ymin)*(double)j/(double)10.0));
    } else if ((y_axis_display_type & AXIS_DATE)==AXIS_DATE) {
      if ((y_axis_display_type & AXIS_UK)==AXIS_UK) {
      } else {
	time_t tt=(time_t)(double)(ymin+(ymax-ymin)*(double)j/(double)10.0);
	struct tm *ts = localtime(&tt);
	str.sprintf("%02d:%02d:%02d",
		ts->tm_hour,ts->tm_min,ts->tm_sec);
      }
    }

    len = (int) str.length();
    glPushMatrix();
    glRotatef( -90.0, 1.0, 0.0, 0.0 ); //rx
    for (i = 0; i < len; i++) {
      glutStrokeCharacter(glut_font, str.latin1()[i]);
    }
    glPopMatrix();
    glTranslatef(0.0, 0.0, 200.0*sfact);
  }
  glPopMatrix();

  /* Y-Axis -x,+z */
  glPushMatrix();
  glScalef( 0.001/sfact, 0.001/sfact, 0.001/sfact );
  glTranslatef(-1000.0*sfact, 1050.0*sfact, -1000.0*sfact);
  glTranslatef(0.0, 0.0, 50.0*sfact);
  for (j=0;j<=10;j++) {

    if ((y_axis_display_type & AXIS_NUMBER)==AXIS_NUMBER) {
      str.sprintf("%.2f",(double)(ymin+(ymax-ymin)*(double)j/(double)10.0));
    } else if ((y_axis_display_type & AXIS_DATE)==AXIS_DATE) {
      if ((y_axis_display_type & AXIS_UK)==AXIS_UK) {
      } else {
	time_t tt=(time_t)(double)(ymin+(ymax-ymin)*(double)j/(double)10.0);
	struct tm *ts = localtime(&tt);
	str.sprintf("%02d:%02d:%02d",
		ts->tm_hour,ts->tm_min,ts->tm_sec);
      }
    }

    len = (int) str.length();
    glPushMatrix();
    glRotatef( 90.0, 1.0, 0.0, 0.0 ); //rx
    glRotatef( 90.0, 0.0, 1.0, 0.0);   //rz
    for (i = 0; i < len; i++) {
      glutStrokeCharacter(glut_font, str.latin1()[i]);
    }
    glPopMatrix();
    glTranslatef(0.0, 0.0, 200.0*sfact);
  }
  glPopMatrix();

  /* Z-Axis +x,-y */
  glPushMatrix();
  glScalef( 0.001/sfact, 0.001/sfact, 0.001/sfact );
  glTranslatef(1050.0*sfact, -1000.0*sfact, -1000.0*sfact);
  glTranslatef(0.0, 50.0*sfact, 0.0);
  for (j=0;j<=10;j++) {
    if ((z_axis_display_type & AXIS_NUMBER)==AXIS_NUMBER) {
      str.sprintf("%.2f",(double)(zmin+(zmax-zmin)*(double)j/(double)10.0));
    } else if ((z_axis_display_type & AXIS_DATE)==AXIS_DATE) {
      if ((z_axis_display_type & AXIS_UK)==AXIS_UK) {
      } else {
	time_t tt=(time_t)(double)(zmin+(zmax-zmin)*(double)j/(double)10.0);
	struct tm *ts = localtime(&tt);
	str.sprintf("%02d:%02d:%02d",
		ts->tm_hour,ts->tm_min,ts->tm_sec);
      }
    }

    // DISABLED and replaced 1999/05/05
    // sprintf(str,"%.2f",(double)(zmin+(zmax-zmin)*(double)j/(double)10.0));

    len = (int) str.length();
    glPushMatrix();
    for (i = 0; i < len; i++) {
      glutStrokeCharacter(glut_font, str.latin1()[i]);
    }
    glPopMatrix();
    glTranslatef(0.0, 200.0*sfact, 0.0);
  }
  glPopMatrix();

  /* Z-Axis -x,+y */
  glPushMatrix();
  glScalef( 0.001/sfact, 0.001/sfact, 0.001/sfact );
  glTranslatef(-1000.0*sfact, -1000.0*sfact, +1250.0*sfact);
  glTranslatef(0.0, 50.0*sfact, 0.0);
  for (j=0;j<=10;j++) {
    if ((z_axis_display_type & AXIS_NUMBER)==AXIS_NUMBER) {
      str.sprintf("%.2f",(double)(zmin+(zmax-zmin)*(double)j/(double)10.0));
    } else if ((z_axis_display_type & AXIS_DATE)==AXIS_DATE) {
      if ((z_axis_display_type & AXIS_UK)==AXIS_UK) {
      } else {
	time_t tt=(time_t)(double)(zmin+(zmax-zmin)*(double)j/(double)10.0);
	struct tm *ts = localtime(&tt);
	str.sprintf("%02d:%02d:%02d",
		ts->tm_hour,ts->tm_min,ts->tm_sec);
      }
    }

    len = (int) str.length();
    glPushMatrix();
    glRotatef( 90.0, 0.0, 1.0, 0.0);   //rz
    for (i = 0; i < len; i++) {
      glutStrokeCharacter(glut_font, str.latin1()[i]);
    }
    glPopMatrix();
    glTranslatef(0.0, 200.0*sfact, 0.0);
  }
  glPopMatrix();

  glPopMatrix();
  glEndList();
  return 0;
}

GLuint AIPlot3D::makePolygons()
{	
    int x,y;

    glNewList( polygons, GL_COMPILE );
    glPushMatrix();

    for (y=0;y<actdepth;y++) {
      if (ytrace[y]>=ymin && ytrace[y]<=ymax) {
	glColor3f( 0.0,0.0,0.0);
	glBegin( GL_QUADS );
	for (x=0;x<bufsize-1;x++) {
	  if (xtrace[x]>=xmin && xtrace[x]<=xmax) {
	    glVertex3f(
		       2.0*(xtrace[x]-xmin)/(xmax-xmin)-1.0,
		       2.0*(zmatrix[y][x]-zmin)/(zmax-zmin)-1.0,
		       2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0
		       );
	    glVertex3f(
		       2.0*(xtrace[x+1]-xmin)/(xmax-xmin)-1.0,
		       2.0*(zmatrix[y][x+1]-zmin)/(zmax-zmin)-1.0,
		       2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0
		       );
	    glVertex3f(
		       2.0*(xtrace[x+1]-xmin)/(xmax-xmin)-1.0,
		       -1.0,
		       2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0
		       );
	    glVertex3f(
		       2.0*(xtrace[x]-xmin)/(xmax-xmin)-1.0,
		       -1.0,
		       2.0*(ytrace[y]-ymin)/(ymax-ymin)-1.0
		       );	   
	  }
	}
	glEnd();      
      }
    }

    glPopMatrix();
    glEndList();
    return 0;
}


void AIPlot3D::setZoom( double p )
{
    percent=(GLfloat)p;

    glViewport( width()-(width()*percent/100.0)/2-width()/2, 
		height()-(height()*percent/100.0)/2-height()/2, 
		(GLint)width()*percent/100.0, 
		(GLint)height()*percent/100.0);

    //resize(width(),height());
    repaint(FALSE);
}


/*!
  Set the rotation angle of the object to \e degrees around the X axis.
*/


void AIPlot3D::setXRotation( double degrees )
{
    xRot = (GLfloat)((int)(degrees) % 360);
    repaint(FALSE);
}


/*!
  Set the rotation angle of the object to \e degrees around the Y axis.
*/

void AIPlot3D::setYRotation( double degrees )
{
    yRot = (GLfloat)((int)(degrees) % 360);
    repaint(FALSE);
}


/*!
  Set the rotation angle of the object to \e degrees around the Z axis.
*/

void AIPlot3D::setZRotation( double degrees )
{
    zRot = (GLfloat)((int)(degrees) % 360);
    repaint(FALSE);
}


void AIPlot3D::setXRange(double xmn,double xmx)
{
  xmin=xmn;
  xmax=xmx;
  recalc=TRUE;
  //repaint(FALSE);
}

void AIPlot3D::setYRange(double ymn,double ymx)
{
  ymin=ymn;
  ymax=ymx;
  recalc=TRUE;
  //repaint(FALSE);
}

void AIPlot3D::setZRange(double zmn,double zmx)
{
  zmin=zmn;
  zmax=zmx;
  recalc=TRUE;
  //repaint(FALSE);
}

void   AIPlot3D::addZTrace(double y,double *p)
{
  int i;

  if (actdepth<bufdepth) {
    actdepth++;
  } else {
    for (i=0;i<bufdepth-1;i++) {
      memcpy(zmatrix[i],zmatrix[i+1],bufsize*sizeof(double));
    }
    memmove(ytrace,ytrace+1,(bufdepth-1)*sizeof(double));
  }

  ytrace[actdepth-1]=y;
  memcpy(zmatrix[actdepth-1],(char*)p,bufsize*sizeof(double));

  recalc=TRUE;
  //repaint(FALSE);
}

void   AIPlot3D::setXTrace(double *x)
{
  if (actdepth<bufdepth) {
    memcpy(xtrace,x,bufsize*sizeof(double));
  }
}

void AIPlot3D::setGraphType(int type)
{
  graphtype=type;
  recalc=TRUE;
  repaint(FALSE);
}

void AIPlot3D::updateObjects()
{
  switch(graphtype) {
  case AI_PLOT3D_COLQUAD:
  case AI_PLOT3D_FLATWALL:
    makeSquares();		
    break;
  case AI_PLOT3D_HLINES:
  case AI_PLOT3D_HCLINES:
    //glEnable(GL_POLYGON_OFFSET_FILL);
    makePolygons();	
    makeCurves();	
    //glDisable(GL_POLYGON_OFFSET_FILL);  
    break;
  case AI_PLOT3D_LINES:
  case AI_PLOT3D_CLINES:
  default:
    makeCurves();		
    break;
  }

  makeGrid();
  makeScales();
}


void AIPlot3D::setPlotColor(QColor c)
{
  setPlotColor(c.red(), c.green(), c.blue());
}

void AIPlot3D::setBackgroundColor(QColor c)
{
  setBackgroundColor(c.red(), c.green(), c.blue());
}

void AIPlot3D::setGridColor(QColor c)
{
  setGridColor(c.red(), c.green(), c.blue());
}

void AIPlot3D::setCommonColor(QColor c)
{
  setCommonColor(c.red(), c.green(), c.blue());
}

void AIPlot3D::setPlotColor(int r,int g,int b)
{
  c_plot.setRgb(r,g,b);
}

void AIPlot3D::setBackgroundColor(int r, int g, int b)
{
  c_bg.setRgb(r,g,b);

  glClearColor((double)c_bg.red()/256,
	       (double)c_bg.green()/256,
	       (double)c_bg.blue()/256, 
	       0.0 ); // Let OpenGL clear to bg

}

QColor AIPlot3D::getBackgroundColor()
{
  return c_bg;
}

void AIPlot3D::setGridColor(int r, int g, int b)
{
  c_grid.setRgb(r,g,b);
  makeScales();
  makeGrid();
}

void AIPlot3D::setCommonColor(int r, int g, int b)
{
  c_comm.setRgb(r,g,b);
}


void AIPlot3D::setAutoScale(bool s)
{
	autoscale=s;
}

void AIPlot3D::setXAxisDisplay(int type)
{
  x_axis_display_type=type;
}

void AIPlot3D::setYAxisDisplay(int type)
{
  y_axis_display_type=type;
}

void AIPlot3D::setZAxisDisplay(int type)
{
  z_axis_display_type=type;
}

void AIPlot3D::print()
{
#ifdef SGI
#ifdef GLP
	GLfloat Black[3] = {0.0, 0.0, 0.0};
	GLfloat White[3] = {1.0, 1.0, 1.0};
	FILE *fp;

	if((fp = fopen("print.ps","w")) == NULL) {
		fprintf(stderr,"CanÆt open \\print.ps\n");
		return;
	}

        //glViewport(0,0,500,500);

	glpBeginPage(GLP_PS /*PS*/,"SceneViewer","SceneViewer","",fp);
	GLPERR;	
	glpSetPage(8.5,11.0,GLP_ORIENT_PORTRAIT,1,.25,.25,.25,.25);
	GLPERR;	
	glpSetDevice(GLP_DEVICE_TYPE_RGB,0);
	GLPERR;
	//glpEnable(GLP_OPTION_SUBDIVIDE);
	//glpSetInteger(GLP_OPTION_SUBDIVIDE_LEVEL,4);
	//glpEnable(GLP_OPTION_OCCLUSION_CULL);
	//glpEnable(GLP_OPTION_DB_PAGETIMER);
	// Set the correct background color
	//bgColor[0] = bgc[0];
	//bgColor[1] = bgc[1];
	//bgColor[2] = bgc[2];
	glpSetViewport(GLP_VIEWPORT_FILL,NULL,White);
	GLPERR;
	glpBegin(GLP_3D_COLOR,100000);	
	GLPERR;
	paintGL();
	//currentViewer->getGLRenderAction()->invalidateState();
	//currentViewer->getGLRenderAction()->apply(sceneGraph);
	//currentViewer->getGLRenderAction()->invalidateState();
	glpEnd();
	GLPERR;
	glpEndPage();
	GLPERR;

	system("lp print.ps");
	//fclose(fp);
#endif
#endif
}

// AIPlot3D Mouse Event Methods

void AIPlot3D::mousePressEvent( QMouseEvent *e )
{
  debug("AIPLOT3D.CPP Mouse Press Event");
  lastx=e->x();
  lasty=e->y();
}

void AIPlot3D::mouseMoveEvent( QMouseEvent *e )
{
  double yangle=((double)e->x()-(double)lastx)/(double)width()*360.0;
  double xangle=((double)e->y()-(double)lasty)/(double)height()*360.0;

  debug("AIPLOT3D.CPP Mouse Move Event e->x()=%d e->y()=%d xangle=%f yangle=%f",e->x(),e->y(),xangle,yangle);

  xRot+=xangle;
  yRot+=yangle;
  lastx=e->x();
  lasty=e->y();

  repaint(FALSE);
}

void AIPlot3D::mouseReleaseEvent( QMouseEvent *e )
{
  debug("AIPLOT3D.CPP Mouse Release Event");
}


//#########################################################################
//#########################################################################
//#########################################################################
//#########################################################################
//#########################################################################
//#########################################################################


//#########################################################################
//#########################################################################
//#########################################################################
//#########################################################################
//#########################################################################
//#########################################################################


//#########################################################################
//#########################################################################
//#########################################################################
//#########################################################################
//#########################################################################
//#########################################################################


