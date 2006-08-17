// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __astec_prepa_app_pch_h_
#define __astec_prepa_app_pch_h_

#define APP_NAME "ASTEC - Preparation"
#define APP_VERSION "Alpha-1"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef min
#undef max
#undef Yield
#undef GetObject
#undef GetMessage

#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qlistbox.h>
#include <qpopupmenu.h>
#include <qcursor.h>
#include <qgroupbox.h>
#include <qframe.h>
#include <qspinbox.h>
#include <qaction.h>
#include <qgl.h>
#include <qradiobutton.h>
#include <qmainwindow.h>
#include <qdialog.h>
#include <qtoolbar.h>
#include <qsignalmapper.h> 
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qtextedit.h>
#include <qtimer.h>
#include <qstatusbar.h>
#include <qsettings.h>
#include <qheader.h>
#include <qfile.h>
#include <qobject.h>
#include <qlistview.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qsplashscreen.h>
#include <qtabwidget.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qwidgetstack.h>
#include <qvbox.h>
#include <qdialog.h>
#include <qtextedit.h>
#include <qheader.h>
#include <qdial.h>
#include <qhbox.h>
#include <qtoolbutton.h>
#include <qtooltip.h>
#include <qdragobject.h> 
#include <qcheckbox.h>
#include <qsimplerichtext.h>
#include <qhbox.h>
#include <qdial.h>
#include <qgroupbox.h>
#include <qslider.h>
#include <qdatetimeedit.h>
#include <qbuttongroup.h>
#include <qstring.h>
#include <qiconset.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qmainwindow.h>
#include <qdockwindow.h>
#include <qapplication.h>

#include <map>
#include <vector>
#include <set>

#include "geometry/Types.h"
typedef std::vector< geometry::Point2f > T_PointVector;
typedef T_PointVector::iterator         IT_PointVector;
typedef T_PointVector::const_iterator  CIT_PointVector;

#endif // __astec_prepa_app_pch_h_
