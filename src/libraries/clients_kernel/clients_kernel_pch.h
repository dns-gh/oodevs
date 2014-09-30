// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __clients_kernel_pch_h_
#define __clients_kernel_pch_h_

#pragma warning( push, 0 )
#include <QtCore/qdatetime.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtGui/qapplication.h>
#include <QtGui/qfont.h>
#include <QtGui/qwidget.h>
#include <QtOpenGL/qgl.h>
#pragma warning( pop )

#include <string>
#include <vector>
#include <map>
#include <set>
#undef GetObject

#undef max
#undef min

#include <tools/Exception.h>

#include "Types.h"
#include <xeumeuleu/xml.hpp>

inline std::ostream& operator<<( std::ostream& os, const QString& s )
{
    return os << s.toStdString();
}

inline xml::xostream& operator<<( xml::xostream& xos, const QString& s )
{
    return xos << s.toStdString();
}

inline std::istream& operator>>( std::istream& is, QString& s )
{
    std::string tmp;
    is >> tmp;
    s = QString::fromStdString( tmp );
    return is;
}

inline xml::xistream& operator>>( xml::xistream& xis, QString& s )
{
    std::string tmp;
    xis >> tmp;
    s = QString::fromStdString( tmp );
    return xis;
}

#endif // __clients_kernel_pch_h_
