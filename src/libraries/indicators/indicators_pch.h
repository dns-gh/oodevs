// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __indicators_pch_h_
#define __indicators_pch_h_

#pragma warning( disable: 4996 ) // Function call with parameters that may be unsafe
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )
#include <map>
#include <string>
#include <vector>
#include <ostream>
#include <xeumeuleu/xml.hpp>

inline std::ostream& operator<<( std::ostream& os, const QString& s )
{
    return os << s.toStdString();
}

inline xml::xostream& operator<<( xml::xostream& xos, const QString& s )
{
    return xos << s.toStdString();
}

#endif // __indicators_pch_h_
