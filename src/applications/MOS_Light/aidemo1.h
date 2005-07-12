/*
	Written 1998-1999 by Uwe Langenkamp

	This software may be used and distributed according to the terms
	of the GNU Public License, incorporated herein by reference.

	The author may be reached as ul@it-guru.de
*/

#ifndef AiDemo1_H
#define AiDemo1_H

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
#include <qmenubar.h>
#include <qtimer.h>

// what on earth is this bull.... #define for?
#define STDC_HEADERS
#define HAVE_UNISTD_H
#include <stdio.h>
#include <stdlib.h>

#include "aiplot.h"
#include "aibar.h"
#include "aimeter.h"

#define SINE  0
#define ORBIT 1

class AiDemo1 : public QWidget
{
  Q_OBJECT

public:
  AiDemo1( QWidget *parent=0, const char *name=0 );
  ~AiDemo1();
  void set_curve(int curve);
  int get_curve(void);
  
signals:

protected:
  void timerEvent( QTimerEvent *te );

private slots:

  // MENU
  void destroyAll();
  void print();
  void o_sine();
  void o_orbit();
  void o_line();
  void zoom100();
  void zoom50();
  void zoom10();
  void d_trend();
  void help_menu_activated(int item);

private:

  int tm; // Timer ID
  int curve;

  QMenuBar*        menubar;

  QPopupMenu* m_file;
  QPopupMenu* m_online;
  QPopupMenu* m_zoom;
  QPopupMenu* m_database;
  QPopupMenu* m_help;

  QBoxLayout* box;

  QGridLayout  *layout;

  AIPlot*  graphic;
  AIPlot*  graph2;
  AIMeter* tacho;
  AIMeter* rms1;
  AIMeter* rms2;
  AIBar* bar1;
  AIBar* bar2;
  AIBar* bar3;
  AIBar* bar4;
  AIBar* bar5;
  AIBar* bar6;
  AIBar* bar7;
  AIBar* bar8;

  QPointArray *qa;
};

#endif // AiDemo1_H


