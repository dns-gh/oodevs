/*
	Written 1998-1999 by Uwe Langenkamp

	This software may be used and distributed according to the terms
	of the GNU Public License, incorporated herein by reference.

	The author may be reached as ul@it-guru.de
*/

                           
//FIXME: complete revision nessesary here.

#include "MOS_Light_Pch.h"

#include <qwidget.h>
#include <qpainter.h>
#include <qmenubar.h>
#include <qpainter.h>
#include <qapp.h>
#include <qsocknot.h>
#include <qpopmenu.h>
#include <qlabel.h>
#include <qfont.h>
#include <qframe.h>
#include <qlayout.h>
#include <qwidcoll.h>
#include <qobjcoll.h>
#include <qkeycode.h>
#include <qtimer.h>
#include <qbttngrp.h>
#include <qfiledlg.h>
#include <qstrlist.h>
#include <qmsgbox.h>

#include "aidemo1.h"

// some math.h do not define M_PI
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NPOINTS 400

AiDemo1::AiDemo1( QWidget *parent, const char *name ) : QWidget(parent,name)
{
  //
  // MENUBAR
  //
  QBoxLayout *topLayout = new QVBoxLayout( this, 5 ); 
  QBoxLayout *mlayout;
  QBoxLayout *layout1 = new QVBoxLayout(); 
  QBoxLayout *layout2 = new QVBoxLayout(); 
  QBoxLayout *layout3 = new QVBoxLayout(); 
  QBoxLayout *layout4 = new QVBoxLayout(); 
  QBoxLayout *layout5 = new QVBoxLayout(); 
  QBoxLayout *layout6 = new QVBoxLayout(); 
  QBoxLayout *layout7 = new QVBoxLayout(); 
  QBoxLayout *layout8 = new QVBoxLayout(); 
  QLabel *label;
                 
  m_file = new QPopupMenu;
  m_file->insertItem( "&Print",this, SLOT(print()) );
  m_file->insertSeparator();
  m_file->insertItem( "E&xit",qApp, SLOT(quit()) );
  
  m_online = new QPopupMenu;
  m_online->insertItem( "&Start",this, SLOT(o_sine()) );
  m_online->insertItem( "&Stop",this, SLOT(destroyAll()) );
  
  m_zoom = new QPopupMenu;
  m_zoom->insertItem( "&100%",this, SLOT(zoom100()) );
  m_zoom->insertItem( "&50%",this, SLOT(zoom50()) );
  m_zoom->insertItem( "&10%",this, SLOT(zoom10()) );

  m_database = new QPopupMenu;

  m_help = new QPopupMenu;
  m_help->insertItem( "&About");
  connect(m_help, SIGNAL(activated(int)), SLOT(help_menu_activated(int)));
 

  //FIXME: qt - no tooltips on the menubar and popup menus???
  //menubar = new KMenuBar( this );
  menubar = new QMenuBar( this );
  menubar->insertItem( "&File", m_file );
  menubar->insertItem( "&Online", m_online );
  menubar->insertItem( "&Scale", m_zoom );
  menubar->insertSeparator();
  menubar->insertItem( "&Help", m_help);
  menubar->setMinimumSize(0,menubar->height());
  menubar->setMaximumSize(10000,menubar->height());

  /////////////////////////////////////////////////////////////

  //setMenu(menubar);
  // ...and tell the layout about it.
  topLayout->setMenuBar( menubar );

  layout = new QGridLayout(4,5,1,"layout");
  //box = new QVBoxLayout();
  topLayout->addLayout(layout);
  
  this->setMinimumSize( 400+4,
                        300+menubar->height()
                        );

  QWidget *w;
  w = new QWidget(this);
  //box->addWidget(w);
 
  graphic=new AIPlot(this,"graphic",NPOINTS);
  graphic->setRanges(-200,200,-350.0,350.0);
  graphic->setPlotColor(1,255,127,0);
  graphic->setDoubleBuffer(TRUE);

  graph2=new AIPlot(this,"graphic2",NPOINTS);
  graph2->setRanges(-500,500,-500.0,500.0);
  graph2->setPlotType(AI_PLOT_POLAR);
  graph2->setDoubleBuffer(TRUE);

  tacho=new AIMeter(this,"tacho",1);
  tacho->setRanges(0.0,3500.0);
  tacho->setType(AI_METER_TACHO);
  tacho->setRangeType("1/min");

  rms1=new AIMeter(this,"rms1",1);
  rms1->setRanges(0.0,150.0);
  rms1->setRangeType("æm");

  layout->addLayout(layout2,0,4);
  layout->addLayout(layout3,1,3);
  layout->addLayout(layout4,1,4);
  layout->addLayout(layout5,2,3);
  layout->addLayout(layout6,2,4);
  layout->addLayout(layout7,3,3);
  layout->addLayout(layout8,3,4);

  layout->addMultiCellWidget(graphic,0,1,1,2,0);
  layout->addMultiCellWidget(graph2,2,3,1,2,0);

  rms2=new AIMeter(this,"rms2",1);
  rms2->setRanges(0.0,150.0);
  rms2->setRangeType("æm");

  bar1=new AIBar(this,"bar1",1);
  bar1->setRanges(-200.0,200.0);
  bar1->setRangeType("æm");

  bar2=new AIBar(this,"bar2",4);
  bar2->stackBars();
  //bar2->showScale(FALSE);    
  bar2->setPlotColor(0, 0, 255, 0);   // Bar No 1, R,G,B
  bar2->setPlotColor(1, 255, 255, 0); // Bar No 2, R,G,B
  bar2->setPlotColor(2, 255, 0, 0);   // Bar No 3, R,G,B
  bar2->setPlotColor(3, 0, 0, 255);   // Bar No 4, R,G,B
  bar2->setRanges(0.0,400.0);
  bar2->setRangeType("æm");

  bar3=new AIBar(this,"bar1",1);
  bar3->setRanges(-200.0,200.0);
  bar3->setRangeType("æm");
  bar4=new AIBar(this,"bar2",1);
  bar4->setRanges(-200.0,200.0);
  bar4->setRangeType("æm");

  bar5=new AIBar(this,"bar1",1);
  bar5->setRanges(-200.0,200.0);
  bar5->setRangeType("æm");
  bar6=new AIBar(this,"bar2",1);
  bar6->setRanges(-200.0,200.0);
  bar6->setRangeType("æm");

  bar7=new AIBar(this,"bar1",4);
  bar7->setRanges(-200.0,200.0);
  bar7->setRangeType("æm");
  bar7->setPlotColor(1,255,127,0);
  bar7->setPlotColor(2,0,0,255);
  bar7->setPlotColor(3,127,0,255);
  bar7->setPlotColor(3,127,127,255);

  bar8=new AIBar(this,"bar2",4);
  bar8->setRanges(-200.0,200.0);
  bar8->setRangeType("æm");
  bar8->setPlotColor(1,255,127,0);
  bar8->setPlotColor(2,0,0,255);
  bar8->setPlotColor(3,127,0,255);
  bar8->setPlotColor(3,127,127,255);

  label = new QLabel("Trace 1(0)",this,"label");
  layout1->addWidget(bar1,4);
  layout1->addWidget(label,1);
  label = new QLabel("Trace 2(0)",this,"label");
  layout2->addWidget(bar2,4);
  layout2->addWidget(label,1);
  label = new QLabel("Trace 1(50)",this,"label");
  layout3->addWidget(bar3,4);
  layout3->addWidget(label,1);
  label = new QLabel("Trace 2(50)",this,"label");
  layout4->addWidget(bar4,4);
  layout4->addWidget(label,1);
  label = new QLabel("Trace 1(100)",this,"label");
  layout5->addWidget(bar5,4);
  layout5->addWidget(label,1);
  label = new QLabel("Trace 2(100)",this,"label");
  layout6->addWidget(bar6,4);
  layout6->addWidget(label,1);
  label = new QLabel("Trace 1(RMS)",this,"label");
  layout7->addWidget(bar7,4);
  layout7->addWidget(label,1);
  label = new QLabel("Trace 1(RMS)",this,"label");
  layout8->addWidget(bar8,4);
  layout8->addWidget(label,1);

  mlayout=new QVBoxLayout(); 
  layout->addLayout(mlayout,0,0);
  label = new QLabel("Tachometer",this,"label");
  mlayout->addWidget(tacho,4);
  mlayout->addWidget(label,1);

  mlayout=new QVBoxLayout(); 
  layout->addLayout(mlayout,1,0);
  label = new QLabel("RMS Meter Chan 1",this,"label");
  mlayout->addWidget(rms1,4);
  mlayout->addWidget(label,1);

  mlayout=new QVBoxLayout(); 
  layout->addLayout(mlayout,2,0);
  label = new QLabel("RMS Meter Chan 2",this,"label");
  mlayout->addWidget(rms2,4);
  mlayout->addWidget(label,1);
    
  mlayout=new QVBoxLayout(); 
  layout->addLayout(mlayout,3,0);
  label = new QLabel("QT/AI\nDemo",this,"label");

  label->setFont(QFont("Helvetica",32,QFont::Bold));
  mlayout->addWidget(label);
    
  resize(width(),height());
  
}


