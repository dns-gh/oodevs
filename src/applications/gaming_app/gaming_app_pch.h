// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#pragma warning( disable : 4231 )

#ifndef __gaming_app_pch_h_
#define __gaming_app_pch_h_

#define APP_NAME    "SWORD Officer Training - Gaming"
#define APP_VERSION "1.0"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef min
#undef max
#undef Yield
#undef GetObject
#undef GetMessage

#pragma warning( push, 0 )
#include <qaction.h>
#include <qapplication.h>
#include <qbrush.h>
#include <qbuttongroup.h>
#include <qcanvas.h>
#include <qcheckbox.h>
#include <qcolor.h>
#include <qcombobox.h>
#include <qcursor.h>
#include <qdatetimeedit.h>
#include <qdial.h>
#include <qdialog.h>
#include <qdockwindow.h>
#include <qdragobject.h> 
#include <qfile.h>
#include <qfiledialog.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qframe.h>
#include <qgl.h>
#include <qgrid.h>
#include <qgroupbox.h>
#include <qhbox.h>
#include <qheader.h>
#include <qhgroupbox.h>
#include <qiconset.h>
#include <qiconview.h>
#include <qimage.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qlistview.h>
#include <qmainwindow.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qobjectlist.h>
#include <qpainter.h>
#include <qpalette.h>
#include <qpen.h>
#include <qpixmap.h>
#include <qpoint.h>
#include <qpointarray.h>
#include <qpopupmenu.h>
#include <qprogressbar.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qrect.h>
#include <qsettings.h>
#include <qsignalmapper.h> 
#include <qsimplerichtext.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qsplashscreen.h>
#include <qsplitter.h>
#include <qstatusbar.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtabwidget.h>
#include <qtextedit.h>
#include <qtimer.h>
#include <qtoolbar.h>
#include <qtoolbox.h>
#include <qtoolbutton.h>
#include <qtooltip.h>
#include <qvalidator.h>
#include <qvaluelist.h>
#include <qvbox.h>
#include <qvgroupbox.h>
#include <qwidget.h>
#include <qwidgetstack.h>
#pragma warning( pop )

#include <map>
#include <vector>
#include <set>

#include "geometry/Types.h"
typedef std::vector< geometry::Point2f > T_PointVector;
typedef T_PointVector::iterator         IT_PointVector;
typedef T_PointVector::const_iterator  CIT_PointVector;

#endif // __gaming_app_pch_h_
