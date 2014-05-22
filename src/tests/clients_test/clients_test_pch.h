// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __clients_test_pch_h_
#define __clients_test_pch_h_

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#define BOOST_TODO BOOST_WARN_MESSAGE( 0, "TODO !" )

#include <turtle/mock.hpp>
#pragma warning( push, 0 )
#include <QtCore/QString>
#include <QtCore/QStringlist>
#include <QtGui/QApplication>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>

#include <tools/Exception.h>

inline std::ostream& operator<<( std::ostream& os, const QString& s )
{
    return os << s.toStdString();
}

inline std::ostream& operator<<( std::ostream& os, const QStringList& l )
{
    for( auto it = l.begin(); it != l.end(); ++it )
    {
        os << '"' << *it << '"';
        if( it + 1 != l.end() )
            os << ' ';
    }
    return os;
}

inline xml::xostream& operator<<( xml::xostream& xos, const QString& s )
{
    return xos << s.toStdString();
}

#pragma warning( disable : 4505 )

std::string BOOST_RESOLVE( const std::string& filename );

#endif // __clients_test_pch_h_