AiDemo1::~AiDemo1()
{
}

void AiDemo1::print()
{
  //graphic->printIt();
}

void AiDemo1::destroyAll()
{
  killTimer(tm);
}


void AiDemo1::o_sine()
{
  destroyAll();
  graphic->setRanges(-200,200,-350.0,350.0);
  graph2->setRanges(-500,500,-500.0,500.0);
  graphic->show();
  resize(width(),height());
  tm=startTimer(150);
  set_curve(SINE);
}

void AiDemo1::o_orbit()
{
  destroyAll();
  graphic->setRanges(-200,200,-350.0,350.0);
  graph2->setRanges(-500,500,-500.0,500.0);
  graphic->show();
  resize(width(),height());
  tm=startTimer(150);
  set_curve(ORBIT);
}

void AiDemo1::o_line()
{
}

void AiDemo1::d_trend()
{
}

void AiDemo1::set_curve(int s_curve)
{
  curve=s_curve;
}

int AiDemo1::get_curve(void)
{
  return curve;
}

void AiDemo1::zoom100()
{
  graphic->setRanges(-200,200,-350.0,350.0);
  graph2->setRanges(-500,500,-500.0,500.0);
  bar1->setRanges(-200,200);
  bar2->setRanges(0,400);
  bar3->setRanges(-200,200);
  bar4->setRanges(-200,200);
  bar5->setRanges(-200,200);
  bar6->setRanges(-200,200);
  bar7->setRanges(-200,200);
  bar8->setRanges(-200,200);
}

