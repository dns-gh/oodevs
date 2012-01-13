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

#pragma warning( disable : 4996 ) // Function call with parameters that may be unsafe

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
#include <iostream>
#undef GetObject

#undef max
#undef min

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

#endif // __clients_kernel_pch_h_
