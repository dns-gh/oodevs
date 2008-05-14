// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __clients_gui_pch_h_
#define __clients_gui_pch_h_

#pragma warning( disable : 4231 4290 4702 )

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef min
#undef max
#undef Yield
#undef GetObject
#undef GetMessage

#pragma warning( push, 0 )
#include <qaction.h>
#include <qbitmap.h>
#include <qbuttongroup.h>
#include <qcanvas.h>
#include <qcheckbox.h>
#include <qcolordialog.h>
#include <qcombobox.h>
#include <qcstring.h>
#include <qcursor.h>
#include <qdatetimeedit.h>
#include <qdial.h>
#include <qdialog.h>
#include <qdict.h>
#include <qdockwindow.h>
#include <qdragobject.h> 
#include <qfile.h>
#include <qfiledialog.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qframe.h>
#include <qgl.h>
#include <qgroupbox.h>
#include <qhbox.h>
#include <qheader.h>
#include <qiconset.h>
#include <qimage.h>
#include <qinputdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qlistview.h>
#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qprocess.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qregexp.h>
#include <qsettings.h>
#include <qsignalmapper.h> 
#include <qsimplerichtext.h>
#include <qslider.h>
#include <qsound.h>
#include <qspinbox.h>
#include <qsplashscreen.h>
#include <qstatusbar.h>
#include <qstring.h>
#include <qtable.h>
#include <qtabwidget.h>
#include <qtextedit.h>
#include <qtimer.h>
#include <qtoolbar.h>
#include <qtoolbox.h>
#include <qtoolbutton.h>
#include <qtooltip.h>
#include <qvalidator.h>
#include <qvbox.h>
#include <qvbuttongroup.h>
#include <qwidgetstack.h>
#pragma warning( pop )

#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <cassert>

#include "geometry/Types.h"

typedef std::vector< geometry::Point2f > T_PointVector;
typedef T_PointVector::iterator         IT_PointVector;
typedef T_PointVector::const_iterator  CIT_PointVector;

#endif // __clients_gui_pch_h_
