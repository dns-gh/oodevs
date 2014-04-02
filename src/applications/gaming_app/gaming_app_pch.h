// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gaming_app_pch_h_
#define __gaming_app_pch_h_

#pragma warning( disable: 4231 4355 )

#include <windows.h>
#undef min
#undef max
#undef Yield
#undef GetObject
#undef GetMessage

#pragma warning( push, 0 )

#include <QtGui/QtGui>

#include <QtWebKit/qwebview.h>

#include <QtOpenGL/qgl.h>

#include <Qtcore/qtimer.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qobject.h>
#include <QtCore/qpoint.h>
#include <QtCore/qrect.h>
#include <QtCore/qsettings.h>
#include <QtCore/qsignalmapper.h>
#include <QtCore/qfile.h>
#include <QtGui/qtreeview.h>
#include <QtGui/qstandarditemmodel.h>
#include <QtGui/qitemselectionmodel.h>

#include <Qt3Support/q3grid.h>
#include <Qt3Support/q3groupbox.h>
#include <Qt3Support/q3hbox.h>
#include <Qt3Support/q3hgroupbox.h>
#include <Qt3Support/q3frame.h>
#include <Qt3Support/q3popupmenu.h>
#include <Qt3Support/q3progressbar.h>
#include <Qt3Support/q3buttongroup.h>
#include <Qt3Support/q3canvas.h>
#include <Qt3Support/q3vbox.h>
#include <Qt3Support/q3vgroupbox.h>
#include <Qt3Support/q3boxlayout.h>
#include <Qt3Support/q3gridlayout.h>
#include <Qt3Support/q3textbrowser.h>

#include "clients_gui/SubObjectName.h"

#pragma warning( pop )

#include <map>
#include <vector>
#include <set>

#include <tools/Exception.h>
#include <tools/Map.h>
#include <tools/Path.h>
#include "protocol/Protocol.h"
#include "tools/FileWrapper.h"
#include "tools/XmlStreamOperators.h"

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

#endif // __gaming_app_pch_h_
