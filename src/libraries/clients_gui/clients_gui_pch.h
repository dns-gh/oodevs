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

#pragma warning( disable : 4231 4290 4702 4355 )

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef min
#undef max
#undef Yield
#undef GetObject
#undef GetMessage

#define QMENUDATA_H

#pragma warning( push, 0 )

#include <QtGui/QtGui>

#include <QtOpenGL/qgl.h>

#include <Qt/qapplication.h>

#include <QtCore/qobject.h>
#include <QtCore/qpoint.h>
#include <QtCore/qrect.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qdatetime.h>
#include <QtCore/QFile>
#include <QtCore/qregexp.h>
#include <QtCore/qsettings.h>
#include <QtCore/qsignalmapper.h>
#include <QtCore/qtimer.h>

#include <Qt3Support/q3button.h>
#include <Qt3Support/q3canvas.h>
#include <Qt3Support/q3cstring.h>
#include <Qt3Support/q3datetimeedit.h>
#include <Qt3Support/q3dict.h>
#include <Qt3Support/q3dragobject.h>
#include <Qt3Support/q3filedialog.h>
#include <Qt3Support/q3frame.h>
#include <Qt3Support/q3valuelist.h>
#include <Qt3Support/q3groupbox.h>
#include <Qt3Support/q3hbox.h>
#include <Qt3Support/q3header.h>
#include <Qt3Support/q3listbox.h>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3popupmenu.h>
#include <Qt3Support/q3process.h>
#include <Qt3Support/q3simplerichtext.h>
#include <Qt3Support/q3table.h>
#include <Qt3Support/q3textedit.h>
#include <Qt3Support/q3toolbar.h>
#include <Qt3Support/q3boxlayout.h>
#include <Qt3Support/q3vbox.h>
#include <Qt3Support/q3action.h>
#include <Qt3Support/q3buttongroup.h>
#include <Qt3Support/q3widgetstack.h>
#include <Qt3Support/q3gridlayout.h>
#include <Qt3Support/q3pointarray.h>
#include <Qt3Support/q3vgroupbox.h>

#pragma warning( pop )

#include <xeumeuleu/xml.hpp>

#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <cassert>

#include <geometry/Types.h>

typedef std::vector< geometry::Point2f > T_PointVector;
typedef T_PointVector::iterator         IT_PointVector;
typedef T_PointVector::const_iterator  CIT_PointVector;

inline std::ostream& operator<<( std::ostream& os, const QString& s )
{
    return os << s.toStdString();
}

inline xml::xostream& operator<<( xml::xostream& os, const QString& value )
{
    return os << value.toStdString();
}

#endif // __clients_gui_pch_h_