void AiDemo1::zoom50()
{
  graphic->setRanges(-200,200,-700.0,700.0);
  graph2->setRanges(-1000,1000,-1000.0,1000.0);
  bar1->setRanges(-400,400);
  bar2->setRanges(0,800);
  bar3->setRanges(-400,400);
  bar4->setRanges(-400,400);
  bar5->setRanges(-400,400);
  bar6->setRanges(-400,400);
  bar7->setRanges(-400,400);
  bar8->setRanges(-400,400);
}

void AiDemo1::zoom10()         
{
  graphic->setRanges(-200,200,-3500.0,3500.0);
  graph2->setRanges(-5000,5000,-5000.0,5000.0);
  bar1->setRanges(-2000,2000);
  bar2->setRanges(0,4000);
  bar3->setRanges(-2000,2000);
  bar4->setRanges(-2000,2000);
  bar5->setRanges(-2000,2000);
  bar6->setRanges(-2000,2000);
  bar7->setRanges(-2000,2000);
  bar8->setRanges(-2000,2000);
}

void AiDemo1::timerEvent( QTimerEvent *te )
{
  int x;
  double xmin=-200,xmax=200,rmssum=0;
  static double x1[NPOINTS],y1[NPOINTS];
  static double x2[NPOINTS],y2[NPOINTS];
  static double x3[NPOINTS],y3[NPOINTS];
  static double rms;
  static double speed=0;
  static int speed_up=TRUE;
  
  static char shift=0;

  switch(get_curve()) {
  case SINE:    
  case ORBIT:
    if (speed_up) {
      speed+=14.5f;
      if (speed>=3000.0f) {
	speed_up=FALSE;
	speed=3000.0f;
      }
    } else {
      //speed-=9.3f;
      //if (speed<=0.0f)
      //	speed_up=TRUE;
    }

    for (x=0;x<NPOINTS;x++) {
      x2[x]=x1[x]=((double)x)*(xmax-xmin)/NPOINTS+xmin;
      y1[x]=((speed/20.0f)*sin((float)(x+shift)/250.0*2.0*M_PI)
	     -(speed/40.0f+10.0)*sin((float)(2*x+shift)/250.0*2.0*M_PI));
      y2[x]=((speed/18.0f)*cos((float)(x+shift)/250.0*2.0*M_PI)
	     +(speed/100.0f+20.0)*cos((float)(3*x+shift)/250.0*2.0*M_PI));
    }

    shift+=10;
    graphic->setData(0,NPOINTS,x1,y1);    
    graphic->setData(1,NPOINTS,x2,y2);    
    graphic->repaint(FALSE);

    bar1->setData(0,y1[0]);    
    bar1->repaint(FALSE);

    bar2->setData(0,fabs(y1[20]));
    bar2->setData(1,fabs(y1[0]));    
    bar2->setData(2,fabs(y2[0]));    
    bar2->setData(3,fabs(y2[10]));    
    bar2->repaint(FALSE);

    bar3->setData(0,y1[50]);    
    bar3->repaint(FALSE);
    bar4->setData(0,y2[50]);    
    bar4->repaint(FALSE);
    bar5->setData(0,y1[100]);    
    bar5->repaint(FALSE);
    bar6->setData(0,y2[100]);    
    bar6->repaint(FALSE);
    
    rmssum=0.0f;
    for (x=0;x<NPOINTS;x++) {
      rmssum+=pow(y1[x],2);
    }
    rms=sqrt(rmssum/NPOINTS);

    rms1->setData(0,rms);    
    rms1->repaint(FALSE);    
    bar7->setData(0,rms);    
    bar7->setData(1,y1[0]);    
    bar7->setData(2,y1[50]);    
    bar7->setData(3,y1[100]);    
    bar7->repaint(FALSE);

    rmssum=0.0f;
    for (x=0;x<NPOINTS;x++) {
      rmssum+=pow(y2[x],2);
    }
    rms=sqrt(rmssum/NPOINTS);

    rms2->setData(0,rms);
    rms2->repaint(FALSE);    
    bar8->setData(0,rms);    
    bar8->setData(1,y2[0]);    
    bar8->setData(2,y2[50]);    
    bar8->setData(3,y2[100]);    
    bar8->repaint(FALSE);
    tacho->setData(0,speed);    
    tacho->repaint(FALSE);    

    graph2->setData(0,NPOINTS,y1,y2);    
    graph2->repaint(FALSE);
    break;
  }
}

