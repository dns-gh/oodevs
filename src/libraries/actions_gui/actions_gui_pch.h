// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __actions_gui_pch_h_
#define __actions_gui_pch_h_

#pragma warning( push, 0 )
#include <windows.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <Qtcore/qtimer.h>
#include <QtCore/qdatetime.h>

#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qscrollarea.h>
#include <QtGui/qtreeview.h>
#include <QtGui/qlistwidget.h>
#include <QtGui/qstandarditemmodel.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qtextedit.h>

#include <Qt3Support/q3groupbox.h>
#include <Qt3Support/q3hbox.h>
#include <Qt3Support/q3scrollview.h>
#include <Qt3Support/q3popupmenu.h>
#include <Qt3Support/q3vbox.h>

#include <QtOpenGL/qgl.h>
#pragma warning( pop )

#include <string>
#include <map>
#include <set>
#include <vector>

#undef min
#undef max

#include <tools/Exception.h>

#include <geometry/types.h>

#endif // __actions_gui_pch_h_
