/*
	Written 1998-1999 by Uwe Langenkamp

	This software may be used and distributed according to the terms
	of the GNU Public License, incorporated herein by reference.

	The author may be reached as ul@it-guru.de
*/

#ifndef AIPlot3D_H
#define AIPlot3D_H

#include <qwidget.h>
#include <qcolor.h>
#include <qpopmenu.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qprinter.h>
#include <qpushbt.h>
#include <qradiobt.h>
#include <qbttngrp.h>
#include <qtooltip.h>


#include <stdio.h>
#include <stdlib.h>

#include <qgl.h>
#include <GL/glut.h>

/*******************************************************************/
/******* Begin Draw Window Definition  *****************************/
/*******************************************************************/

#define AI_PLOT3D_LINES    0
#define AI_PLOT3D_CLINES   1
#define AI_PLOT3D_HLINES   2
#define AI_PLOT3D_HCLINES  3
#define AI_PLOT3D_COLQUAD  4
#define AI_PLOT3D_FLATWALL 5

#define AXIS_NUMBER 0x0000001
#define AXIS_DATE   0x0000002
#define AXIS_GERMAN 0x0000004
#define AXIS_UK     0x0000008


class AIPlot3D : public QGLWidget
{
    Q_OBJECT
public:
    // Main Methods
    AIPlot3D(QWidget *parent,const char *name=0,int b=512,int d=20);
    ~AIPlot3D();

	void print();

    void   addZTrace(double y,double *p);
    void   setXTrace(double *x);
    void   setXRange(double xmn,double xmx);
    void   setYRange(double ymn,double ymx);
    void   setZRange(double zmn,double zmx);

    void setPlotColor(int r,int g,int b);
    void setPlotColor(QColor c);
    void setBackgroundColor(QColor c);
    void setBackgroundColor(int r, int g, int b);
    void setGridColor(QColor c);
    void setGridColor(int r, int g, int b);
    void setCommonColor(QColor c);
    void setCommonColor(int r, int g, int b);

    QColor getBackgroundColor();

    void setAutoScale(bool s);
	
    virtual void setXAxisDisplay(int type);
    virtual void setYAxisDisplay(int type);
    virtual void setZAxisDisplay(int type);

    int          bufsize,bufdepth,actdepth;
    GLfloat      **nmatrix;
    double       **zmatrix;
    double       *xtrace;
    double       *ytrace;

    double       xmin,xmax;
    double       ymin,ymax;
    double       zmin,zmax;
 
public slots:

    void		setXRotation( double degrees );
    void		setYRotation( double degrees );
    void		setZRotation( double degrees );
    void		setGraphType(int type);    
    void		setZoom( double percent );

protected:

    void		paintGL();
    void		resizeGL( int w, int h );

    virtual GLuint 	makeGrid();
    virtual GLuint 	makeCurves();
    virtual GLuint 	makeSquares();
    virtual GLuint 	makePolygons();
    virtual GLuint 	makeScales();

    virtual void   mousePressEvent( QMouseEvent *e );
    virtual void   mouseReleaseEvent( QMouseEvent *e );
    virtual void   mouseMoveEvent( QMouseEvent *e );

protected:
    QColor       c_plot,c_bg,c_grid,c_comm;
    int          z_axis_display_type;
    int          x_axis_display_type;
    int          y_axis_display_type;

private:

    void    updateObjects();

    GLuint grid;
    GLuint curves;
    GLuint squares;
    GLuint polygons;
    GLuint scales;
    GLfloat xRot, yRot, zRot, scale;
    GLfloat percent;
    int    graphtype;
    bool   recalc;
    bool   autoscale;

    int    lastx,lasty;
};

/*******************************************************************/
/********* End Draw Window Definition  *****************************/
/*******************************************************************/
#endif // #ifdef AIPLot3D_H