void AiDemo1::help_menu_activated(int item)
{
  QString version="AiDemo1 for QT Version 0.1";
  switch (item) {
  case 0:
    QMessageBox::message( "About aidemo1",version+"\n\nGPL General Public License \nAuthor: Uwe Langenkamp (ul@it-guru.de)\nVMS AiDemo1 for X Window System\nbased on Advanced Instrumentation Library and QT.");
    break;
#ifdef KDE
  case 1:
    kapp->invokeHTMLHelp("kmonitor/index.html","");
    break;
#endif
  }
}

/* --| main |---------------------------------------------------------------- */

int main( int argc, char ** argv )
{ 
  char buf[100];
  QApplication *a;
  QColor *bg,*c,*light,*dark;
  QColorGroup *fg,*active,*disabled;
  QPalette *fgpal;
  AiDemo1 *m;
  int x=0,y=0,width=1024,height=768;
  
  int addr;
  QWidget *plug_parent=NULL;

  // Check Args
  if (argc>1) {
    x=atoi(argv[1]);
    y=atoi(argv[2]);
    width=atoi(argv[3]);
    height=atoi(argv[4]);
  }
  fprintf(stderr,"new QApp()\n");
  a=new QApplication(argc, argv);
  
  QFont font("Helvetica",12);
  a->setFont(font,TRUE);  

  //QWidget *parent=new QWidget(0,"w",Qt::WStyle_Customize | Qt::WStyle_NoBorder);
  m=new AiDemo1();
  fprintf(stderr,"m=%d\n",(long)&m);

  m->show();
  //parent->show();

#ifdef CHANGE_COLOR
  c=new QColor();
  bg=new QColor();
  light=new QColor();
  dark=new QColor();

  bg->setRgb(80,110,150);
  light->setRgb(180,180,210);
  dark->setRgb(30,30,80);

  c->setRgb(255,255,255);
  fg = new QColorGroup(*c,*bg,*light,*dark,*bg,*c,*bg);
  active = new QColorGroup(*bg,*bg,*light,*dark,*bg,*c,*bg);

  c->setRgb(127,127,127);
  disabled = new QColorGroup(*c,*bg,*light,*dark,*bg,*c,*bg);

  fgpal = new QPalette(*fg,*disabled,*active);
  a->setPalette(*fgpal,TRUE);
#endif // #ifdef CHANGE_COLOR

  fprintf(stderr,"a->exec()\n");
  return a->exec();
}



