// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __selftraining_app_pch_h_
#define __selftraining_app_pch_h_

#pragma warning( disable: 4231 4355 )
#include <string>
#include <exception>
#include <map>
#include <memory>

#pragma warning( push, 0 )

#include <QtCore/qstring.h>
#include <QtCore/qsettings.h>
#include <QtCore/qtextcodec.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qtimer.h>
#include <QtCore/qlibrary.h>
#include <QtCore/qt_windows.h>
#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qtranslator.h>

#include <QtGui/qwidget.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qcursor.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qtabbar.h>
#include <QtGui/qtabwidget.h>
#include <QtGui/qapplication.h>
#include <QtGui/qfont.h>
#include <QtGui/qimage.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qcursor.h>
#include <QtGui/qpainter.h>
#include <QtGui/qpalette.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qevent.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qdesktopwidget.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qfiledialog.h>

#include <Qt3Support/q3progressbar.h>
#include <Qt3Support/q3gridlayout.h>
#include <Qt3Support/q3boxlayout.h>
#include <Qt3Support/q3header.h>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3textedit.h>
#include <Qt3Support/q3vbox.h>
#include <Qt3Support/q3widgetstack.h>
#include <Qt3Support/q3popupmenu.h>
#include <Qt3Support/q3url.h>
#include <Qt3Support/q3filedialog.h>
#include <Qt3Support/q3listbox.h>
#include <Qt3Support/q3mainwindow.h>
#include <Qt3Support/q3groupbox.h>
#include <Qt3Support/q3button.h>

#pragma warning( pop )

#undef min
#undef max

#endif // __selftraining_app_pch_h_
