// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#pragma warning( disable : 4231 )
#pragma warning( disable : 4355 ) // 'this' : used in base member initializer list
#pragma warning( disable : 4996 ) // Function call with parameters that may be unsafe

#ifndef __preparation_app_pch_h_
#define __preparation_app_pch_h_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef min
#undef max
#undef Yield
#undef GetObject
#undef GetMessage

#pragma warning( push, 0 )

#include <QtGui/QtGui>

#include <QtOpenGL/qgl.h>

#include <QtCore/qfile.h>
#include <QtCore/qobject.h>
#include <QtCore/qsettings.h>
#include <QtCore/qsignalmapper.h>
#include <QtCore/qstring.h>
#include <Qtcore/qtimer.h>
#include <QtCore/qtextcodec.h>

#include <Qt3Support/q3button.h>
#include <Qt3Support/q3buttongroup.h>
#include <Qt3Support/q3datetimeedit.h>
#include <Qt3Support/q3dragobject.h>
#include <Qt3Support/q3filedialog.h>
#include <Qt3Support/q3frame.h>
#include <Qt3Support/q3grid.h>
#include <Qt3Support/q3groupbox.h>
#include <Qt3Support/q3hbox.h>
#include <Qt3Support/q3buttongroup.h>
#include <Qt3Support/q3header.h>
#include <Qt3Support/q3hgroupbox.h>
#include <Qt3Support/q3listbox.h>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3popupmenu.h>
#include <Qt3Support/q3simplerichtext.h>
#include <Qt3Support/q3table.h>
#include <Qt3Support/q3textedit.h>
#include <Qt3Support/q3toolbar.h>
#include <Qt3Support/q3vbox.h>
#include <Qt3Support/q3widgetstack.h>
#include <Qt3Support/q3boxlayout.h>
#include <Qt3Support/q3gridlayout.h>
#include <Qt3Support/q3textstream.h>
#include <Qt3Support/q3syntaxhighlighter.h>
#include <Qt3Support/q3vgroupbox.h>

#pragma warning( pop )

#include <map>
#include <vector>
#include <set>

#include <geometry/Types.h>
typedef std::vector< geometry::Point2f > T_PointVector;
typedef T_PointVector::iterator         IT_PointVector;
typedef T_PointVector::const_iterator  CIT_PointVector;

#include <xeumeuleu/xml.hpp>

inline std::ostream& operator<<( std::ostream& os, const QString& s )
{
    return os << s.toStdString();
}

inline xml::xostream& operator<<( xml::xostream& xos, const QString& s )
{
    return xos << s.toStdString();
}

#endif // __preparation_app_pch_h_
