// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gaming_pch_h_
#define __gaming_pch_h_

#pragma warning( disable: 4290 ) // C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#pragma warning( disable: 4702 ) // unreachable code
#pragma warning( disable: 4355 ) // 'this' : used in base member initializer list
#pragma warning( disable: 4503 ) // decorated name length exceeded, name was truncated

#include <windows.h>
#undef min
#undef max
#undef Yield
#undef GetObject
#undef GetMessage

#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#include <QtOpenGL/qgl.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qsettings.h>
#pragma warning( pop )
#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <set>

#include "protocol/Protocol.h"
#include <tools/Exception.h>

#include <geometry/Types.h>
typedef std::vector< geometry::Point2f > T_PointVector;
typedef T_PointVector::iterator         IT_PointVector;
typedef T_PointVector::const_iterator  CIT_PointVector;

#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Extension_ABC.h"

#undef uint

#include <xeumeuleu/xml.hpp>

inline std::ostream& operator<<( std::ostream& os, const QString& s )
{
    return os << s.toStdString();
}

inline xml::xostream& operator<<( xml::xostream& xos, const QString& s )
{
    return xos << s.toStdString();
}

#endif // __gaming_pch_h_
