// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __clients_test_pch_h_
#define __clients_test_pch_h_

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <google/protobuf/stubs/common.h>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#define BOOST_TODO BOOST_WARN_MESSAGE( 0, "TODO !" )

#include <turtle/mock.hpp>
#pragma warning( push, 0 )
#include <QtCore/QString>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>

inline std::ostream& operator<<( std::ostream& os, const QString& s )
{
    return os << s.toAscii().constData();
}

inline xml::xostream& operator<<( xml::xostream& xos, const QString& s )
{
    return xos << s.toAscii().constData();
}

#pragma warning( disable : 4505 )

std::string BOOST_RESOLVE( const std::string& filename );

struct ProtobufConfig {
    ProtobufConfig()
    {
        GOOGLE_PROTOBUF_VERIFY_VERSION;
    }
    ~ProtobufConfig()
    {
        google::protobuf::ShutdownProtobufLibrary();
    }
};

BOOST_GLOBAL_FIXTURE( ProtobufConfig );

#endif // __clients_test_pch_h_
