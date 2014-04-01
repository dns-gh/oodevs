// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __preparation_pch_h_
#define __preparation_pch_h_

#pragma warning( disable : 4290 )
#pragma warning( disable : 4702 )
#pragma warning( disable : 4355 ) // 'this' : used in base member initializer list

#include <windows.h>
#undef min
#undef max
#undef Yield
#undef GetObject
#undef GetMessage

#pragma warning( push, 0 )
#include <QtCore/qdatetime.h>
#include <QtOpenGL/qgl.h>
#include <QtCore/qregexp.h>
#include <QtCore/qstring.h>
#pragma warning( pop )

#include <string>
#include <map>
#include <vector>
#include <set>

#include <tools/Exception.h>
#include <tools/Map.h>
#include <tools/Path.h>
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

#endif // __preparation_pch_h_
